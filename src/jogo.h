#ifndef JOGO_H
#define JOGO_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
// Estrutura para guardar o tabuleiro e o seu tamanho
typedef struct {
    int linhas;
    int colunas;
    char** grelha;
} Tabuleiro;

typedef struct {
    int lin, col;
    char anterior; // letra anterior antes de mudar
    char tipo; // 'b' ou 'r'
} Jogada;

typedef struct {
    Jogada* jogadas;
    int topo;
    int capacidade;
} Pilha;

Tabuleiro* carregar(const char* ficheiro, Pilha* pilha);
void ler(Tabuleiro* tab);
void branco(Tabuleiro* tab, int lin, int col, Pilha* pilha);
void riscar(Tabuleiro* tab, int lin, int col, Pilha* pilha);
void freeTabuleiro(Tabuleiro* tab);
int verificarBranco(Tabuleiro* tab, int lin, int col);
int verificarRisca(Tabuleiro* tab, int lin, int col);
int verifica (Tabuleiro* tab);
void inicializarPilha(Pilha* pilha, int capacidade);
void empurrarPilha(Pilha* pilha, int lin, int col, char anterior, char tipo);
void freePilha(Pilha* pilha);
void guardar(Tabuleiro* tab, Pilha* pilha, const char* ficheiro);
void desfazer(Tabuleiro* tab, Pilha* pilha);
void ajudar(Tabuleiro* tab, Pilha* pilha, int *cont);
void resolver(Tabuleiro* tab, Pilha* pilha);
void redimensionarPilha(Pilha* pilha);
int verificaConectividade(Tabuleiro* tab);
void dfs(Tabuleiro* tab, int lin, int col, int visitado[tab->linhas][tab->colunas]);

#endif