#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "jogo.h"

Tabuleiro* carregar(const char* ficheiro) {
    FILE* f = fopen(ficheiro, "r");
    printf("A carregar o tabuleiro em %s...\n", ficheiro);
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

    char c;
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            do {
                c = fgetc(f);
            } while (c == ' ' || c == '\n');
            tab->grelha[i][j] = c;
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

void branco(Tabuleiro* tab, int lin, int col) { 
    if (lin >= 0 && lin < tab->linhas && col >= 0 && col < tab->colunas) {
        if (tab->grelha[lin][col] == '#') {
            printf("Posição já riscada! Tente de novo.\n");
            return;
        }
        tab->grelha[lin][col] = toupper(tab->grelha[lin][col]);
    } else {
        printf("Posição inválida! Tente de novo.\n");
    }
}

void riscar(Tabuleiro* tab, int lin, int col) {
    if (lin >= 0 && lin < tab->linhas && col >= 0 && col < tab->colunas) {
        tab->grelha[lin][col] = '#';
    } else {
        printf("Posição inválida! Tente de novo.\n");
    }
}

void freeTabuleiro(Tabuleiro* tab) {
    for (int i = 0; i < tab->linhas; i++)
        free(tab->grelha[i]);
    free(tab->grelha);
    free(tab);
}

int verificarBranco(Tabuleiro* tab, int lin, int col) {
    char current = tab->grelha[lin][col];

    // Check vertically (up and down)
    for (int i = 0; i < tab->linhas; i++) {
        if (i != lin && tab->grelha[i][col] == current) return 0;
    }

    // Check horizontally (left and right)
    for (int j = 0; j < tab->colunas; j++) {
        if (j != col && tab->grelha[lin][j] == current) return 0;
    }

    // Check surrounding '#' conditions
    int up = (lin > 0) ? tab->grelha[lin - 1][col] == '#' : 0;
    int down = (lin < tab->linhas - 1) ? tab->grelha[lin + 1][col] == '#' : 0;
    int left = (col > 0) ? tab->grelha[lin][col - 1] == '#' : 0;
    int right = (col < tab->colunas - 1) ? tab->grelha[lin][col + 1] == '#' : 0;

    if (up && down && left && right) return 0;

    return 1; // Valid move
}

int verificarRisca(Tabuleiro* tab, int lin, int col) {
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Up, Down, Left, Right

    for (int i = 0; i < 4; i++) {
        int newLin = lin + directions[i][0];
        int newCol = col + directions[i][1];

        if (newLin >= 0 && newLin < tab->linhas && newCol >= 0 && newCol < tab->colunas) {
            if (tab->grelha[newLin][newCol] < 'A' || tab->grelha[newLin][newCol] > 'Z') return 0;
        }
    }
    return 1; // Jogo válido
}

int verifica (Tabuleiro* tab) {
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            if (tab->grelha[i][j] == '#') {
                if (!verificarRisca(tab, i, j)) {
                    return 0;
                }
            }
            else if (tab->grelha[i][j] >= 'A' && tab->grelha[i][j] <= 'Z') {
                if (!verificarBranco(tab, i, j)) {
                    return 0;
                }
            }
        }
    }
    return 1;
}