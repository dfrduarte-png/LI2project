#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "jogo.h"

Tabuleiro* carregar(const char* ficheiro) {
    FILE* f = fopen(ficheiro, "r");
    printf("Carregando tabuleiro de %s...\n", ficheiro);
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

void branco(Tabuleiro* tab, int lin, int col, int verifica) { // implementar a parte da verificação
    if (lin >= 0 && lin < tab->linhas && col >= 0 && col < tab->colunas) {
        tab->grelha[lin][col] = toupper(tab->grelha[lin][col]);
    } else {
        printf("Posição inválida! Tente de novo.\n");
    }
}

void riscar(Tabuleiro* tab, int lin, int col, int verifica) { // implementar a parte da verificação
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
