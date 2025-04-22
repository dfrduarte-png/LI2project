#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jogo.h"

void criar_ficheiro_teste(const char* nome_ficheiro) {
    FILE* f = fopen(nome_ficheiro, "w");
    fprintf(f, "5 5\n");
    fprintf(f, "ecadc\n");
    fprintf(f, "dcdec\n");
    fprintf(f, "bddce\n");
    fprintf(f, "cdeeb\n");
    fprintf(f, "accbb\n");
    fclose(f);
}

void test_carregar(void) {
    criar_ficheiro_teste("tabuleiro.txt");
    Tabuleiro* tab = carregar("tabuleiro.txt");

    CU_ASSERT_PTR_NOT_NULL(tab);
    CU_ASSERT_EQUAL(tab->linhas, 5);
    CU_ASSERT_EQUAL(tab->colunas, 5);
    CU_ASSERT_EQUAL(tab->grelha[0][0], 'e');
    CU_ASSERT_EQUAL(tab->grelha[1][1], 'c');
    CU_ASSERT_EQUAL(tab->grelha[2][2], 'd');
    CU_ASSERT_EQUAL(tab->grelha[3][3], 'e');
    CU_ASSERT_EQUAL(tab->grelha[4][4], 'b');

    freeTabuleiro(tab);
}

void test_ler(void) {
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

    FILE* buffer = freopen("output.txt", "w", stdout);
    CU_ASSERT_PTR_NOT_NULL(buffer);
    ler(tab);
    fflush(stdout);
    
    // Verifica se a chamada para freopen foi bem-sucedida
    if (freopen("/dev/tty", "w", stdout) == NULL) {
        perror("Erro ao restaurar stdout");
    }

    FILE* f = fopen("output.txt", "r");
    CU_ASSERT_PTR_NOT_NULL(f);

    char linha[100];
    // Verifica se a leitura foi bem-sucedida
    if (fgets(linha, sizeof(linha), f) != NULL) {
        CU_ASSERT_STRING_EQUAL(linha, "x y \n");
    }
    if (fgets(linha, sizeof(linha), f) != NULL) {
        CU_ASSERT_STRING_EQUAL(linha, "z w \n");
    }

    fclose(f);
    freeTabuleiro(tab);
}

void test_branco(void) {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 1;
    tab->colunas = 1;
    tab->grelha = malloc(sizeof(char*));
    tab->grelha[0] = malloc(sizeof(char));
    tab->grelha[0][0] = 'a';

    branco(tab, 0, 0, 0);
    CU_ASSERT_EQUAL(tab->grelha[0][0], 'A');

    branco(tab, 1, 1, 0); // Posição inválida

    freeTabuleiro(tab);
}

void test_riscar(void) {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 1;
    tab->colunas = 1;
    tab->grelha = malloc(sizeof(char*));
    tab->grelha[0] = malloc(sizeof(char));
    tab->grelha[0][0] = 'a';

    riscar(tab, 0, 0, 0);
    CU_ASSERT_EQUAL(tab->grelha[0][0], '#');

    riscar(tab, 1, 1, 0); // Posição inválida

    freeTabuleiro(tab);
}

// Teste para a função ajudar
void test_ajuda(void) {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 5;
    tab->colunas = 5;
    tab->grelha = malloc(5 * sizeof(char*));
    for (int i = 0; i < 5; i++) {
        tab->grelha[i] = malloc(5 * sizeof(char));
        for (int j = 0; j < 5; j++) {
            tab->grelha[i][j] = 'a' + (i + j) % 26; // Preenche com letras
        }
    }
    

    int verifica = 1;
    ajudar(tab, &verifica);
    CU_ASSERT_EQUAL(verifica, 1);

    freeTabuleiro(tab);
}

int main() {
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Teste do jogo", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_carregar", test_carregar);
    CU_add_test(suite, "test_ler", test_ler);
    CU_add_test(suite, "test_branco", test_branco);
    CU_add_test(suite, "test_riscar", test_riscar);
    CU_add_test(suite, "test_ajuda", test_ajuda);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

