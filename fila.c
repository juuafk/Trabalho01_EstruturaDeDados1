#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

// ============================
// DEFINIÇÃO DA STRUCT FILA
// ============================
struct fila {
    int capacidade;
    int inicio;
    int fim;
    int tamanho;
    void* dados[]; 
};

// ==========================
// IMPLEMENTAÇÃO DAS FUNÇÕES
// ==========================

// CRIAR OU DESTRUIR FILAS
Fila* criar_fila(int capacidade) {
    size_t tamanho_total = sizeof(Fila) + capacidade * sizeof(void*);
    Fila* f = (Fila*) malloc(tamanho_total);

    if (f == NULL) return NULL; // ERRO!

    f->capacidade = capacidade;
    f->inicio = 0;
    f->fim = -1;
    f->tamanho = 0;

    return f;
}

// VERIFICAR SITUAÇÃO DA FILA
int fila_vazia(Fila* f) {
    return f->tamanho == 0;
}

int fila_cheia(Fila* f) {
    return f->tamanho == f->capacidade;
}

int fila_get_tamanho(Fila* f) {
    if (f) return f->tamanho;
    return 0;
}

// ALTERAR FILAS EXISTENTES
void destruir_fila(Fila* f) {
    if (f == NULL) return;

    free(f);
}

void enfileirar(Fila* f, void* elemento) {
    if (fila_cheia(f)) {
        printf("AVISO: tentativa de enfileirar em uma fila cheia!\n");
        return;
    }
    f->fim = (f->fim + 1) % f->capacidade;
    f->dados[f->fim] = elemento;
    f->tamanho++;
}

void* desenfileirar(Fila* f) {
    if (fila_vazia(f)) {
        return NULL;
    }
    void* elemento = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % f->capacidade;
    f->tamanho--;
    return elemento;
}