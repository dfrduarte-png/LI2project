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

    remove("tabuleiro.txt");  // Remove o ficheiro de teste
}

void test_carregar_invalido(void) {
    Pilha pilha;
    inicializarPilha(&pilha, 10);
    
    // Create test file with INVALID dimensions
    FILE* file = fopen("tabinvalido.txt", "w");
    if (!file) {
        CU_FAIL("Failed to create test file");
        return;
    }
    
    // Write ONLY invalid dimensions (no board data needed)
    fprintf(file, "-5 6");  // Negative rows, non-square
    
    fclose(file);
    
    // Test loading
    Tabuleiro* tab = carregar("tabinvalido.txt", &pilha);
    CU_ASSERT_PTR_NULL(tab);  // Should return NULL
    
    // Cleanup
    remove("tabinvalido.txt");  // Delete test file
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
    empurrarPilha(&pilha, 0, 0, 'x', 'y');
    CU_ASSERT_EQUAL(pilha.topo, 0);
    CU_ASSERT_EQUAL(pilha.jogadas[0].lin, 0);
    CU_ASSERT_EQUAL(pilha.jogadas[0].col, 0);
    CU_ASSERT_EQUAL(pilha.jogadas[0].anterior, 'x');
    CU_ASSERT_EQUAL(pilha.jogadas[0].tipo, 'y');
    
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

void test_guardar(void){
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

    // Criar a pilha
    Pilha pilha;
    inicializarPilha(&pilha, 10);

    // Adicionar jogadas à pilha
    empurrarPilha(&pilha, 0, 0, 'a', 'b');
    empurrarPilha(&pilha, 1, 1, 'c', 'b');

    // Chamar a função a ser testada
    guardar(tab, &pilha, "output.txt");

    // Verificar se o arquivo foi criado corretamente
    FILE* f = fopen("output.txt", "r");
    CU_ASSERT_PTR_NOT_NULL(f);

    if (f) fclose(f);
    
    // Liberação de memória
    freeTabuleiro(tab);
    freePilha(&pilha);

    if (f) remove("output.txt"); // Limpar o arquivo de teste após o teste
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

    int resultado = verificarRisca(tab, 0, 0, tab->grelha[0][0]); // Pass the missing argument
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

    int resultado = verificarBranco(tab, 0, 0, /*tab->grelha[0][0]*/ 1); // Pass the missing argument
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

    int resultado = verifica(tab, 0); // Pass the missing argument
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
    CU_ASSERT_EQUAL(tab->grelha[0][0], 'A'); // ← Correto conforme lógica atual
    

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
    ajudar(tab, &pilha, &cont, 0); // 0 para não imprimir

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

    int resultado = verificaConectividade(tab, 1);
    CU_ASSERT_EQUAL(resultado, 0); // Deve retornar 0 (conectado)

    // Segundo cenário: todas casas brancas conectadas
    tab->grelha[0][1] = 'C';
    tab->grelha[1][1] = 'D';
    tab->grelha[2][1] = 'E';

    resultado = verificaConectividade(tab, 1); // Pass the missing argument
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



void test_verificaBranco2(void) {
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

    int resultado = verificaBranco2(tab); // Pass the missing argument
    CU_ASSERT_EQUAL(resultado, 1); // Deve retornar 1 pois não há brancos duplicados

    freeTabuleiro(tab);
}




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
    desfazer(tab, &pilha, 0); // Add the missing argument(s) as required by the function definition
    CU_ASSERT_EQUAL(tab->grelha[0][0], 'a');

    freeTabuleiro(tab);
    freePilha(&pilha);
}



void test_resolver(void){
    // Implementar o teste para a função resolver
    // Exemplo de implementação:
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

    // Chamar a função resolver
    resolver(tab, NULL, 0, 0); // Passar NULL para a pilha se não for necessário

    // Verificar o resultado esperado
    CU_ASSERT_EQUAL(tab->grelha[0][0], 'A'); // Exemplo de verificação
    CU_ASSERT_EQUAL(tab->grelha[0][1], 'B'); // Exemplo de verificação
    CU_ASSERT_EQUAL(tab->grelha[1][0], 'C'); // Exemplo de verificação
    CU_ASSERT_EQUAL(tab->grelha[1][1], 'D'); // Exemplo de verificação

    freeTabuleiro(tab);
}


void test_riscarduplicados(void) {
    // Implementar o teste para a função riscarDuplicados
    // Exemplo de implementação:
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

    // Inicializar a pilha
    Pilha pilha;
    inicializarPilha(&pilha, 10);

    // Chamar a função riscarDuplicados
    riscarDuplicados(tab, &pilha); // Passar os argumentos necessários

    // Liberar a memória da pilha
    freePilha(&pilha);

    // Verificar o resultado esperado
    CU_ASSERT_EQUAL(tab->grelha[0][1], 'B'); // Exemplo de verificação

    freeTabuleiro(tab);
}

void test_vizinhosbrancos(void) {
    // Implementar o teste para a função vizinhosBrancos
    // Exemplo de implementação:
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

    // Chamar a função vizinhosBrancos
    Pilha pilha;
    inicializarPilha(&pilha, 10); // Inicializar a pilha
    int resultado = vizinhosBrancos(tab, &pilha, 0, 0); // Passar os argumentos necessários
    freePilha(&pilha); // Liberar a memória da pilha

    // Verificar o resultado esperado
    CU_ASSERT_EQUAL(resultado, 1); // Exemplo de verificação

    freeTabuleiro(tab);
}

void test_carregar_tabuleiro_inexistente(void) {
    Tabuleiro* tab = carregar("nao_existe.txt", NULL);
    CU_ASSERT_PTR_NULL(tab);
}

void test_posicoes_extremas_tabuleiro(void) {
    Tabuleiro* tab = carregar("tab.txt", NULL);
    Pilha pilha;
    inicializarPilha(&pilha, 10);
    // Testar cantos do tabuleiro
    riscar(tab, 0, 0, &pilha);
    CU_ASSERT_EQUAL(tab->grelha[0][0], '#');
    riscar(tab, tab->linhas-1, tab->colunas-1, &pilha);
    CU_ASSERT_EQUAL(tab->grelha[tab->linhas-1][tab->colunas-1], '#');
    riscar(tab, 0, tab->colunas-1, &pilha);
    CU_ASSERT_EQUAL(tab->grelha[0][tab->colunas-1], '#');
    riscar(tab, tab->linhas-1, 0, &pilha);
    CU_ASSERT_EQUAL(tab->grelha[tab->linhas-1][0], '#');
    freeTabuleiro(tab);
    freePilha(&pilha);
}

void test_carregar_dados_corrompidos(void) {
    // Criar arquivo com dados inválidos
    FILE* f = fopen("corrupt.txt", "w");
    fputs("A B C\nX Y Z\n", f);
    fclose(f);
    
    Tabuleiro* tab = carregar("corrupt.txt", NULL);
    CU_ASSERT_PTR_NULL(tab);

    remove("corrupt.txt"); // Limpar arquivo de teste
}

static int malloc_counter = 0;
static int fail_at = 0;

void * __real_malloc(size_t size);
void   __real_free(void *ptr);

void * __wrap_malloc(size_t size) {
    malloc_counter++;
    if (malloc_counter == fail_at) {
        return NULL;
    }
    return __real_malloc(size);
}

void __wrap_free(void *ptr) {
    __real_free(ptr);
}

static const char *TMP_FILENAME = "tmp_tab.txt";

static void write_minimal_board(const char *fname) {
    FILE *f = fopen(fname, "w");
    if (!f) {
        fprintf(stderr, "Unable to create %s\n", fname);
        exit(1);
    }
    fprintf(f, "3 3\n");
    fprintf(f, "abc\n");
    fprintf(f, "def\n");
    fprintf(f, "ghi\n");
    fprintf(f, "--\n");
    fclose(f);
}

static void test_carregar_fail_tabuleiro(void) {
    write_minimal_board(TMP_FILENAME);

    Pilha pilha;
    inicializarPilha(&pilha, 10);

    malloc_counter = 0;
    fail_at = 1;

    Tabuleiro *t = carregar(TMP_FILENAME, &pilha);
    CU_ASSERT_PTR_NULL(t);

    freePilha(&pilha);
}

static void test_carregar_fail_grelha_ptr_array(void) {
    write_minimal_board(TMP_FILENAME);

    Pilha pilha;
    inicializarPilha(&pilha, 10);

    malloc_counter = 0;
    fail_at = 2;

    Tabuleiro *t = carregar(TMP_FILENAME, &pilha);
    CU_ASSERT_PTR_NULL(t);

    freePilha(&pilha);
}


static void test_carregar_fail_first_row(void) {
    write_minimal_board(TMP_FILENAME);

    Pilha pilha;
    inicializarPilha(&pilha, 10);

    malloc_counter = 0;
    fail_at = 3;

    Tabuleiro *t = carregar(TMP_FILENAME, &pilha);
    CU_ASSERT_PTR_NULL(t);

    freePilha(&pilha);
}


static void test_carregar_success_path(void) {
    write_minimal_board(TMP_FILENAME);

    Pilha pilha;
    inicializarPilha(&pilha, 10);

    malloc_counter = 0;
    fail_at = 0;

    Tabuleiro *t = carregar(TMP_FILENAME, &pilha);
    CU_ASSERT_PTR_NOT_NULL(t);

    if (t) {
        CU_ASSERT_EQUAL(t->linhas, 3);
        CU_ASSERT_EQUAL(t->colunas, 3);

        CU_ASSERT(t->grelha != NULL);
        if (t->grelha) {
            CU_ASSERT(t->grelha[0] != NULL);
            CU_ASSERT(t->grelha[1] != NULL);
            CU_ASSERT(t->grelha[2] != NULL);
            if (t->grelha[0] && t->grelha[1] && t->grelha[2]) {
                CU_ASSERT_EQUAL(t->grelha[0][0], 'a');
                CU_ASSERT_EQUAL(t->grelha[0][1], 'b');
                CU_ASSERT_EQUAL(t->grelha[0][2], 'c');
                CU_ASSERT_EQUAL(t->grelha[1][0], 'd');
                CU_ASSERT_EQUAL(t->grelha[1][1], 'e');
                CU_ASSERT_EQUAL(t->grelha[1][2], 'f');
                CU_ASSERT_EQUAL(t->grelha[2][0], 'g');
                CU_ASSERT_EQUAL(t->grelha[2][1], 'h');
                CU_ASSERT_EQUAL(t->grelha[2][2], 'i');
            }
        }

        freeTabuleiro(t);
    }

    freePilha(&pilha);
}

static void test_carregar_fail_read_incomplete_grid(void) {
    /* Create a file that ends in the middle of the grid read: */
    const char *fname = "tmp_tab_incomplete.txt";
    FILE *f = fopen(fname, "w");
    if (!f) {
        fprintf(stderr, "Unable to create %s\n", fname);
        exit(1);
    }
    /* Header says 3×3, but we only supply 2 characters total: */
    fprintf(f, "3 3\n");
    fprintf(f, "ab");   /* No newline, no more file data */
    fclose(f);

    Pilha pilha;
    inicializarPilha(&pilha, 10);

    /* Do not force any malloc to fail; we want the fscanf to fail instead. */
    malloc_counter = 0;
    fail_at = 0;

    Tabuleiro *t = carregar(fname, &pilha);
    CU_ASSERT_PTR_NULL(t);

    freePilha(&pilha);
    remove(fname);
}

static void test_carregar_with_moves_updates_grid_and_stack(void) {
    const char *fname = "tmp_tab_with_moves.txt";
    FILE *f = fopen(fname, "w");
    if (!f) {
        fprintf(stderr, "Unable to create %s\n", fname);
        exit(1);
    }
    fprintf(f, "3 3\n");
    fprintf(f, "abc\n");
    fprintf(f, "def\n");
    fprintf(f, "ghi\n");
    fprintf(f, "--\n");
    fprintf(f, "b a 1\n");
    fprintf(f, "r c 3\n");
    fclose(f);

    Pilha pilha;
    inicializarPilha(&pilha, 10);
    malloc_counter = 0;
    fail_at = 0;

    Tabuleiro *t = carregar(fname, &pilha);
    CU_ASSERT_PTR_NOT_NULL(t);
    if (t) {
        /* Check grid[0][0] became 'A' (branco on 'a',1) */
        CU_ASSERT_EQUAL(t->grelha[0][0], 'A');
        /* Check grid[2][2] became '#' (riscar on 'c',3) */
        CU_ASSERT_EQUAL(t->grelha[2][2], '#');
        freeTabuleiro(t);
    }

    freePilha(&pilha);
    remove(fname);
}

static void test_carregar_fail_second_row(void) {
    write_minimal_board(TMP_FILENAME);

    Pilha pilha;
    inicializarPilha(&pilha, 10);

    malloc_counter = 0;
    fail_at = 4;

    Tabuleiro *t = carregar(TMP_FILENAME, &pilha);
    CU_ASSERT_PTR_NULL(t);

    freePilha(&pilha);
}

void test_branco_pos_invalida(void) {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 1;
    tab->colunas = 1;
    tab->grelha = malloc(sizeof(char*));
    tab->grelha[0] = malloc(sizeof(char));
    tab->grelha[0][0] = 'a';

    Pilha pilha;
    inicializarPilha(&pilha, 10);

    // Testar branco em posição inválida
    branco(tab, -1, -1, &pilha); // Posição inválida
    CU_ASSERT_EQUAL(tab->grelha[0][0], 'a'); // Não deve alterar

    freeTabuleiro(tab);
    freePilha(&pilha);
}

void test_riscar_pos_invalida(void) {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 1;
    tab->colunas = 1;
    tab->grelha = malloc(sizeof(char*));
    tab->grelha[0] = malloc(sizeof(char));
    tab->grelha[0][0] = 'a';

    Pilha pilha;
    inicializarPilha(&pilha, 10);

    // Testar riscar em posição inválida
    riscar(tab, -1, -1, &pilha); // Posição inválida
    CU_ASSERT_EQUAL(tab->grelha[0][0], 'a'); // Não deve alterar

    freeTabuleiro(tab);
    freePilha(&pilha);
}

void test_verificarBranco_cercado(void) {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 3;
    tab->colunas = 3;
    tab->grelha = malloc(3 * sizeof(char*));
    for (int i = 0; i < 3; i++) {
        tab->grelha[i] = malloc(3 * sizeof(char));
        for (int j = 0; j < 3; j++) {
            tab->grelha[i][j] = '#'; // Preencher com letras brancas
        }
    }
    tab->grelha[1][1] = 'A'; // Casa branca (1,1)

    // Testar se a função verificaBranco retorna 1 para uma casa cercada
    int resultado = verificarBranco(tab, 1, 1, 0); // Casa (1,1) cercada por '#'
    CU_ASSERT_EQUAL(resultado, 1); // Deve retornar 1 pois a casa está cercada

    freeTabuleiro(tab);
}

void test_verificarBranco_iguais_horizontais(void) {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 3;
    tab->colunas = 3;
    tab->grelha = malloc(3 * sizeof(char*));
    for (int i = 0; i < 3; i++) {
        tab->grelha[i] = malloc(3 * sizeof(char));
        for (int j = 0; j < 3; j++) {
            tab->grelha[i][j] = 'b'; // Preencher com letras
        }
    }
    for (int j = 0; j < 3; j++) {
        tab->grelha[0][j] = 'A'; // Preencher com letras brancas
    }

    // Testar se a função verificaBranco retorna 1 para casas brancas iguais horizontais
    int resultado = verificarBranco(tab, 0, 0, 1); // Casa (0,0) cercada por 'A' horizontalmente
    CU_ASSERT_EQUAL(resultado, 1); // Deve retornar 1 pois a casa está cercada por casas brancas iguais

    freeTabuleiro(tab);
}

void test_verificarBranco_iguais_verticais(void) {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 3;
    tab->colunas = 3;
    tab->grelha = malloc(3 * sizeof(char*));
    for (int i = 0; i < 3; i++) {
        tab->grelha[i] = malloc(3 * sizeof(char));
        for (int j = 0; j < 3; j++) {
            tab->grelha[i][j] = 'b'; // Preencher com letras
        }
    }
    for (int j = 0; j < 3; j++) {
        tab->grelha[j][0] = 'A'; // Preencher com letras brancas
    }

    // Testar se a função verificaBranco retorna 1 para casas brancas iguais verticalmente
    int resultado = verificarBranco(tab, 0, 0, 1); // Casa (0,0) cercada por 'A' horizontalmente
    CU_ASSERT_EQUAL(resultado, 1); // Deve retornar 1 pois a casa está cercada por casas brancas iguais

    freeTabuleiro(tab);
}

void test_caminhoDesconectado(void) {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    tab->linhas = 3;
    tab->colunas = 3;
    tab->grelha = malloc(3 * sizeof(char*));
    for (int i = 0; i < 3; i++) {
        tab->grelha[i] = malloc(3 * sizeof(char));
        for (int j = 0; j < 3; j++) {
            tab->grelha[i][j] = 'b';
        }
    }

    // Primeiro cenário: casas brancas desconectadas
    tab->grelha[0][0] = 'A'; // Casa branca 1
    tab->grelha[1][2] = '#'; // Casa riscada
    tab->grelha[2][1] = '#'; // Casa riscada
    tab->grelha[2][2] = 'B'; // Casa branca 2, isolada

    int resultado = verificaConectividade(tab, 1);
    CU_ASSERT_EQUAL(resultado, 1); // Deve retornar 0 (conectado)

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
    CU_add_test(suite, "test_ajudar", test_ajudar);
    CU_add_test(suite,"test_resolver", test_resolver);
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
    CU_add_test(suite, "test_verificaBranco2", test_verificaBranco2);
    CU_add_test(suite, "test_riscarduplicados", test_riscarduplicados);
    CU_add_test(suite, "test_vizinhosbrancos", test_vizinhosbrancos);
    CU_add_test(suite, "test_carregar_tabuleiro_inexistente", test_carregar_tabuleiro_inexistente);
    CU_add_test(suite, "test_posicoes_extremas_tabuleiro", test_posicoes_extremas_tabuleiro);
    CU_add_test(suite, "test_carregar_dados_corrompidos", test_carregar_dados_corrompidos);
    CU_add_test(suite, "test_carregar_invalido", test_carregar_invalido);
    CU_add_test(suite, "test_carregar_fail_tabuleiro", test_carregar_fail_tabuleiro);
    CU_add_test(suite, "test_carregar_fail_grelha_ptr_array", test_carregar_fail_grelha_ptr_array);
    CU_add_test(suite, "test_carregar_fail_first_row", test_carregar_fail_first_row);
    CU_add_test(suite, "test_carregar_success_path", test_carregar_success_path);
    CU_add_test(suite, "test_carregar_fail_read_incomplete_grid", test_carregar_fail_read_incomplete_grid);
    CU_add_test(suite, "test_carregar_with_moves_updates_grid_and_stack", test_carregar_with_moves_updates_grid_and_stack);
    CU_add_test(suite, "test_carregar_fail_second_row", test_carregar_fail_second_row);
    CU_add_test(suite, "test_branco_pos_invalida", test_branco_pos_invalida);
    CU_add_test(suite, "test_riscar_pos_invalida", test_riscar_pos_invalida);
    CU_add_test(suite, "test_verificarBranco_cercado", test_verificarBranco_cercado);
    CU_add_test(suite, "test_verificarBranco_iguais_horizontais", test_verificarBranco_iguais_horizontais);
    CU_add_test(suite, "test_verificarBranco_iguais_verticais", test_verificarBranco_iguais_verticais);
    CU_add_test(suite, "test_caminhoDesconectado", test_caminhoDesconectado);


    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    
    unsigned int failures = CU_get_number_of_failures();
    CU_cleanup_registry();
    return (int)failures;  // Return 0 if all passed, >0 otherwise
}
