#include "svg.h"
#include "formas.h"
#include "fila.h"
#include "campo.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ESTRUTURA POSICAO_LOCAL
typedef struct posicao_local {
    double x;
    double y;
} PosicaoLocal;

// DESENHAR_FORMA_INDIVIDUAL
static void desenhar_forma_individual(FILE* svg_file, Forma* f) {
    if (f == NULL) return;

    char tipo = forma_get_tipo(f);
    int id = forma_get_id(f);
    const char* cor_borda = forma_get_cor_borda(f);
    const char* cor_preenchimento = forma_get_cor_preenchimento(f);
    int esmagada = forma_get_esmagada(f);
    
    fprintf(svg_file, "  <g opacity=\"0.5\">\n"); 

    if (esmagada) {
        double w, h, x_esmagada, y_esmagada;

        if (tipo == 'r') {
            forma_get_retangulo_dados(f, &x_esmagada, &y_esmagada, &w, &h);
        } else if (tipo == 't') {
            forma_get_texto_bbox(f, &x_esmagada, &y_esmagada, &w, &h);
        } else if (tipo == 'c') {
            double r = forma_get_r(f);
            double x_ancora = forma_get_x(f);
            double y_ancora = forma_get_y(f);
            x_esmagada = x_ancora - r; y_esmagada = y_ancora - r; w = 2 * r; h = 2 * r;
        } else {
            double x_ancora = forma_get_x(f);
            double y_ancora = forma_get_y(f);
            x_esmagada = x_ancora; y_esmagada = y_ancora; w = 10; h = 10;
        }

        fprintf(svg_file, "      <line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"#FF0000\" stroke-width=\"2\"/>\n", 
                 x_esmagada, y_esmagada, x_esmagada + w, y_esmagada + h);
        fprintf(svg_file, "      <line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"#FF0000\" stroke-width=\"2\"/>\n", 
                 x_esmagada + w, y_esmagada, x_esmagada, y_esmagada + h);
    }

    switch (tipo) {
        case 'c': {
            double x_c = forma_get_x(f); double y_c = forma_get_y(f); double r = forma_get_r(f);
            fprintf(svg_file, "      <circle cx=\"%f\" cy=\"%f\" r=\"%f\" fill=\"%s\" stroke=\"%s\" stroke-width=\"1\" id=\"c%d\"/>\n",
                     x_c, y_c, r, cor_preenchimento, cor_borda, id);
            break;
        }
        case 'r': {
            double x_r, y_r, w, h;
            forma_get_retangulo_dados(f, &x_r, &y_r, &w, &h); 
            fprintf(svg_file, "      <rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" fill=\"%s\" stroke=\"%s\" stroke-width=\"1\" id=\"r%d\"/>\n",
                     x_r, y_r, w, h, cor_preenchimento, cor_borda, id);
            break;
        }
        case 'l': {
            double x1 = forma_get_x1(f); double y1 = forma_get_y1(f);
            double x2 = forma_get_x2(f); double y2 = forma_get_y2(f);
            fprintf(svg_file, "      <line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"%s\" stroke-width=\"2\" id=\"l%d\"/>\n",
                     x1, y1, x2, y2, cor_borda, id); 
            break;
        }
        case 't': {
            double x_t = forma_get_x(f); 
            double y_t = forma_get_y(f);
            const char* texto = forma_get_texto(f);
            const char* ancora_char_ptr = forma_get_ancora(f);
    
            char ancora_char = ancora_char_ptr[0]; 
    
            const char* ancora_svg_str;

            if (ancora_char == 'm') {
                ancora_svg_str = "middle";
            } 
            else if (ancora_char == 'f') {
                ancora_svg_str = "end"; 
            }
            else { 
                ancora_svg_str = "start"; 
            }

            fprintf(svg_file, "      <text x=\"%f\" y=\"%f\" font-family=\"Arial\" font-size=\"12\" fill=\"%s\" stroke=\"%s\" stroke-width=\"0.5\" text-anchor=\"%s\" id=\"t%d\">%s</text>\n",
            x_t, y_t, cor_preenchimento, cor_borda, ancora_svg_str, id, texto);
            break;
        }
        default:
            break;
    }
    fprintf(svg_file, "  </g>\n");
}

// DESENHAR_ASTERISCO
static void desenhar_asterisco(FILE* svg_file, double x, double y) {
    double tamanho = 6.0;
    
    fprintf(svg_file, "  <line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"red\" stroke-width=\"2\"/>\n",
            x, y - tamanho, x, y + tamanho);
    
    fprintf(svg_file, "  <line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"red\" stroke-width=\"2\"/>\n",
            x - tamanho, y, x + tamanho, y);
    
    fprintf(svg_file, "  <line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"red\" stroke-width=\"2\"/>\n",
            x - tamanho * 0.7, y - tamanho * 0.7, x + tamanho * 0.7, y + tamanho * 0.7);
    
    fprintf(svg_file, "  <line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"red\" stroke-width=\"2\"/>\n",
            x - tamanho * 0.7, y + tamanho * 0.7, x + tamanho * 0.7, y - tamanho * 0.7);
}

// DESENHAR_TRAJETORIA
static void desenhar_trajetoria(FILE* svg_file, double x_ini, double y_ini, double x_fin, double y_fin) {
    fprintf(svg_file, "  <line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"blue\" stroke-width=\"1\" stroke-dasharray=\"5,5\"/>\n",
            x_ini, y_ini, x_fin, y_ini);
    
    fprintf(svg_file, "  <line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"blue\" stroke-width=\"1\" stroke-dasharray=\"5,5\"/>\n",
            x_fin, y_ini, x_fin, y_fin);
}

// SVG_DESENHAR
void svg_desenhar(const char* path_saida, Fila* formas_chao, Fila* formas_derrotadas, Campo* campo) {
    if (path_saida == NULL) return;

    FILE* svg_file = fopen(path_saida, "w");
    if (svg_file == NULL) {
        perror("Erro ao abrir o arquivo SVG para escrita :(");
        return;
    }

    fprintf(svg_file, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
    fprintf(svg_file, "<svg xmlns:svg=\"http://www.w3.org/2000/svg\" xmlns=\"http://www.w3.org/2000/svg\" width=\"1000\" height=\"800\">\n");
    
    if (campo != NULL) {
        Fila* trajetorias = campo_get_trajetorias(campo);
        if (trajetorias != NULL) {
            Fila* fila_aux = fila_clonar(trajetorias);
            Trajetoria* t;
            while ((t = desenfileirar(fila_aux)) != NULL) {
                double x_ini = trajetoria_get_x_inicial(t);
                double y_ini = trajetoria_get_y_inicial(t);
                double x_fin = trajetoria_get_x_final(t);
                double y_fin = trajetoria_get_y_final(t);
                desenhar_trajetoria(svg_file, x_ini, y_ini, x_fin, y_fin);
            }
            destruir_fila(fila_aux);
        }
    }
    
    if (formas_derrotadas != NULL) {
        Fila* fila_aux = fila_clonar(formas_derrotadas); 
        Forma* f_derrotada;
        while ((f_derrotada = desenfileirar(fila_aux)) != NULL) {
            desenhar_forma_individual(svg_file, f_derrotada);
        }
        destruir_fila(fila_aux);
    }

    if (formas_chao != NULL) {
        Fila* fila_aux = fila_clonar(formas_chao); 
        Forma* f_chao;
        while ((f_chao = desenfileirar(fila_aux)) != NULL) {
            desenhar_forma_individual(svg_file, f_chao);
        }
        destruir_fila(fila_aux);
    }
    
    if (campo != NULL) {
        Fila* esmagamentos = campo_get_esmagamentos(campo);
        if (esmagamentos != NULL) {
            Fila* fila_aux = fila_clonar(esmagamentos);
            PosicaoLocal* p;
            while ((p = desenfileirar(fila_aux)) != NULL) {
                desenhar_asterisco(svg_file, p->x, p->y);
            }
            destruir_fila(fila_aux);
        }
    }
    
    fprintf(svg_file, "</svg>\n");
    
    fclose(svg_file);
}