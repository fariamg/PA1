#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define ALGINSERTION 1
#define ALGSELECTION 2
#define ALGQSORT 3
#define ALGQSORT3 4
#define ALGQSORTINS 5
#define ALGQSORT3INS 6
#define ALGSHELLSORT 7
#define ALGRECSEL 8

// define a struct de algoritmos possíveis, recebendo um int identificdaor e um
// ponteiro char para o nome
typedef struct alg {
  int num;
  char *name;
} alg_t;

// array de algoritmos possíveis
// cada elemento é um struct alg, com o número do algoritmo e o nome
// o último elemento é um struct com num=0 e name=0, que indica o fim do array
alg_t algvet[] = {{ALGINSERTION, "i"}, {ALGSELECTION, "s"}, {ALGQSORT, "q"},
                  {ALGQSORT3, "q3"},   {ALGQSORTINS, "qi"}, {ALGQSORT3INS, "q3i"},
                  {ALGSHELLSORT, "h"}, {ALGRECSEL, "rs"},   {0, 0}};

// função aque recebe um ponteiro char e retorna o número do algoritmo
// correspondente se o nome não for encontrado, retorna 0 a função percorre o
// array de algoritmos, comparando o nome do algoritmo com o nome recebido
int name2num(char *name) {
  int i = 0;
  while (algvet[i].num) {
    if (!strcmp(algvet[i].name, name))
      return algvet[i].num;
    i++;
  }
  return 0;
}

// função que recebe um número e retorna o nome do algoritmo correspondente
// se o número não for encontrado, retorna 0
// a função percorre o array de algoritmos, comparando o número do algoritmo com
// o número recebido
char *num2name(int num) {
  int i = 0;
  while (algvet[i].num) {
    if (algvet[i].num == num)
      return algvet[i].name;
    i++;
  }
  return 0;
}

// estrutura para armazenar as opções de linha de comando
// size: tamanho do vetor
// seed: semente para inicialização do vetor (seemente para PRNG)
// alg: número do algoritmo a ser utilizado
// preenchida pela parse_args
typedef struct opt {
  int size;
  int seed;
  int alg;
} opt_t;

// estrutura para armazenar o desempenho do algoritmo
// número de comparações, movimentações e chamadas de função
// cmp: número de comparações
// move: número de movimentações de dados
// calls: número de chamadas de função
typedef struct sortperf {
  int cmp;
  int move;
  int calls;
} sortperf_t;

void resetcounter(sortperf_t *s) {
  // Descricao: inicializa estrutura
  // Entrada:
  // Saida: s
  s->cmp = 0;
  s->move = 0;
  s->calls = 0;
}

void inccmp(sortperf_t *s, int num) {
  // Descricao: incrementa o numero de comparacoes em num
  // Entrada: s, num
  // Saida: s
  s->cmp += num;
}

void incmove(sortperf_t *s, int num) {
  // Descricao: incrementa o numero de movimentacoes de dados em num
  // Entrada: s, num
  // Saida: s
  s->move += num;
}

void inccalls(sortperf_t *s, int num) {
  // Descricao: incrementa o numero de chamadas de função em num
  // Entrada: s, num
  // Saida: s
  s->calls += num;
}

char *printsortperf(sortperf_t *s, char *str, char *pref) {
  // Descricao: gera string com valores de sortperf
  // Entrada: s, pref
  // Saida: str

  sprintf(str, "%s cmp %d move %d calls %d", pref, s->cmp, s->move, s->calls);
  return str;
}

void initVector(int *vet, int size) {
  // Descricao: inicializa vet com valores aleatorios
  // Entrada: vet
  // Saida: vet
  int i;
  // inicializa a parte alocada da vetor com valores aleatorios
  for (i = 0; i < size; i++) {
    vet[i] = (int)(drand48() * size);
  }
}

void printVector(int *vet, int size) {
  // Descricao: printa o vetor
  // Entrada: vet
  // Saida: vet
  int i;
  for (i = 0; i < size; i++) {
    printf("%d ", vet[i]);
  }
  printf("\n");
}

// função que recebe dois ponteiros para inteiros e troca os valores entre eles
// incrementa o número de movimentações de dados em 3
// a função é utilizada para trocar os valores de dois elementos do vetor
void swap(int *xp, int *yp, sortperf_t *s) {
  int temp = *xp;
  *xp = *yp;
  *yp = temp;
  incmove(s, 3);
}

// SHELLSORT
// Utilizando sequencia n/2^i
// O algoritmo Shellsort é uma versão otimizada do Insertion Sort. Ele
// divide o vetor em subvetores, ordenando-os usando o Insertion Sort. A
// diferença é que o Shellsort permite que os elementos estejam distantes uns
// dos outros, o que ajuda a mover os elementos mais distantes para suas
// posições corretas mais rapidamente. Isso reduz o número de trocas
// necessárias, tornando o algoritmo mais eficiente.
//
// COMPLEXIDADE:
// O(n²) no pior caso, O(n log n) no melhor caso, não possui demonstração exata de caso medio
//
// EXEMPLO:
// [5, 2, 4, 1, 3]
// Passo 1: [5, 2, 4, 1, 3] → [2, 5, 4, 1, 3] → [2, 1, 4, 5, 3]
// Passo 2: [2, 1, 4, 5, 3] → [1, 2, 4, 5, 3] → [1, 2, 3, 4, 5]
// Passo 3: [1, 2, 4, 5, 3] → [1, 2, 3, 4, 5] → [1, 2, 3, 4, 5]
void shellSort(int *A, int n, sortperf_t *s) {
  inccalls(s, 1); // incrementa o número de chamadas de função

  // define um range de comparação, inicialmente n/2 reduzindo a metade por iteração; ate chegar a 1 (insertion sort
  // padrão)
  for (int range = n / 2; range >= 1; range /= 2) {
    for (int i = range; i < n; i++) { // percorre o vetor
      int j = i;
      int temp = A[i]; // conta-se movimentação porque a convenção diz que mover dados de memoria para uma variável
                       // temporaria é movimentação de dados
      incmove(s, 1); // incrementa o número de movimentações

      for (; j >= range && A[j - range] > temp; j -= range) {
        inccmp(s, 1); // incrementa o número de comparações
        A[j] = A[j - range]; // desloca o elemento para frente
        incmove(s, 1); // incrementa o número de movimentações
      }
      inccmp(s, 1); // incrementa o número de comparações
      A[j] = temp;
      incmove(s, 1); // incrementa o número de movimentações
    }
  }
}

// recursive selection sort
void recursiveSelectionSort(int arr[], int l, int r, sortperf_t *s) {
  // find the minimum element in the unsorted subarray `[i…n-1]`
  // and swap it with `arr[i]`
  int min = l;
  inccalls(s, 1);
  for (int j = l + 1; j <= r; j++) {
    // if `arr[j]` is less, then it is the new minimum
    inccmp(s, 1);
    if (arr[j] < arr[min]) {
      min = j; // update the index of minimum element
    }
  }

  // swap the minimum element in subarray `arr[i…n-1]` with `arr[i]`
  if (min != l)
    swap(&arr[min], &arr[l], s);

  if (l + 1 < r) {
    recursiveSelectionSort(arr, l + 1, r, s);
  }
}

// SELECTION SORT
// Um algoritmo simples de ordenação que divide o array em duas partes: uma parte
// ordenada (à esquerda) e uma parte não ordenada (à direita). A cada iteração,
// ele seleciona o menor elemento da parte não ordenada e o move para o final da
// parte ordenada.
//
// COMPLEXIDADE:
// O(n²) no pior caso, O(n²) no melhor caso e O(n²) no caso médio
//
// EXEMPLO:
// [5, 2, 4, 1, 3]
// Passo 1: [5, 2, 4, 1, 3] → min=1 → Troca 5↔1 → [1, 2, 4, 5, 3]
// Passo 2: [1 | 2, 4, 5, 3] → min=2 → Nenhuma troca
// Passo 3: [1, 2 | 4, 5, 3] → min=3 → Troca 4↔3 → [1, 2, 3, 5, 4]
// Passo 4: [1, 2, 3 | 5, 4] → min=4 → Troca 5↔4 → [1, 2, 3, 4, 5]
//
// PARÂMETROS:
// arr = vetor a ser ordenado
// l (left) = limite inferior do vetor, inicializa em 0
// r (right) = limite superior do vetor, inicializa em size - 1 (ultimo elemento)
// s = ponteiro para a estrutura de desempenho do algoritmo
void selectionSort(int arr[], int l, int r, sortperf_t *s) {
  inccalls(s, 1); // incrementa o número de chamadas de função

  for (int i = l; i <= r; i++) {
    int min = i; // inicializa o índice do menor elemento

    // procura o menor valor no subarray não ordenado
    for (int j = i + 1; j <= r; j++) {
      inccmp(s, 1);
      if (arr[j] < arr[min])
        min = j; // atualiza o índice do menor elemento
    }

    // troca o menor elemento encontrado com o primeiro elemento do subarray
    if (min != i)
      swap(&arr[min], &arr[i], s);
  }
  return;
}

// INSERTION SORT
// Um algoritmo simples de ordenação que constrói uma sequência ordenada
// (à esquerda) e uma sequência não ordenada (à direita). A cada iteração, ele
// pega o primeiro elemento da parte não ordenada e o insere na posição correta
// da parte ordenada. Como jogador de cartas, ele pega uma carta e a coloca na posição
// correta entre as cartas já ordenadas. O algoritmo é eficiente para listas pequenas.
//
// COMPLEXIDADE:
// O(n²) no pior caso, O(n) no melhor caso e O(n²) no caso médio
//
// EXEMPLO:
// [5, 2, 4, 1, 3]
// Passo 1: [5 | 2, 4, 1, 3] → Insere 2 → [2, 5 | 4, 1, 3]
// Passo 2: [2, 5 | 4, 1, 3] → Insere 4 → [2, 4, 5 | 1, 3]
// Passo 3: [2, 4, 5 | 1, 3] → Insere 1 → [1, 2, 4, 5 | 3]
// Passo 4: [1, 2, 4, 5 | 3] → Insere 3 → [1, 2, 3, 4, 5]
//
// PARÂMETROS:
// v = vetor a ser ordenado
// l (left) = limite inferior do vetor, inicializa em 0
// r (right) = limite superior do vetor, inicializa em size - 1 (ultimo elemento)
// s = ponteiro para a estrutura de desempenho do algoritmo
void insertionSort(int v[], int l, int r, sortperf_t *s) {
  inccalls(s, 1); // incrementa o número de chamadas de função

  for (int i = l + 1; i <= r; i++) {
    int aux = v[i]; // armazena o valor atual
    incmove(s, 1); // incrementa o número de movimentações

    int j = i - 1; // inicializa o índice do elemento anterior

    // desloca os elementos maiores que 'aux' para frente
    // j >= l faz com que o loop não ultrapasse o limite inferior do vetor
    while (j >= l && v[j] > aux) {
      inccmp(s, 1); // so conta comparações de elementos (discarta as comparações de controle)
      v[j + 1] = v[j]; // desloca o elemento para frente
      incmove(s, 1); // incrementa o número de movimentações
      j--; // decrementa o índice
    }
    inccmp(s, 1); // incrementa o número de comparações

    if (i != j) { // verifica se o índice i é diferente de j
      v[j + 1] = aux; // insere o elemento na posição correta
      incmove(s, 1); // incrementa o número de movimentações
    }
  }
}

// median of 3 integers
int median(int a, int b, int c) {
  if ((a <= b) && (b <= c))
    return b; // a b c
  if ((a <= c) && (c <= b))
    return c; // a c b
  if ((b <= a) && (a <= c))
    return a; // b a c
  if ((b <= c) && (c <= a))
    return c; // b c a
  if ((c <= a) && (a <= b))
    return a; // c a b
  return b; // c b a
}

// quicksort partition using median of 3
void partition3(int *A, int l, int r, int *i, int *j, sortperf_t *s) {
  inccalls(s, 1);

  *i = l; // inicializa o índice i com o limite inferior do vetor
  *j = r; // inicializa o índice j com o limite superior do vetor

  int pivot = median(A[l], A[r], A[(*i + *j) / 2]); // inicializa o pivô com a mediana dos três elementos
  do {
    while (A[*i] < pivot) { // enquanto o elemento em i for menor que o pivô
      inccmp(s, 1);
      (*i)++;
    }
    inccmp(s, 1);

    while (A[*j] > pivot) { // enquanto o elemento em j for maior que o pivô
      inccmp(s, 1);
      (*j)--;
    }
    inccmp(s, 1);

    if (*i <= *j) { // se os índices não se cruzaram, troca os elementos
      swap(&A[*i], &A[*j], s);
      (*i)++;
      (*j)--;
    }

  } while (*i <= *j);
}

// standard quicksort partition
void partition(int *A, int l, int r, int *i, int *j, sortperf_t *s) {
  inccalls(s, 1);

  *i = l; // inicializa o índice i com o limite inferior do vetor
  *j = r; // inicializa o índice j com o limite superior do vetor

  int pivot = A[(*i + *j) / 2]; // inicializa o pivô com o primeiro elemento

  do {
    while (A[*i] < pivot) { // enquanto o elemento em i for menor que o pivô
      inccmp(s, 1);
      (*i)++;
    }
    inccmp(s, 1);

    while (A[*j] > pivot) { // enquanto o elemento em j for maior que o pivô
      inccmp(s, 1);
      (*j)--;
    }
    inccmp(s, 1);

    if (*i <= *j) { // se os índices não se cruzaram, troca os elementos
      swap(&A[*i], &A[*j], s);
      (*i)++;
      (*j)--;
    }

  } while (*i <= *j);
}

// standard quicksort
void quickSort(int *A, int l, int r, sortperf_t *s) {
  inccalls(s, 1);
  if (l >= r) // caso base
    return;

  int i = 0, j = 0;

  partition(A, l, r, &i, &j, s);

  if (l < j) // verifica se o índice l é menor que j
    quickSort(A, l, j, s);

  if (i < r) // verifica se o índice i é menor que r
    quickSort(A, i, r, s);
}

// quicksort with median of 3
void quickSort3(int *A, int l, int r, sortperf_t *s) {
  inccalls(s, 1);
  if (l >= r)
    return;

  int i = 0, j = 0;

  partition3(A, l, r, &i, &j, s);

  // partições recursivas
  if (l < j)
    quickSort3(A, l, j, s);

  if (i < r)
    quickSort3(A, i, r, s);
}

// quicksort with insertion for small partitions
void quickSortIns(int *A, int l, int r, sortperf_t *s) {
  inccalls(s, 1);
  if (l >= r)
    return;

  int i = 0, j = 0;

  // primeira partição
  partition(A, l, r, &i, &j, s);

  // partições recursivas, com verificação para o insertion sort
  // se o tamanho da partição for menor ou igual a 50, utiliza o insertion sort
  if (l < j) {
    if ((j - l) <= 50) {
      insertionSort(A, l, j, s);
    } else {
      quickSortIns(A, l, j, s);
    }
  }

  if (i < r) {
    if ((r - i) <= 50) {
      insertionSort(A, i, r, s);
    } else {
      quickSortIns(A, i, r, s);
    }
  }
}

// quicksort with insertion for small partitions and median of 3
void quickSort3Ins(int *A, int l, int r, sortperf_t *s) {
  inccalls(s, 1);
  if (l >= r)
    return;

  int i = 0, j = 0;

  // primeira partição
  partition3(A, l, r, &i, &j, s);

  // partições recursivas, com verificação para o insertion sort
  // se o tamanho da partição for menor ou igual a 50, utiliza o insertion sort
  if (l < j) {
    if ((j - l) <= 50) {
      insertionSort(A, l, j, s);
    } else {
      quickSort3Ins(A, l, j, s);
    }
  }

  if (i < r) {
    if ((r - i) <= 50) {
      insertionSort(A, i, r, s);
    } else {
      quickSort3Ins(A, i, r, s);
    }
  }
}

void uso()
// Descricao: imprime as opcoes de uso
// Entrada: nao tem
// Saida: impressao das opcoes de linha de comando
{
  fprintf(stderr, "sortperf\n");
  fprintf(stderr, "\t-z <int>\t(vector size)\n");
  fprintf(stderr, "\t-s <int>\t(initialization seed)\n");
  fprintf(stderr, "\t-a <s|i|q|q3|qi|q3i|h|rs>\t(algorithm)\n");
  fprintf(stderr, "\t    s\tselection\n");
  fprintf(stderr, "\t    i\tinsertion\n");
  fprintf(stderr, "\t    q\tquicksort\n");
  fprintf(stderr, "\t    q3\tquicksort+median3\n");
  fprintf(stderr, "\t    qi\tquicksort+insertion\n");
  fprintf(stderr, "\t    q3i\tquicksort+median3+insertion\n");
  fprintf(stderr, "\t    h\theapsort\n");
  fprintf(stderr, "\t    rs\trecursive selection\n");
}

void parse_args(int argc, char **argv, opt_t *opt)
// Descricao: le as opcoes da linha de comando e inicializa variaveis
// Entrada: argc, argv, opt
// Saida: opt
{
  // variaveis externas do getopt
  extern char *optarg;
  extern int optind;

  // variavel auxiliar
  int c;

  // inicializacao variaveis globais para opcoes com valores padrão
  opt->seed = 1; // seed para PRNG
  opt->size = 10; // tamanho de vetor
  opt->alg = 1; // algoritmo a ser utilizado

  // getopt - letra indica a opcao, : junto a letra indica parametro
  // no caso de escolher mais de uma operacao, vale a ultima
  while ((c = getopt(argc, argv, "z:s:a:h")) != EOF) {
    switch (c) {
    case 'z': // define o tamanho do vetor pela entrada no input
      opt->size = atoi(optarg);
      break;
    case 's': // define a seed do PRNG pela entrada no input
      opt->seed = atoi(optarg);
      break;
    case 'a': // define o algoritmo a ser usado entrada no input
      opt->alg = name2num(optarg);
      break;
    case 'h':
    default:
      uso();
      exit(1);
    }
  }
  if (!opt->alg) { // se não for valido o algoritmo, o programa é encerrado
    uso();
    exit(1);
  }
}

// essa função calcula a diferença de tempo entre dois instantes
// é util para calcular o tempo de execução de certa função
// t1 = tempo inicial
// t2 = tempo final
// res = diferença de tempo
// diferença calculada em nanosegundos
void clkDiff(struct timespec t1, struct timespec t2, struct timespec *res)
// Descricao: calcula a diferenca entre t2 e t1, que e armazenada em res
// Entrada: t1, t2
// Saida: res
{
  if (t2.tv_nsec < t1.tv_nsec) {
    // ajuste necessario, utilizando um segundo de tv_sec
    res->tv_nsec = 1000000000 + t2.tv_nsec - t1.tv_nsec;
    res->tv_sec = t2.tv_sec - t1.tv_sec - 1;
  } else {
    // nao e necessario ajuste
    res->tv_nsec = t2.tv_nsec - t1.tv_nsec;
    res->tv_sec = t2.tv_sec - t1.tv_sec;
  }
}

// argc = int que representa o numero de parametros passados
// argv = array com os parametros passados
int main(int argc, char **argv) {
  sortperf_t s;
  int *vet;
  char buf[200];
  char pref[100];
  opt_t opt;
  struct timespec inittp, endtp, restp;
  int retp;

  // parse_args
  parse_args(argc, argv, &opt);

  // malloc with opt.size+1 for heapsort
  // aloca um local de memoria a mais para poder utilizar o algoritmo heapsort
  // vetor de ponteiros para inteiro
  vet = (int *)malloc((opt.size + 1) * sizeof(int));

  // initialize
  resetcounter(&s); // zera todos os contadores do sortperf (guarda os dados de
                    // desempenho do algoritmo)
  srand48(opt.seed); // inicializa o PRNG com a seed passada na linha de comando
  initVector(vet, opt.size); // inicializa o vetor com valores randomicos
  vet[opt.size] = vet[0]; // for heapsort
  // if (opt.size < 100) printVector(vet, opt.size);

  retp = clock_gettime(CLOCK_MONOTONIC,
                       &inittp); // recebe tempo atual (tempo inicial) e atribui a inittp

  // execute algorithm
  switch (opt.alg) {
  case ALGINSERTION:
    insertionSort(vet, 0, opt.size - 1, &s);
    break;
  case ALGSELECTION:
    selectionSort(vet, 0, opt.size - 1, &s);
    break;
  case ALGQSORT:
    quickSort(vet, 0, opt.size - 1, &s);
    break;
  case ALGQSORT3:
    quickSort3(vet, 0, opt.size - 1, &s);
    break;
  case ALGQSORTINS:
    quickSortIns(vet, 0, opt.size - 1, &s);
    break;
  case ALGQSORT3INS:
    quickSort3Ins(vet, 0, opt.size - 1, &s);
    break;
  case ALGSHELLSORT:
    shellSort(vet, opt.size, &s);
    break;
  case ALGRECSEL:
    recursiveSelectionSort(vet, 0, opt.size - 1, &s);
    break;
  }
  retp = clock_gettime(CLOCK_MONOTONIC,
                       &endtp); // recebe tempo atual (tempo final) e atribui a inittp
  clkDiff(inittp, endtp, &restp);

  // if (opt.size<100) printVector(vet, opt.size);

  // print stats
  sprintf(pref, "alg %s seed %d size %d time %ld.%.9ld", num2name(opt.alg), opt.seed, opt.size, restp.tv_sec,
          restp.tv_nsec);

  printsortperf(&s, buf, pref);
  printf("%s\n", buf);

  exit(0);
}