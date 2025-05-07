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

    // Inicializando a pilha
    Pilha pilha;
    inicializarPilha(&pilha, 10);

    // Carregando o tabuleiro
    Tabuleiro* tab = carregar("tabuleiro.txt", &pilha);

    // Verifique se o tabuleiro não é NULL
    if (tab == NULL) {
        CU_FAIL("Falha ao carregar o tabuleiro");
        freePilha(&pilha);  // Libera a pilha caso o carregamento falhe
        return;  // Interrompe o teste se o tabuleiro for NULL
    }

    // Realiza os asserts para validar os dados carregados
    CU_ASSERT_PTR_NOT_NULL(tab);
    CU_ASSERT_EQUAL(tab->linhas, 5);
    CU_ASSERT_EQUAL(tab->colunas, 5);
    CU_ASSERT_EQUAL(tab->grelha[0][0], 'e');
    CU_ASSERT_EQUAL(tab->grelha[1][1], 'c');
    CU_ASSERT_EQUAL(tab->grelha[2][2], 'd');
    CU_ASSERT_EQUAL(tab->grelha[3][3], 'e');
    CU_ASSERT_EQUAL(tab->grelha[4][4], 'b');

    // Libera a memória alocada para o tabuleiro e pilha
    freeTabuleiro(tab);
    freePilha(&pilha);
}


void test_inicializarPilha(void) {
    Pilha pilha;
    inicializarPilha(&pilha, 5);
    CU_ASSERT_PTR_NOT_NULL(pilha.jogadas);
    CU_ASSERT_EQUAL(pilha.topo, -1);
    CU_ASSERT_EQUAL(pilha.capacidade, 5);

    // Libera a memória alocada para a pilha
    freePilha(&pilha);
}


void test_freeTabuleiro(void) {
    // Aloca e inicializa o tabuleiro
    Tabuleiro* tab = (Tabuleiro*)malloc(sizeof(Tabuleiro));
    tab->linhas = 5;
    tab->colunas = 5;
    tab->grelha = (char**)malloc((size_t)tab->linhas * sizeof(char*));

    for (int i = 0; i < tab->linhas; i++) {
        tab->grelha[i] = (char*)malloc((size_t)tab->colunas * sizeof(char));
    }

    // Preenche a grelha com valores de exemplo
    tab->grelha[0][0] = 'A';
    tab->grelha[4][4] = 'B';

    // Libera o tabuleiro
    freeTabuleiro(tab);

}


void test_freePilha(void) {
    Pilha pilha;
    inicializarPilha(&pilha, 1);  // Aloca a pilha

    // Realize as operações necessárias no teste...
    
    // Verifique se a pilha está sendo limpa corretamente
    freePilha(&pilha); // Libere a memória da pilha
}



void test_redimensionarPilha(void) {
    Pilha pilha;
    inicializarPilha(&pilha, 2);
    pilha.topo = 1; // Simulando que a pilha está cheia

    redimensionarPilha(&pilha);
    CU_ASSERT_EQUAL(pilha.capacidade, 4); // Verifica se a capacidade foi dobrada

    freePilha(&pilha);
}

void test_empurrarPilha(void) {
    Pilha pilha;
    inicializarPilha(&pilha, 2);

    empurrarPilha(&pilha, 0, 0, 'a', 'b');
    CU_ASSERT_EQUAL(pilha.topo, 0);
    CU_ASSERT_EQUAL(pilha.jogadas[0].lin, 0);
    CU_ASSERT_EQUAL(pilha.jogadas[0].col, 0);
    CU_ASSERT_EQUAL(pilha.jogadas[0].anterior, 'a');
    CU_ASSERT_EQUAL(pilha.jogadas[0].tipo, 'b');

    freePilha(&pilha);
}


void test_guardar(void) {
    Pilha pilha;
    inicializarPilha(&pilha, 10);

    // Realizar operações no jogo aqui...

    // Após o uso, liberar a memória
    freePilha(&pilha);
}



void test_verificarRisca(void) {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 2;
    tab->colunas = 2;
    tab->grelha = malloc(2 * sizeof(char*));
    tab->grelha[0] = malloc(2 * sizeof(char));
    tab->grelha[1] = malloc(2 * sizeof(char));
    tab->grelha[0][0] = '#';
    tab->grelha[0][1] = 'a';
    tab->grelha[1][0] = 'b';
    tab->grelha[1][1] = 'c';

    int resultado = verificarRisca(tab, 0, 0);
    CU_ASSERT_EQUAL(resultado, 1); // Deve retornar 1 pois a casa (0,0) é riscada

    freeTabuleiro(tab);
}

void test_verificaBranco(void){
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 2;
    tab->colunas = 2;
    tab->grelha = malloc(2 * sizeof(char*));
    tab->grelha[0] = malloc(2 * sizeof(char));
    tab->grelha[1] = malloc(2 * sizeof(char));
    tab->grelha[0][0] = 'A';
    tab->grelha[0][1] = 'b';
    tab->grelha[1][0] = 'c';
    tab->grelha[1][1] = 'd';

    int resultado = verificarBranco(tab, 0, 0);
    CU_ASSERT_EQUAL(resultado,0); // Deve retornar 0 pois a casa (0,0)   é branca

    freeTabuleiro(tab);
}


void test_verifica(void){
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 2;
    tab->colunas = 2;
    tab->grelha = malloc(2 * sizeof(char*));
    tab->grelha[0] = malloc(2 * sizeof(char));
    tab->grelha[1] = malloc(2 * sizeof(char));
    tab->grelha[0][0] = 'A';
    tab->grelha[0][1] = 'B';
    tab->grelha[1][0] = 'C';
    tab->grelha[1][1] = 'D';

    int resultado = verifica(tab);
    CU_ASSERT_EQUAL(resultado, 0); // Deve retornar 0 pois o tabuleiro está correto

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

    if (freopen("/dev/tty", "w", stdout) == NULL) {
        perror("freopen failed");
        exit(EXIT_FAILURE);
    }

    FILE* f = fopen("output.txt", "r");
    CU_ASSERT_PTR_NOT_NULL(f);

    char linha[100];
    CU_ASSERT_PTR_NOT_NULL(fgets(linha, sizeof(linha), f));
    CU_ASSERT_STRING_EQUAL(linha, "    a b \n");

    CU_ASSERT_PTR_NOT_NULL(fgets(linha, sizeof(linha), f));
    CU_ASSERT_STRING_EQUAL(linha, "    ---\n");

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
    CU_ASSERT_EQUAL(tab->grelha[0][0], '#'); // Corrigido: agora espera '#'

    freeTabuleiro(tab);
    freePilha(&pilha);
}


void test_ajudar(void) {
    // Setup: Criar o tabuleiro com 5x5
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 5;
    tab->colunas = 5;
    tab->grelha = malloc(5 * sizeof(char*));
    for (int i = 0; i < 5; i++) {
        tab->grelha[i] = malloc(5 * sizeof(char));
        for (int j = 0; j < 5; j++) {
            tab->grelha[i][j] = (char)('a' + (i + j) % 26); // Preencher com letras aleatórias
        }
    }

    // Criar algumas condições específicas
    tab->grelha[0][0] = 'A'; // Letra branca
    tab->grelha[1][0] = 'a'; // Letra minúscula que deve ser riscada
    tab->grelha[0][1] = 'B'; // Letra que será pintada de branco
    tab->grelha[2][2] = 'C'; // Outra letra maiúscula
    tab->grelha[3][3] = 'c'; // Outra letra minúscula
    tab->grelha[4][4] = '#'; // Casa riscada que deve pintar vizinhos

    // Testar casas vazias adjacentes a duas ou mais letras brancas
    tab->grelha[1][1] = 'x'; // ou qualquer letra minúscula
    tab->grelha[2][1] = 'A'; // Letra branca adjacente
    tab->grelha[1][2] = 'A'; // Letra branca adjacente
    tab->grelha[4][3] = 'a'; // ← necessário para que a função branco pinte de branco


    // Inicializar pilha e contador
    Pilha pilha;
    inicializarPilha(&pilha, 10);
    int cont = 0;

    // Chamar a função a ser testada
    ajudar(tab, &pilha, &cont);

    // Teste 1: Verificar se a letra minúscula foi riscada
    CU_ASSERT_EQUAL(tab->grelha[1][0], '#'); // Letra 'a' deve ser riscada

    // Teste 3: Verificar se a casa vizinha foi pintada de branco após a riscação
    CU_ASSERT_EQUAL(tab->grelha[0][1], 'B'); // A casa vizinha à 'A' deve ser pintada de branco

    // Teste 4: Verificar se as casas vizinhas a uma casa riscada (#) foram pintadas de branco
    CU_ASSERT_EQUAL(tab->grelha[4][3], 'A'); // A casa acima de '#' (4,3) deve ser branca (A)

    // Teste 5: Verificar se o contador foi incrementado
    CU_ASSERT_EQUAL(cont, 1); // O contador deve ser 1 pois houve modificações

    // Liberação de memória
    freeTabuleiro(tab);
    freePilha(&pilha);
}




void test_verificaConectividade(void) {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 3;
    tab->colunas = 3;
    tab->grelha = malloc(3 * sizeof(char*));
    for (int i = 0; i < 3; i++) {
        tab->grelha[i] = malloc(3 * sizeof(char));
        for (int j = 0; j < 3; j++) {
            tab->grelha[i][j] = '.'; // Espaços vazios
        }
    }

    // Primeiro cenário: casas brancas desconectadas
    tab->grelha[0][0] = 'A'; // Casa branca 1
    tab->grelha[2][2] = 'B'; // Casa branca 2, isolada

    int resultado = verificaConectividade(tab);
    CU_ASSERT_EQUAL(resultado, 1); // Deve retornar 1 (desconectado)

    // Segundo cenário: todas casas brancas conectadas
    tab->grelha[0][1] = 'C';
    tab->grelha[1][1] = 'D';
    tab->grelha[2][1] = 'E';

    resultado = verificaConectividade(tab);
    CU_ASSERT_EQUAL(resultado, 0); // Deve retornar 0 (todas conectadas)

    freeTabuleiro(tab);
}


void test_dfs(void) {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 3;
    tab->colunas = 3;
    tab->grelha = malloc(3 * sizeof(char*));
    for (int i = 0; i < 3; i++) {
        tab->grelha[i] = malloc(3 * sizeof(char));
        for (int j = 0; j < 3; j++) {
            tab->grelha[i][j] = 'A'; // Preencher com letras brancas
        }
    }

    int visitado[3][3] = {0}; // Inicializa a matriz de visitados

    dfs(tab, 0, 0, visitado); // Chama a função DFS

    // Verifica se todas as casas foram visitadas
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            CU_ASSERT_EQUAL(visitado[i][j], 1); // Todas as casas devem ser visitadas
        }
    }

    // Liberação de memória
    freeTabuleiro(tab);
}
/*
void test_resolver(void) {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 5;
    tab->colunas = 5;
    tab->grelha = malloc((size_t)tab->linhas * sizeof(char*));
    for (int i = 0; i < tab->linhas; i++) {
        tab->grelha[i] = malloc((size_t)tab->colunas * sizeof(char));
        for (int j = 0; j < tab->colunas; j++) {
            tab->grelha[i][j] = (char)('a' + (rand() % 26)); // Gerar letras aleatórias entre 'a' e 'z'
        }
    }
    

    // Inicializar pilha
    Pilha pilha;
    inicializarPilha(&pilha, 10);

    // Chama a função a ser testada
    resolver(tab, &pilha);

    // Testes: Verificar o estado final do tabuleiro após resolver
    // 1. Verifica se não há mais letras minúsculas no tabuleiro (deveriam ser riscadas ou pintadas)
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            // A função deve riscar ou pintar as letras minúsculas. Não deve haver letras minúsculas ao final
            if (tab->grelha[i][j] >= 'a' && tab->grelha[i][j] <= 'z') {
                CU_FAIL("O tabuleiro contém letras minúsculas após a resolução.");
            }
        }
    }

    // 2. Verifica se todas as letras minúsculas adjacentes a uma letra riscada (#) foram pintadas de branco (A-Z)
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            if (tab->grelha[i][j] == '#') {
                int direcoes[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // cima, baixo, esquerda, direita
                for (int k = 0; k < 4; k++) {
                    int newLin = i + direcoes[k][0];
                    int newCol = j + direcoes[k][1];
                
                    if (newLin >= 0 && newLin < tab->linhas && newCol >= 0 && newCol < tab->colunas) {
                        // Verifica se uma letra adjacente a '#' (riscada) foi pintada de branco
                        if (tab->grelha[newLin][newCol] >= 'a' && tab->grelha[newLin][newCol] <= 'z') {
                            CU_ASSERT_EQUAL(tab->grelha[newLin][newCol], '#');
                        }
                    }
                }
            }
        }
    }

    // 3. Verifica se as letras únicas foram corretamente identificadas (riscar ou pintar)
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            char c = tab->grelha[i][j];
            if (c >= 'a' && c <= 'z') {
                // Se a letra é única, deveria ter sido riscada
                CU_ASSERT_EQUAL(tab->grelha[i][j], '#');
            }
        }
    }

    // Libera memória
    freeTabuleiro(tab);
    freePilha(&pilha);
}
*/

void test_desfazer(void){
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 1;
    tab->colunas = 1;
    tab->grelha = malloc(sizeof(char*));
    tab->grelha[0] = malloc(sizeof(char));
    tab->grelha[0][0] = 'a';

    Pilha pilha;
    inicializarPilha(&pilha, 10);

    empurrarPilha(&pilha, 0, 0, 'a', 'b');
    desfazer(tab, &pilha);
    CU_ASSERT_EQUAL(tab->grelha[0][0], 'a');

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
    // CU_add_test(suite,"test_resolver", test_resolver);
    CU_add_test(suite,"test_desfazer", test_desfazer);
    CU_add_test(suite, "test_freeTabuleiro", test_freeTabuleiro);
    CU_add_test(suite, "test_freePilha", test_freePilha);
    CU_add_test(suite, "test_empurrarPilha", test_empurrarPilha);
    CU_add_test(suite, "test_redimensionarPilha", test_redimensionarPilha);
    CU_add_test(suite, "test_guardar", test_guardar);
    CU_add_test(suite, "test_verificarRisca", test_verificarRisca);
    CU_add_test(suite, "test_verificaBranco", test_verificaBranco);
    CU_add_test(suite, "test_verifica", test_verifica);
    CU_add_test(suite, "test_inicializarPilha", test_inicializarPilha);
    CU_add_test(suite, "test_verificaConectividade", test_verificaConectividade);
    CU_add_test(suite, "test_dfs", test_dfs);


    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
