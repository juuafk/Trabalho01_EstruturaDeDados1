#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cmd.h"
#include "formas.h"
#include "fila.h"
#include "processamento.h"
#include "campo.h"

// LER_ARQUIVO_GEO
int ler_arquivo_geo(const char* arquivo_geo, Fila* formas_chao) {
    FILE* arquivo = fopen(arquivo_geo, "r");
    if (arquivo == NULL) {
        printf("ERRO: nao foi possivel abrir o arquivo .geo: %s :(\n", arquivo_geo);
        return 0;
    }

    int maior_id_encontrada = 0;
    int formas_lidas = 0; 
    char linha[1024];
    char comando[8];

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (linha[0] == '\n' || linha[0] == '#') continue;
        
        sscanf(linha, "%s", comando);
        Forma* nova_forma = NULL;
        int id = 0;

        if (strcmp(comando, "r") == 0) {
            double x, y, w, h;
            char cor_borda[32], cor_preenchimento[32];
            sscanf(linha, "r %d %lf %lf %lf %lf %s %s", &id, &x, &y, &w, &h, cor_borda, cor_preenchimento);
            nova_forma = criar_retangulo(id, x, y, w, h, cor_borda, cor_preenchimento);
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
        else if (strcmp(comando, "t") == 0) {
            double x, y;
            char cor_borda[32], cor_preenchimento[32], ancora;
            char texto[256];
            char ancora_str_temp[10];
            int offset = 0;

            int lidos = sscanf(linha, "t %d %lf %lf %s %s %s %n", &id, &x, &y, cor_borda, cor_preenchimento, ancora_str_temp, &offset);
            
            if (lidos == 6 && offset > 0) {
                ancora = ancora_str_temp[0];
                char* texto_inicio = linha + offset;
                
                while (*texto_inicio == ' ' || *texto_inicio == '\t') {
                    texto_inicio++;
                }
                
                strncpy(texto, texto_inicio, sizeof(texto) - 1);
                texto[sizeof(texto) - 1] = '\0';
                
                char* nl = strchr(texto, '\n');
                if (nl) *nl = '\0';
                char* cr = strchr(texto, '\r');
                if (cr) *cr = '\0';
                char ancora_final[2] = {ancora, '\0'};
                
                nova_forma = criar_texto(id, x, y, texto, ancora_final, cor_borda, cor_preenchimento);
            }
        }
        
        if (nova_forma != NULL) {
            enfileirar(formas_chao, nova_forma);
            formas_lidas++;  
            if (id > maior_id_encontrada) {
                maior_id_encontrada = id;
            }
        }
    }
    
    fclose(arquivo);
    
    printf("Arquivo .geo lido: %d formas carregadas no chao\n", formas_lidas);
    
    return maior_id_encontrada;
}

// LER_ARQUIVO_QRY
void ler_arquivo_qry(char* path_qry, char* path_svg_qry, 
    Disparador** disparadores, int num_max_disparadores,
    Carregador** carregadores, int num_max_carregadores, 
    Campo* campo_jogo, 
    Fila* formas_derrotadas,
    int* proxima_id_global, FILE* arquivo_log) {

    FILE* arquivo = fopen(path_qry, "r");
    if (arquivo == NULL) {
        printf("ERRO: nao foi possivel abrir o arquivo .qry: %s\n :(", path_qry);
        return;
    }

    char linha[1024];
    char comando[16];

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (linha[0] == '\n' || linha[0] == '#') continue;

        sscanf(linha, "%s", comando);

        if (arquivo_log != NULL) {
            fprintf(arquivo_log, "\ncomando: %s", linha);
        }
        
        if (strcmp(comando, "pd") == 0) {
            int id;
            double x, y;
            sscanf(linha, "pd %d %lf %lf", &id, &x, &y);
            comando_pd(disparadores, num_max_disparadores, id, x, y, arquivo_log);
        } 
        else if (strcmp(comando, "lc") == 0) {
            int id_c, n;
            sscanf(linha, "lc %d %d", &id_c, &n);
            comando_lc(carregadores, num_max_carregadores, id_c, n, campo_jogo, arquivo_log);
        } 
        else if (strcmp(comando, "atch") == 0) {
            int id_d, id_c_esq, id_c_dir; 
            sscanf(linha, "atch %d %d %d", &id_d, &id_c_esq, &id_c_dir);
            comando_atch(disparadores, num_max_disparadores, carregadores, num_max_carregadores, id_d, id_c_esq, id_c_dir, arquivo_log);
        }
        else if (strcmp(comando, "shft") == 0) {
            int id_d, n; 
            char lado;
            sscanf(linha, "shft %d %c %d", &id_d, &lado, &n); 
            comando_shft(disparadores, num_max_disparadores, id_d, lado, n, arquivo_log);
        } 
else if (strcmp(comando, "dsp") == 0) {
    int id_d;
    double dx, dy;
    char visualizar = '\0';
    
    int lidos = sscanf(linha, "dsp %d %lf %lf %c", &id_d, &dx, &dy, &visualizar);
    
    if (lidos < 3) {
        sscanf(linha, "dsp %d %lf %lf", &id_d, &dx, &dy);
    }
    
    comando_dsp(disparadores, num_max_disparadores, id_d, dx, dy, campo_jogo, visualizar, arquivo_log); 
}
        else if (strcmp(comando, "rjd") == 0) {
            int id_d;
            char lado;
            double dx, dy, ix, iy;
            sscanf(linha, "rjd %d %c %lf %lf %lf %lf", &id_d, &lado, &dx, &dy, &ix, &iy);
            comando_rjd(disparadores, num_max_disparadores, id_d, lado, dx, dy, ix, iy, campo_jogo, arquivo_log); 
        } 
        else if (strcmp(comando, "calc") == 0) {
            comando_calc(campo_jogo, formas_derrotadas, proxima_id_global, arquivo_log); 
        } 
        else if (strcmp(comando, "mve") == 0) {
            int id_r;
            double x, y;
            sscanf(linha, "mve %d %lf %lf", &id_r, &x, &y);
            comando_mve(campo_jogo, id_r, x, y, arquivo_log); 
        } 
        else if (strcmp(comando, "rep") == 0) {
            int id_d, id_c;
            sscanf(linha, "rep %d %d", &id_d, &id_c);
            comando_rep(disparadores, num_max_disparadores, carregadores, num_max_carregadores, id_d, id_c, arquivo_log);
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
            
            comando_exp(formas_derrotadas, path_svg_exp, arquivo_log);
        } 
        else if (strcmp(comando, "fim") == 0) {
            if (arquivo_log != NULL) fprintf(arquivo_log, "fim do processamento do QRY!\n");
            break; 
        }
    }

    fclose(arquivo);
}