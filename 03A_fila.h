#ifndef FILA_H
#define FILA_H

/* FILA  
==========================================================================
A fila é uma estrutura de dados do tipo FIFO (First In, First Out),
em que o primeiro elemento a ser inserido é o primeiro a ser removido.
No projeto, será utilizada para representar as formas disponíveis no chão.
==========================================================================
*/

typedef struct fila Fila;


// CRIAR OU DESTRUIR FILAS
// função que cria uma fila vazia
Fila* criar_fila(int capacidade);

// função que libera a memória da fila
void destruir_fila(Fila* f);


// ALTERAR FILAS EXISTENTES
// função que adiciona elemento no final da fila
void enfileirar(Fila* f, void* elemento);

// função que remove o elemento do início da fila
void* desenfileirar(Fila* f);


// VERIFICAR SITUAÇÃO DA FILA
// função que verifica se a fila está vazia (retorna 1 caso estiver)
int fila_vazia(Fila* f);

// função que verifica se a fila está cheia (retorna 1 caso estiver)
int fila_cheia(Fila* f);

#endif