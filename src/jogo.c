#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "jogo.h"

Tabuleiro* carregar(const char* ficheiro) {
    FILE* f = fopen(ficheiro, "r");
    if (!f) {
        perror("Erro ao abrir ficheiro");
        return NULL;
    }

    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    if (!fscanf(f, "%d %d", &tab->linhas, &tab->colunas)) {
        printf("Erro ao ler o tamanho do tabuleiro\n");
        fclose(f);
        free(tab);
        return NULL;
    }

    tab->grelha = malloc(tab->linhas * sizeof(char*));
    for (int i = 0; i < tab->linhas; i++)
        tab->grelha[i] = malloc(tab->colunas * sizeof(char));

    // Carrega o estado inicial do tabuleiro
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            fscanf(f, " %c", &tab->grelha[i][j]);
        }
    }

    // Aplica as alterações registradas no arquivo
    char acao, col;
    int lin;
    while (fscanf(f, " %c %c %d", &acao, &col, &lin) == 3) {
        int coluna = col - 'a'; // Converte a letra da coluna para índice
        if (acao == 'b') {
            tab->grelha[lin - 1][coluna] = toupper(tab->grelha[lin - 1][coluna]);
        } else if (acao == 'r') {
            tab->grelha[lin - 1][coluna] = '#';
        }
    }

    fclose(f);
    return tab;
}

void ler(Tabuleiro* tab) {
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            printf("%c ", tab->grelha[i][j]);
        }
        printf("\n");
    }
}

// Função para inicializar a pilha
void inicializarPilha(Pilha* pilha, int capacidade) {
    pilha->jogadas = (Jogada*)malloc(capacidade * sizeof(Jogada));
    pilha->topo = -1;
    pilha->capacidade = capacidade;
}

// Função para liberar a pilha
void liberarPilha(Pilha* pilha) {
    free(pilha->jogadas);
}

// Função para adicionar uma jogada à pilha
void empurrarPilha(Pilha* pilha, int lin, int col, char anterior) {
    if (pilha->topo < pilha->capacidade - 1) {
        pilha->topo++;
        pilha->jogadas[pilha->topo].lin = lin;
        pilha->jogadas[pilha->topo].col = col;
        pilha->jogadas[pilha->topo].anterior = anterior;
    } else {
        printf("Erro: A pilha está cheia!\n");
    }
}

// Função para remover uma jogada da pilha (desfazer)
void desfazer(Tabuleiro* tab, Pilha* pilha, int* verifica) {
    if (pilha->topo == -1) {
        printf("Não há jogadas para desfazer!\n");
        return;
    }

    // Recupera a última jogada
    Jogada ultimaJogada = pilha->jogadas[pilha->topo];

    // Restaura o tabuleiro para o estado anterior
    tab->grelha[ultimaJogada.lin][ultimaJogada.col] = ultimaJogada.anterior;
    
    // Ajusta o estado de verificação
    *verifica = 1;  // Definimos como 1 para revalidar o tabuleiro

    pilha->topo--;  // Remove a última jogada da pilha
    printf("Última jogada desfeita.\n");
}

int verificarBranco(Tabuleiro* tab, int lin, int col) {
    for (int i = lin + 1; i < tab->linhas; i++) {
        if (tab->grelha[i][col] == tab->grelha[lin][col]) return 0;
    }
    for (int j = col + 1; j < tab->colunas; j++) {
        if (tab->grelha[lin][j] == tab->grelha[lin][col]) return 0;
    }
    for (int i = lin - 1; i >= 0; i--) {
        if (tab->grelha[i][col] == tab->grelha[lin][col]) return 0;
    }
    for (int j = col - 1; j >= 0; j--) {
        if (tab->grelha[lin][j] == tab->grelha[lin][col]) return 0;
    }
    if (lin == 0 && col == 0) {
        if (tab->grelha[lin + 1][col] == '#' && tab->grelha[lin][col + 1] == '#') return 0;
    }
    else if (lin == 0 && col == tab->colunas - 1) {
        if (tab->grelha[lin + 1][col] == '#' && tab->grelha[lin][col - 1] == '#') return 0;
    }
    else if (lin == tab->linhas - 1 && col == 0) {
        if (tab->grelha[lin - 1][col] == '#' && tab->grelha[lin][col + 1] == '#') return 0;
    }
    else if (lin == tab->linhas - 1 && col == tab->colunas - 1) {
        if (tab->grelha[lin - 1][col] == '#' && tab->grelha[lin][col - 1] == '#') return 0;
    }
    else if (lin == 0) {
        if (tab->grelha[lin + 1][col] == '#' && tab->grelha[lin][col - 1] == '#' && tab->grelha[lin][col + 1] == '#') return 0;
    }
    else if (lin == tab->linhas - 1) {
        if (tab->grelha[lin - 1][col] == '#' && tab->grelha[lin][col - 1] == '#' && tab->grelha[lin][col + 1] == '#') return 0;
    }
    else if (col == 0) {
        if (tab->grelha[lin - 1][col] == '#' && tab->grelha[lin + 1][col] == '#' && tab->grelha[lin][col + 1] == '#') return 0;
    }
    else if (col == tab->colunas - 1) {
        if (tab->grelha[lin - 1][col] == '#' && tab->grelha[lin + 1][col] == '#' && tab->grelha[lin][col - 1] == '#') return 0;
    }
    else {
        if (tab->grelha[lin - 1][col] == '#' && tab->grelha[lin + 1][col] == '#' && tab->grelha[lin][col - 1] == '#' && tab->grelha[lin][col + 1] == '#') return 0;
    }
    return 1; // Não existe linha ou coluna que seja ingual e branca
}

void branco(Tabuleiro* tab, int lin, int col, int* verifica, Pilha* pilha) {
    if (lin >= 0 && lin < tab->linhas && col >= 0 && col < tab->colunas) {
        if (tab->grelha[lin][col] == '#') {
            printf("Posição já riscada! Tente de novo.\n");
            return;
        }

        // Guarda o estado anterior da célula
        empurrarPilha(pilha, lin, col, tab->grelha[lin][col]);
        
        tab->grelha[lin][col] = toupper(tab->grelha[lin][col]);
        if (*verifica == 1) *verifica = verificarBranco(tab, lin, col);
    } else {
        printf("Posição inválida! Tente de novo.\n");
    }
}

int verificarRisca(Tabuleiro* tab, int lin, int col) {
    if (lin == 0 && col == 0) {
        if (tab->grelha[lin + 1][col] == '#' || tab->grelha[lin][col + 1] == '#') return 0;
    }
    else if (lin == 0 && col == tab->colunas - 1) {
        if (tab->grelha[lin + 1][col] == '#' || tab->grelha[lin][col - 1] == '#') return 0;
    }
    else if (lin == tab->linhas - 1 && col == 0) {
        if (tab->grelha[lin - 1][col] == '#' || tab->grelha[lin][col + 1] == '#') return 0;
    }
    else if (lin == tab->linhas - 1 && col == tab->colunas - 1) {
        if (tab->grelha[lin - 1][col] == '#' || tab->grelha[lin][col - 1] == '#') return 0;
    }
    else if (lin == 0) {
        if (tab->grelha[lin + 1][col] == '#' || tab->grelha[lin][col - 1] == '#' || tab->grelha[lin][col + 1] == '#') return 0;
    }
    else if (lin == tab->linhas - 1) {
        if (tab->grelha[lin - 1][col] == '#' || tab->grelha[lin][col - 1] == '#' || tab->grelha[lin][col + 1] == '#') return 0;
    }
    else if (col == 0) {
        if (tab->grelha[lin - 1][col] == '#' || tab->grelha[lin + 1][col] == '#' || tab->grelha[lin][col + 1] == '#') return 0;
    }
    else if (col == tab->colunas - 1) {
        if (tab->grelha[lin - 1][col] == '#' || tab->grelha[lin + 1][col] == '#' || tab->grelha[lin][col - 1] == '#') return 0;
    }
    else {
        if (tab->grelha[lin - 1][col] == '#' || tab->grelha[lin + 1][col] == '#' || tab->grelha[lin][col - 1] == '#' || tab->grelha[lin][col + 1] == '#') return 0;
    }
    return 1; // Jogo válido
}

void riscar(Tabuleiro* tab, int lin, int col, int* verifica, Pilha* pilha) {
    if (lin >= 0 && lin < tab->linhas && col >= 0 && col < tab->colunas) {
        // Guarda o estado anterior da célula
        empurrarPilha(pilha, lin, col, tab->grelha[lin][col]);
        
        tab->grelha[lin][col] = '#';  // Marca a célula como riscada
        if (*verifica == 1) *verifica = verificarRisca(tab, lin, col);
    } else {
        printf("Posição inválida! Tente de novo.\n");
    }
}

void freeTabuleiro(Tabuleiro* tab) {
    for (int i = 0; i < tab->linhas; i++) {
        free(tab->grelha[i]);  // Libera cada linha
    }
    free(tab->grelha);  // Libera a matriz
    free(tab);  // Libera o tabuleiro
}

void guardar(Tabuleiro* tab, Pilha* pilha) {
    FILE* f = fopen("tabuleiro.txt", "w");
    if (!f) {
        perror("Erro ao abrir ficheiro para gravar");
        return;
    }

    // Escreve o tamanho do tabuleiro
    fprintf(f, "%d %d\n", tab->linhas, tab->colunas);

    // Escreve o estado atual do tabuleiro
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            fprintf(f, "%c", tab->grelha[i][j]);
        }
        fprintf(f, "\n");
    }

    // Escreve as jogadas feitas na pilha
    for (int i = 0; i <= pilha->topo; i++) {
        Jogada jogada = pilha->jogadas[i];
        char acao = (jogada.anterior == '#') ? 'r' : 'b';  // Se anterior for '#', foi riscado, senão foi branco
        fprintf(f, "%c %c %d\n", acao, 'a' + jogada.col, jogada.lin + 1);
    }

    fclose(f);
    printf("Estado do jogo salvo com sucesso!\n");
}

