#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processamento.h"
#include "formas.h" 
#include "svg.h"
#include "campo.h"    
#include "pilha.h"
#include "fila.h"

#define MAX_COR_LEN 8
#define MAX_TEXTO_LEN 50
#define MAX_ANCORA_LEN 10 

// PONTUAÇÃO DO JOGO
static double pontuacao_total_acumulada = 0.0;

// ESTRUTURA DISPARADOR
struct disparador {
    int id;
    double x;
    double y;
    Carregador* carregador_esquerda; 
    Carregador* carregador_direita; 
    Forma* em_posicao_disparo;
};

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
void comando_lc(Carregador** carregadores, int num_c, int id_c, int n, Campo* c, FILE* log) {
    Fila* formas_chao = campo_get_formas_chao(c);

    int indice = id_c - 1;
    if (indice < 0 || indice >= num_c) {
        printf("AVISO (lc): ID de carregador %d invalido (fora dos limites 1...%d)\n", id_c, num_c);
        if (log) fprintf(log, "AVISO (lc): ID %d invalido\n", id_c);
        return;
    }

    if (carregadores[indice] == NULL) {
        carregadores[indice] = criar_pilha(n + 10);
        if (log) fprintf(log, "Criando novo carregador (pilha) %d\n", id_c);
    }

    Carregador* c_alvo = carregadores[indice];
    int carregadas = 0;

    if (log) fprintf(log, "Comando LC: Carregando %d formas no carregador %d...\n", n, id_c);

    for (int i = 0; i < n; i++) {
        if (fila_vazia(formas_chao)) {
            printf("AVISO (lc): Fila 'formas_chao' esvaziou antes de carregar %d formas\n", n);
            if (log) fprintf(log, "AVISO (lc): Formas_chao esvaziou\n");
            break;
        }

        Forma* forma = (Forma*) desenfileirar(formas_chao);
        
        empilhar(c_alvo, forma); 

        carregadas++;
    }
    
    printf("Comando LC: %d formas carregadas no Carregador %d\n", carregadas, id_c);
    if (log) fprintf(log, "Comando LC concluido: %d formas empilhadas\n", carregadas);
}

// COMANDO_ATCH
void comando_atch(Disparador** disparadores, int num_max_disparadores, 
Carregador** carregadores, int num_max_carregadores, int id_d, int id_c_esq, int id_c_dir, FILE* log) {
    
    int indice_d = id_d - 1;
    int indice_ce = id_c_esq - 1; 
    int indice_cd = id_c_dir - 1; 

    if (indice_d < 0 || indice_d >= num_max_disparadores || disparadores[indice_d] == NULL) {
        printf("AVISO (atch): Disparador %d nao existe!\n", id_d);
        if (log) fprintf(log, "AVISO (atch): Disparador %d nao existe!\n", id_d);
        return;
    }
    Disparador* d_alvo = disparadores[indice_d];

    // lado esquerdo
    if (id_c_esq != -1) { 
        if (indice_ce < 0 || indice_ce >= num_max_carregadores) {
            printf("AVISO (atch): ID carregador esquerdo %d invalido (fora dos limites 1...%d)!\n", 
                   id_c_esq, num_max_carregadores);
            return;
        }
        
        if (carregadores[indice_ce] == NULL) {
            carregadores[indice_ce] = criar_pilha(50);
            if (log) fprintf(log, "Carregador %d criado vazio (nao foi carregado com 'lc')\n", id_c_esq);
            printf("AVISO (atch): Carregador esquerdo %d criado vazio (use 'lc' para carregar formas)\n", id_c_esq);
        }
        
        d_alvo->carregador_esquerda = carregadores[indice_ce];
        if (log) fprintf(log, "Carregador %d encaixado na esquerda\n", id_c_esq);
    }

    // lado direito
    if (id_c_dir != -1) {
        if (indice_cd < 0 || indice_cd >= num_max_carregadores) {
            printf("AVISO (atch): ID carregador direito %d invalido (fora dos limites 1...%d)!\n", 
                   id_c_dir, num_max_carregadores);
            return;
        }

        if (carregadores[indice_cd] == NULL) {
            carregadores[indice_cd] = criar_pilha(50);
            if (log) fprintf(log, "Carregador %d criado vazio (nao foi carregado com 'lc')\n", id_c_dir);
            printf("AVISO (atch): Carregador direito %d criado vazio (use 'lc' para carregar formas)\n", id_c_dir);
        }
        
        d_alvo->carregador_direita = carregadores[indice_cd];
        if (log) fprintf(log, "Carregador %d encaixado na direita\n", id_c_dir);
    }

    printf("Comando ATCH: carregadores encaixados no disparador %d\n", id_d);
    if (log) fprintf(log, "Comando ATCH para disparador %d concluido\n", id_d);
}

// COMANDO_SHFT
void comando_shft(Disparador** disparadores, int num_d, int id_d, char lado, int n, FILE* log) {
    int indice = id_d - 1;

    // validação disparador
    if (indice < 0 || indice >= num_d || disparadores[indice] == NULL) {
        printf("AVISO (shft): Disparador %d nao existe!\n", id_d);
        if (log) fprintf(log, "AVISO: Disparador %d nao existe!\n", id_d);
        return;
    }
    Disparador* d_alvo = disparadores[indice];

    printf("Comando SHFT: Acionando '%c' (direcao) %d vezes no disparador %d\n", lado, n, id_d);

    Carregador* carregador_fonte = NULL;
    Carregador* carregador_deposito = NULL;

    if (lado == 'e') {
        carregador_fonte = d_alvo->carregador_direita;      
        carregador_deposito = d_alvo->carregador_esquerda;  
    } else if (lado == 'd') {
        carregador_fonte = d_alvo->carregador_esquerda;    
        carregador_deposito = d_alvo->carregador_direita;  
    } else {
        printf("AVISO: lado '%c' invalido! Use 'e' ou 'd'\n", lado);
        if (log) fprintf(log, "AVISO: lado '%c' invalido\n", lado);
        return;
    }
    
    // validação carregadores
    if (carregador_fonte == NULL || carregador_deposito == NULL) {
        printf("AVISO: Os dois carregadores precisam estar encaixados (atch) para usar shft!\n");
        if (log) fprintf(log, "AVISO: arregadores nao encaixados\n");
        return; 
    }

    if (log) fprintf(log, "Movimentando %d municoes para o lado '%c'\n", n, lado);

    int movidas = 0;
    for (int i = 0; i < n; i++) {
        if (pilha_vazia(carregador_fonte)) {
            printf("AVISO: Carregador fonte esvaziou na iteracao %d!\n", i + 1);
            if (log) fprintf(log, "AVISO: Carregador fonte esvaziou (iteracao %d de %d)\n", i + 1, n);
            break; 
        }

        Forma* forma_antiga = d_alvo->em_posicao_disparo;
        Forma* forma_nova = (Forma*) desempilhar(carregador_fonte);

        if (forma_antiga != NULL) {
            empilhar(carregador_deposito, forma_antiga);
            if (log) fprintf(log, "Iteracao %d: Forma ID %d movida para o deposito '%c'\n", 
                           i + 1, forma_get_id(forma_antiga), lado);
        }

        d_alvo->em_posicao_disparo = forma_nova;
        if (log) fprintf(log, "Iteracao %d: Forma ID %d movida para posicao de disparo\n", 
                       i + 1, forma_get_id(forma_nova));
        
        movidas++;
    }
    
    printf("Comando SHFT concluido: %d formas movimentadas!\n", movidas);
    if (log) fprintf(log, "Comando SHFT concluido (%d de %d solicitadas)\n", movidas, n);
}

// COMANDO_DSP
void comando_dsp(Disparador** disparadores, int num_d, int id_d, double dx, double dy, Campo* c, char visualizar, FILE* log) {
    Fila* arena = campo_get_arena(c);
    int indice = id_d - 1;

    // validação disparador
    if (indice < 0 || indice >= num_d || disparadores[indice] == NULL) {
        printf("AVISO (dsp): Disparador %d nao existe!\n", id_d);
        if (log) fprintf(log, "AVISO (dsp): Disparador %d nao existe!\n", id_d);
        return;
    }
    Disparador* d_alvo = disparadores[indice]; 

    Forma* forma_para_disparar = d_alvo->em_posicao_disparo;
    if (forma_para_disparar == NULL) {
        printf("AVISO (dsp): Nenhuma forma em posicao de disparo para o disparador %d! Use 'shft'\n", id_d);
        if (log) fprintf(log, "AVISO (dsp): Nenhuma forma em posicao de disparo (arma vazia)\n");
        return;
    }

    forma_trocar_cores(forma_para_disparar);

    forma_set_vetor(forma_para_disparar, dx, dy);
    double x_inicial = d_alvo->x; 
    double y_inicial = d_alvo->y;
    
    double x_final = x_inicial + dx;
    double y_final = y_inicial + dy;
    
    forma_set_posicao(forma_para_disparar, x_final, y_final);
    
    if (visualizar == 'v') {
        campo_adicionar_trajetoria(c, x_inicial, y_inicial, x_final, y_final);
        if (log) fprintf(log, "Trajetoria visualizada de (%.2f, %.2f) ate (%.2f, %.2f)\n", x_inicial, y_inicial, x_final, y_final);
    }
    
    enfileirar(arena, forma_para_disparar);
    d_alvo->em_posicao_disparo = NULL;

    printf("Comando DSP: Disparador %d disparou forma ID %d de (%.2f, %.2f) com vetor (%.2f, %.2f)\n", 
    id_d, forma_get_id(forma_para_disparar), x_inicial, y_inicial, dx, dy);

    if (log) fprintf(log, "forma ID %d disparada para a arena em (%.2f, %.2f)\n", 
    forma_get_id(forma_para_disparar), x_final, y_final);
}

// COMANDO_RJD
void comando_rjd(Disparador** disparadores, int num_max_disparadores, int id_d, char lado, 
double dx, double dy, double ix, double iy, Campo* c, FILE* log) {
    
    Fila* arena = campo_get_arena(c);
    int indice = id_d - 1;

    // validações
    if (indice < 0 || indice >= num_max_disparadores || disparadores[indice] == NULL) {
        printf("AVISO (rjd): Disparador %d nao existe!\n", id_d);
        if (log) fprintf(log, "AVISO (rjd): Disparador %d nao existe!\n", id_d);
        return;
    }

    Disparador* d_alvo = disparadores[indice];
    Carregador* carregador_fonte = NULL;

    if (lado == 'e') {
        carregador_fonte = d_alvo->carregador_direita;
    } else if (lado == 'd') {
        carregador_fonte = d_alvo->carregador_esquerda;
    } else {
        printf("AVISO (rjd): Lado '%c' invalido! Use 'e' ou 'd'\n", lado);
        if (log) fprintf(log, "AVISO (rjd): Lado '%c' invalido\n", lado);
        return;
    }

    if (carregador_fonte == NULL || pilha_vazia(carregador_fonte)) {
        printf("AVISO (rjd): Carregador '%c' nao encaixado ou vazio no disparador %d!\n", lado, id_d);
        if (log) fprintf(log, "AVISO (rjd): Carregador nao encaixado ou vazio\n");
        return;
    }

    double current_x = d_alvo->x;
    double current_y = d_alvo->y;

    current_x += dx; 
    current_y += dy;
    int disparos_feitos = 0;

    int total_formas = pilha_get_tamanho(carregador_fonte);
    
    Forma** formas_array = (Forma**) malloc(total_formas * sizeof(Forma*));
    
    for (int i = total_formas - 1; i >= 0; i--) {
        formas_array[i] = (Forma*) desempilhar(carregador_fonte);
    }
    
    for (int i = total_formas - 1; i >= 0; i--) {
        Forma* forma_para_disparar = formas_array[i];
        
        forma_trocar_cores(forma_para_disparar);
        
        forma_set_vetor(forma_para_disparar, dx, dy);
        forma_set_posicao(forma_para_disparar, current_x, current_y);
        
        enfileirar(arena, forma_para_disparar);
        
        if (log) {
            fprintf(log, "Rajada do disparador %d: Forma ID %d para (%.2f, %.2f) com Vetor (%.2f, %.2f)!\n", 
                    id_d, forma_get_id(forma_para_disparar), current_x, current_y, dx, dy);
        }
        
        current_x += ix;
        current_y += iy;
        disparos_feitos++;
    }
    
    free(formas_array);

    printf("Comando RJD: Disparador %d executou rajada de %d formas para o lado '%c'\n", 
           id_d, disparos_feitos, lado);
    if (log) {
        fprintf(log, "Comando RJD concluido: %d formas disparadas\n", disparos_feitos);
    }
}

// COMANDO_CALC
void comando_calc(Campo* c, Fila* formas_derrotadas, int* proxima_id_global, FILE* log) {
    Fila* arena = campo_get_arena(c);
    Fila* formas_chao = campo_get_formas_chao(c);

    int tamanho = fila_get_tamanho(arena);

    if (tamanho < 2) {
        if (tamanho == 1) {
            Forma* f = (Forma*) desenfileirar(arena);
            forma_trocar_cores(f);
            enfileirar(formas_chao, f);
        }
        printf("Comando CALC: Nao tem formas suficientes para colidir\n");
        if (log) fprintf(log, "CALC: Nao tem formas suficientes para colidir\n");
        
        if (log) fprintf(log, "CALC concluido! PONTUACAO TOTAL: %.2f\n", pontuacao_total_acumulada);
        return;
    }

    Fila* arena_aux = criar_fila();
    for (int i = 0; i < tamanho; i++) {
        Forma* f_I = (Forma*) desenfileirar(arena);
        enfileirar(arena_aux, f_I);
    }
    tamanho = fila_get_tamanho(arena_aux);

    Forma** vetor_arena = (Forma**) malloc(tamanho * sizeof(Forma*));
    for (int i = 0; i < tamanho; i++) {
        vetor_arena[i] = (Forma*) desenfileirar(arena_aux);
    }
    destruir_fila(arena_aux);

    printf("Comando CALC: processando colisoes...\n");
    if (log) fprintf(log, "Iniciando CALC:\n");

    double area_destruida_neste_round = 0.0;
    char cor_temp_buffer[MAX_COR_LEN]; 

    for (int i = 0; i < tamanho - 1; i += 2) {
        Forma* f_I = vetor_arena[i];
        Forma* f_J = vetor_arena[i + 1];

        if (f_I == NULL || f_J == NULL) continue;

        forma_set_esmagada(f_I, 0); 
        forma_set_esmagada(f_J, 0);

        if (formas_sobrepoem(f_I, f_J)) {
            double area_I = forma_calcular_area(f_I);
            double area_J = forma_calcular_area(f_J);

            if (log) fprintf(log, "Colisao: ID %d (Area %.2f) vs ID %d (Area %.2f)\n", 
                             forma_get_id(f_I), area_I, forma_get_id(f_J), area_J);

            if (area_I < area_J) { 
                if (log) fprintf(log, "Vencedor: %d Perdedor: %d (ESMAGADO)\n", 
                                 forma_get_id(f_J), forma_get_id(f_I));

                pontuacao_total_acumulada += area_I; 
                area_destruida_neste_round += area_I; 

                double x_esmag, y_esmag;
                forma_get_posicao(f_I, &x_esmag, &y_esmag);
                campo_adicionar_esmagamento(c, x_esmag, y_esmag);

                forma_set_esmagada(f_I, 1);
                enfileirar(formas_derrotadas, forma_clonar(f_I, forma_get_id(f_I))); 
                destruir_forma(f_I); 
                vetor_arena[i] = NULL;

                forma_trocar_cores(f_J); 
                enfileirar(formas_chao, f_J);
                vetor_arena[i + 1] = NULL;

            } 
            else if (area_I > area_J) { 
                if (log) fprintf(log, "Vencedor: %d Perdedor: %d (VOLTA AO CHAO, COR ALTERADA)\n", 
                                 forma_get_id(f_I), forma_get_id(f_J));
                
                const char* cor_do_vencedor = get_cor_ganhadora(f_I, cor_temp_buffer);
                forma_set_cor_borda(f_J, cor_do_vencedor);
                                 
                forma_trocar_cores(f_I); 
                enfileirar(formas_chao, f_I);
                vetor_arena[i] = NULL;
                
                forma_trocar_cores(f_J);
                enfileirar(formas_chao, f_J);
                vetor_arena[i + 1] = NULL;
            } 
            else { 
                if (log) fprintf(log, "Empate: ambos voltam ao chao (areas iguais)\n");
                
                forma_trocar_cores(f_I); 
                enfileirar(formas_chao, f_I);
                vetor_arena[i] = NULL;
                
                forma_trocar_cores(f_J);
                enfileirar(formas_chao, f_J);
                vetor_arena[i + 1] = NULL;
            }
        } 
        else { 
            if (log) fprintf(log, "Nao Sobrepoem: ID %d vs ID %d\n", forma_get_id(f_I), forma_get_id(f_J));

            forma_trocar_cores(f_I);
            enfileirar(formas_chao, f_I);
            vetor_arena[i] = NULL;
            
            forma_trocar_cores(f_J);
            enfileirar(formas_chao, f_J);
            vetor_arena[i + 1] = NULL;
        }
    }

    for (int i = 0; i < tamanho; i++) {
        if (vetor_arena[i] != NULL) {
            forma_trocar_cores(vetor_arena[i]); 
            enfileirar(formas_chao, vetor_arena[i]);
        }
    }

    free(vetor_arena);
    
    if (log) {
        fprintf(log, "CALC: Area total destruida na rodada: %.2f\n", area_destruida_neste_round);
        fprintf(log, "CALC concluido! PONTUACAO TOTAL: %.2f\n", pontuacao_total_acumulada);
    }
}

// COMANDO_MVE
void comando_mve(Campo* c, int id_r, double x, double y, FILE* log) {
    Fila* formas_chao = campo_get_formas_chao(c);

    Fila* fila_aux = criar_fila(fila_get_tamanho(formas_chao));
    Forma* forma_alvo = NULL;
    
    while (!fila_vazia(formas_chao)) {
        Forma* f = (Forma*) desenfileirar(formas_chao);
        if (forma_get_id(f) == id_r) {
            forma_alvo = f;
        } 
        else {
            enfileirar(fila_aux, f);
        }
    }

    if (forma_alvo != NULL) {
        forma_set_posicao(forma_alvo, x, y);
        
        printf("Forma %d movida para (%.2f, %.2f)\n", id_r, x, y);
        if (log) fprintf(log, "Forma %d movida para (%.2f, %.2f)\n", id_r, x, y);
        enfileirar(fila_aux, forma_alvo);
    }
     else {
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
    
    fprintf(log, "Relatorio (rep):\n");
    
    // RELATÓRIO - disparador
    int indice_d = id_d - 1;
    if (indice_d >= 0 && indice_d < num_d && disparadores[indice_d] != NULL) {
        Disparador* d = disparadores[indice_d];
        fprintf(log, "Disparador %d: Pos=(%.2f, %.2f)\n", id_d, d->x, d->y);
        fprintf(log, "Carregador Esq: %s\n", (d->carregador_esquerda == NULL) ? "NENHUM" : "ENCAIXADO");
        fprintf(log, "Carregador Dir: %s\n", (d->carregador_direita == NULL) ? "NENHUM" : "ENCAIXADO");
        if (d->em_posicao_disparo != NULL) {
            fprintf(log, "Em Posicao: Forma ID %d\n", forma_get_id(d->em_posicao_disparo));
        }
         else {
            fprintf(log, "Em Posicao: NENHUMA\n");
        }
    } 
    else {
        fprintf(log, "Disparador %d: nao encontrado\n", id_d);
    }
    
    // RELATÓRIO - carregador
    int indice_c = id_c - 1;
    if (indice_c >= 0 && indice_c < num_c && carregadores[indice_c] != NULL) {
        Carregador* c = carregadores[indice_c];
        fprintf(log, "Carregador %d: %d formas carregadas\n", id_c, pilha_get_tamanho(c));
    } 
    else {
        fprintf(log, "Carregador %d: nao encontrado\n", id_c);
    }
}

// COMANDO_EXP
void comando_exp(Fila* formas_derrotadas, const char* path_svg_exp, FILE* log) {
    printf("Comando EXP: Exportando formas derrotadas para %s\n", path_svg_exp);
    if (log) fprintf(log, "Exportando formas derrotadas para %s\n", path_svg_exp);
    
    svg_desenhar(path_svg_exp, NULL, formas_derrotadas, NULL); 
}