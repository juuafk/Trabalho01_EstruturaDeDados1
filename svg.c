#include <stdio.h>
#include <stdlib.h>
#include "svg.h"
#include "formas.h"
#include "fila.h"

// ======================
// FUNÇÃO DESENHAR FORMA
// ======================
static void desenhar_forma_individual(FILE* svg_arquivo, Forma* forma) {
    if (forma == NULL) return;

    char tipo = forma_get_tipo(forma);
    const char* cor_borda = forma_get_cor_borda(forma);
    const char* cor_preenchimento = forma_get_cor_preenchimento(forma);

    switch (tipo) {
        case 'c': {
            double x, y, r;
            forma_get_circulo_dados(forma, &x, &y, &r);
            fprintf(svg_arquivo, "\t<circle cx=\"%f\" cy=\"%f\" r=\"%f\" stroke=\"%s\" fill=\"%s\" />\n",
                    x, y, r, cor_borda, cor_preenchimento);
            break;
        }
        case 'r': {
            double x, y, w, h;
            forma_get_retangulo_dados(forma, &x, &y, &w, &h);
            fprintf(svg_arquivo, "\t<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" stroke=\"%s\" fill=\"%s\" />\n",
                    x, y, w, h, cor_borda, cor_preenchimento);
            break;
        }
        case 't': {
            double x1, y1, x2, y2, x3, y3;
            forma_get_triangulo_dados(forma, &x1, &y1, &x2, &y2, &x3, &y3);
            fprintf(svg_arquivo, "\t<polygon points=\"%f,%f %f,%f %f,%f\" stroke=\"%s\" fill=\"%s\" />\n",
                    x1, y1, x2, y2, x3, y3, cor_borda, cor_preenchimento);
            break;
        }
        case 'l': {
            double x1, y1, x2, y2;
            forma_get_linha_dados(forma, &x1, &y1, &x2, &y2);
            fprintf(svg_arquivo, "\t<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"%s\" />\n",
                    x1, y1, x2, y2, cor_borda);
            break;
        }
        case 'x': {
            double x, y;
            char ancora_char;
            const char* texto_conteudo;
            forma_get_texto_dados(forma, &x, &y, &ancora_char, &texto_conteudo); 

            const char* ancora_svg = "start";
            if (ancora_char == 'm') ancora_svg = "middle";
            if (ancora_char == 'f') ancora_svg = "end";

            fprintf(svg_arquivo, "\t<text x=\"%f\" y=\"%f\" stroke=\"%s\" fill=\"%s\" text-anchor=\"%s\" font-family=\"Arial\" font-size=\"10\">%s</text>\n",
                    x, y, cor_borda, cor_preenchimento, ancora_svg, texto_conteudo);
            break;
        }
    }
}

// =======================================
// FUNÇÕES PÚBLICAS (DECLARADAS EM SVG.H)
// =======================================

FILE* svg_iniciar(const char* nome_arquivo_svg) {
    FILE* arquivo = fopen(nome_arquivo_svg, "w");
    if (arquivo == NULL) {
        printf("ERRO: Nao foi possivel criar o arquivo SVG: %s\n", nome_arquivo_svg);
        return NULL;
    }
    fprintf(arquivo, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"100%%\" height=\"100%%\">\n");
    fprintf(arquivo, "\t<rect width=\"100%%\" height=\"100%%\" fill=\"white\" />\n");
    return arquivo;
}

void svg_desenhar_fila_de_formas(FILE* svg_arquivo, Fila* formas) {
    if (svg_arquivo == NULL || formas == NULL || fila_vazia(formas)) {
        return;
    }
    
    int tamanho = fila_get_tamanho(formas);
    Fila* fila_copia = criar_fila(tamanho);
    
    for (int i = 0; i < tamanho; i++) {
        void* elemento = desenfileirar(formas);
        desenhar_forma_individual(svg_arquivo, (Forma*) elemento);
        enfileirar(fila_copia, elemento); 
    }
    
    while(!fila_vazia(fila_copia)){
        enfileirar(formas, desenfileirar(fila_copia));
    }
    
    destruir_fila(fila_copia);
}

void svg_finalizar(FILE* svg_arquivo) {
    if (svg_arquivo == NULL) return;
    fprintf(svg_arquivo, "</svg>\n");
    fclose(svg_arquivo);
}