#ifndef PILHA_H
#define PILHA_H

/* PILHA
=======================================================================
A pilha é uma estrutura de dados do tipo LIFO (Last In, First Out),
em que o último elemento a ser inserido é o primeiro a ser removido.
No projeto, será utilizada para representar os carregadores de formas.
=======================================================================
*/

typedef struct pilha Pilha;


// CRIAR OU DESTRUIR PILHAS
// função que cria uma pilha vazia
Pilha* criar_pilha(int capacidade);

// função que libera a memória da pilha
void destruir_pilha(Pilha* p);


// ALTERAR PILHAS EXISTENTES
// função que adiciona elemento no topo da pilha
void empilhar(Pilha* p, void* elemento);

// função que remove o elemento do topo da pilha
void* desempilhar(Pilha* p);


// VERIFICAR SITUAÇÃO DA PILHA
// função que verifica se a pilha está vazia (retorna 1 caso estiver)
int pilha_vazia(Pilha* p);

// função que verifica se a pilha está cheia (retorna 1 caso estiver)
int pilha_cheia(Pilha* p);

#endif