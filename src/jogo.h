#ifndef JOGO_H
#define JOGO_H

typedef struct {
    int linhas;
    int colunas;
    char** grelha;
} Tabuleiro;

typedef struct {
    int lin;
    int col;
    char anterior;  // O estado anterior da célula (pode ser '#', 'B', ou 'R')
} Jogada;

typedef struct {
    Jogada* jogadas;
    int topo;
    int capacidade;
} Pilha;

Tabuleiro* carregar(const char* ficheiro);
void ler(Tabuleiro* tab);
int verificarBranco(Tabuleiro* tab, int lin, int col);
void branco(Tabuleiro* tab, int lin, int col, int *verifica, Pilha* pilha);
int verificarRisca(Tabuleiro* tab, int lin, int col);
void riscar(Tabuleiro* tab, int lin, int col, int *verifica, Pilha* pilha);
void desfazer(Tabuleiro* tab, Pilha* pilha, int* verifica);
void freeTabuleiro(Tabuleiro* tab);
void inicializarPilha(Pilha* pilha, int capacidade);
void liberarPilha(Pilha* pilha);

#endif
