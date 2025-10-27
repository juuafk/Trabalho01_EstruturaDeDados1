#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cmd.h"   
#include "formas.h"
#include "fila.h"
#include "processamento.h"

// =======================
// LEITOR DO ARQUIVO .GEO
// =======================
int ler_arquivo_geo(const char* arquivo_geo, Fila* formas_chao) {
    FILE* arquivo = fopen(arquivo_geo, "r");
    if (arquivo == NULL) {
        printf("ERRO: nao foi possivel abrir o arquivo .geo: %s\n :(", arquivo_geo);
        return 0; 
    }

    int maior_id_encontrada = 0;

    char linha[1024];
    char comando[8];

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (linha[0] == '\n' || linha[0] == '#') {
            continue;
        }

        sscanf(linha, "%s", comando);
        Forma* nova_forma = NULL;
        int id = 0;

        if (strcmp(comando, "r") == 0) {
            double x, y, w, h;
            char cor_borda[32], cor_preenchimento[32];
            sscanf(linha, "r %d %lf %lf %lf %lf %s %s", &id, &x, &y, &w, &h, cor_borda, cor_preenchimento);
            nova_forma = criar_retangulo(id, x, y, w, h, cor_borda, cor_preenchimento);
        }
        else if (strcmp(comando, "t") == 0) {
            double x1, y1, x2, y2, x3, y3;
            char cor_borda[32], cor_preenchimento[32];
            sscanf(linha, "t %d %lf %lf %lf %lf %lf %lf %s %s", &id, &x1, &y1, &x2, &y2, &x3, &y3, cor_borda, cor_preenchimento);
            nova_forma = criar_triangulo(id, x1, y1, x2, y2, x3, y3, cor_borda, cor_preenchimento);
        }
        else if (strcmp(comando, "c") == 0) {
            double x, y, r;
            char cor_borda[32], cor_preenchimento[32];
            sscanf(linha, "c %d %lf %lf %lf %s %s", &id, &x, &y, &r, cor_borda, cor_preenchimento);
            nova_forma = criar_circulo(id, x, y, r, cor_borda, cor_preenchimento);
        }
        else if (strcmp(comando, "l") == 0) {
            double x1, y1, x2, y2;
            char cor[32];
            sscanf(linha, "l %d %lf %lf %lf %lf %s", &id, &x1, &y1, &x2, &y2, cor);
            nova_forma = criar_linha(id, x1, y1, x2, y2, cor);
        }
        else if (strcmp(comando, "x") == 0) {
            char ancora;
            char cor_borda[32], cor_preenchimento[32], texto[128];
            double x, y;
            sscanf(linha, "x %d %c %lf %lf %s %s %s", &id, &ancora, &x, &y, cor_borda, cor_preenchimento, texto);
            nova_forma = criar_texto(id, ancora, x, y, texto, cor_borda, cor_preenchimento);
        }

        if (nova_forma != NULL) {
            enfileirar(formas_chao, nova_forma);
            if (id > maior_id_encontrada) {
                maior_id_encontrada = id;
            }
        }
    }

    fclose(arquivo);

    return maior_id_encontrada;
}

// =======================
// LEITOR DO ARQUIVO .QRY
// =======================
void ler_arquivo_qry(const char* path_qry, const char* path_svg_qry, Disparador** disparadores, int num_d, 
Carregador** carregadores, int num_c, Fila* formas_chao, Fila* arena, Fila* formas_derrotadas, int* proxima_id_global, FILE* log) {

    FILE* arquivo = fopen(path_qry, "r");
    if (arquivo == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo .qry: %s\n", path_qry);
        return;
    }

    char linha[1024];
    char comando[16];

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (linha[0] == '\n' || linha[0] == '#') {
            continue;
        }

        sscanf(linha, "%s", comando);


        if (log != NULL) {
            fprintf(log, "\nComando: %s", linha);
        }

        if (strcmp(comando, "pd") == 0) {
            int id;
            double x, y;
            sscanf(linha, "pd %d %lf %lf", &id, &x, &y);
            comando_pd(disparadores, num_d, id, x, y, log);
        } 
        else if (strcmp(comando, "lc") == 0) {
            int id_c, n;
            sscanf(linha, "lc %d %d", &id_c, &n);
            comando_lc(carregadores, num_c, id_c, n, formas_chao, log);
        } 
        else if (strcmp(comando, "atch") == 0) {
            int id_d, id_ce, id_cd;
            sscanf(linha, "atch %d %d %d", &id_d, &id_ce, &id_cd);
            comando_atch(disparadores, num_d, carregadores, num_c, id_d, id_ce, id_cd, log);
        } 
        else if (strcmp(comando, "shft") == 0) {
            int id_d;
            char lado;
            sscanf(linha, "shft %d %c", &id_d, &lado);
            comando_shft(disparadores, num_d, id_d, lado, log);
        } 
        else if (strcmp(comando, "dsp") == 0) {
            int id_d;
            double dx, dy;
            sscanf(linha, "dsp %d %lf %lf", &id_d, &dx, &dy);
            comando_dsp(disparadores, num_d, id_d, dx, dy, arena, log);
        } 
        else if (strcmp(comando, "rjd") == 0) {
            int id_d;
            char lado;
            double dx, dy, ix, iy;
            sscanf(linha, "rjd %d %c %lf %lf %lf %lf", &id_d, &lado, &dx, &dy, &ix, &iy);
            comando_rjd(disparadores, num_d, id_d, lado, dx, dy, ix, iy, arena, log);
        } 
        else if (strcmp(comando, "calc") == 0) {
            comando_calc(arena, formas_chao, formas_derrotadas, proxima_id_global, log);
        } 
        else if (strcmp(comando, "mve") == 0) {
            int id_r;
            double x, y;
            sscanf(linha, "mve %d %lf %lf", &id_r, &x, &y);
            comando_mve(formas_chao, id_r, x, y, log);
        } 
        else if (strcmp(comando, "rep") == 0) {
            int id_d, id_c;
            sscanf(linha, "rep %d %d", &id_d, &id_c);
            comando_rep(disparadores, num_d, carregadores, num_c, id_d, id_c, log);
        } 
        else if (strcmp(comando, "exp") == 0) {

            char path_svg_exp[1024];
            strcpy(path_svg_exp, path_svg_qry); 
            
            char* ponto_svg = strstr(path_svg_exp, ".svg");
            if (ponto_svg != NULL) {
                strcpy(ponto_svg, "-d.svg");
            } else {
                strcat(path_svg_exp, "-d.svg"); 
            }
            
            comando_exp(formas_derrotadas, path_svg_exp, log);
        } 
        else if (strcmp(comando, "fim") == 0) {
            if (log != NULL) fprintf(log, "fim do processamento do QRY\n");
            break; 
        }
    }

    fclose(arquivo);
}