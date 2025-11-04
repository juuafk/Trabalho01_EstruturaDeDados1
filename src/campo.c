#include "campo.h"
#include "fila.h"
#include <stdlib.h>
#include <stdio.h>

// ESTRUTURA TRAJETORIA
struct trajetoria {
    double x_inicial;
    double y_inicial;
    double x_final;
    double y_final;
};

// ESTRUTURA POSICAO ESMAGAMENTO
typedef struct posicao {
    double x;
    double y;
} Posicao;

// ESTRUTURA INTERNA CAMPO
struct campo_s {
    Fila* formas_chao;
    Fila* arena;
    Fila* trajetorias;
    Fila* esmagamentos;
};

// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// CAMPO_CRIAR
Campo* campo_criar() {
    Campo* campo = (Campo*) malloc(sizeof(Campo));
    if (campo == NULL) {
        perror("erro ao alocar campo!");
        return NULL;
    }
    
    campo->formas_chao = criar_fila(); 
    campo->arena = criar_fila();
    campo->trajetorias = criar_fila();
    campo->esmagamentos = criar_fila();
    
    if (campo->formas_chao == NULL || campo->arena == NULL || 
        campo->trajetorias == NULL || campo->esmagamentos == NULL) {
        fprintf(stderr, "erro ao criar filas internas do campo!\n");
        if (campo->formas_chao) destruir_fila(campo->formas_chao);
        if (campo->arena) destruir_fila(campo->arena);
        if (campo->trajetorias) destruir_fila(campo->trajetorias);
        if (campo->esmagamentos) destruir_fila(campo->esmagamentos);
        free(campo);
        return NULL;
    }
    return campo;
}

// CAMPO_DESTRUIR
void campo_destruir(Campo* campo) {
    if (campo == NULL) return;
    
    if (campo->formas_chao != NULL) {
        fila_destruir_com_formas(campo->formas_chao); 
    }
    if (campo->arena != NULL) {
        fila_destruir_com_formas(campo->arena); 
    }
    
    if (campo->trajetorias != NULL) {
        while (!fila_vazia(campo->trajetorias)) {
            Trajetoria* t = (Trajetoria*) desenfileirar(campo->trajetorias);
            free(t);
        }
        destruir_fila(campo->trajetorias);
    }
    
    if (campo->esmagamentos != NULL) {
        while (!fila_vazia(campo->esmagamentos)) {
            Posicao* p = (Posicao*) desenfileirar(campo->esmagamentos);
            free(p);
        }
        destruir_fila(campo->esmagamentos);
    }

    free(campo);
}

// FUNÇÕES GETTER
// CAMPO_GET_FORMAS_CHAO
Fila* campo_get_formas_chao(Campo* campo) {
    if (campo == NULL) return NULL;
    return campo->formas_chao;
}

// CAMPO_GET_ARENA
Fila* campo_get_arena(Campo* campo) {
    if (campo == NULL) return NULL;
    return campo->arena;
}

// CAMPO_GET_TRAJETORIAS
Fila* campo_get_trajetorias(Campo* campo) {
    if (campo == NULL) return NULL;
    return campo->trajetorias;
}

// CAMPO_GET_ESMAGAMENTOS
Fila* campo_get_esmagamentos(Campo* campo) {
    if (campo == NULL) return NULL;
    return campo->esmagamentos;
}

// FUNÇÕES PARA ADICIONAR ELEMENTOS
// CAMPO_ADICIONAR_TRAJETORIA
void campo_adicionar_trajetoria(Campo* campo, double x_ini, double y_ini, double x_fin, double y_fin) {
    if (campo == NULL) return;
    
    Trajetoria* t = (Trajetoria*) malloc(sizeof(Trajetoria));
    if (t == NULL) return;
    
    t->x_inicial = x_ini;
    t->y_inicial = y_ini;
    t->x_final = x_fin;
    t->y_final = y_fin;
    
    enfileirar(campo->trajetorias, t);
}

// CAMPO_ADICIONAR_ESMAGAMENTO
void campo_adicionar_esmagamento(Campo* campo, double x, double y) {
    if (campo == NULL) return;
    
    Posicao* p = (Posicao*) malloc(sizeof(Posicao));
    if (p == NULL) return;
    
    p->x = x;
    p->y = y;
    
    enfileirar(campo->esmagamentos, p);
}

// FUNÇÕES GETTER PARA TRAJETÓRIA
// TRAJETORIA_GET_X_INICIAL
double trajetoria_get_x_inicial(Trajetoria* t) {
    if (t == NULL) return 0.0;
    return t->x_inicial;
}

// TRAJETORIA_GET_Y_INICIAL
double trajetoria_get_y_inicial(Trajetoria* t) {
    if (t == NULL) return 0.0;
    return t->y_inicial;
}

// TRAJETORIA_GET_X_FINAL
double trajetoria_get_x_final(Trajetoria* t) {
    if (t == NULL) return 0.0;
    return t->x_final;
}

// TRAJETORIA_GET_Y_FINAL
double trajetoria_get_y_final(Trajetoria* t) {
    if (t == NULL) return 0.0;
    return t->y_final;
}