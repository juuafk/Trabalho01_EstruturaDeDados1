#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processamento.h"
#include "formas.h" 
#include "svg.h"


// ===============================
// DEFINIÇÃO DA STRUCT DISPARADOR
// ===============================
struct disparador {
    int id;
    double x;
    double y;
    Carregador* carregador_esquerda; 
    Carregador* carregador_direita;  
    Forma* em_posicao_disparo;    
};

// FUNÇÃO AUXILIAR

// GET_COR_COMPLEMENTAR
// -> associa cores complementares
const char* get_cor_complementar(const char* cor) {
    if (cor == NULL) return "grey";

    if (strcmp(cor, "red") == 0) return "cyan";
    if (strcmp(cor, "cyan") == 0) return "red";

    if (strcmp(cor, "green") == 0) return "magenta";
    if (strcmp(cor, "magenta") == 0) return "green";
    
    if (strcmp(cor, "blue") == 0) return "orange";
    if (strcmp(cor, "orange") == 0) return "blue";
    
    if (strcmp(cor, "black") == 0) return "white";
    if (strcmp(cor, "white") == 0) return "black";
    
    if (strcmp(cor, "yellow") == 0) return "purple";
    if (strcmp(cor, "purple") == 0) return "yellow";

    if (strcmp(cor, "grey") == 0) return "white";
    
    return "grey";
}

// ================================
// FUNCOES DE CRIAÇÃO E DESTRUIÇÃO
// ================================

// INICIALIZAR_DISPARADORES
Disparador** inicializar_disparadores(int quantidade) {
    Disparador** d_vetor = (Disparador**) malloc(quantidade * sizeof(Disparador*));
    if (d_vetor == NULL) return NULL;

    for (int i = 0; i < quantidade; i++) {
        d_vetor[i] = NULL;
    }
    return d_vetor;
}

// DESTRUIR_DISPARADORES
void destruir_disparadores(Disparador** disparadores, int num_disparadores) {
    if (disparadores == NULL) return;

    for (int i = 0; i < num_disparadores; i++) {
        if (disparadores[i] != NULL) {
            free(disparadores[i]);
        }
    }
    free(disparadores);
}

// INICIALIZAR_CARREGADORES
Carregador** inicializar_carregadores(int quantidade) {
    Carregador** c_vetor = (Carregador**) malloc(quantidade * sizeof(Carregador*));
    if (c_vetor == NULL) return NULL;
    
    for (int i = 0; i < quantidade; i++) {
        c_vetor[i] = NULL;
    }
    return c_vetor;
}

// DESTRUIR_CARREGADORES
void destruir_carregadores(Carregador** carregadores, int num_carregadores) {
    if (carregadores == NULL) return;

    for (int i = 0; i < num_carregadores; i++) {
        if (carregadores[i] != NULL) {
            while (!pilha_vazia(carregadores[i])) {
                Forma* f = (Forma*) desempilhar(carregadores[i]);
                destruir_forma(f);
            }
            destruir_pilha(carregadores[i]);
        }
    }
    free(carregadores);
}

// =======================================
// FUNCOES QUE PROCESSAM OS COMANDOS .QRY
// =======================================

// COMANDO_PD
void comando_pd(Disparador** disparadores, int num_d, int id, double x, double y, FILE* log) {
    int indice = id - 1; 

    if (indice < 0 || indice >= num_d) {
        printf("AVISO (pd): ID de disparador %d eh invalido!\n", id);
        if (log) fprintf(log, "AVISO: ID de disparador %d invalido (maximo %d)\n", id, num_d);
        return;
    }

    if (disparadores[indice] != NULL) {
        printf("AVISO (pd): Disparador com ID %d ja existe!\n", id);
        if (log) fprintf(log, "AVISO: Disparador com ID %d ja existe\n", id);
        return;
    }

    Disparador* novo_d = (Disparador*) malloc(sizeof(Disparador));
    novo_d->id = id;
    novo_d->x = x;
    novo_d->y = y;
    novo_d->carregador_esquerda = NULL;
    novo_d->carregador_direita = NULL;
    novo_d->em_posicao_disparo = NULL;

    disparadores[indice] = novo_d;

    printf("Comando PD: Disparador %d posicionado em (%.2f, %.2f)\n", id, x, y);
    if (log) fprintf(log, "Disparador %d posicionado em (%.2f, %.2f)\n", id, x, y);
}

// COMANDO_LC
void comando_lc(Carregador** carregadores, int num_c, int id_c, int n, Fila* formas_chao, FILE* log) {
    int indice = id_c - 1;

    if (indice < 0 || indice >= num_c) {
        printf("AVISO (lc): ID de carregador %d eh invalido!\n", id_c);
        if (log) fprintf(log, "AVISO: ID de carregador %d invalido (maximo %d)\n", id_c, num_c);
        return;
    }

    if (carregadores[indice] != NULL) {
        printf("AVISO (lc): Carregador %d ja foi carregado antes!\n", id_c);
        if (log) fprintf(log, "AVISO: Carregador %d ja existe\n", id_c);
        return;
    }

    Carregador* novo_c = criar_pilha(50);
    carregadores[indice] = novo_c;

    printf("Comando LC: Criando carregador %d e carregando com %d formas...\n", id_c, n);
    if (log) fprintf(log, "Criando carregador %d e carregando %d formas:\n", id_c, n);
    
    int formas_carregadas = 0;
    for (int i = 0; i < n; i++) {
        if (fila_vazia(formas_chao)) {
            printf("AVISO: o chao ficou vazio! nao tem mais formas para carregar\n");
            if (log) fprintf(log, "AVISO: O chao ficou vazio\n");
            break;
        }
        Forma* f = (Forma*) desenfileirar(formas_chao);
        
        if (log) {
            fprintf(log, "Carregada forma ID %d\n", forma_get_id(f));
        }
        
        empilhar(novo_c, f);
        formas_carregadas++;
    }

    printf("%d formas carregadas com sucesso no carregador %d\n", formas_carregadas, id_c);
    if (log) fprintf(log, "Total de %d formas carregadas\n", formas_carregadas);
}

// COMANDO_ATCH
void comando_atch(Disparador** disparadores, int num_d, Carregador** carregadores, int num_c, int id_d, int id_ce, int id_cd, FILE* log) {
    int indice_d = id_d - 1;
    int indice_ce = id_ce - 1;
    int indice_cd = id_cd - 1;

    if (indice_d < 0 || indice_d >= num_d || disparadores[indice_d] == NULL) {
        printf("AVISO (atch): Disparador %d nao existe!\n", id_d);
        if (log) fprintf(log, "AVISO: Disparador %d nao existe!\n", id_d);
        return;
    }
    if (indice_ce < 0 || indice_ce >= num_c || carregadores[indice_ce] == NULL) {
        printf("AVISO (atch): Carregador esquerdo %d nao existe!\n", id_ce);
        if (log) fprintf(log, "AVISO: Carregador esquerdo %d nao existe!\n", id_ce);
        return;
    }
    if (indice_cd < 0 || indice_cd >= num_c || carregadores[indice_cd] == NULL) {
        printf("AVISO (atch): Carregador direito %d nao existe!\n", id_cd);
        if (log) fprintf(log, "AVISO: Carregador direito %d nao existe!\n", id_cd);
        return;
    }


    Disparador* d_alvo = disparadores[indice_d];
    d_alvo->carregador_esquerda = carregadores[indice_ce];
    d_alvo->carregador_direita = carregadores[indice_cd];

    printf("Comando ATCH: Carregadores %d (esq) e %d (dir) encaixados no disparador %d!\n", id_ce, id_cd, id_d);
    if (log) fprintf(log, "Carregador %d (esq) e %d (dir) encaixados no disparador %d.\n", id_ce, id_cd, id_d);
}

// COMANDO_SHFT
void comando_shft(Disparador** disparadores, int num_d, int id_d, char lado, FILE* log) {
    int indice = id_d - 1;

    if (indice < 0 || indice >= num_d || disparadores[indice] == NULL) {
        printf("AVISO (shft): Disparador %d nao existe!\n", id_d);
        if (log) fprintf(log, "AVISO: Disparador %d nao existe!\n", id_d);
        return;
    }
    Disparador* d_alvo = disparadores[indice];

    printf("Comando SHFT: Acionando lado '%c' do disparador %d.\n", lado, id_d);

    Carregador* carregador_fonte = NULL;
    Carregador* carregador_deposito = NULL;

    if (lado == 'd') {
        carregador_fonte = d_alvo->carregador_direita;
        carregador_deposito = d_alvo->carregador_esquerda;
    } else if (lado == 'e') {
        carregador_fonte = d_alvo->carregador_esquerda;
        carregador_deposito = d_alvo->carregador_direita;
    } else {
        printf("AVISO: Lado '%c' invalido! Use 'e' ou 'd'\n", lado);
        if (log) fprintf(log, "AVISO: Lado '%c' invalido\n", lado);
        return;
    }
    
    if (carregador_fonte == NULL || carregador_deposito == NULL) {
        printf("AVISO: Os dois carregadores precisam estar encaixados (atch) para usar shft!\n");
        if (log) fprintf(log, "AVISO: Carregadores nao encaixados\n");
        return;
    }
    if (pilha_vazia(carregador_fonte)) {
        printf("AVISO: Carregador fonte (lado '%c') esta vazio!\n", lado);
        if (log) fprintf(log, "AVISO: Carregador fonte '%c' esta vazio\n", lado);
        return;
    }

    Forma* forma_antiga = d_alvo->em_posicao_disparo;
    Forma* forma_nova = (Forma*) desempilhar(carregador_fonte);

    if (forma_antiga != NULL) {
        empilhar(carregador_deposito, forma_antiga);
        if (log) fprintf(log, "Forma ID %d (antiga) movida para o carregador de deposito\n", forma_get_id(forma_antiga));
    }

    d_alvo->em_posicao_disparo = forma_nova;
    printf("Forma ID %d (nova) agora esta em posicao de disparo!\n", forma_get_id(forma_nova));
    if (log) fprintf(log, "Forma ID %d (nova) em posicao de disparo\n", forma_get_id(forma_nova));
}

// COMANDO_DSP
void comando_dsp(Disparador** disparadores, int num_d, int id_d, double dx, double dy, Fila* arena, FILE* log) {
    int indice = id_d - 1;

    if (indice < 0 || indice >= num_d || disparadores[indice] == NULL) {
        printf("AVISO (dsp): Disparador %d nao existe!\n", id_d);
        if (log) fprintf(log, "AVISO: Disparador %d nao existe!\n", id_d);
        return;
    }
    Disparador* d_alvo = disparadores[indice];

    if (d_alvo->em_posicao_disparo == NULL) {
        printf("AVISO (dsp): Nenhuma forma em posicao de disparo para o disparador %d! Use 'shft'\n", id_d);
        if (log) fprintf(log, "AVISO: Nenhuma forma em posicao de disparo\n");
        return;
    }

    Forma* forma_disparada = d_alvo->em_posicao_disparo;
    d_alvo->em_posicao_disparo = NULL; 
  
    forma_mover(forma_disparada, dx, dy);

    enfileirar(arena, forma_disparada);

    printf("Comando DSP: Disparador %d disparou a forma ID %d para a arena!\n", id_d, forma_get_id(forma_disparada));
    if (log) {
        fprintf(log, "Disparada forma ID %d para a arena com deslocamento (%.2f, %.2f)\n", forma_get_id(forma_disparada), dx, dy);
    }
}

// COMANDO_RJD
void comando_rjd(Disparador** disparadores, int num_d, int id_d, char lado, double dx, double dy, double ix, double iy, Fila* arena, FILE* log) {
    int indice = id_d - 1;

    if (indice < 0 || indice >= num_d || disparadores[indice] == NULL) {
        printf("AVISO (rjd): Disparador %d nao existe!\n", id_d);
        if (log) fprintf(log, "AVISO: Disparador %d nao existe!\n", id_d);
        return;
    }
    Disparador* d_alvo = disparadores[indice];

    Carregador* carregador_alvo = NULL;
    if (lado == 'e') {
        carregador_alvo = d_alvo->carregador_esquerda;
    } else if (lado == 'd') {
        carregador_alvo = d_alvo->carregador_direita;
    } else {
        printf("AVISO (rjd): Lado '%c' invalido!\n", lado);
        if (log) fprintf(log, "AVISO: Lado '%c' invalido\n", lado);
        return;
    }

    if (carregador_alvo == NULL || pilha_vazia(carregador_alvo)) {
        printf("AVISO (rjd): Carregador do lado '%c' nao esta encaixado ou esta vazio!\n", lado);
        if (log) fprintf(log, "AVISO: Carregador '%c' vazio ou nao encaixado\n", lado);
        return;
    }

    printf("Comando RJD: Disparando TODAS as formas do lado '%c' do disparador %d!\n", lado, id_d);
    if (log) fprintf(log, "Rajada (rjd) do lado '%c' do disparador %d:\n", lado, id_d);

    double dx_atual = dx;
    double dy_atual = dy;
    int disparos = 0;

    while(!pilha_vazia(carregador_alvo)) {
        Forma* forma_disparada = (Forma*) desempilhar(carregador_alvo);

        forma_set_posicao(forma_disparada, d_alvo->x + dx_atual, d_alvo->y + dy_atual);

        enfileirar(arena, forma_disparada);
        if (log) fprintf(log, "Disparada forma ID %d para (%.2f, %.2f)\n", forma_get_id(forma_disparada), d_alvo->x + dx_atual, d_alvo->y + dy_atual);
        
        disparos++;
        dx_atual += ix;
        dy_atual += iy;
    }
    
    printf("Rajada concluida! %d formas disparadas para a arena\n", disparos);
    if (log) fprintf(log, "Rajada concluida, %d formas disparadas\n", disparos);
}

// COMANDO_CALC
void comando_calc(Fila* arena, Fila* formas_chao, Fila* formas_derrotadas, int* proxima_id_global, FILE* log) {
    int tamanho = fila_get_tamanho(arena);
    
    if (tamanho < 2) {
        if (tamanho == 1) {
            enfileirar(formas_chao, desenfileirar(arena));
        }
        printf("Comando CALC: Nao tem formas suficientes para colidir\n");
        if (log) fprintf(log, "CALC: Nao tem formas suficientes para colidir\n");
        return;
    }

    Forma** vetor_arena = (Forma**) malloc(tamanho * sizeof(Forma*));
    for (int i = 0; i < tamanho; i++) {
        vetor_arena[i] = (Forma*) desenfileirar(arena);
        forma_set_esmagada(vetor_arena[i], 0); 
    }

    printf("Comando CALC: Processando colisoes...\n");
    if (log) fprintf(log, "Iniciando CALC:\n");

    for (int i = 0; i < tamanho; i++) {
        Forma* f_I = vetor_arena[i];
        if (f_I == NULL) continue; 

        for (int j = i + 1; j < tamanho; j++) {
            Forma* f_J = vetor_arena[j];
            if (f_J == NULL) continue; 

            if (formas_sobrepoem(f_I, f_J)) {
                double area_I = forma_calcular_area(f_I);
                double area_J = forma_calcular_area(f_J);
                if (log) fprintf(log, "Colisao: ID %d (Area %.2f) vs ID %d (Area %.2f)\n", forma_get_id(f_I), area_I, forma_get_id(f_J), area_J);

                if (area_I < area_J) {
                    if (log) fprintf(log, "Vencedor: %d Perdedor: %d (ESMAGADO)\n", forma_get_id(f_J), forma_get_id(f_I));
                    enfileirar(formas_derrotadas, forma_clonar(f_I, forma_get_id(f_I))); 
                    destruir_forma(f_I); 
                    vetor_arena[i] = NULL;
                    break; 
                } 
                else {
                    if (log) fprintf(log, "Vencedor: %d Perdedor: %d (cor alterada)\n", forma_get_id(f_I), forma_get_id(f_J));
                    const char* cor_ataque;
                    if (forma_get_tipo(f_I) == 'l') { 
                        cor_ataque = get_cor_complementar(forma_get_cor_borda(f_I));
                    } else {
                        cor_ataque = forma_get_cor_preenchimento(f_I);
                    }

                    forma_set_cor_borda(f_J, cor_ataque);
                    if (log) fprintf(log, "Cor da borda de %d mudou para %s\n", forma_get_id(f_J), cor_ataque);
                    
                    enfileirar(formas_derrotadas, forma_clonar(f_J, forma_get_id(f_J)));
                    
                    enfileirar(formas_chao, f_J); 
                    vetor_arena[j] = NULL; 

                    Forma* clone = forma_clonar(f_I, (*proxima_id_global)++);
                    forma_trocar_cores(clone);
                    if (log) fprintf(log, "Vencedor %d clonado (nova ID %d) e cores trocadas\n", forma_get_id(f_I), forma_get_id(clone));
                    enfileirar(formas_chao, clone); 
                }
            }
        }
    }

    for (int i = 0; i < tamanho; i++) {
        if (vetor_arena[i] != NULL) {
            enfileirar(formas_chao, vetor_arena[i]);
        }
    }
    
    free(vetor_arena);
}

// COMANDO_MVE
void comando_mve(Fila* formas_chao, int id_r, double x, double y, FILE* log) {
    Fila* fila_aux = criar_fila(fila_get_tamanho(formas_chao));
    Forma* forma_alvo = NULL;
    
    while (!fila_vazia(formas_chao)) {
        Forma* f = (Forma*) desenfileirar(formas_chao);
        if (forma_get_id(f) == id_r) {
            forma_alvo = f;
        } else {
            enfileirar(fila_aux, f);
        }
    }

    if (forma_alvo != NULL) {
        forma_set_posicao(forma_alvo, x, y);
        
        printf("Forma %d movida para (%.2f, %.2f)\n", id_r, x, y);
        if (log) fprintf(log, "Forma %d movida para (%.2f, %.2f)\n", id_r, x, y);
        enfileirar(fila_aux, forma_alvo);
    } else {
        printf("AVISO (mve): Forma %d nao encontrada no chao\n", id_r);
        if (log) fprintf(log, "AVISO (mve): Forma %d nao encontrada no chao\n", id_r);
    }
    
    while (!fila_vazia(fila_aux)) {
        enfileirar(formas_chao, desenfileirar(fila_aux));
    }
    destruir_fila(fila_aux);
}

// COMANDO_REP
void comando_rep(Disparador** disparadores, int num_d, Carregador** carregadores, int num_c, int id_d, int id_c, FILE* log) {
    if (log == NULL) return;
    
    fprintf(log, "-> Relatorio (rep):\n");
    
    // Relatório - disparador
    int indice_d = id_d - 1;
    if (indice_d >= 0 && indice_d < num_d && disparadores[indice_d] != NULL) {
        Disparador* d = disparadores[indice_d];
        fprintf(log, "Disparador %d: Pos=(%.2f, %.2f)\n", id_d, d->x, d->y);
        fprintf(log, "Carregador Esq: %s\n", (d->carregador_esquerda == NULL) ? "NENHUM" : "ENCAIXADO");
        fprintf(log, "Carregador Dir: %s\n", (d->carregador_direita == NULL) ? "NENHUM" : "ENCAIXADO");
        if (d->em_posicao_disparo != NULL) {
            fprintf(log, "Em Posicao: Forma ID %d\n", forma_get_id(d->em_posicao_disparo));
        } else {
            fprintf(log, "Em Posicao: NENHUMA\n");
        }
    } else {
        fprintf(log, "Disparador %d: nao encontrado\n", id_d);
    }
    
    // RELATÓRIO - carregador
    int indice_c = id_c - 1;
    if (indice_c >= 0 && indice_c < num_c && carregadores[indice_c] != NULL) {
        Carregador* c = carregadores[indice_c];
        fprintf(log, "Carregador %d: %d formas carregadas\n", id_c, pilha_get_tamanho(c));
    } else {
        fprintf(log, "Carregador %d: nao encontrado\n", id_c);
    }
}

// COMANDO_EXP
void comando_exp(Fila* formas_derrotadas, const char* path_svg_exp, FILE* log) {
    printf("Comando EXP: Exportando formas derrotadas para %s\n", path_svg_exp);
    if (log) fprintf(log, "Exportando formas derrotadas para %s\n", path_svg_exp);
    
    svg_desenhar(path_svg_exp, formas_derrotadas, NULL); 
}