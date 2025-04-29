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
    fprintf(f, "--\n");
    fprintf(f, "b a 1\n");
    fprintf(f, "r b 2\n");
    fclose(f);
}

void test_carregar(void) {
    criar_ficheiro_teste("tabuleiro.txt");
    Pilha pilha;
    inicializarPilha(&pilha, 10);
    Tabuleiro* tab = carregar("tabuleiro.txt", &pilha);

    CU_ASSERT_PTR_NOT_NULL(tab);
    CU_ASSERT_EQUAL(tab->linhas, 5);
    CU_ASSERT_EQUAL(tab->colunas, 5);
    CU_ASSERT_EQUAL(tab->grelha[0][0], 'E'); // Verifica se a jogada foi aplicada
    CU_ASSERT_EQUAL(tab->grelha[1][1], '#'); // Verifica se a jogada foi aplicada
    CU_ASSERT_EQUAL(tab->grelha[2][2], 'd');
    CU_ASSERT_EQUAL(tab->grelha[3][3], 'e');
    CU_ASSERT_EQUAL(tab->grelha[4][4], 'b');

    freeTabuleiro(tab);
    freePilha(&pilha);
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

    freopen("/dev/tty", "w", stdout);

    FILE* f = fopen("output.txt", "r");
    CU_ASSERT_PTR_NOT_NULL(f);

    char linha[100];
    CU_ASSERT_PTR_NOT_NULL(fgets(linha, sizeof(linha), f));
    CU_ASSERT_STRING_EQUAL(linha, "    a b \n");

    CU_ASSERT_PTR_NOT_NULL(fgets(linha, sizeof(linha), f));
    CU_ASSERT_STRING_EQUAL(linha, "    --\n");

    CU_ASSERT_PTR_NOT_NULL(fgets(linha, sizeof(linha), f));
    CU_ASSERT_STRING_EQUAL(linha, " 1| x y \n");

    CU_ASSERT_PTR_NOT_NULL(fgets(linha, sizeof(linha), f));
    CU_ASSERT_STRING_EQUAL(linha, " 2| z w \n");

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

    Pilha pilha;
    inicializarPilha(&pilha, 10);

    branco(tab, 0, 0, &pilha);
    CU_ASSERT_EQUAL(tab->grelha[0][0], 'A');

    tab->grelha[0][0] = '#';
    branco(tab, 0, 0, &pilha);
    CU_ASSERT_EQUAL(tab->grelha[0][0], '#'); // Não deve alterar

    freeTabuleiro(tab);
    freePilha(&pilha);
}

void test_riscar(void) {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 1;
    tab->colunas = 1;
    tab->grelha = malloc(sizeof(char*));
    tab->grelha[0] = malloc(sizeof(char));
    tab->grelha[0][0] = 'a';

    Pilha pilha;
    inicializarPilha(&pilha, 10);

    riscar(tab, 0, 0, &pilha);
    CU_ASSERT_EQUAL(tab->grelha[0][0], '#');

    tab->grelha[0][0] = 'A';
    riscar(tab, 0, 0, &pilha);
    CU_ASSERT_EQUAL(tab->grelha[0][0], 'A'); // Não deve alterar

    freeTabuleiro(tab);
    freePilha(&pilha);
}

void test_ajudar(void) {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 5;
    tab->colunas = 5;
    tab->grelha = malloc(5 * sizeof(char*));
    for (int i = 0; i < 5; i++) {
        tab->grelha[i] = malloc(5 * sizeof(char));
        for (int j = 0; j < 5; j++) {
            tab->grelha[i][j] = 'a' + (i + j) % 26;
        }
    }
    tab->grelha[0][0] = 'A'; // Letra branca
    tab->grelha[1][0] = 'a'; // Letra igual à branca

    Pilha pilha;
    inicializarPilha(&pilha, 10);

    ajudar(tab, &pilha);

    CU_ASSERT_EQUAL(tab->grelha[1][0], '#'); // Deve ser riscada
    CU_ASSERT_EQUAL(tab->grelha[0][1], 'B'); // Deve ser pintada de branco

    freeTabuleiro(tab);
    freePilha(&pilha);
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
    CU_add_test(suite, "test_ajudar", test_ajudar);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
