#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20
#define NOME_LEN 30
#define TIPO_LEN 20
#define BUFFER 128

// Estrutura que representa um componente da torre de resgate
typedef struct {
    char nome[NOME_LEN];
    char tipo[TIPO_LEN];
    int prioridade;
} Componente;

/* ---------- UTILITÁRIOS DE ENTRADA ---------- */

// Lê uma linha com fgets e remove o '\n' final (se existir)
void lerLinhaSeguro(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin) == NULL) {
        // Entrada inesperada: forçar string vazia
        buffer[0] = '\0';
        return;
    }
    // remover newline
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';
}

// Copia vetor de componentes (usado para preservar original ao testar vários algoritmos)
void copiarComponentes(Componente dest[], Componente src[], int n) {
    for (int i = 0; i < n; ++i) dest[i] = src[i];
}

/* ---------- FUNÇÕES DE EXIBIÇÃO ---------- */

// Exibe o vetor de componentes formatado
void mostrarComponentes(Componente comps[], int n) {
    if (n == 0) {
        printf("\nNenhum componente cadastrado.\n");
        return;
    }
    printf("\n--- Lista de Componentes (%d) ---\n", n);
    printf("%-3s | %-28s | %-15s | %-9s\n", "ID", "NOME", "TIPO", "PRIORIDADE");
    printf("----+------------------------------+-----------------+-----------\n");
    for (int i = 0; i < n; ++i) {
        printf("%-3d | %-28s | %-15s | %-9d\n",
               i+1, comps[i].nome, comps[i].tipo, comps[i].prioridade);
    }
}

/* ---------- ALGORITMOS DE ORDENAÇÃO (cada um conta comparações) ---------- */

/*
 * Bubble Sort por nome (string)
 * Parâmetros:
 *  - arr: vetor de componentes
 *  - n: número de elementos
 *  - comparacoes: ponteiro para contador de comparações (será atualizado)
 */
void bubbleSortNome(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    if (n <= 1) return;
    for (int i = 0; i < n - 1; ++i) {
        int trocou = 0;
        for (int j = 0; j < n - i - 1; ++j) {
            (*comparacoes)++;
            if (strcmp(arr[j].nome, arr[j+1].nome) > 0) {
                // troca
                Componente tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
                trocou = 1;
            }
        }
        // otimização: se nenhuma troca, já está ordenado
        if (!trocou) break;
    }
}

/*
 * Insertion Sort por tipo (string)
 * Parâmetros idênticos: arr, n, comparacoes
 */
void insertionSortTipo(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < n; ++i) {
        Componente key = arr[i];
        int j = i - 1;
        // mover elementos maiores (por tipo) para frente
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(arr[j].tipo, key.tipo) > 0) {
                arr[j+1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j+1] = key;
    }
}

/*
 * Selection Sort por prioridade (int) - menor prioridade primeiro (1..10)
 * Parâmetros idênticos: arr, n, comparacoes
 */
void selectionSortPrioridade(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; ++i) {
        int menorIdx = i;
        for (int j = i + 1; j < n; ++j) {
            (*comparacoes)++;
            if (arr[j].prioridade < arr[menorIdx].prioridade) {
                menorIdx = j;
            }
        }
        if (menorIdx != i) {
            Componente tmp = arr[i];
            arr[i] = arr[menorIdx];
            arr[menorIdx] = tmp;
        }
    }
}

/* ---------- BUSCA BINÁRIA (aplicável apenas se ordenado por nome) ---------- */

/*
 * Busca binária por nome em vetor arr de tamanho n.
 * Retorna índice (0..n-1) se encontrado, -1 caso contrário.
 * Conta comparações em comparacoes (incrementado a cada comparação de string).
 *
 * Observação: exige que arr esteja ordenado por nome em ordem ascendente.
 */
int buscaBinariaPorNome(Componente arr[], int n, const char alvo[], long *comparacoes) {
    int esq = 0, dir = n - 1;
    *comparacoes = 0;
    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;
        (*comparacoes)++;
        int cmp = strcmp(alvo, arr[meio].nome);
        if (cmp == 0) return meio;
        else if (cmp < 0) dir = meio - 1;
        else esq = meio + 1;
    }
    return -1;
}

/* ---------- MEDIÇÃO DE TEMPO (wrapper genérico) ---------- */

/*
 * Mede o tempo de execução do algoritmo passado (assinatura: void f(Componente*,int,long*))
 * - algoritmo: ponteiro para a função de ordenação
 * - arr: vetor de componentes (será modificado pela ordenação)
 * - n: tamanho
 * - comparacoes: saída com o total de comparações feitas pela ordenação
 * - tempo: saída com tempo em segundos
 */
void medirTempo(void (*algoritmo)(Componente*, int, long*),
                Componente arr[], int n, long *comparacoes, double *tempo) {
    clock_t ini = clock();
    algoritmo(arr, n, comparacoes);
    clock_t fim = clock();
    *tempo = (double)(fim - ini) / CLOCKS_PER_SEC;
}

/* ---------- FUNÇÃO PRINCIPAL / MENU ---------- */

int main() {
    Componente original[MAX_COMPONENTES];
    Componente working[MAX_COMPONENTES];
    int n = 0;
    char buffer[BUFFER];

    printf("=== MONTAGEM DA TORRE DE RESGATE - PRIORITIZACAO DE COMPONENTES ===\n");
    printf("Cadastro de componentes (max %d). Use nomes, tipos e prioridade (1..10).\n\n", MAX_COMPONENTES);

    // Entrada de dados (até 20 componentes). O usuário pode encerrar com linha vazia no nome.
    while (n < MAX_COMPONENTES) {
        printf("Cadastrar componente #%d (ou ENTER em nome para terminar):\n", n+1);
        printf(" Nome: ");
        lerLinhaSeguro(buffer, BUFFER);
        if (strlen(buffer) == 0) break; // finaliza entrada
        strncpy(original[n].nome, buffer, NOME_LEN - 1);
        original[n].nome[NOME_LEN - 1] = '\0';

        printf(" Tipo: ");
        lerLinhaSeguro(buffer, BUFFER);
        strncpy(original[n].tipo, buffer, TIPO_LEN - 1);
        original[n].tipo[TIPO_LEN - 1] = '\0';

        // ler prioridade com segurança: ler linha e converter
        while (1) {
            printf(" Prioridade (1 a 10): ");
            lerLinhaSeguro(buffer, BUFFER);
            int p;
            if (sscanf(buffer, "%d", &p) == 1 && p >= 1 && p <= 10) {
                original[n].prioridade = p;
                break;
            } else {
                printf("  Entrada invalida. Informe um inteiro entre 1 e 10.\n");
            }
        }
        n++;
        printf(" Componente cadastrado!\n\n");
    }

    if (n == 0) {
        printf("\nNenhum componente cadastrado. Encerrando.\n");
        return 0;
    }

    // flags para saber se working está ordenado por nome (útil para busca binária)
    int ordenadoPorNome = 0;

    // Menu interativo
    int opc;
    do {
        printf("\n======== MENU DE ACOES ========\n");
        printf("1 - Mostrar componentes cadastrados\n");
        printf("2 - Ordenar por NOME (Bubble Sort) e mostrar (conta comparacoes e tempo)\n");
        printf("3 - Ordenar por TIPO (Insertion Sort) e mostrar (conta comparacoes e tempo)\n");
        printf("4 - Ordenar por PRIORIDADE (Selection Sort) e mostrar (conta comparacoes e tempo)\n");
        printf("5 - Buscar componente por NOME (Busca binaria) - exige ordenacao por NOME\n");
        printf("6 - Restaurar vetor original\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        lerLinhaSeguro(buffer, BUFFER);
        if (sscanf(buffer, "%d", &opc) != 1) { opc = -1; }

        switch (opc) {
            case 1:
                // mostrar original (estado atual)
                printf("\n[ESTADO ATUAL DOS COMPONENTES]\n");
                mostrarComponentes(original, n);
                break;

            case 2: {
                // Bubble por nome
                copiarComponentes(working, original, n);
                long comparacoes = 0;
                double tempo = 0.0;
                medirTempo(bubbleSortNome, working, n, &comparacoes, &tempo);
                printf("\nResultados - Bubble Sort por NOME:\n");
                printf(" Comparacoes: %ld\n", comparacoes);
                printf(" Tempo: %.6f segundos\n", tempo);
                mostrarComponentes(working, n);
                // atualiza flag
                ordenadoPorNome = 1;
                // opcional: perguntar se quer tornar isso o estado "original"
                break;
            }

            case 3: {
                // Insertion por tipo
                copiarComponentes(working, original, n);
                long comparacoes = 0;
                double tempo = 0.0;
                medirTempo(insertionSortTipo, working, n, &comparacoes, &tempo);
                printf("\nResultados - Insertion Sort por TIPO:\n");
                printf(" Comparacoes: %ld\n", comparacoes);
                printf(" Tempo: %.6f segundos\n", tempo);
                mostrarComponentes(working, n);
                ordenadoPorNome = 0; // não está ordenado por nome
                break;
            }

            case 4: {
                // Selection por prioridade
                copiarComponentes(working, original, n);
                long comparacoes = 0;
                double tempo = 0.0;
                medirTempo(selectionSortPrioridade, working, n, &comparacoes, &tempo);
                printf("\nResultados - Selection Sort por PRIORIDADE:\n");
                printf(" Comparacoes: %ld\n", comparacoes);
                printf(" Tempo: %.6f segundos\n", tempo);
                mostrarComponentes(working, n);
                ordenadoPorNome = 0;
                break;
            }

            case 5: {
                // Busca binária por nome (exige que working esteja ordenado por nome)
                if (!ordenadoPorNome) {
                    printf("\nAVISO: a busca binaria requer que os componentes estejam ordenados por NOME.\n");
                    printf("Escolha a opcao 2 (Bubble Sort por NOME) antes de usar a busca binaria.\n");
                } else {
                    char alvo[NOME_LEN];
                    printf("\nInforme o NOME do componente-chave a buscar: ");
                    lerLinhaSeguro(alvo, NOME_LEN);
                    if (strlen(alvo) == 0) {
                        printf("Nenhum nome informado. Abortando busca.\n");
                        break;
                    }
                    long comparacoes = 0;
                    clock_t ini = clock();
                    int idx = buscaBinariaPorNome(working, n, alvo, &comparacoes);
                    clock_t fim = clock();
                    double tempo = (double)(fim - ini) / CLOCKS_PER_SEC;
                    if (idx != -1) {
                        printf("\nComponente encontrado (indice %d):\n", idx+1);
                        printf(" Nome: %s | Tipo: %s | Prioridade: %d\n",
                               working[idx].nome, working[idx].tipo, working[idx].prioridade);
                        printf(" Comparacoes na busca binaria: %ld\n", comparacoes);
                        printf(" Tempo da busca: %.6f segundos\n", tempo);
                    } else {
                        printf("\nComponente NAO encontrado.\n");
                        printf(" Comparacoes na busca binaria: %ld\n", comparacoes);
                        printf(" Tempo da busca: %.6f segundos\n", tempo);
                    }
                }
                break;
            }

            case 6:
                // restaurar original e resetar flag
                printf("\nVetor original preservado. Nada a restaurar explicitamente (modo de uso atual preserva original).\n");
                ordenadoPorNome = 0;
                break;

            case 0:
                printf("\nFinalizando modulo de montagem. Boa sorte na fuga!\n");
                break;

            default:
                printf("\nOpcao invalida. Tente novamente.\n");
        }

    } while (opc != 0);

    return 0;
}


