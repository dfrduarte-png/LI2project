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
// Riscar todas as letras iguais a uma letra branca na mesma linha e/ou coluna
for (int i = 0; i < tab->linhas; i++) {
    for (int j = 0; j < tab->colunas; j++) {
        if (isupper(tab->grelha[i][j])) { // Se a casa contém uma letra branca
            char letra = tab->grelha[i][j];
            // Riscar todas as letras iguais na mesma linha
            for (int k = 0; k < tab->colunas; k++) {
                if (tab->grelha[i][k] == letra) {
                    riscar(tab, i, k, verifica);
                }
            }
            // Riscar todas as letras iguais na mesma coluna
            for (int k = 0; k < tab->linhas; k++) {
                if (tab->grelha[k][j] == letra) {
                    riscar(tab, k, j, verifica);
                }
            }
        }
    }
}

// Pintar de branco todas as casas vizinhas de uma casa riscada
for (int i = 0; i < tab->linhas; i++) {
    for (int j = 0; j < tab->colunas; j++) {
        if (tab->grelha[i][j] == '#') { // Se a casa está riscada
            // Pintar de branco as casas vizinhas
            branco(tab, i - 1, j, verifica); // Acima
            branco(tab, i + 1, j, verifica); // Abaixo
            branco(tab, i, j - 1, verifica); // Esquerda
            branco(tab, i, j + 1, verifica); // Direita
        }
    }
}

// Pintar de branco uma casa quando seria impossível que esta fosse riscada por isolar casas brancas
for (int i = 0; i < tab->linhas; i++) {
    for (int j = 0; j < tab->colunas; j++) {
        if (islower(tab->grelha[i][j])) { // Se a casa contém uma letra minúscula
            if (!verificarRisca(tab, i, j)) { // Se seria impossível riscar esta casa
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

