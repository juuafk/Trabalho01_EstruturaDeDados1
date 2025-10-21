#include <stdio.h>
#include <stdlib.h>
#include "processamento.h"
#include "formas.h"

// ===============================
// DEFINIÇÃO DA STRUCT DISPARADOR
// ===============================
struct disparador {
    Forma* forma_base;
    Pilha* carregador_esquerda;
    Pilha* carregador_direita;
    char lado_ativo;
};

// ============================================
// IMPLEMENTAÇÃO DAS FUNÇÕES DE GERENCIAMENTO
// ============================================ 

Disparador** inicializar_disparadores(int quantidade) {
    Disparador** d_vetor = (Disparador**) malloc(quantidade * sizeof(Disparador*));
    if (d_vetor == NULL) return NULL;

    for (int i = 0; i < quantidade; i++) {
        d_vetor[i] = NULL;
    }
    return d_vetor;
}

void destruir_disparadores(Disparador** disparadores, int num_disparadores) {
    if (disparadores == NULL) return;

    for (int i = 0; i < num_disparadores; i++) {
        if (disparadores[i] != NULL) {
            destruir_pilha(disparadores[i]->carregador_esquerda);
            destruir_pilha(disparadores[i]->carregador_direita);
            free(disparadores[i]);
        }
    }
    free(disparadores);
}

// ==========================================
// IMPLEMENTAÇÃO DAS FUNÇÕES DE COMANDO .QRY
// ==========================================

void comando_pd(Disparador** disparadores, int num_disparadores, int id, Fila* formas_chao) {
    Forma* forma_alvo = NULL;
    int tamanho_original = fila_get_tamanho(formas_chao);
    Fila* fila_aux = criar_fila(tamanho_original);

    for (int i = 0; i < tamanho_original; i++) {
        Forma* f_atual = (Forma*) desenfileirar(formas_chao);
        if (forma_get_id(f_atual) == id) {
            forma_alvo = f_atual;
        } else {
            enfileirar(fila_aux, f_atual);
        }
    }

    while(!fila_vazia(fila_aux)) {
        enfileirar(formas_chao, desenfileirar(fila_aux));
    }
    destruir_fila(fila_aux);

    if (forma_alvo == NULL) {
        printf("AVISO (pd): Forma com ID %d nao encontrada no chao!\n", id);
        return;
    }

    int indice_vazio = -1;
    for (int i = 0; i < num_disparadores; i++) {
        if (disparadores[i] == NULL) {
            indice_vazio = i;
            break;
        }
    }
    
    if (indice_vazio == -1) {
        printf("AVISO (pd): Nao ha espaco para novos disparadores!\n");
        enfileirar(formas_chao, forma_alvo);
        return;
    }

    Disparador* novo_d = (Disparador*) malloc(sizeof(Disparador));
    novo_d->forma_base = forma_alvo;
    novo_d->carregador_esquerda = criar_pilha(50); 
    novo_d->carregador_direita = criar_pilha(50);
    novo_d->lado_ativo = 'e';

    disparadores[indice_vazio] = novo_d;

    printf("Comando PD: Disparador %d posicionado com sucesso! :D\n", id);
}

// IMPLEMENTAR AINDAAA!!
void comando_lc(Disparador** disparadores, int id_disparador, char lado, int n, Fila* formas_chao) {}
void comando_shft(Disparador* disparador, char lado, int n) {}
void comando_dsp(Disparador* disparador, double dx, double dy, Fila* arena) {}
void comando_rjd(Disparador* disparador, char lado, double dx, double dy, double ix, double iy, Fila* arena) {}
void comando_calc(Fila* arena, double* area_esmagada_total) {}