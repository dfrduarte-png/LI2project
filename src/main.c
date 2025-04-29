#include <stdio.h>
#include <stdlib.h>
#include "jogo.h"

int main() {
    Tabuleiro* tab = NULL;
    int lin;
    char acao, col;
    int ajuda_continua = 0;

    while (1) {

        if (tab) {
            if (ajuda_continua && verifica(tab) != 0) {
                printf("\nAjuda contínua aplicada:\n");
                ajudar(tab);
            }
            //ler(tab);
        }

        if (tab) ler(tab);
        printf("\nDigite ação(g = gravar, l = ler, b = branca, r = riscar, v = verificar, a = ajuda, A = ajuda contínua, R = resolver, d = desfazer, s = sair): ");
        if (!scanf(" %c", &acao)) {
            printf("Entrada inválida! Tente novamente.\n");
            while (getchar() != '\n'); // Limpar o buffer
            continue;
        }
        else if (acao == 's') break;
        else if (acao == 'g') {
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
                continue;
            }
        }// Implementar a função de gravar o estado do jogo
        else if (acao == 'l') {
            char ficheiro[25];
            printf("Digite o nome do ficheiro: ");
            if (!scanf("%s", ficheiro)) {
                printf("Entrada inválida! Tente novamente.\n");
                while (getchar() != '\n');
                continue;
            }
            if (tab) {
                freeTabuleiro(tab);
            }
            Tabuleiro* new_tab = carregar(ficheiro);
            if (!new_tab) {
                continue;
            }
            
            tab = new_tab;
        }
        else if (acao == 'v') {
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
                continue;
            }
            else if (!verifica(tab)) printf("O jogo está válido!\n"); //se a função verifica == 0, o jogo está valido
        }
        else if (acao == 'a') {
            ajudar(tab); // Implementar a função ajudar
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
                continue;
            }
        }
        
        else if (acao == 'A') {
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
                continue;
            }
            ajuda_continua = 1;
            printf("Ajuda contínua ativada.\n");
        }

        else if (acao == 'R') {// Implementar a função resolver
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
                continue;
            }
        }
        else if (acao == 'd') {// Implementar a função desfazer
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
                continue;
            }
        }
        else if (acao == 'b') {
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
                continue;
            }
            printf("Digite coluna (letra) e linha (numero): ");
            if (!scanf(" %c %d", &col, &lin)) {
                printf("Entrada inválida! Tente novamente.\n");
                while (getchar() != '\n'); // Limpar o buffer
                continue;
            }
            int coluna = col - 'a'; // Converter letra para índice
            if (coluna < 0 || coluna >= tab->colunas || lin - 1 < 0 || lin - 1 >= tab->linhas) {
                printf("Posição inválida! Tente novamente.\n");
                continue;
            }
            branco(tab, lin - 1, coluna);
        } 
        else if (acao == 'r') {
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
                continue;
            }
            printf("Digite coluna (letra) e linha (numero): ");
            if (!scanf(" %c %d", &col, &lin)) {
                printf("Entrada inválida! Tente novamente.\n");
                while (getchar() != '\n'); // Limpar o buffer
                continue;
            }
            int coluna = col - 'a'; // Converter letra para índice
            if (coluna < 0 || coluna >= tab->colunas || lin - 1 < 0 || lin - 1 >= tab->linhas) {
                printf("Posição inválida! Tente novamente.\n");
                continue;
            }
            riscar(tab, lin - 1, coluna);
        }
        else printf("Ação inválida! Tente novamente.\n");
    }
    if (tab) {
        freeTabuleiro(tab);
    }
    return 0;
}