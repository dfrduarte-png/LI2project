#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "jogo.h"

// Função para criar um arquivo de teste
void criar_ficheiro_teste(const char* nome_ficheiro) {
    FILE* f = fopen(nome_ficheiro, "w");
    fprintf(f, "3 3\n");
    fprintf(f, "a b c\n");
    fprintf(f, "d e f\n");
    fprintf(f, "g h i\n");
    fclose(f);
}

// Teste da função carregar
void testar_carregar() {
    criar_ficheiro_teste("tabuleiro.txt");
    Tabuleiro* tab = carregar("tabuleiro.txt");
    
    assert(tab != NULL);
    assert(tab->linhas == 3);
    assert(tab->colunas == 3);
    assert(tab->grelha[0][0] == 'a');
    assert(tab->grelha[1][1] == 'e');
    assert(tab->grelha[2][2] == 'i');

    freeTabuleiro(tab);
}

// Teste da função ler
void testar_ler() {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 2;
    tab->colunas = 2;
    tab->grelha = malloc(2 * sizeof(char*));
    tab->grelha[0] = malloc(2 * sizeof(char));
    tab->grelha[1] = malloc(2 * sizeof(char));
    tab->grelha[0][0] = 'x';
    tab->grelha[0][1] = 'y';
    tab->grelha[1][0] = 'z';
    tab->grelha[1][1] = 'w';

    // Redirecionar a saída para um buffer
    FILE* buffer = fopen("output.txt", "w");
    if (buffer) {
        // Substituir stdout temporariamente
        FILE* original_stdout = stdout;
        stdout = buffer;

        ler(tab);

        // Restaurar stdout
        stdout = original_stdout;
        fclose(buffer);
    }

    // Ler o conteúdo do arquivo de saída
    buffer = fopen("output.txt", "r");
    char linha[100];
    fgets(linha, sizeof(linha), buffer);
    assert(strcmp(linha, "x y \n") == 0);
    fgets(linha, sizeof(linha), buffer);
    assert(strcmp(linha, "z w \n") == 0);
    fclose(buffer);

    freeTabuleiro(tab);
}

// Teste da função branco
void testar_branco() {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 1;
    tab->colunas = 1;
    tab->grelha = malloc(sizeof(char*));
    tab->grelha[0] = malloc(sizeof(char));
    tab->grelha[0][0] = 'a';

    branco(tab, 0, 0, 0);
    assert(tab->grelha[0][0] == 'A'); // Verifica se a letra foi transformada em maiúscula

    branco(tab, 1, 1, 0); // Testa posição inválida

    freeTabuleiro(tab);
}

// Teste da função riscar
void testar_riscar() {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 1;
    tab->colunas = 1;
    tab->grelha = malloc(sizeof(char*));
    tab->grelha[0] = malloc(sizeof(char));
    tab->grelha[0][0] = 'a';

    // Testa a função riscar
    riscar(tab, 0, 0, 0);
    assert(tab->grelha[0][0] == '#'); // Verifica se o caractere foi riscado

    riscar(tab, 1, 1, 0); // Testa posição inválida

    freeTabuleiro(tab);
}

// Função principal para executar os testes
int main() {
    // Testes das funções
    testar_carregar();
    testar_ler();
    testar_branco();
    testar_riscar();

    printf("Todos os testes passaram!\n");
    return 0;
}
