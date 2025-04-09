#include <stdio.h>
#include <stdlib.h>
#include "jogo.h"

int main() {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    if (!tab) {
        printf("Erro ao alocar memória para o tabuleiro!\n");
        return 1;
    }
    int le = 0;

    int lin;
    char acao, col;

    while (1) {
        if (le) ler(tab);
        printf("\nDigite ação(g = gravar, l = ler, b = branca, r = riscar, v = verificar estado, a = ajuda, A = ajuda contínua, R = resolver, d = desfazer, s = sair): ");
        if (!scanf(" %c", &acao)) {
            printf("Entrada inválida! Tente novamente.\n");
            while (getchar() != '\n'); // Limpar o buffer
            continue;
        }
        else if (acao == 's') break;
        else if (acao == 'g') {
            if (!le) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
                continue;
            }
        }// Implementar a função de gravar o estado do jogo
        else if (acao == 'l') {
            char ficheiro[100];
            printf("Digite o nome do ficheiro: ");
            if (!scanf("%s", ficheiro)) {
                printf("Entrada inválida! Tente novamente.\n");
                while (getchar() != '\n'); // Limpar o buffer
                continue;
            }
            if (!(tab = carregar(ficheiro))) continue;
            le = 1;
        }
        else if (acao == 'v') {
            if (!le) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
                continue;
            }
            else if (verifica(tab)) {
                printf("Válido!\n");
            } else {
                printf("Inválido\n");
            }
        }
        else if (acao == 'a') {
            if (!le) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
                continue;
            }
        }// Implementar a função de ajuda
        else if (acao == 'A') {
            if (!le) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
                continue;
            }
        }// Implementar a função A
        else if (acao == 'R') {
            if (!le) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
                continue;
            }
        }// Implementar a função resolver
        else if (acao == 'd') {
            if (!le) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
                continue;
            }
        }// Implementar a função desfazer
        else if (acao == 'b') {
            if (!le) {
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
            branco(tab, lin - 1, coluna);
        } 
        else if (acao == 'r') {
            if (!le) {
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
            if (verificarRisca(tab, lin - 1, coluna))riscar(tab, lin - 1, coluna);
            else printf("Ação inválida! Tente novamente.\n");
        }
        else printf("Ação inválida! Tente novamente.\n");
    }

    if (tab) {
        freeTabuleiro(tab);
    }
    return 0;
}
