#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100 // capacidade máxima da mochila (para vetor)

// ==========================
// ESTRUTURAS DE DADOS
// ==========================

typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// ==========================
// VARIÁVEIS GLOBAIS
// ==========================
Item mochilaVetor[MAX];
int totalItens = 0;
No* inicioLista = NULL;

int comparacoesSequencial = 0;
int comparacoesBinaria = 0;

// ==========================
// FUNÇÕES - VERSÃO COM VETOR
// ==========================

// Inserir item no vetor
void inserirItemVetor() {
    if (totalItens >= MAX) {
        printf("\nMochila cheia!\n");
        return;
    }
    Item novo;
    printf("\nNome do item: ");
    scanf("%s", novo.nome);
    printf("Tipo do item: ");
    scanf("%s", novo.tipo);
    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    mochilaVetor[totalItens++] = novo;
    printf("Item inserido com sucesso!\n");
}

// Remover item por nome (vetor)
void removerItemVetor() {
    char nome[30];
    printf("\nDigite o nome do item a remover: ");
    scanf("%s", nome);
    int i, encontrado = 0;
    for (i = 0; i < totalItens; i++) {
        if (strcmp(mochilaVetor[i].nome, nome) == 0) {
            encontrado = 1;
            for (int j = i; j < totalItens - 1; j++) {
                mochilaVetor[j] = mochilaVetor[j + 1];
            }
            totalItens--;
            printf("Item removido com sucesso!\n");
            break;
        }
    }
    if (!encontrado)
        printf("Item nao encontrado!\n");
}

// Listar todos os itens (vetor)
void listarItensVetor() {
    if (totalItens == 0) {
        printf("\nMochila vazia!\n");
        return;
    }
    printf("\n--- ITENS NA MOCHILA (VETOR) ---\n");
    for (int i = 0; i < totalItens; i++) {
        printf("%d) %s - Tipo: %s - Qtde: %d\n", i + 1,
               mochilaVetor[i].nome,
               mochilaVetor[i].tipo,
               mochilaVetor[i].quantidade);
    }
}

// Busca sequencial (vetor)
int buscarSequencialVetor(char nome[]) {
    comparacoesSequencial = 0;
    for (int i = 0; i < totalItens; i++) {
        comparacoesSequencial++;
        if (strcmp(mochilaVetor[i].nome, nome) == 0)
            return i;
    }
    return -1;
}

// Ordenação (Bubble Sort)
void ordenarVetor() {
    for (int i = 0; i < totalItens - 1; i++) {
        for (int j = 0; j < totalItens - i - 1; j++) {
            if (strcmp(mochilaVetor[j].nome, mochilaVetor[j + 1].nome) > 0) {
                Item temp = mochilaVetor[j];
                mochilaVetor[j] = mochilaVetor[j + 1];
                mochilaVetor[j + 1] = temp;
            }
        }
    }
}

// Busca binária (vetor)
int buscarBinariaVetor(char nome[]) {
    int ini = 0, fim = totalItens - 1;
    comparacoesBinaria = 0;
    while (ini <= fim) {
        int meio = (ini + fim) / 2;
        comparacoesBinaria++;
        int cmp = strcmp(nome, mochilaVetor[meio].nome);
        if (cmp == 0)
            return meio;
        else if (cmp < 0)
            fim = meio - 1;
        else
            ini = meio + 1;
    }
    return -1;
}

// ==========================
// FUNÇÕES - VERSÃO COM LISTA
// ==========================

// Inserir item na lista encadeada
void inserirItemLista() {
    No* novo = (No*)malloc(sizeof(No));
    printf("\nNome do item: ");
    scanf("%s", novo->dados.nome);
    printf("Tipo do item: ");
    scanf("%s", novo->dados.tipo);
    printf("Quantidade: ");
    scanf("%d", &novo->dados.quantidade);
    novo->proximo = inicioLista;
    inicioLista = novo;
    printf("Item inserido com sucesso!\n");
}

// Remover item por nome (lista encadeada)
void removerItemLista() {
    char nome[30];
    printf("\nDigite o nome do item a remover: ");
    scanf("%s", nome);
    No* atual = inicioLista;
    No* anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->dados.nome, nome) == 0) {
            if (anterior == NULL) {
                inicioLista = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            free(atual);
            printf("Item removido com sucesso!\n");
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    printf("Item nao encontrado!\n");
}

// Listar todos os itens (lista encadeada)
void listarItensLista() {
    if (inicioLista == NULL) {
        printf("\nMochila vazia!\n");
        return;
    }
    printf("\n--- ITENS NA MOCHILA (LISTA) ---\n");
    No* atual = inicioLista;
    int i = 1;
    while (atual != NULL) {
        printf("%d) %s - Tipo: %s - Qtde: %d\n", i++,
               atual->dados.nome,
               atual->dados.tipo,
               atual->dados.quantidade);
        atual = atual->proximo;
    }
}

// Buscar item (sequencial em lista)
No* buscarItemLista(char nome[]) {
    comparacoesSequencial = 0;
    No* atual = inicioLista;
    while (atual != NULL) {
        comparacoesSequencial++;
        if (strcmp(atual->dados.nome, nome) == 0)
            return atual;
        atual = atual->proximo;
    }
    return NULL;
}

// ==========================
// MENUS DE OPERAÇÃO
// ==========================

void menuVetor() {
    int opc;
    char nome[30];
    do {
        printf("\n--- MOCHILA (VETOR) ---\n");
        printf("1 - Inserir item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item (sequencial)\n");
        printf("5 - Ordenar e buscar (binaria)\n");
        printf("0 - Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opc);

        switch (opc) {
            case 1:
                inserirItemVetor();
                break;
            case 2:
                removerItemVetor();
                break;
            case 3:
                listarItensVetor();
                break;
            case 4:
                printf("Nome do item a buscar: ");
                scanf("%s", nome);
                {
                    int pos = buscarSequencialVetor(nome);
                    if (pos != -1)
                        printf("Item encontrado! Comparacoes: %d\n", comparacoesSequencial);
                    else
                        printf("Item nao encontrado! Comparacoes: %d\n", comparacoesSequencial);
                }
                break;
            case 5:
                ordenarVetor();
                printf("Itens ordenados!\n");
                printf("Nome do item a buscar: ");
                scanf("%s", nome);
                {
                    int pos = buscarBinariaVetor(nome);
                    if (pos != -1)
                        printf("Item encontrado! Comparacoes: %d\n", comparacoesBinaria);
                    else
                        printf("Item nao encontrado! Comparacoes: %d\n", comparacoesBinaria);
                }
                break;
        }
    } while (opc != 0);
}

void menuLista() {
    int opc;
    char nome[30];
    do {
        printf("\n--- MOCHILA (LISTA ENCADEADA) ---\n");
        printf("1 - Inserir item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item\n");
        printf("0 - Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opc);

        switch (opc) {
            case 1:
                inserirItemLista();
                break;
            case 2:
                removerItemLista();
                break;
            case 3:
                listarItensLista();
                break;
            case 4:
                printf("Nome do item a buscar: ");
                scanf("%s", nome);
                {
                    No* encontrado = buscarItemLista(nome);
                    if (encontrado != NULL)
                        printf("Item encontrado! Comparacoes: %d\n", comparacoesSequencial);
                    else
                        printf("Item nao encontrado! Comparacoes: %d\n", comparacoesSequencial);
                }
                break;
        }
    } while (opc != 0);
}

// ==========================
// FUNÇÃO PRINCIPAL
// ==========================
int main() {
    int opc;
    do {
        printf("\n========== MENU PRINCIPAL ==========\n");
        printf("1 - Usar mochila com vetor\n");
        printf("2 - Usar mochila com lista encadeada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc);

        switch (opc) {
            case 1:
                menuVetor();
                break;
            case 2:
                menuLista();
                break;
            case 0:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opc != 0);
    return 0;
}


