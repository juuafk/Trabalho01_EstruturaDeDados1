#ifndef PROCESSAMENTO_H
#define PROCESSAMENTO_H

#include <stdio.h> 
#include "formas.h"
#include "pilha.h"
#include "fila.h"
#include "campo.h"

// =======================================
// MÓDULO DE PROCESSAMENTO
// ---------------------------------------
// FUNÇÃO: gerir o estado lógico do jogo.
// =======================================

// DISPARADOR
typedef struct disparador Disparador;

// CARREGADOR (uma pilha)
typedef Pilha Carregador; 

// ================================
// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// ================================

// INICIALIZAR_DISPARADORES
// -> inicializa a quantidade especificada de disparadores, que irão atirar formas na arena,
// -> retorna: um ponteiro apontando para a sequência criada.
Disparador** inicializar_disparadores(int quantidade);

// DESTRUIR_DISPARADORES
// -> libera toda a memória alocada para a quantidade de disparadores especificada.
void destruir_disparadores(Disparador** disparadores, int num_disparadores);

// INICIALIZAR_CARREGADORES
// -> inicializa a quantidade especificada de carregadores, que irão ser como pentes de munição.
// -> retorna: um ponteiro apontando para a sequência criada.
Carregador** inicializar_carregadores(int quantidade);

// DESTRUIR_CARREGADORES
// -> libera toda memória alocada pra os carregadores, além das formas 
// contidas neles.
void destruir_carregadores(Carregador** carregadores, int num_carregadores);

// ========================================
// FUNÇÕES QUE PROCESSAM OS COMANDOS .QRY
// ========================================

// PD: posiciona um novo disparador no mapa com ID e nas coordenadas (x, y).
void comando_pd(Disparador** disparadores, int num_d, int id, double x, double y, FILE* log);

// LC: cria um novo carregador, o enche com 'n' formas do chão.
void comando_lc(Carregador** carregadores, int num_c, int id_c, int n, Campo* c, FILE* log);

// ATCH: encaixa dois carregadores existentes (esquerdo e direito) num disparador que já existe.
void comando_atch(Disparador** disparadores, int num_max_disparadores, Carregador** carregadores, int num_max_carregadores, 
int id_d, int id_c_esq, int id_c_dir, FILE* log);

// SHFT: move uma forma do topo de um dos carregadores para a posição de disparo.
void comando_shft(Disparador** disparadores, int num_max_disparadores, int id_d, char lado, int n, FILE* log);

// DSP: dispara a forma na posição de disparo para a fila da arena.
void comando_dsp(Disparador** disparadores, int num_d, int id_d, double dx, double dy, Campo* c, char visualizar, FILE* log);

// RJD: dispara um carregador inteiro na arena.
void comando_rjd(Disparador** disparadores, int num_max_disparadores, 
int id_d, char lado, double dx, double dy, double ix, double iy, Campo* c, FILE* log); 

// CALC: processa todas as colisões na arena, utilizando o ID global para novos clones.
void comando_calc(Campo* c, Fila* formas_derrotadas, int* proxima_id_global, FILE* log);

// MVE: move uma forma que está no chão para uma nova coordenada.
void comando_mve(Campo* c, int id_r, double x, double y, FILE* log);

// REP: escreve um relatório de estado de um disparador e um carregador no arquivo de log (.txt).
void comando_rep(Disparador** disparadores, int num_d, Carregador** carregadores, int num_c, int id_d, int id_c, FILE* log);

// EXP: gera um novo arquivo .svg contendo apenas as formas que foram derrotadas nas colisões.
void comando_exp(Fila* formas_derrotadas, const char* path_svg_exp, FILE* log);

#endif