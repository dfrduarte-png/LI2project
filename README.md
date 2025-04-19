# Projeto de LI2

## Descrição

Este projeto é um jogo implementado em C, onde o jogador interage com um tabuleiro de dimensões definidas num ficheiro `.txt`.

As regras desse puzzle são as seguintes:

* Cada casa contém um símbolo (uma letra inicialmente minúscula);
* Em cada linha e coluna só pode existir uma única réplica de cada símbolo que é pintada a branco (coloca-se a letra em maiúsculas);
* Todas as outras réplicas desse símbolo têm que ser riscadas (substituídas por um cardinal);
* Se uma casa está riscada, todas as casas vizinhas ortogonais têm que estar pintadas a branco;
* É necessário existir um caminho ortogonal entre todas as casas brancas do tabuleiro.

## Instalação

1. Clona o repositório ou copia o código-fonte para o teu computador.
2. Garante que tens o compilador `gcc` instalado e o utilitário `make`.

## Como executar

1. Abre o terminal na pasta do projeto.
2. Compila o código com o comando:`make`
3. Executa o jogo com:`./jogo`
4. Certifica-te que tens um ficheiro `.txt` com a definição do tabuleiro na mesma pasta, ou modifica o código para apontar para o caminho correto.

## Exemplo de ficheiro de tabuleiro

```
5 5
e c a d c
d c d e c
b d d c e
c d e e b
a c c b b
```

E da sua solução:

```
E # A D C
D C # E #
B # D C E
C D E # B
A # C B #
```

## Desenvolvidores

@dfrduarte-png

@beatriZzz24

@zemiguellopes

## Ilustração das regras

Se olharmos para o canto inferior direito do tabuleiro, temos três B colados. Como dois B não podem estar na mesma linha ou coluna, sabemos que o canto teria que ser um dos seguintes casos:

```
e c a d c    e c a d c
d c d e c    d c d e c
b d d c e    b d d c e
c d e e #    c d e e B
a c c # B    a c c B #
```

Destes dois casos, só o segundo é válido porque no primeiro caso o B estaria isolado de todas as letras pintadas a branco (i.e., maiúsculas) do resto do tabuleiro. Logo ficamos com o seguinte:

```
e c a d c
d c d e c
b d d c e
c d e e B
a c c B #
```

Na quarta coluna temos as letras `e c e`. Se riscarmos a letra c que está no meio teríamos:

```
e c a d c
d c d e c
b d d # e
c d e e B
a c c B #
```

Pela regra das casas riscadas temos:

```
e c a d c
d c d E c
b d D # E
c d e E B
a c c B #
```

Mas isso faz com que ficam dois símbolos iguais, E, na mesma coluna, o que não é possível! Logo, o c tem que ser pintado de branco:

```
e c a d c
d c d e c
b d d C e
c d e e B
a c c B #
```

Temos um outro caso na segunda coluna, que contém três símbolos c, estando dois deles juntos. Logo, se o símbolo c isolado fosse pintado de branco:

```
e c a d c
d c d e c
b d d C e
c d e e B
a C c B #
```

Então, pela regra das peças brancas:

```
e # a d c
d # d e c
b d d C e
c d e e B
a C c B #
```

O que não é possível porque duas casas vizinhas ortogonais ficariam riscadas. Logo a peça isolada tem que ser riscada!

```
e c a d c
d c d e c
b d d C e
c d e e B
a # c B #
```

Pela regra das casas riscadas, as vizinhas teriam que ser brancas:

```
e c a d c
d c d e c
b d d C e
c D e e B
A # C B #
```

E seguidamente, pela regra das brancas:

```
e c a d c
d c d e c
b # d C e
c D e e B
A # C B #
```

E pela das riscadas:

```
e c a d c
d C d e c
B # D C e
c D e e B
A # C B #
```

E a seguir pela regra das brancas:

```
e # a d c
d C # e #
B # D C e
c D e e B
A # C B #
```

E pela regra das riscadas:

```
E # A d C
d C # E #
B # D C E
c D e e B
A # C B #
```

E pela das brancas:

```
E # A d C
d C # E #
B # D C E
c D e # B
A # C B #
```

E pela regra das riscadas:

```
E # A d C
d C # E #
B # D C E
c D E # B
A # C B #
```

As letras que sobram seguem todas a regra de que todas as peças brancas têm que ser atingíveis através de um caminho ortogonal de peças brancas.

* O d na primeira linha tem forçosamente que ser branco porque caso contrário o C no canto ficaria isolado;
* O mesmo para o d da segunda linha;
* E finalmente para o c da penúltima linha.