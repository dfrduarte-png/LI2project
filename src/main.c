#include <stdio.h>
#include <stdlib.h>
#include "jogo.h"

int main() {
    Tabuleiro* tab = NULL;
    int lin;
    char acao, col;
    char ficheiro[25];

    // Inicializando a pilha para armazenar as jogadas
    Pilha pilha;
    inicializarPilha(&pilha, 10);

    int ciclo = 1;
    while (ciclo) {
        if (tab) ler(tab);
        printf("\nDigite ação(g = gravar, l = ler, b = branca, r = riscar, v = verificar, a = ajuda, A = ajuda contínua, R = resolver, d = desfazer, s = sair): ");
        if (!scanf(" %c", &acao)) {
            printf("Entrada inválida! Tente novamente.\n");
            while (getchar() != '\n'); // Limpar o buffer
        }
        else if (acao == 's') ciclo = 0;
        else if (acao == 'g') {
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
            }
            else guardar (tab, &pilha, ficheiro); 
        }
        else if (acao == 'l') {
            printf("Digite o nome do ficheiro: ");
            if (!scanf("%s", ficheiro)) {
                printf("Entrada inválida! Tente novamente.\n");
                while (getchar() != '\n');
            }
            else {
                if (tab) {
                    freeTabuleiro(tab);
                }
                Tabuleiro* new_tab = carregar(ficheiro, &pilha);
                if (new_tab) tab = new_tab;
            }
        }
        else if (acao == 'v') {
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
            }
            else if (!verifica(tab, 1) && !verificaConectividade(tab, 1)) printf("O jogo está válido!\n"); //se a função verifica == 0, o jogo está valido
        }
        else if (acao == 'a') {
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
            }
            else {
                int cont = 0; // Só posto para não dar erro
                ajudar(tab, &pilha, &cont); // Implementar a função ajudar, o 0 não muda nada
            }
        }
        else if (acao == 'A') {
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
            }
            else {
                int cont = 1;
                while (cont) {
                    ajudar(tab, &pilha, &cont);
                }
            }
        }
        else if (acao == 'R') {// Implementar a função resolver
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
            }
            else {
                pilha.resolverConcluido = 0; // para saber quando devo desfazer um bloco ou nao na funcao desfazer
                resolver(tab, &pilha, 0, 0, 0);
                pilha.resolverConcluido = 1;
            }
        }
        else if (acao == 'd') {
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
            }
            else desfazer(tab, &pilha, 1);  // Desfaz a última jogada
        }
        else if (acao == 'b') {
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
            }
            else {
                printf("Digite coluna (letra) e linha (numero): ");
                if (!scanf(" %c %d", &col, &lin)) {
                    printf("Entrada inválida! Tente novamente.\n");
                    while (getchar() != '\n'); // Limpar o buffer
                }
                else {
                    int coluna = col - 'a'; // Converter letra para índice
                    int linha = lin - 1; // Converter linha para índice
                    if (coluna < 0 || coluna >= tab->colunas || linha < 0 || linha >= tab->linhas) {
                        printf("Posição inválida! Tente novamente.\n");
                    }
                    else {
                        if (tab->grelha[linha][coluna] >= 'A' && tab->grelha[linha][coluna] <= 'Z') {
                            printf("Posição já preenchida!\n");
                        }
                        else if (tab->grelha[linha][coluna] == '#') {
                            printf("Posição já riscada! Tente de novo.\n");
                        }
                        else {
                            printf("\n");
                            branco(tab, lin - 1, coluna, &pilha);
                        }
                    }
                }
            }
        } 
        else if (acao == 'r') {
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
            }
            else {
                printf("Digite coluna (letra) e linha (numero): ");
                if (!scanf(" %c %d", &col, &lin)) {
                    printf("Entrada inválida! Tente novamente.\n");
                    while (getchar() != '\n'); // Limpar o buffer
                }
                else {
                    int coluna = col - 'a'; // Converter letra para índice
                    int linha = lin - 1; // Converter linha para índice
                    if (tab->grelha[linha][coluna] >= 'A' && tab->grelha[linha][coluna] <= 'Z') {
                        printf("Posição já preenchida! Tente de novo.\n");
                    }
                    else if (tab->grelha[linha][coluna] == '#') {
                        printf("Posição já riscada!\n");
                    }
                    else if (coluna < 0 || coluna >= tab->colunas || lin - 1 < 0 || lin - 1 >= tab->linhas) {
                        printf("Posição inválida! Tente novamente.\n");
                    }
                    else {
                        printf("\n");
                        riscar(tab, linha, coluna, &pilha);
                    }
                }
            }
        }
        else printf("Ação inválida! Tente novamente.\n");
    }
    if (tab) {
        freeTabuleiro(tab);
    }
    freePilha(&pilha);

    return 0;
}