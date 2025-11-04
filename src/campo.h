#ifndef CAMPO_H
#define CAMPO_H

#include "formas.h" 
#include "fila.h"   

// =========================================
// DEFINIÇÃO DA STRUCT CAMPO (ARENA E CHÃO)
// =========================================
typedef struct campo_s Campo; 

// ===============================
// DEFINIÇÃO DA STRUCT TRAJETÓRIA
// ===============================
typedef struct trajetoria Trajetoria;


// =================================
// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// =================================

// CAMPO_CRIAR
// cria e inicializa uma estrutura campo.
Campo* campo_criar();

// CAMPO_DESTRUIR
// libera toda a memória alocada para um campo e suas sub-estruturas, como formas.
void campo_destruir(Campo* campo);

// ===============
// FUNÇÕES GETTER
// ===============

// CAMPO_GET_FORMAS_CHAO
// retorna a fila de formas que estão no "chão" (formas não derrotadas).
Fila* campo_get_formas_chao(Campo* campo);

// CAMPO_GET_ARENA
// retorna a lista que representa a arena. 
Fila* campo_get_arena(Campo* campo);

// CAMPO_GET_TRAJETORIAS
// retorna a fila de trajetórias
Fila* campo_get_trajetorias(Campo* campo);

// CAMPO_GET_ESMAGAMENTOS
// retorna a fila de posições onde ocorreram esmagamentos
Fila* campo_get_esmagamentos(Campo* campo);

// =================================
// FUNÇÕES PARA ADICIONAR ELEMENTOS
// =================================

// CAMPO_ADICIONAR_TRAJETORIA
// adiciona uma nova trajetória ao campo
void campo_adicionar_trajetoria(Campo* campo, double x_ini, double y_ini, double x_fin, double y_fin);

// CAMPO_ADICIONAR_ESMAGAMENTO
// adiciona uma posição de esmagamento ao campo
void campo_adicionar_esmagamento(Campo* campo, double x, double y);

// =================================
// FUNÇÕES GETTER PARA TRAJETÓRIA
// =================================

// TRAJETORIA_GET_X_INICIAL
// retorna a coordenada x inicial da trajetória
double trajetoria_get_x_inicial(Trajetoria* t);

// TRAJETORIA_GET_Y_INICIAL
// retorna a coordenada y inicial da trajetória
double trajetoria_get_y_inicial(Trajetoria* t);

// TRAJETORIA_GET_X_FINAL
// retorna a coordenada x final da trajetória
double trajetoria_get_x_final(Trajetoria* t);

// TRAJETORIA_GET_Y_FINAL
// retorna a coordenada y final da trajetória
double trajetoria_get_y_final(Trajetoria* t);

#endif