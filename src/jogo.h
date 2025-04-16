#ifndef JOGO_H
#define JOGO_H

// Estrutura para guardar o tabuleiro e o seu tamanho
typedef struct {
    int linhas;
    int colunas;
    char** grelha;
} Tabuleiro;

Tabuleiro* carregar(const char* ficheiro);
void ler(Tabuleiro* tab);
int verificarBranco(Tabuleiro* tab, int lin, int col);
void branco(Tabuleiro* tab, int lin, int col, int *verifica);
int verificarRisca(Tabuleiro* tab, int lin, int col);
void riscar(Tabuleiro* tab, int lin, int col, int *verifica);
void freeTabuleiro(Tabuleiro* tab);
void ajudar(Tabuleiro* tab, int *verifica);


#endif