#include "fila.h"
#include "formas.h" 
#include <stdlib.h>
#include <stdio.h>

// ESTRUTURA DE UM NÓ
typedef struct no {
    void* dado;
    struct no* proximo;
} No;

// ESTRUTURA INTERNA FILA
struct fila {
    No* inicio;
    No* fim;
    int tamanho;
};

//  FUNÇÕES DE CRIAÇÃO OU DESTRUIÇÃO
// CRIAR_FILA
Fila* criar_fila() {
    Fila* f = (Fila*) malloc(sizeof(Fila));
    if (f == NULL) {
        perror("AVISO: erro ao alocar fila!");
        return NULL;
    }
    f->inicio = NULL;
    f->fim = NULL;
    f->tamanho = 0; 
    return f;
}

// DESTRUIR_FILA 
void destruir_fila(Fila* f) {
    if (f == NULL) return;
    No* atual = f->inicio;
    No* prox;
    while (atual != NULL) {
        prox = atual->proximo;
        free(atual); 
        atual = prox;
    }
    free(f); 
}

// FILA_DESTRUIR_COM_FORMAS 
void fila_destruir_com_formas(Fila* f) {
    if (f == NULL) return;
    No* atual = f->inicio;
    No* prox;
    while (atual != NULL) {
        prox = atual->proximo;
        if (atual->dado != NULL) {
            destruir_forma((Forma*)atual->dado); 
        }
        free(atual); 
        atual = prox;
    }
    free(f);
}

// VERIFICAR SITUAÇÃO DA FILA
// FILA_VAZIA
int fila_vazia(Fila* f) {
    return f == NULL || f->inicio == NULL;
}

// FILA_GET_TAMANHO
int fila_get_tamanho(Fila* f) {
    if (f == NULL) {
        return 0;
    }
    return f->tamanho;
}

// ALTERAR FILAS EXISTENTES
// ENFILEIRAR
void enfileirar(Fila* f, void* elemento) {
    if (f == NULL) return;
    
    No* novo = (No*) malloc(sizeof(No));
    if (novo == NULL) {
        perror("AVISO: erro ao alocar No para fila!");
        return;
    }
    
    novo->dado = elemento;
    novo->proximo = NULL;

    if (f->fim == NULL) {
        f->inicio = novo;
    } else {
        f->fim->proximo = novo;
    }
   f->fim = novo;
    f->tamanho++;
}

// DESENFILEIRAR
void* desenfileirar(Fila* f) {
    if (f == NULL || f->inicio == NULL) {
        return NULL; 
    }
    
    No* temp = f->inicio;
    void* dado = temp->dado;
    
    f->inicio = f->inicio->proximo;
    if (f->inicio == NULL) {
        f->fim = NULL;
    }
    free(temp);
    f->tamanho--;
    return dado;
}

// MANIPULAÇÃO DE DADOS
// FILA_CLONAR
Fila* fila_clonar(Fila* f) {
    if (f == NULL) return NULL;
    Fila* novo_f = criar_fila();
    if (novo_f == NULL) return NULL;
    
    No* atual = f->inicio;
    while (atual != NULL) {
        enfileirar(novo_f, atual->dado);
        atual = atual->proximo;
    }
    return novo_f;
}