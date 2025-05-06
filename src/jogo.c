#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
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
    if (tab->grelha[lin][col] >= 'A' && tab->grelha[lin][col] <= 'Z') {
        printf("Posição já preenchida!\n");
    }
    else if (tab->grelha[lin][col] == '#') {
        printf("Posição já riscada! Tente de novo.\n");
    }
    else if (lin >= 0 && lin < tab->linhas && col >= 0 && col < tab->colunas) {
        empurrarPilha(pilha, lin, col, tab->grelha[lin][col], 'b');
        tab->grelha[lin][col] = (char)toupper((unsigned char)tab->grelha[lin][col]);
    } else {
        printf("Posição inválida! Tente de novo.\n");
    }
}
void riscar(Tabuleiro* tab, int lin, int col, Pilha* pilha) {
    if (tab->grelha[lin][col] >= 'A' && tab->grelha[lin][col] <= 'Z') {
        printf("Posição já preenchida! Tente de novo.\n");
    }
    else if (tab->grelha[lin][col] == '#') {
        printf("Posição já riscada!\n");
    }
    else if (lin >= 0 && lin < tab->linhas && col >= 0 && col < tab->colunas) {
        empurrarPilha(pilha, lin, col, tab->grelha[lin][col], 'r');
        tab->grelha[lin][col] = '#';
        return;
    } else {
        printf("Posição inválida! Tente de novo.\n");
    }
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
                printf("Se a posição (%c, %d) está riscada a posição (%c, %d) deve ser branca!\n",lin + 'a', col + 1, newCol + 'a', newLin + 1);
                r = 1;
            }
        }
    }
    return r;
    // return 0; // Se não encontrar nenhuma correspondência
    // return 1; // Se encontrar correspondência
}

int verifica (Tabuleiro* tab) {
    int r = 0;
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            if (tab->grelha[i][j] == '#') r += verificarRisca(tab, i, j);
            else if (tab->grelha[i][j] >= 'A' && tab->grelha[i][j] <= 'Z') r += verificarBranco(tab, i, j);
        }
    }
    return r;
}

void inicializarPilha(Pilha* pilha, int capacidade) {
    pilha->capacidade = capacidade;
    pilha->topo = -1;
    pilha->jogadas = (Jogada*)malloc(capacidade * sizeof(Jogada)); // Alocando memória
    if (pilha->jogadas == NULL) {
        printf("Erro ao alocar memória para a pilha\n");
        exit(1);
    }
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
    if (pilha->jogadas != NULL) {
        free(pilha->jogadas);  // Libera a memória alocada para as jogadas
        pilha->jogadas = NULL;  // Evita o uso de um ponteiro pendente
    }
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


void ajudar(Tabuleiro* tab, Pilha* pilha) {
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

    // Pintar de branco casas vizinhas de uma casa riscada e casas que não podem ser riscadas por isolar casas brancas
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tab->grelha[i][j] == '#') {
                if (i > 0 && tab->grelha[i - 1][j] != '#') branco(tab, i - 1, j, pilha); // Pintar de branco acima
                if (i < linhas - 1 && tab->grelha[i + 1][j] != '#') branco(tab, i + 1, j, pilha); // Pintar de branco abaixo
                if (j > 0 && tab->grelha[i][j - 1] != '#') branco(tab, i, j - 1, pilha); // Pintar de branco à esquerda
                if (j < colunas - 1 && tab->grelha[i][j + 1] != '#') branco(tab, i, j + 1, pilha); // Pintar de branco à direita
            } else if (tab->grelha[i][j] == ' ') { // Casa vazia
                int brancasAdjacentes = 0;
                int direcoes[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // cima, baixo, esquerda, direita

                for (int d = 0; d < 4; d++) {
                    int newLin = i + direcoes[d][0];
                    int newCol = j + direcoes[d][1];

                    if (newLin >= 0 && newLin < linhas && newCol >= 0 && newCol < colunas) {
                        if (isupper(tab->grelha[newLin][newCol])) {
                            brancasAdjacentes++;
                        }
                    }
                }

                if (brancasAdjacentes >= 2) {
                    branco(tab, i, j, pilha); // Pintar de branco
                    *cont = 1;
                }
            }
        }
    }

    // Verifica se o tabuleiro está correto
    int resultado = verifica(tab);
    if (resultado == 0) {
        printf("O tabuleiro está correto!\n");
    } else {
        printf("O tabuleiro não está correto!\n");
    }

    // Atualiza o tabuleiro
    // printf("Tabuleiro atualizado:\n");
    //ler(tab);
}





void resolver(Tabuleiro* tab, Pilha* pilha) {
    int casas = tab->colunas * tab->linhas;
    int altera = 1;
    while (altera) {
        altera = 0;
        for (int i = 0; i < tab->linhas; i++) {
            int *contagem = calloc((size_t)casas, sizeof(int));
            int *ultimaColuna = malloc((size_t)casas * sizeof(int)); // Aloca memória para armazenar a última coluna de cada letra

            for (int j = 0; j < tab->colunas; j++) {
                char c = tab->grelha[i][j];
                if (c >= 'a' && c <= 'z') {
                    contagem[c - 'a']++;
                    ultimaColuna[c - 'a'] = j;
                }
                else if (c >= 'A' && c <= 'Z') {
                    contagem[c - 'A']++;
                    ultimaColuna[c - 'A'] = j;
                }
            }

            for (int k = 0; k < casas; k++) {
                if (contagem[k] == 1) {
                    int col = ultimaColuna[k];
                    branco(tab, i, col, pilha);
                }
            }
            if (contagem) free(contagem);
            if (ultimaColuna) free(ultimaColuna);
        }

        for (int j = 0; j < tab->colunas; j++) {
            int *contagem = calloc((size_t)casas, sizeof(int));
            int *ultimaLinha = malloc((size_t)casas * sizeof(int));

            for (int i = 0; i < tab->linhas; i++) {
                char c = (tab->grelha[i][j]);
                if (c >= 'a' && c <= 'z') {
                    contagem[c - 'a']++;
                    ultimaLinha[c - 'a'] = i;
                }
                else if (c >= 'A' && c <= 'Z') {
                    contagem[c - 'A']++;
                    ultimaLinha[c - 'A'] = i;
                }
            }

            for (int k = 0; k < casas; k++) {
                if (contagem[k] == 1) {
                    int lin = ultimaLinha[k];
                    branco(tab, lin, j, pilha);
                }
            }
            if (contagem) free(contagem);
            if (ultimaLinha) free(ultimaLinha);
        }

        // Regra 3: Se uma letra minuscula aparece só uma vez na linha, riscar
        for (int i = 0; i < tab->linhas; i++) {
            int *contagem = calloc((size_t)casas, sizeof(int));
            int *ultimaColuna = malloc((size_t)casas * sizeof(int));
            for (int j = 0; j < tab->colunas; j++) {
                char c = tab->grelha[i][j];
                if (c >= 'a' && c <= 'z') {
                    contagem[c - 'a']++;
                    ultimaColuna[c - 'a'] = j;
                }
            }
            for (int k = 0; k < casas; k++) {
                if (contagem[k] == 1) {
                    int col = ultimaColuna[k];
                    riscar(tab, i, col, pilha);
                }
            }
            free(contagem);
            free(ultimaColuna);
        }

        // Regra 4: Todas as letras minúsculas adjacentes a uma letra riscada devem ficar brancas
        for (int i = 0; i < tab->linhas; i++) {
            for (int j = 0; j < tab->colunas; j++) {
                if (tab->grelha[i][j] == '#') {
                    int direcoes[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // cima, baixo, esquerda, direita
                    for (int k = 0; k < 4; k++) {
                        int newLin = i + direcoes[k][0];
                        int newCol = j + direcoes[k][1];
                
                        if (newLin >= 0 && newLin < tab->linhas && newCol >= 0 && newCol < tab->colunas) {
                            if (tab->grelha[newLin][newCol] < 'A' || tab->grelha[newLin][newCol] > 'Z') {
                                branco(tab, newLin, newCol, pilha);
                                altera = 1; // Indica que houve alteração
                            }
                        }

                    }
                }
            }
        }
    }
    // Verifica se não há casas repetidas
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            if (tab->grelha[i][j] >= 'A' && tab->grelha[i][j] <= 'Z') {
                if (verificarBranco(tab, i, j)) {
                    desfazer(tab, pilha);
                    printf("O tabuleiro não pode ser resolvido!\n");
                    return;
                }
            }
        }
    }
}