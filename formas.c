#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h>

#include "formas.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

// --------------------------------------
// FUNÇÃO AUXILIAR SUBSTITITUTA DE STRDUP
// --------------------------------------
static char* duplicar_string(const char* s) {
    if (s == NULL) {
        return NULL;
    }

    size_t len = strlen(s) + 1;
    char* nova_str = (char*) malloc(len);
    if (nova_str == NULL) {
        return NULL; 
    }

    memcpy(nova_str, s, len); 
    return nova_str;
}

// ==========================
// DEFINIÇÃO DA STRUCT FORMA
// ==========================
struct forma {
    int id;
    char tipo;

    union {
    // RETÂNGULO
    struct {double x, y, largura, altura; } r;

    // TRIÂNGULO
    struct {double x1, y1, x2, y2, x3, y3; } t;

    // CÍRCULO
    struct {double x, y, raio; } c;

    // LINHA
    struct {double x1, y1, x2, y2; } l;

    // TEXTO
    struct {char ancora; double x, y; char* texto; } x;
    } dados;

    char* cor_borda;
    char* cor_preenchimento;
    int esmagada;
};

// =================================
// FUNÇÕES DE CRIAÇÃO (CONSTRUTORES)
// =================================

// CRIAR_RETANGULO
Forma* criar_retangulo(int id, double x, double y, double largura, double altura, const char* cor_borda, const char* cor_preenchimento) {
    Forma* f = (Forma*) malloc(sizeof(Forma));
    f->id = id;
    f->tipo = 'r';
    f->dados.r.x = x;
    f->dados.r.y = y;
    f->dados.r.largura = largura;
    f->dados.r.altura = altura;
    f->cor_borda = duplicar_string(cor_borda);
    f->cor_preenchimento = duplicar_string(cor_preenchimento);
    f->esmagada = 0; 
    return f;
}

// CRIAR_TRIANGULO
Forma* criar_triangulo(int id, double x1, double y1, double x2, double y2, double x3, double y3, const char* cor_borda, const char* cor_preenchimento) {
    Forma* f = (Forma*) malloc(sizeof(Forma));
    f->id = id;
    f->tipo = 't';
    f->dados.t.x1 = x1;
    f->dados.t.y1 = y1;
    f->dados.t.x2 = x2;
    f->dados.t.y2 = y2;
    f->dados.t.x3 = x3;
    f->dados.t.y3 = y3;
    f->cor_borda = duplicar_string(cor_borda);
    f->cor_preenchimento = duplicar_string(cor_preenchimento);
    f->esmagada = 0; 
    return f;
}

// CRIAR_CIRCULO
Forma* criar_circulo(int id, double x, double y, double raio, const char* cor_borda, const char* cor_preenchimento) {
    Forma* f = (Forma*) malloc(sizeof(Forma));
    f->id = id;
    f->tipo = 'c';
    f->dados.c.x = x;
    f->dados.c.y = y;
    f->dados.c.raio = raio;
    f->cor_borda = duplicar_string(cor_borda);
    f->cor_preenchimento = duplicar_string(cor_preenchimento);
    f->esmagada = 0;
    return f;
}

// CRIAR_LINHA
Forma* criar_linha(int id, double x1, double y1, double x2, double y2, const char* cor) {
    Forma* f = (Forma*) malloc(sizeof(Forma));
    f->id = id;
    f->tipo = 'l';
    f->dados.l.x1 = x1;
    f->dados.l.y1 = y1;
    f->dados.l.x2 = x2;
    f->dados.l.y2 = y2;
    f->cor_borda = duplicar_string(cor);
    f->cor_preenchimento = NULL; 
    f->esmagada = 0; 
    return f;
}

// CRIAR_TEXTO
Forma* criar_texto(int id, char ancora, double x, double y, const char* texto, const char* cor_borda, const char* cor_preenchimento) {
    Forma* f = (Forma*) malloc(sizeof(Forma));
    f->id = id;
    f->tipo = 'x';
    f->dados.x.ancora = ancora;
    f->dados.x.x = x;
    f->dados.x.y = y;
    f->dados.x.texto = duplicar_string(texto);
    f->cor_borda = duplicar_string(cor_borda);
    f->cor_preenchimento = duplicar_string(cor_preenchimento);
    f->esmagada = 0;
    return f;
}

// ===============
// FUNÇÕES GETTER
// ===============

// FORMA_GET_ID
int forma_get_id(Forma* forma) {
    return forma->id;
}

// FORMA_GET_TIPO
char forma_get_tipo(Forma* forma) {
    return forma->tipo;
}

// FORMA_GET_X
double forma_get_x(Forma* f) {
    if (f == NULL) return 0;
    switch (f->tipo) {
        case 'c': return f->dados.c.x;
        case 'r': return f->dados.r.x;
        case 'x': return f->dados.x.x;
        case 'l': return f->dados.l.x1; 
        case 't': return f->dados.t.x1; 
    }
    return 0;
}

// FORMA_GET_Y
double forma_get_y(Forma* f) {
    if (f == NULL) return 0;
    switch (f->tipo) {
        case 'c': return f->dados.c.y;
        case 'r': return f->dados.r.y;
        case 'x': return f->dados.x.y;
        case 'l': return f->dados.l.y1; 
        case 't': return f->dados.t.y1; 
    }
    return 0;
}

// FORMA_GET_ESMAGADA
int forma_get_esmagada(Forma* f) {
    if (f == NULL) return 0; 
    return f->esmagada;
}

// FORMA_GET_COR_BORDA
const char* forma_get_cor_borda(Forma* f) {
    return f->cor_borda;
}

// FORMA_GET_COR_PREENCHIMENTO
const char* forma_get_cor_preenchimento(Forma* f) {
    return f->cor_preenchimento;
}

// FORMA_GET_RETANGULO_DADOS
void forma_get_retangulo_dados(Forma* f, double* x, double* y, double* largura, double* altura) {
    if (f && f->tipo == 'r') {
        *x = f->dados.r.x;
        *y = f->dados.r.y;
        *largura = f->dados.r.largura;
        *altura = f->dados.r.altura;
    }
}

// FORMA_GET_TRIANGULO_DADOS
void forma_get_triangulo_dados(Forma* f, double* x1, double* y1, double* x2, double* y2, double* x3, double* y3) {
    if (f && f->tipo == 't') {
        *x1 = f->dados.t.x1;
        *y1 = f->dados.t.y1;
        *x2 = f->dados.t.x2;
        *y2 = f->dados.t.y2;
        *x3 = f->dados.t.x3;
        *y3 = f->dados.t.y3;
    }
}

// FORMA_GET_CIRCULO_DADOS
void forma_get_circulo_dados(Forma* f, double* x, double* y, double* raio) {
    if (f && f->tipo == 'c') {
        *x = f->dados.c.x;
        *y = f->dados.c.y;
        *raio = f->dados.c.raio;
    }
}

//FORMA_GET_LINHA_DADOS
void forma_get_linha_dados(Forma* f, double* x1, double* y1, double* x2, double* y2) {
    if (f && f->tipo == 'l') {
        *x1 = f->dados.l.x1;
        *y1 = f->dados.l.y1;
        *x2 = f->dados.l.x2;
        *y2 = f->dados.l.y2;
    }
}

//FORMA_GET_TEXTO_DADOS
void forma_get_texto_dados(Forma* f, double* x, double* y, char* ancora, const char** texto) {
    if (f && f->tipo == 'x') {
        *x = f->dados.x.x;
        *y = f->dados.x.y;
        *ancora = f->dados.x.ancora;
        *texto = f->dados.x.texto;
    }
}

//FORMA_GET_TEXTO_BBOX
void forma_get_texto_bbox(Forma* f, double* x, double* y, double* largura, double* altura) {
    if (f && f->tipo == 'x') {
        // assumindo altura fixa 10 e largura derivada da área
        *altura = 10.0;
        *largura = (20.0 * strlen(f->dados.x.texto)) / *altura;
        
        switch(f->dados.x.ancora) {
            case 'i': // âncora no início
                *x = f->dados.x.x;
                *y = f->dados.x.y;
                break;
            case 'm': // âncora no meio
                *x = f->dados.x.x - (*largura / 2.0);
                *y = f->dados.x.y - (*altura / 2.0);
                break;
            case 'f': // âncora no fim
                *x = f->dados.x.x - *largura;
                *y = f->dados.x.y - *altura;
                break;
        }
    }
}

// =============================
// FUNÇÕES DE LÓGICA E CÁLCULO
// =============================

double forma_calcular_area(Forma* forma) {
    if (forma == NULL) return 0.0;

    switch (forma->tipo) {
        case 'r':
            return forma->dados.r.largura * forma->dados.r.altura;
        case 'c':
            return M_PI * forma->dados.c.raio * forma->dados.c.raio;
        case 't': {
            double x1 = forma->dados.t.x1, y1 = forma->dados.t.y1;
            double x2 = forma->dados.t.x2, y2 = forma->dados.t.y2;
            double x3 = forma->dados.t.x3, y3 = forma->dados.t.y3;
            return 0.5 * fabs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
        }
        case 'l': {
            // área = 2 * comprimento
            double dx = forma->dados.l.x2 - forma->dados.l.x1;
            double dy = forma->dados.l.y2 - forma->dados.l.y1;
            double comprimento = sqrt(dx * dx + dy * dy);
            return 2 * comprimento;
        }
        case 'x': {
            // área = 20 * quantidade de caracteres
            return 20.0 * strlen(forma->dados.x.texto);
        }
        default:
            return 0.0;
    }
}

// ==========================================
// FUNÇÕES GERAIS PARA MANIPULAÇÃO DE FORMAS
// ==========================================

// DESTRUIR_FORMA
void destruir_forma(Forma* forma) {
    if (forma == NULL) return;

    free(forma->cor_borda);
    if (forma->cor_preenchimento != NULL) {
        free(forma->cor_preenchimento);
    }
    if (forma->tipo == 'x') {
        free(forma->dados.x.texto);
    }

    free(forma);
}

// FORMA_CLONAR
Forma* forma_clonar(Forma* f, int nova_id) {
    if (f == NULL) return NULL;

    switch(f->tipo) {
        case 'c':
            return criar_circulo(nova_id, f->dados.c.x, f->dados.c.y, f->dados.c.raio, f->cor_borda, f->cor_preenchimento);
        case 'r':
            return criar_retangulo(nova_id, f->dados.r.x, f->dados.r.y, f->dados.r.largura, f->dados.r.altura, f->cor_borda, f->cor_preenchimento);
        case 't':
            return criar_triangulo(nova_id, 
                f->dados.t.x1, f->dados.t.y1, 
                f->dados.t.x2, f->dados.t.y2, 
                f->dados.t.x3, f->dados.t.y3, 
                f->cor_borda, f->cor_preenchimento);
        case 'l':
            return criar_linha(nova_id, 
                f->dados.l.x1, f->dados.l.y1, 
                f->dados.l.x2, f->dados.l.y2, 
                f->cor_borda);
        case 'x':
            return criar_texto(nova_id, 
                f->dados.x.ancora, f->dados.x.x, f->dados.x.y, 
                f->dados.x.texto, f->cor_borda, f->cor_preenchimento);
    }
    return NULL;
}

// FORMA_MOVER
void forma_mover(Forma* f, double dx, double dy) {
    if (f == NULL) return;

    switch (f->tipo) {
        case 'c':
            f->dados.c.x += dx;
            f->dados.c.y += dy;
            break;
        case 'r':
            f->dados.r.x += dx;
            f->dados.r.y += dy;
            break;
        case 'l':
            f->dados.l.x1 += dx;
            f->dados.l.y1 += dy;
            f->dados.l.x2 += dx;
            f->dados.l.y2 += dy;
            break;
        case 't':
            f->dados.t.x1 += dx;
            f->dados.t.y1 += dy;
            f->dados.t.x2 += dx;
            f->dados.t.y2 += dy;
            f->dados.t.x3 += dx;
            f->dados.t.y3 += dy;
            break;
        case 'x':
            f->dados.x.x += dx;
            f->dados.x.y += dy;
            break;
    }
}

// FORMA_SET_ESMAGADA
void forma_set_esmagada(Forma* f, int valor) {
    if (f == NULL) return;
    f->esmagada = valor;
}

// FORMA_TROCAR_CORES
void forma_trocar_cores(Forma* f) {
    if (f == NULL) return;
    char* temp_ptr = f->cor_borda;
    f->cor_borda = f->cor_preenchimento;
    f->cor_preenchimento = temp_ptr;
}

// FORMA_SET_COR_BORDA
void forma_set_cor_borda(Forma* f, const char* nova_cor) {
    if (f == NULL) return;
    if (f->cor_borda != NULL) {
        free(f->cor_borda);
    }
    f->cor_borda = duplicar_string(nova_cor);
} 

// FORMA_SET_COR_PREENCHIMENTO
void forma_set_cor_preenchimento(Forma* f, const char* nova_cor) {
    if (f == NULL) return;
    if (f->cor_preenchimento != NULL) {
        free(f->cor_preenchimento);
    }
    f->cor_preenchimento = (nova_cor == NULL) ? NULL : duplicar_string(nova_cor);
}

// FORMA_SET_POSICAO
void forma_set_posicao(Forma* f, double x, double y) {
    if (f == NULL) return;

    double x_atual = forma_get_x(f);
    double y_atual = forma_get_y(f);

    double dx = x - x_atual;
    double dy = y - y_atual;
 
    forma_mover(f, dx, dy);
}

// =======================================================
// FUNÇÕES AUXILIARES ESTÁTICAS PARA DETECÇÃO DE COLISÃO
// =======================================================

// CLAMP
static double clamp(double valor, double min, double max) {
    if (valor < min) return min;
    if (valor > max) return max;
    return valor;
}

// PONTO_NO_CIRCULO
static int ponto_no_circulo(double px, double py, double cx, double cy, double raio) {
    double dx = px - cx;
    double dy = py - cy;
    return (dx * dx + dy * dy) < (raio * raio);
}

// PONTO_NO_RETANGULO
static int ponto_no_retangulo(double px, double py, double rx, double ry, double rw, double rh) {
    return px >= rx && px <= rx + rw && py >= ry && py <= ry + rh;
}

// PONTO_NO_TRIANGULO
static int ponto_no_triangulo(double px, double py, double x1, double y1, double x2, double y2, double x3, double y3) {
    double d1 = (px - x2) * (y1 - y2) - (x1 - x2) * (py - y2);
    double d2 = (px - x3) * (y2 - y3) - (x2 - x3) * (py - y3);
    double d3 = (px - x1) * (y3 - y1) - (x3 - x1) * (py - y1);
    int tem_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    int tem_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    return !(tem_neg && tem_pos);
}

// LINHA_INTERCEPTA_LINHA
static int linha_intercepta_linha(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
    double den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (den == 0) return 0;
    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
    double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
    return t > 0 && t < 1 && u > 0 && u < 1;
}

// LINHA_VS_LINHA_WRAPPER (simplificadora)
static int linha_vs_linha_wrapper(Forma* f1, Forma* f2) {
    double x1, y1, x2, y2, x3, y3, x4, y4;
    forma_get_linha_dados(f1, &x1, &y1, &x2, &y2);
    forma_get_linha_dados(f2, &x3, &y3, &x4, &y4);
    return linha_intercepta_linha(x1, y1, x2, y2, x3, y3, x4, y4);
}

// CIRCULO_VS_CIRCULO
static int circulo_vs_circulo(Forma* f1, Forma* f2) {
    double x1, y1, r1, x2, y2, r2;
    forma_get_circulo_dados(f1, &x1, &y1, &r1);
    forma_get_circulo_dados(f2, &x2, &y2, &r2);
    double dx = x1 - x2;
    double dy = y1 - y2;
    return (dx * dx + dy * dy) < (r1 + r2) * (r1 + r2);
}

// RETANGULO_VS_RETANGULO
static int retangulo_vs_retangulo(Forma* f1, Forma* f2) {
    double x1, y1, w1, h1, x2, y2, w2, h2;
    if (forma_get_tipo(f1) == 'r') forma_get_retangulo_dados(f1, &x1, &y1, &w1, &h1);
    else forma_get_texto_bbox(f1, &x1, &y1, &w1, &h1);
    
    if (forma_get_tipo(f2) == 'r') forma_get_retangulo_dados(f2, &x2, &y2, &w2, &h2);
    else forma_get_texto_bbox(f2, &x2, &y2, &w2, &h2);

    if (x1 + w1 < x2 || x1 > x2 + w2) return 0;
    if (y1 + h1 < y2 || y1 > y2 + h2) return 0;
    return 1;
}

// CIRCULO_VS_RETANGULO
static int circulo_vs_retangulo(Forma* circ, Forma* rect) {
    double cx, cy, raio, rx, ry, rw, rh;
    forma_get_circulo_dados(circ, &cx, &cy, &raio);
    if (forma_get_tipo(rect) == 'r') forma_get_retangulo_dados(rect, &rx, &ry, &rw, &rh);
    else forma_get_texto_bbox(rect, &rx, &ry, &rw, &rh);
    
    double ponto_prox_x = clamp(cx, rx, rx + rw);
    double ponto_prox_y = clamp(cy, ry, ry + rh);

    return ponto_no_circulo(ponto_prox_x, ponto_prox_y, cx, cy, raio);
}

// LINHA_VS_CIRCULO
static int linha_vs_circulo(Forma* linha, Forma* circ) {
    double x1, y1, x2, y2, cx, cy, r;
    forma_get_linha_dados(linha, &x1, &y1, &x2, &y2);
    forma_get_circulo_dados(circ, &cx, &cy, &r);

    if (ponto_no_circulo(x1, y1, cx, cy, r) || ponto_no_circulo(x2, y2, cx, cy, r)) return 1;
    
    double dx = x2 - x1, dy = y2 - y1;
    double len_sq = dx*dx + dy*dy;
    double t = ((cx - x1) * dx + (cy - y1) * dy) / len_sq;
    t = clamp(t, 0, 1);
    double prox_x = x1 + t * dx;
    double prox_y = y1 + t * dy;

    return ponto_no_circulo(prox_x, prox_y, cx, cy, r);
}

// LINHA_VS_RETANGULO
static int linha_vs_retangulo(Forma* linha, Forma* rect) {
    double lx1, ly1, lx2, ly2, rx, ry, rw, rh;
    forma_get_linha_dados(linha, &lx1, &ly1, &lx2, &ly2);
    if (forma_get_tipo(rect) == 'r') forma_get_retangulo_dados(rect, &rx, &ry, &rw, &rh);
    else forma_get_texto_bbox(rect, &rx, &ry, &rw, &rh);

    if (ponto_no_retangulo(lx1, ly1, rx, ry, rw, rh) || ponto_no_retangulo(lx2, ly2, rx, ry, rw, rh)) return 1;

    if (linha_intercepta_linha(lx1, ly1, lx2, ly2, rx, ry, rx + rw, ry)) return 1;
    if (linha_intercepta_linha(lx1, ly1, lx2, ly2, rx + rw, ry, rx + rw, ry + rh)) return 1;
    if (linha_intercepta_linha(lx1, ly1, lx2, ly2, rx + rw, ry + rh, rx, ry + rh)) return 1;
    if (linha_intercepta_linha(lx1, ly1, lx2, ly2, rx, ry + rh, rx, ry)) return 1;
    
    return 0;
}

// LINHA_VS_TRIANGULO
static int linha_vs_triangulo(Forma* linha, Forma* tri) {
    double lx1, ly1, lx2, ly2, tx1, ty1, tx2, ty2, tx3, ty3;
    forma_get_linha_dados(linha, &lx1, &ly1, &lx2, &ly2);
    forma_get_triangulo_dados(tri, &tx1, &ty1, &tx2, &ty2, &tx3, &ty3);

    if (ponto_no_triangulo(lx1, ly1, tx1, ty1, tx2, ty2, tx3, ty3)) return 1;
    if (ponto_no_triangulo(lx2, ly2, tx1, ty1, tx2, ty2, tx3, ty3)) return 1;

    if (linha_intercepta_linha(lx1, ly1, lx2, ly2, tx1, ty1, tx2, ty2)) return 1;
    if (linha_intercepta_linha(lx1, ly1, lx2, ly2, tx2, ty2, tx3, ty3)) return 1;
    if (linha_intercepta_linha(lx1, ly1, lx2, ly2, tx3, ty3, tx1, ty1)) return 1;

    return 0;
}

// TRIANGULO_VS_CIRCULO
static int triangulo_vs_circulo(Forma* tri, Forma* circ) {
    double cx, cy, r, x1, y1, x2, y2, x3, y3;
    forma_get_circulo_dados(circ, &cx, &cy, &r);
    forma_get_triangulo_dados(tri, &x1, &y1, &x2, &y2, &x3, &y3);
    
    if (ponto_no_triangulo(cx, cy, x1, y1, x2, y2, x3, y3)) return 1;
    if (ponto_no_circulo(x1, y1, cx, cy, r) || ponto_no_circulo(x2, y2, cx, cy, r) || ponto_no_circulo(x3, y3, cx, cy, r)) return 1;

    Forma* aresta1 = criar_linha(0, x1, y1, x2, y2, "");
    Forma* aresta2 = criar_linha(0, x2, y2, x3, y3, "");
    Forma* aresta3 = criar_linha(0, x3, y3, x1, y1, "");
    int colide = linha_vs_circulo(aresta1, circ) || linha_vs_circulo(aresta2, circ) || linha_vs_circulo(aresta3, circ);
    destruir_forma(aresta1);
    destruir_forma(aresta2);
    destruir_forma(aresta3);
    
    return colide;
}

// TRIANGULO_VS_RETANGULO
static int triangulo_vs_retangulo(Forma* tri, Forma* rect) {
    double tx1, ty1, tx2, ty2, tx3, ty3, rx, ry, rw, rh;
    forma_get_triangulo_dados(tri, &tx1, &ty1, &tx2, &ty2, &tx3, &ty3);
    if (forma_get_tipo(rect) == 'r') forma_get_retangulo_dados(rect, &rx, &ry, &rw, &rh);
    else forma_get_texto_bbox(rect, &rx, &ry, &rw, &rh);
    
    if (ponto_no_retangulo(tx1, ty1, rx, ry, rw, rh) || ponto_no_retangulo(tx2, ty2, rx, ry, rw, rh) || ponto_no_retangulo(tx3, ty3, rx, ry, rw, rh)) return 1;
    if (ponto_no_triangulo(rx, ry, tx1, ty1, tx2, ty2, tx3, ty3)) return 1;
    
    Forma* aresta1 = criar_linha(0, tx1, ty1, tx2, ty2, "");
    Forma* aresta2 = criar_linha(0, tx2, ty2, tx3, ty3, "");
    Forma* aresta3 = criar_linha(0, tx3, ty3, tx1, ty1, "");
    int colide = linha_vs_retangulo(aresta1, rect) || linha_vs_retangulo(aresta2, rect) || linha_vs_retangulo(aresta3, rect);
    destruir_forma(aresta1);
    destruir_forma(aresta2);
    destruir_forma(aresta3);
    
    return colide;
}

// TRIANGULO_VS_TRIANGULO
static int triangulo_vs_triangulo(Forma* t1, Forma* t2) {
    double t1x1, t1y1, t1x2, t1y2, t1x3, t1y3;
    double t2x1, t2y1, t2x2, t2y2, t2x3, t2y3;
    forma_get_triangulo_dados(t1, &t1x1, &t1y1, &t1x2, &t1y2, &t1x3, &t1y3);
    forma_get_triangulo_dados(t2, &t2x1, &t2y1, &t2x2, &t2y2, &t2x3, &t2y3);
    
    if (ponto_no_triangulo(t1x1, t1y1, t2x1, t2y1, t2x2, t2y2, t2x3, t2y3)) return 1;
    if (ponto_no_triangulo(t1x2, t1y2, t2x1, t2y1, t2x2, t2y2, t2x3, t2y3)) return 1;
    if (ponto_no_triangulo(t1x3, t1y3, t2x1, t2y1, t2x2, t2y2, t2x3, t2y3)) return 1;
    
    if (ponto_no_triangulo(t2x1, t2y1, t1x1, t1y1, t1x2, t1y2, t1x3, t1y3)) return 1;
    if (ponto_no_triangulo(t2x2, t2y2, t1x1, t1y1, t1x2, t1y2, t1x3, t1y3)) return 1;
    if (ponto_no_triangulo(t2x3, t2y3, t1x1, t1y1, t1x2, t1y2, t1x3, t1y3)) return 1;
    
    if (linha_intercepta_linha(t1x1, t1y1, t1x2, t1y2, t2x1, t2y1, t2x2, t2y2)) return 1;
    if (linha_intercepta_linha(t1x1, t1y1, t1x2, t1y2, t2x2, t2y2, t2x3, t2y3)) return 1;
    if (linha_intercepta_linha(t1x1, t1y1, t1x2, t1y2, t2x3, t2y3, t2x1, t2y1)) return 1;

    if (linha_intercepta_linha(t1x2, t1y2, t1x3, t1y3, t2x1, t2y1, t2x2, t2y2)) return 1;
    if (linha_intercepta_linha(t1x2, t1y2, t1x3, t1y3, t2x2, t2y2, t2x3, t2y3)) return 1;
    if (linha_intercepta_linha(t1x2, t1y2, t1x3, t1y3, t2x3, t2y3, t2x1, t2y1)) return 1;

    if (linha_intercepta_linha(t1x3, t1y3, t1x1, t1y1, t2x1, t2y1, t2x2, t2y2)) return 1;
    if (linha_intercepta_linha(t1x3, t1y3, t1x1, t1y1, t2x2, t2y2, t2x3, t2y3)) return 1;
    if (linha_intercepta_linha(t1x3, t1y3, t1x1, t1y1, t2x3, t2y3, t2x1, t2y1)) return 1;

    return 0;
}

// ================================================
// FUNÇÃO PRINCIPAL DE SOBREPOSIÇÃO (DESPACHANTE)
// ================================================

// FORMAS_SOBREPOEM
int formas_sobrepoem(Forma* f1, Forma* f2) {
    char t1 = forma_get_tipo(f1);
    char t2 = forma_get_tipo(f2);
    
    char tipo_real1 = (t1 == 'x') ? 'r' : t1;
    char tipo_real2 = (t2 == 'x') ? 'r' : t2;

    if (tipo_real1 > tipo_real2) {
        Forma* temp_f = f1; f1 = f2; f2 = temp_f;
        char temp_t = tipo_real1; tipo_real1 = tipo_real2; tipo_real2 = temp_t;
    }

    if (tipo_real1 == 'c' && tipo_real2 == 'c') return circulo_vs_circulo(f1, f2);
    if (tipo_real1 == 'c' && tipo_real2 == 'l') return linha_vs_circulo(f2, f1);
    if (tipo_real1 == 'c' && tipo_real2 == 'r') return circulo_vs_retangulo(f1, f2);
    if (tipo_real1 == 'c' && tipo_real2 == 't') return triangulo_vs_circulo(f2, f1);

    if (tipo_real1 == 'l' && tipo_real2 == 'l') return linha_vs_linha_wrapper(f1, f2);
    if (tipo_real1 == 'l' && tipo_real2 == 'r') return linha_vs_retangulo(f1, f2);
    if (tipo_real1 == 'l' && tipo_real2 == 't') return linha_vs_triangulo(f1, f2);

    if (tipo_real1 == 'r' && tipo_real2 == 'r') return retangulo_vs_retangulo(f1, f2);
    if (tipo_real1 == 'r' && tipo_real2 == 't') return triangulo_vs_retangulo(f2, f1);

    if (tipo_real1 == 't' && tipo_real2 == 't') return triangulo_vs_triangulo(f1, f2);

    return 0; 
}