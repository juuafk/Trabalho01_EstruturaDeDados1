#include "campo.h"
#include "fila.h"
#include <stdlib.h>
#include <stdio.h>

// ESTRUTURA INTERNA CAMPO
struct campo_s {
    Fila* formas_chao;
    Fila* arena;
};

// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// CAMPO_CRIAR
Campo* campo_criar() {
    Campo* campo = (Campo*) malloc(sizeof(Campo));
    if (campo == NULL) {
        perror("erro ao alocar campo!");
        return NULL;
    }
    
    campo->formas_chao = criar_fila(500); 
    campo->arena = criar_fila(500);        
    
    if (campo->formas_chao == NULL || campo->arena == NULL) {
        fprintf(stderr, "erro ao criar filas internas do campo!\n");
        if (campo->formas_chao) destruir_fila(campo->formas_chao);
        if (campo->arena) destruir_fila(campo->arena);
        free(campo);

        return NULL;
    }
    return campo;
}

// CAMPO_DESTRUIR
void campo_destruir(Campo* campo) {
    if (campo == NULL) return;
    
    if (campo->formas_chao != NULL) {
        destruir_fila(campo->formas_chao); 
    }
    if (campo->arena != NULL) {
        destruir_fila(campo->arena); 
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