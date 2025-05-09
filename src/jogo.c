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
    if (lin < 0 || lin >= tab->linhas || col < 0 || col >= tab->colunas) {
        printf("Posição inválida! Tente de novo.\n");
        return;
    }

    char c = tab->grelha[lin][col];
    if (c == '#' || (c >= 'A' && c <= 'Z')) return; // já riscado ou branco

    empurrarPilha(pilha, lin, col, c, 'b');
    tab->grelha[lin][col] = (char)toupper((unsigned char)c);
}

void riscar(Tabuleiro* tab, int lin, int col, Pilha* pilha) {
    if (lin < 0 || lin >= tab->linhas || col < 0 || col >= tab->colunas) {
        printf("Posição inválida! Tente de novo.\n");
        return;
    }

    char c = tab->grelha[lin][col];
    if (c == '#' || (c >= 'A' && c <= 'Z')) return; // já riscado ou branco

    empurrarPilha(pilha, lin, col, c, 'r');
    tab->grelha[lin][col] = '#';
}


void freeTabuleiro(Tabuleiro* tab) {
    if (tab != NULL) {  // Verifica se o ponteiro não é NULL
        for (int i = 0; i < tab->linhas; i++) {
            free(tab->grelha[i]);  // Libera cada linha da grelha
        }
        free(tab->grelha);  // Libera o array de ponteiros para as linhas
        free(tab);  // Libera o próprio Tabuleiro
    }
}


int verificarBranco(Tabuleiro* tab, int lin, int col, int vprintar) {
    char current = tab->grelha[lin][col];
    int r = 0;
    // Verificar verticalmente (cima e baixo)
    for (int i = lin + 1; i < tab->linhas; i++) {
        if (tab->grelha[i][col] == current) {
            if (vprintar) printf("A posição (%c, %d) e (%c, %d) são brancas e iguais!\n",col + 'a', lin + 1, col + 'a', i + 1);
            r = 1;
        }
    }
    // Verificar horizontalmente (esquerda e direita)
    for (int j = col + 1; j < tab->colunas; j++) {
        if (tab->grelha[lin][j] == current) {
            if (vprintar) printf("A posição (%c, %d) e (%c, %d) são brancas e iguais!\n",col + 'a', lin + 1, j + 'a', lin + 1);
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
        if (vprintar) printf("A posição (%c, %d) está cercada por '#'!\n", col + 'a', lin + 1);
    }
    return r;
    // return 0; // Se não encontrar nenhuma correspondência (válido)
    // return 1; // Se encontrar correspondência (invalido)
}

int verificarRisca(Tabuleiro* tab, int lin, int col, int vprintar) {
    int direcoes[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // cima, baixo, esquerda, direita
    int r = 0;
    for (int i = 0; i < 4; i++) {
        int newLin = lin + direcoes[i][0];
        int newCol = col + direcoes[i][1];

        if (newLin >= 0 && newLin < tab->linhas && newCol >= 0 && newCol < tab->colunas) {
            if (tab->grelha[newLin][newCol] < 'A' || tab->grelha[newLin][newCol] > 'Z') {
                if (vprintar) printf("Se a posição (%c, %d) está riscada a posição (%c, %d) deve ser branca!\n",col + 'a', lin + 1, newCol + 'a', newLin + 1);
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

int verificaConectividade(Tabuleiro* tab, int vprintar) {
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
                if (vprintar) printf("Casa branca (%c, %d) está desconectada!\n", j + 'a', i + 1);
                return 1; // Conectividade quebrada
            }
        }
    }

    if (vprintar) printf("Todas as casas brancas estão conectadas!\n");
    return 0; // Tudo certo
}

int verifica (Tabuleiro* tab, int vprintar) {
    int r = 0;
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            if (tab->grelha[i][j] == '#') r |= verificarRisca(tab, i, j, vprintar);
            else if (tab->grelha[i][j] >= 'A' && tab->grelha[i][j] <= 'Z') r |= verificarBranco(tab, i, j, vprintar);
        }
    }
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


void ajudar(Tabuleiro* tab, Pilha* pilha, int *cont) {
    *cont = 0;
    int linhas = tab->linhas;
    int colunas = tab->colunas;

    // Riscar letras iguais a uma letra branca na mesma linha e coluna
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (isupper(tab->grelha[i][j])) { // Letra branca
                char letraBranca = tab->grelha[i][j];
                for (int k = 0; k < colunas; k++) {
                    if (tab->grelha[i][k] == tolower(letraBranca)) {
                        riscar(tab, i, k, pilha);
                        *cont = 1;
                    }
                }
                for (int k = 0; k < linhas; k++) {
                    if (tab->grelha[k][j] == tolower(letraBranca)) {
                        riscar(tab, k, j, pilha);
                        *cont = 1;
                    }
                }
            }
        }
    }

    // Pintar de branco casas vizinhas de uma casa riscada
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tab->grelha[i][j] == '#') {
                if (i > 0 && tab->grelha[i - 1][j] != '#') branco(tab, i - 1, j, pilha); // Pintar de branco acima
                if (i < linhas - 1 && tab->grelha[i + 1][j] != '#') branco(tab, i + 1, j, pilha); // Pintar de branco abaixo
                if (j > 0 && tab->grelha[i][j - 1] != '#') branco(tab, i, j - 1, pilha); // Pintar de branco à esquerda
                if (j < colunas - 1 && tab->grelha[i][j + 1] != '#') branco(tab, i, j + 1, pilha); // Pintar de branco à direita
            }
        }
    }

    // Verifica se o tabuleiro está correto
    int resultado = verifica(tab, 0);
    if (resultado == 0) {
        printf("O tabuleiro está correto!\n");
    } else {
        printf("O tabuleiro não está correto!\n");
    }

    // Atualiza o tabuleiro
    // printf("Tabuleiro atualizado:\n");
    //ler(tab);
}

int verificaBranco2(Tabuleiro* tab) {
    // Verifica se há letras duplicadas ou letras cercadas por #
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            char atual = tab->grelha[i][j];
            if (atual < 'A' || atual > 'Z') continue; // Ignora se não for letra

            // Verifica duplicatas na mesma linha
            for (int jj = j + 1; jj < tab->colunas; jj++) {
                if (tab->grelha[i][jj] == atual) return 1; // ERRO: duplicata
            }

            // Verifica duplicatas na mesma coluna
            for (int ii = i + 1; ii < tab->linhas; ii++) {
                if (tab->grelha[ii][j] == atual) return 1; // ERRO: duplicata
            }

            // Verifica se está cercado por '#'
            int cima     = (i > 0) ? tab->grelha[i - 1][j] == '#' : 1;
            int baixo    = (i < tab->linhas - 1) ? tab->grelha[i + 1][j] == '#' : 1;
            int esquerda = (j > 0) ? tab->grelha[i][j - 1] == '#' : 1;
            int direita  = (j < tab->colunas - 1) ? tab->grelha[i][j + 1] == '#' : 1;
            if (cima && baixo && esquerda && direita) return 1; // ERRO: cercado
        }
    }
    return 0; // OK: sem duplicatas nem letras cercadas
}



/*
void resolver(Tabuleiro* tab, Pilha* pilha, int vprintar, int in, int jn) {
    for (int i = in; i < tab->linhas; i++) {
        for (int j = jn; j < tab->colunas; j++) {
            char c = tab->grelha[i][j];

            // Se for uma letra por decidir (minúscula)
            if (c >= 'a' && c <= 'z') {
                int marcador = pilha->topo;

                // --- TENTAR RISCAR ---
                riscar(tab, i, j, pilha);
                if (vizinhosBrancos(tab, pilha, i, j) &&
                    riscarDuplicados(tab, pilha) &&
                    verificaBranco2(tab)) {

                    resolver(tab, pilha, vprintar, i, j); // tenta resolver a seguir

                    if (!verifica(tab, vprintar)) return; // encontrou solução
                }

                // desfaz tentativa de riscar
                while (pilha->topo > marcador) desfazer(tab, pilha);

                // --- TENTAR BRANCO ---
                marcador = pilha->topo;
                branco(tab, i, j, pilha);
                if (riscarDuplicados(tab, pilha) &&
                   verificaBranco2(tab)) {

                    resolver(tab, pilha, vprintar, i, j); // tenta resolver a seguir

                    if (!verifica(tab, vprintar)) return;
                }

                // desfaz tentativa de branco
                while (pilha->topo > marcador) desfazer(tab, pilha);

                return; // nenhuma opção deu certo, volta (backtrack)
            }
            jn = 0; // Reseta jn para 0 após cada linha
        }
    }
}
*/