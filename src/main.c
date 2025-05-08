#include <stdio.h>
#include <stdlib.h>
#include "jogo.h"

int main() {
    Tabuleiro* tab = NULL;
    int lin;
    char acao, col;
    char ficheiro[25];

    // Inicializando a pilha para armazenar as jogadas
    Pilha* pilha = malloc(sizeof(Pilha));
    inicializarPilha(pilha, 10);

    while (1) {
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
            guardar (tab, pilha, ficheiro); 
        }
        else if (acao == 'l') {
            printf("Digite o nome do ficheiro: ");
            if (!scanf("%s", ficheiro)) {
                printf("Entrada inválida! Tente novamente.\n");
                while (getchar() != '\n');
                continue;
            }
            if (tab) {
                freeTabuleiro(tab);
            }
            Tabuleiro* new_tab = carregar(ficheiro, pilha);
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
            else if (!verifica(tab, 1) && !verificaConectividade(tab, 1)) printf("O jogo está válido!\n"); //se a função verifica == 0, o jogo está valido
        }
        else if (acao == 'a') {
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
                continue;
            }
            int cont = 0; // Só posto para não dar erro
            ajudar(tab, &pilha, &cont); // Implementar a função ajudar, o 0 não muda nada
        }
<<<<<<< HEAD
=======
        
        else if (acao == 'A') {
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
                continue;
            }
            int cont = 1;
            while (cont) {
                ajudar(tab, &pilha, &cont);
            }
        }
>>>>>>> main
        else if (acao == 'R') {// Implementar a função resolver
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
                continue;
            }
<<<<<<< HEAD
            resolver(tab, pilha);
=======
            resolver(tab, &pilha, 0, 0, 0);
>>>>>>> main
        }
        else if (acao == 'd') {
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
                continue;
            }
            desfazer(tab, pilha);  // Desfaz a última jogada
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
            int linha = lin - 1; // Converter linha para índice
            if (coluna < 0 || coluna >= tab->colunas || linha < 0 || linha >= tab->linhas) {
                printf("Posição inválida! Tente novamente.\n");
                continue;
            }
<<<<<<< HEAD
            else if (tab->grelha[lin - 1][coluna] >= 'A' && tab->grelha[lin - 1][coluna] <= 'Z') {
                printf("Posição já preenchida!\n");
            }
            else if (tab->grelha[lin - 1][coluna] == '#') {
=======
            else if (tab->grelha[linha][coluna] >= 'A' && tab->grelha[linha][coluna] <= 'Z') {
                printf("Posição já preenchida!\n");
            }
            else if (tab->grelha[linha][coluna] == '#') {
>>>>>>> main
                printf("Posição já riscada! Tente de novo.\n");
            }
            else {
                printf("\n");
<<<<<<< HEAD
                branco(tab, lin - 1, coluna, pilha);
=======
                branco(tab, lin - 1, coluna, &pilha);
>>>>>>> main
            }
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
            int linha = lin - 1; // Converter linha para índice
            if (tab->grelha[linha][coluna] >= 'A' && tab->grelha[linha][coluna] <= 'Z') {
                printf("Posição já preenchida! Tente de novo.\n");
            }
            else if (tab->grelha[linha][coluna] == '#') {
                printf("Posição já riscada!\n");
            }
<<<<<<< HEAD
            else if (coluna < 0 || coluna >= tab->colunas || linha < 0 || linha >= tab->linhas) {
=======
            else if (coluna < 0 || coluna >= tab->colunas || lin - 1 < 0 || lin - 1 >= tab->linhas) {
>>>>>>> main
                printf("Posição inválida! Tente novamente.\n");
            }
            else {
                printf("\n");
<<<<<<< HEAD
                riscar(tab, linha, coluna, pilha);
=======
                riscar(tab, linha, coluna, &pilha);
>>>>>>> main
            }
        }
        else if (acao == 'A') {
            if (!tab) {
                printf("Tabuleiro não carregado! Tente novamente.\n");
                continue;
            }
        }
        else printf("Ação inválida! Tente novamente.\n");
    }
    if (tab) {
        freeTabuleiro(tab);
    }
    freePilha(pilha);

    return 0;
}