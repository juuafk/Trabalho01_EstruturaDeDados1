#ifndef PROCESSAMENTO_H
#define PROCESSAMENTO_H

#include "formas.h"
#include "pilha.h"
#include "fila.h"

/* PROCESSAMENTO
------------------------------------------
FUNÇÃO DO ARQUIVO: gerenciar o estado
do jogo e executar as ações que modificam 
esse estado.
------------------------------------------
*/

typedef struct disparador Disparador;

//CRIAR E DESTRUIR DIPARADORES
/* Função que cria e inicializa um array de disparadores,
com a quantidade especificada no paramêtro (começando no 0),
sendo que cada disparador já conterá seus dois 
carregadores (pilhas) internos e vazios.
*/
Disparador** inicializar_disparadores(int quantidade);

/* Função que libera toda a memória alocada para a quantidade
de disparadores especificada.
*/
void destruir_disparadores(Disparador** disparadores, int num_disparadores);


/* 
======================================
FUNÇÕES QUE PROCESSAM OS COMANDOS .QRY
======================================
*/

// pd - Posiciona um disparador nas coordenadas x, y
void comando_pd(Disparador** disparadores, int id, double x, double y);

// lc - Carrega o carregador de um lado específico ('e' ou 'd') de um disparador com n formas do chão 
void comando_lc(Disparador** disparadores, int id_disparador, char lado, int n, Fila* formas_chao);

// shft - Pressiona o botão de seleção de carga (ativa o mecanismo de troca) 
void comando_shft(Disparador* disparador, char lado, int n);

// dsp - Dispara a forma atualmente em posição de disparo 
void comando_dsp(Disparador* disparador, double dx, double dy, Fila* arena);

// rjd - Executa uma rajada de disparos
void comando_rjd(Disparador* disparador, char lado, double dx, double dy, double ix, double iy, Fila* arena);

// calc - Processa as colisões na arena e atualiza a área total esmagada
void comando_calc(Fila* arena, double* area_esmagada_total);

#endif