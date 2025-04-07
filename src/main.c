#include <stdio.h>
#include <stdlib.h>
#include "jogo.h"

int main() {
    Tabuleiro* tab = carregar("tabuleiro.txt");
    if (!tab) return 1;

    int lin, verifica = 1; // Inicializa verifica como 1 (jogo válido)
    char acao, col;

    while (1) {
        printf("\nDigite ação (g = gravar, l = ler jogo, b = branca, r = riscar, v = verificar estado, a = ajuda, A = ???, R = cheater, d = desfazer, s = sair): ");
        if (!scanf(" %c", &acao)) {
            printf("Entrada inválida! Tente novamente.\n");
            while (getchar() != '\n'); // Limpar o buffer
            continue;
        }
        else if (acao == 's') break;
        else if (acao == 'g') ;// Implementar a função de gravar o estado do jogo
        else if (acao == 'l') ler(tab);
        else if (acao == 'v') {
            if (!verifica) { //verifica == 0
                printf("O jogo nao esta valido\n");
            } else {
                printf("O jogo esta válido\n");
            }
        }
        else if (acao == 'a') ;// Implementar a função de ajuda
        else if (acao == 'A') ;// Implementar a função A
        else if (acao == 'R') ;// Implementar a função R
        else if (acao == 'd') ;// Implementar a função desfazer
        else if (acao == 'b' || acao == 'r') {
            printf("Digite coluna (letra) e linha (numero): ");
            if (!scanf(" %c %d", &col, &lin)) {
                printf("Entrada inválida! Tente novamente.\n");
                while (getchar() != '\n'); // Limpar o buffer
                continue;
            }
            int coluna = col - 'a'; // Converter letra para índice

            if (acao == 'b')
                branco(tab, lin - 1, coluna, &verifica);
            else if (acao == 'r')
                riscar(tab, lin - 1, coluna, &verifica);
        } 
        else printf("Ação inválida! Tente novamente.\n");
    }

    freeTabuleiro(tab); 
    return 0;
}
