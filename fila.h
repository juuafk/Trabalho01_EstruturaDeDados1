#ifndef FILA_H
#define FILA_H

/* FILA  
==================================================
A fila é uma estrutura de dados do tipo FIFO 
(First In, First Out), em que o primeiro elemento
a ser inserido é o primeiro a ser removido.
No projeto, será utilizada para representar
as formas disponíveis no chão.
==================================================
*/

typedef struct fila Fila;


// CRIAR OU DESTRUIR FILAS

// CRIAR_FILA
// cria uma fila vazia
Fila* criar_fila(int capacidade);

// DESTRUIR FILA
// libera a memória da fila
void destruir_fila(Fila* f);


// VERIFICAR SITUAÇÃO DA FILA

// FILA_VAZIA
// verifica se a fila está vazia (retorna 1 caso estiver)
int fila_vazia(Fila* f);

// FILA_CHEIA
// verifica se a fila está cheia (retorna 1 caso estiver)
int fila_cheia(Fila* f);

// FILA_GET_TAMANHO
// retorna o numero de elementos na fila
int fila_get_tamanho(Fila* f);


// ALTERAR FILAS EXISTENTES

// ENFILEIRAR
// adiciona elemento no final da fila
void enfileirar(Fila* f, void* elemento);

// DESENFILEIRAR
// remove o elemento do início da fila
void* desenfileirar(Fila* f);

#endif