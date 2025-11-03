#ifndef FILA_H
#define FILA_H

#include "formas.h" 

// FILA  
// ==================================================
// A fila é uma estrutura de dados do tipo FIFO 
// (First In, First Out), em que o primeiro elemento
// a ser inserido é o primeiro a ser removido.
// No projeto, será utilizada para representar
// as formas disponíveis no chão.
// ==================================================


// DEFINIÇÃO DA STRUCT FILA
typedef struct fila Fila; 

// ==================================
//  FUNÇÕES DE CRIAÇÃO OU DESTRUIÇÃO
// ==================================

// CRIAR_FILA
// cria uma estrutura de dados FILA vazia.
Fila* criar_fila(); 

// DESTRUIR_FILA
// libera a memória alocada para a fila.
void destruir_fila(Fila* f); 

// FILA_DESTRUIR_COM_FORMAS
// libera a memória da fila junto com qualquer sub-estrutura com ela.
void fila_destruir_com_formas(Fila* f); 

// ===========================
// VERIFICAR SITUAÇÃO DA FILA
// ===========================

// FILA_VAZIA
// verifica se a fila está vazia (retorna 1 caso estiver).
int fila_vazia(Fila* f);

// FILA_GET_TAMANHO
// verifica o tamanho da fila.
int fila_get_tamanho(Fila* f);

// =========================
// ALTERAR FILAS EXISTENTES
// =========================

// ENFILEIRAR
// adiciona elemento no final da fila.
void enfileirar(Fila* f, void* elemento);

// DESENFILEIRAR
// remove o elemento do início da fila.
void* desenfileirar(Fila* f);

// =====================
// MANIPULAÇÃO DE DADOS
// =====================

// FILA_CLONAR
// clona uma fila.
Fila* fila_clonar(Fila* f); 

#endif