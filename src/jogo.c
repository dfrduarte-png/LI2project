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

void branco(Tabuleiro* tab, int lin, int col, int *verifica) { // implementar a parte da verificação
    if (lin >= 0 && lin < tab->linhas && col >= 0 && col < tab->colunas) {
        if (tab->grelha[lin][col] == '#') {
            printf("Posição já riscada! Tente de novo.\n");
            return;
        }
        tab->grelha[lin][col] = toupper(tab->grelha[lin][col]);
        if (*verifica == 1) *verifica = verificarBranco(tab, lin, col); // So verifica se o jogo estava válido
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

void riscar(Tabuleiro* tab, int lin, int col, int *verifica) {
    if (lin >= 0 && lin < tab->linhas && col >= 0 && col < tab->colunas) {
        tab->grelha[lin][col] = '#';
        if (*verifica == 1) *verifica = verificarRisca(tab, lin, col); // So verifica se o jogo estava válido
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


void ajudar(Tabuleiro* tab, int *verifica) {
 // Primeiro, riscar todas as letras iguais na mesma linha e coluna
 for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            if (islower(tab->grelha[i][j])) { // Se a casa contém uma letra minúscula
                // Verifica se a letra pode ser riscada
                if (verificarRisca(tab, i, j)) {
                    riscar(tab, i, j, verifica);
                }
            }
        }
    }


 // Verificar se existem casas que podem ser pintadas de branco e pinta as de branco 
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            if (islower(tab->grelha[i][j])) { // Se a casa contém uma letra minúscula
                // Verifica se a letra pode ser pintada de branco
                if (verificarBranco(tab, i, j)) {
                    branco(tab, i, j, verifica);
                }
            }
        }
    }

    // Verifica se o jogo está válido
    if (*verifica == 1) {
        printf("O jogo está válido.\n");
    } else {
        printf("O jogo não está válido.\n");
    }
    // Imprime o tabuleiro atualizado
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            printf("%c ", tab->grelha[i][j]);
        }
        printf("\n");
    }

}

