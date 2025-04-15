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

void branco(Tabuleiro* tab, int lin, int col) { 
    if (tab->grelha[lin][col] >= 'A' && tab->grelha[lin][col] <= 'Z') {
        printf("Posição já preenchida!\n");
        return;
    }
    else if (tab->grelha[lin][col] == '#') {
        printf("Posição já riscada! Tente de novo.\n");
        return;
    }
    else if (lin >= 0 && lin < tab->linhas && col >= 0 && col < tab->colunas) {
        tab->grelha[lin][col] = toupper(tab->grelha[lin][col]);
        return;
    } else {
        printf("Posição inválida! Tente de novo.\n");
    }
}

void riscar(Tabuleiro* tab, int lin, int col) {
    if (tab->grelha[lin][col] >= 'A' && tab->grelha[lin][col] <= 'Z') {
        printf("Posição já preenchida! Tente de novo.\n");
        return;
    }
    else if (tab->grelha[lin][col] == '#') {
        printf("Posição já riscada!\n");
        return;
    }
    else if (lin >= 0 && lin < tab->linhas && col >= 0 && col < tab->colunas) {
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

int verificarBranco(Tabuleiro* tab, int lin, int col) { // acho que falta verificações e não está a fazer nada de momento
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

    // Check surrounding '#' conditions
    int up = (lin > 0) ? tab->grelha[lin - 1][col] == '#' : 0;
    int down = (lin < tab->linhas - 1) ? tab->grelha[lin + 1][col] == '#' : 0;
    int left = (col > 0) ? tab->grelha[lin][col - 1] == '#' : 0;
    int right = (col < tab->colunas - 1) ? tab->grelha[lin][col + 1] == '#' : 0;

    if (up && down && left && right) r = 1;
    return r;
    // return 0; // Se não encontrar nenhuma correspondência
    // return 1; // Se encontrar correspondência
}

int verificarRisca(Tabuleiro* tab, int lin, int col) { // acho que falta verificações
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