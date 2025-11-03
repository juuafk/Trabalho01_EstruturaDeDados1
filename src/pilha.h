#ifndef PILHA_H
#define PILHA_H

// PILHA
// ===============================================
// A pilha é uma estrutura de dados do tipo LIFO
// (Last In, First Out), em que o último elemento
// a ser inserido é o primeiro a ser removido.
// No projeto, será utilizada para representar
// os carregadores de formas.
// ===============================================


// DEFINIÇÃO DA STRUCT PILHA
typedef struct pilha Pilha;

// ==================================
//  FUNÇÕES DE CRIAÇÃO OU DESTRUIÇÃO
// ==================================

// CRIAR_PILHA
// cria uma pilha vazia.
Pilha* criar_pilha(int capacidade);

// DESTRUIR_PILHA
// libera a memória da pilha.
void destruir_pilha(Pilha* p);

// ============================
// VERIFICAR SITUAÇÃO DA PILHA
// ============================

// PILHA_VAZIA
// verifica se a pilha está vazia (retorna 1 caso estiver).
int pilha_vazia(Pilha* p);

// PILHA_CHEIA
// verifica se a pilha está cheia (retorna 1 caso estiver).
int pilha_cheia(Pilha* p);

// PILHA_GET_TAMANHO
// retorna o número de elementos na pilha.
int pilha_get_tamanho(Pilha* p);

// ==========================
// ALTERAR PILHAS EXISTENTES
// ==========================

// EMPILHAR
// adiciona elemento no topo da pilha.
void empilhar(Pilha* p, void* elemento);

// DESEMPILHAR
// remove o elemento do topo da pilha.
void* desempilhar(Pilha* p);

#endif