#ifndef CAMPO_H
#define CAMPO_H

#include "formas.h" 
#include "fila.h"   

// =========================================
// DEFINIÇÃO DA STRUCT CAMPO (ARENA E CHÃO)
// =========================================
typedef struct campo_s Campo; 

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

#endif