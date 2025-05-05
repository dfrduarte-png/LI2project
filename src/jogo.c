#include "jogo.h"

Tabuleiro* carregar(const char* ficheiro, Pilha* pilha) {
    FILE* f = fopen(ficheiro, "r");
    if (!f) {
        perror("Erro ao abrir ficheiro");
        return NULL;
    }
    Tabuleiro* tab = malloc((size_t)sizeof(Tabuleiro));
    if (!fscanf(f, "%d %d", &tab->linhas, &tab->colunas)) {
        printf("Erro ao ler o tamanho do tabuleiro\n");
        fclose(f);
        free(tab);
        return NULL;
    }
    else if (tab->linhas <= 0 || tab->colunas <= 0 || tab->linhas != tab->colunas) {
        printf("Tamanho inválido do tabuleiro\n");
        fclose(f);
        free(tab);
        return NULL;
    }

    tab->grelha = malloc((size_t)tab->linhas * sizeof(char*));
    for (int i = 0; i < tab->linhas; i++) {
        tab->grelha[i] = malloc((size_t)tab->colunas * sizeof(char));
        for (int j = 0; j < tab->colunas; j++) {
            if (fscanf(f, " %c", &tab->grelha[i][j]) != 1) {
                printf("Erro ao ler o tabuleiro do ficheiro.\n");
                fclose(f);
                freeTabuleiro(tab);
                return NULL;
            }
        }
    }

    // Ignorar separador "--"
    char linhaBuffer[100];
    while (fgets(linhaBuffer, sizeof(linhaBuffer), f)) {
        if (linhaBuffer[0] == '-' && linhaBuffer[1] == '-') break;
    }

    // Ler jogadas
    char tipo, coluna;
    int linha;
    while (fscanf(f, " %c %c %d", &tipo, &coluna, &linha) == 3) {
        int lin = linha - 1;
        int col = coluna - 'a';
        if (lin >= 0 && lin < tab->linhas && col >= 0 && col < tab->colunas) {
            char anterior = tab->grelha[lin][col];
            empurrarPilha(pilha, lin, col, anterior, tipo);
            tab->grelha[lin][col] = (tipo == 'b') ? (char)toupper((unsigned char)anterior) : '#';
        }
    }
    fclose(f);
    return tab;
}

void ler(Tabuleiro* tab) {
    printf("    ");
    for (int i = 0; i < tab->linhas; i++) {
        printf("%c ", i + 'a');
    }
    printf("\n    -");
    for (int i = 1; i < tab->linhas; i++) {
        printf("--");
    }
    printf("\n");
    for (int i = 0; i < tab->linhas; i++) {
        printf("%2d| ", i + 1);
        for (int j = 0; j < tab->colunas; j++) {
            printf("%c ", tab->grelha[i][j]);
        }
        printf("\n");
    }
}

void branco(Tabuleiro* tab, int lin, int col, Pilha* pilha) { 
    if (lin >= 0 && lin < tab->linhas && col >= 0 && col < tab->colunas) {
        empurrarPilha(pilha, lin, col, tab->grelha[lin][col], 'b');
        tab->grelha[lin][col] = (char)toupper((unsigned char)tab->grelha[lin][col]);
    } else {
        printf("Posição inválida! Tente de novo.\n");
    }
}
void riscar(Tabuleiro* tab, int lin, int col, Pilha* pilha) {
    if (lin >= 0 && lin < tab->linhas && col >= 0 && col < tab->colunas) {
        empurrarPilha(pilha, lin, col, tab->grelha[lin][col], 'r');
        tab->grelha[lin][col] = '#';
        return;
    } else {
        printf("Posição inválida! Tente de novo.\n");
    }
}

void freeTabuleiro(Tabuleiro* tab) {
    for (int i = 0; i < tab->linhas; i++) free(tab->grelha[i]);
    free(tab->grelha);
    free(tab);
}

int verificarBranco(Tabuleiro* tab, int lin, int col) {
    char current = tab->grelha[lin][col];
    int r = 0;
    // Verificar verticalmente (cima e baixo)
    for (int i = lin + 1; i < tab->linhas; i++) {
        if (tab->grelha[i][col] == current) {
            printf("A posição (%c, %d) e (%c, %d) são brancas e iguais!\n",col + 'a', lin + 1, col + 'a', i + 1);
            r = 1;
        }
    }
    // Verificar horizontalmente (esquerda e direita)
    for (int j = col + 1; j < tab->colunas; j++) {
        if (tab->grelha[lin][j] == current) {
            printf("A posição (%c, %d) e (%c, %d) são brancas e iguais!\n",col + 'a', lin + 1, j + 'a', lin + 1);
            r = 1;
        }
    }
    // Verificar diagonalmente (cima-esquerda, cima-direita, baixo-esquerda, baixo-direita)
    int up = (lin > 0) ? tab->grelha[lin - 1][col] == '#' : 1;
    int down = (lin < tab->linhas - 1) ? tab->grelha[lin + 1][col] == '#' : 1;
    int left = (col > 0) ? tab->grelha[lin][col - 1] == '#' : 1;
    int right = (col < tab->colunas - 1) ? tab->grelha[lin][col + 1] == '#' : 1;

    if (up && down && left && right) {
        r = 1;
        printf("A posição (%c, %d) está cercada por '#'!\n", col + 'a', lin + 1);
    }
    return r;
    // return 0; // Se não encontrar nenhuma correspondência (válido)
    // return 1; // Se encontrar correspondência (invalido)
}

int verificarRisca(Tabuleiro* tab, int lin, int col) {
    int direcoes[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // cima, baixo, esquerda, direita
    int r = 0;
    for (int i = 0; i < 4; i++) {
        int newLin = lin + direcoes[i][0];
        int newCol = col + direcoes[i][1];

        if (newLin >= 0 && newLin < tab->linhas && newCol >= 0 && newCol < tab->colunas) {
            if (tab->grelha[newLin][newCol] < 'A' || tab->grelha[newLin][newCol] > 'Z') {
                printf("Se a posição (%c, %d) está riscada a posição (%c, %d) deve ser branca!\n",col + 'a', lin + 1, newCol + 'a', newLin + 1);
                r = 1;
            }
        }
    }
    return r;
    // return 0; // Se não encontrar nenhuma correspondência
    // return 1; // Se encontrar correspondência
}

void dfs(Tabuleiro* tab, int lin, int col, int visitado[tab->linhas][tab->colunas]) {
    if (lin < 0 || lin >= tab->linhas || col < 0 || col >= tab->colunas) return;
    if (visitado[lin][col]) return;
    if (tab->grelha[lin][col] < 'A' || tab->grelha[lin][col] > 'Z') return; // Não é branco

    visitado[lin][col] = 1;

    dfs(tab, lin - 1, col, visitado); // cima
    dfs(tab, lin + 1, col, visitado); // baixo
    dfs(tab, lin, col - 1, visitado); // esquerda
    dfs(tab, lin, col + 1, visitado); // direita
}

int verificaConectividade(Tabuleiro* tab) {
    int visitado[tab->linhas][tab->colunas];
    memset(visitado, 0, sizeof(visitado));

    // Encontrar a primeira casa branca
    int encontrou = 0, startLin = 0, startCol = 0;
    for (int i = 0; i < tab->linhas && !encontrou; i++) {
        for (int j = 0; j < tab->colunas && !encontrou; j++) {
            if (tab->grelha[i][j] >= 'A' && tab->grelha[i][j] <= 'Z') {
                startLin = i;
                startCol = j;
                encontrou = 1;
            }
        }
    }

    if (!encontrou) {
        return 0; // Não há casas brancas, considerado válido
    }

    // Iniciar DFS
    dfs(tab, startLin, startCol, visitado); // modifica visitado para marcar as casas visitadas

    // Verificar se todas as casas brancas foram visitadas
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            if (tab->grelha[i][j] >= 'A' && tab->grelha[i][j] <= 'Z' && !visitado[i][j]) {
                printf("Casa branca (%c, %d) está desconectada!\n", j + 'a', i + 1);
                return 1; // Conectividade quebrada
            }
        }
    }

    printf("Todas as casas brancas estão conectadas!\n");
    return 0; // Tudo certo
}

int verifica (Tabuleiro* tab) {
    int r = 0;
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            if (tab->grelha[i][j] == '#') r += verificarRisca(tab, i, j);
            else if (tab->grelha[i][j] >= 'A' && tab->grelha[i][j] <= 'Z') r += verificarBranco(tab, i, j);
        }
    }
    r += verificaConectividade(tab);
    return r;
}

// Função para inicializar a pilha
void inicializarPilha(Pilha* pilha, int capacidade) {
    pilha->jogadas = malloc((size_t)capacidade * sizeof(Jogada));
    pilha->topo = -1;
    pilha->capacidade = capacidade;
}

void redimensionarPilha(Pilha* pilha) {
    pilha->capacidade *= 2; // Dobra a capacidade
    pilha->jogadas = realloc(pilha->jogadas, (size_t)pilha->capacidade * sizeof(Jogada));
    if (!pilha->jogadas) {
        perror("Erro ao redimensionar a pilha");
        exit(EXIT_FAILURE);
    }
    printf("Pilha redimensionada para capacidade %d.\n", pilha->capacidade);
}

void freePilha(Pilha* pilha) {
    free(pilha->jogadas);
}

// Função para adicionar uma jogada à pilha
void empurrarPilha(Pilha* pilha, int lin, int col, char anterior, char tipo) {
    // Verifica se a pilha está cheia
    if (pilha->topo == pilha->capacidade - 1) {
        redimensionarPilha(pilha); // Redimensiona a pilha se necessário
    }

    // Adiciona a jogada à pilha
    pilha->jogadas[++pilha->topo] = (Jogada){lin, col, anterior, tipo};
}

void guardar(Tabuleiro* tab, Pilha* pilha, const char* ficheiro) {
    FILE* f = fopen(ficheiro, "r");
    if (!f) {
        perror("Erro ao abrir ficheiro para leitura");
        return;
    }

    // Copiar as primeiras (dimensao + 1) linhas
    int linhas_a_copiar = tab->linhas + 1; // Dimensão do tabuleiro + 1 (linha das dimensões)
    char** linhas = malloc((size_t)linhas_a_copiar * sizeof(char*)); // Aloca memória para armazenar as linhas
    for (int i = 0; i < linhas_a_copiar; i++) {
        linhas[i] = malloc(100 * sizeof(char)); // Aloca memória para cada linha
        if (!fgets(linhas[i], 100, f)) {
            perror("Erro ao ler o tabuleiro do ficheiro");
            fclose(f);
            for (int j = 0; j <= i; j++) free(linhas[j]); // Libera memória alocada
            free(linhas);
            return;
        }
    }
    fclose(f);

    // Abrir o arquivo no modo de escrita para apagar o conteúdo
    f = fopen(ficheiro, "w");
    if (!f) {
        perror("Erro ao abrir ficheiro para escrita");
        for (int i = 0; i < linhas_a_copiar; i++) free(linhas[i]); // Libera memória alocada
        free(linhas);
        return;
    }

    // Reescrever as linhas copiadas
    for (int i = 0; i < linhas_a_copiar; i++) {
        fputs(linhas[i], f);
        free(linhas[i]); // Libera memória da linha após escrevê-la
    }
    free(linhas); // Libera o array de ponteiros

    // Linha separadora
    fprintf(f, "--\n");

    // Escrever as jogadas feitas na pilha
    for (int i = 0; i <= pilha->topo; i++) {
        Jogada jogada = pilha->jogadas[i];
        fprintf(f, "%c %c %d\n", jogada.tipo, 'a' + jogada.col, jogada.lin + 1);
    }

    fclose(f);
    printf("Estado do jogo salvo com sucesso!\n");
}

// Função para remover uma jogada da pilha (desfazer)
void desfazer(Tabuleiro* tab, Pilha* pilha) {
    if (pilha->topo == -1) {
        printf("Não há jogadas para desfazer!\n");
        return;
    }

    Jogada ultimaJogada = pilha->jogadas[pilha->topo--];
    tab->grelha[ultimaJogada.lin][ultimaJogada.col] = ultimaJogada.anterior;
    printf("Última jogada desfeita.\n");
}

void validarUnicidade(Tabuleiro *tab, Pilha *pilha) {
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            char c = tab->grelha[i][j];
            int unicoLinha = 1, unicoColuna = 1;

            for (int k = 0; k < tab->linhas; k++) {
                if (k != i && tab->grelha[k][j] == c) {
                    unicoLinha = 0;
                    break;
                }
            }

            for (int l = 0; l < tab->colunas; l++) {
                if (l != j && tab->grelha[i][l] == c) {
                    unicoColuna = 0;
                    break;
                }
            }

            if (unicoLinha && unicoColuna) {
                branco(tab, i, j, pilha);
            }
        }
    }
}

void riscarDuplicados(Tabuleiro *tab, Pilha *pilha) {
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            char c = tab->grelha[i][j];

            // Só nos interessa se for uma letra branca (maiúscula)
            if (c >= 'A' && c <= 'Z') {
                char original = c + ('a' - 'A'); // Convertendo de maiúscula para minúscula

                // Riscamos todas as outras ocorrências na mesma linha
                for (int col = 0; col < tab->colunas; col++) {
                    if (col != j && tab->grelha[i][col] == original) {
                        riscar(tab, i, col, pilha);
                    }
                }

                // Riscamos todas as outras ocorrências na mesma coluna
                for (int lin = 0; lin < tab->linhas; lin++) {
                    if (lin != i && tab->grelha[lin][j] == original) {
                        riscar(tab, lin, j, pilha);
                    }
                }
            }
        }
    }
}

void riscaEverifica(Tabuleiro *tab, Pilha *pilha) {
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            if (tab->grelha[i][j] >= 'a' && tab->grelha[i][j] <= 'z') {
                int marcador = pilha->topo;
                riscar(tab, i, j, pilha);

                int direcoes[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
                for (int k = 0; k < 4; k++) {
                    int newLin = i + direcoes[k][0];
                    int newCol = j + direcoes[k][1];
                    if (newLin >= 0 && newLin < tab->linhas && newCol >= 0 && newCol < tab->colunas) {
                        branco(tab, newLin, newCol, pilha);
                    }
                }

                // Verificar se há conflito de letras brancas
                int conflito = 0;
                for (int k = 0; k < 4; k++) {
                    int newLin = i + direcoes[k][0];
                    int newCol = j + direcoes[k][1];
                    if (newLin >= 0 && newLin < tab->linhas && newCol >= 0 && newCol < tab->colunas) {
                        if (verificarBranco(tab, newLin, newCol)) {
                            conflito = 1;
                            break;
                        }
                    }
                }

                for (int x = 0; x < tab->linhas && !conflito; x++) {
                    for (int y = 0; y < tab->colunas && !conflito; y++) {
                        char c1 = tab->grelha[x][y];
                        if (c1 < 'A' || c1 > 'Z') continue; // ignora não-brancas
                        for (int xx = 0; xx < tab->linhas; xx++) {
                            if (xx != x && tab->grelha[xx][y] == c1) {
                                conflito = 1;
                                break;
                            }
                        }
                        for (int yy = 0; yy < tab->colunas; yy++) {
                            if (yy != y && tab->grelha[x][yy] == c1) {
                                conflito = 1;
                                break;
                            }
                        }
                    }
                }

                if (conflito) {
                    while (pilha->topo > marcador) desfazer(tab, pilha);
                    branco(tab, i, j, pilha);
                }
            }
        }
    }
}

void resolver(Tabuleiro* tab, Pilha* pilha) {
    int altera = 1;
    while (altera) {
        altera = 0;
        // 1. Garante unicidade de letras na linha e coluna
        validarUnicidade(tab, pilha);

        // 2. Tenta riscar e reverter se necessário
        riscaEverifica(tab, pilha);

        // 3. Riscamos cópias minúsculas de letras já brancas
        riscarDuplicados(tab, pilha);
    }

    if (verificaConectividade(tab)) {
        printf("O tabuleiro não pode ser resolvido!\n");
    } else {
        printf("Jogo resolvido!\n");
    }
}