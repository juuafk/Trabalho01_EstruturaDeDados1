#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

// ==========================
// DEFINIÇÃO DA STRUCT PILHA
// ==========================
struct pilha {
    int capacidade;
    int topo;
    void* dados[]; 
};

// ==========================
// IMPLEMENTAÇÃO DAS FUNÇÕES
// ==========================

// CRIAR OU DESTRUIR PILHAS
// CRIAR_PILHA
Pilha* criar_pilha(int capacidade) {
    size_t tamanho_total = sizeof(Pilha) + capacidade * sizeof(void*);
    Pilha* p = (Pilha*) malloc(tamanho_total);
    
    if (p == NULL) return NULL; // ERRO! :P

    p->capacidade = capacidade;
    p->topo = -1; 
    return p;
}

// DESTRUIR_PILHA
void destruir_pilha(Pilha* p) {
    if (p == NULL) return;

    free(p);
}

// VERIFICAR SITUAÇÃO DA PILHA
// PILHA_VAZIA   
int pilha_vazia(Pilha* p) {
    return p->topo == -1;
}

// PILHA_CHEIA
int pilha_cheia(Pilha* p) {
    return p->topo == p->capacidade - 1;
}

// PILHA_GET_TAMANHO
int pilha_get_tamanho(Pilha* p) {
    if (p == NULL) return 0;
    return p->topo + 1;
}

// ALTERAR PILHAS EXISTENTES
// EMPILHAR
void empilhar(Pilha* p, void* elemento) {
    if (pilha_cheia(p)) {
        printf("AVISO: tentativa de empilhar em uma pilha cheia!\n");
        return;
    }
    p->topo++;
    p->dados[p->topo] = elemento;
}

// DESEMPILHAR
void* desempilhar(Pilha* p) {
    if (pilha_vazia(p)) {
        return NULL;
    }
    void* elemento = p->dados[p->topo];
    p->topo--;
    return elemento;
}