#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "formas.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

// ==========================
// DEFINIÇÃO DA STRUCT FORMA
// ==========================
struct forma {
    int id;
    char tipo; // 'r', 't', 'c', 'l', 'x'

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
};

// =================================
// FUNÇÕES DE CRIAÇÃO (CONSTRUTORES)
// =================================

Forma* criar_retangulo(int id, double x, double y, double largura, double altura, const char* cor_borda, const char* cor_preenchimento) {
    Forma* f = (Forma*) malloc(sizeof(Forma));
    f->id = id;
    f->tipo = 'r';
    f->dados.r.x = x;
    f->dados.r.y = y;
    f->dados.r.largura = largura;
    f->dados.r.altura = altura;
    f->cor_borda = strdup(cor_borda);
    f->cor_preenchimento = strdup(cor_preenchimento);
    return f;
}

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
    f->cor_borda = strdup(cor_borda);
    f->cor_preenchimento = strdup(cor_preenchimento);
    return f;
}

Forma* criar_circulo(int id, double x, double y, double raio, const char* cor_borda, const char* cor_preenchimento) {
    Forma* f = (Forma*) malloc(sizeof(Forma));
    f->id = id;
    f->tipo = 'c';
    f->dados.c.x = x;
    f->dados.c.y = y;
    f->dados.c.raio = raio;
    f->cor_borda = strdup(cor_borda);
    f->cor_preenchimento = strdup(cor_preenchimento);
    return f;
}

Forma* criar_linha(int id, double x1, double y1, double x2, double y2, const char* cor) {
    Forma* f = (Forma*) malloc(sizeof(Forma));
    f->id = id;
    f->tipo = 'l';
    f->dados.l.x1 = x1;
    f->dados.l.y1 = y1;
    f->dados.l.x2 = x2;
    f->dados.l.y2 = y2;
    f->cor_borda = strdup(cor);
    f->cor_preenchimento = NULL; 
    return f;
}

Forma* criar_texto(int id, char ancora, double x, double y, const char* texto, const char* cor_borda, const char* cor_preenchimento) {
    Forma* f = (Forma*) malloc(sizeof(Forma));
    f->id = id;
    f->tipo = 'x';
    f->dados.x.ancora = ancora;
    f->dados.x.x = x;
    f->dados.x.y = y;
    f->dados.x.texto = strdup(texto);
    f->cor_borda = strdup(cor_borda);
    f->cor_preenchimento = strdup(cor_preenchimento);
    return f;
}

// ===================================
// FUNÇÃO DE DESTRUIÇÃO (DESTRUTORES)
// ===================================

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

// =================
// FUNÇÕES "GETTER"
// =================

int forma_get_id(Forma* forma) {
    return forma->id;
}

char forma_get_tipo(Forma* forma) {
    return forma->tipo;
}

void forma_get_retangulo_dados(Forma* f, double* x, double* y, double* largura, double* altura) {
    if (f && f->tipo == 'r') {
        *x = f->dados.r.x;
        *y = f->dados.r.y;
        *largura = f->dados.r.largura;
        *altura = f->dados.r.altura;
    }
}

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

void forma_get_circulo_dados(Forma* f, double* x, double* y, double* raio) {
    if (f && f->tipo == 'c') {
        *x = f->dados.c.x;
        *y = f->dados.c.y;
        *raio = f->dados.c.raio;
    }
}

void forma_get_linha_dados(Forma* f, double* x1, double* y1, double* x2, double* y2) {
    if (f && f->tipo == 'l') {
        *x1 = f->dados.l.x1;
        *y1 = f->dados.l.y1;
        *x2 = f->dados.l.x2;
        *y2 = f->dados.l.y2;
    }
}

void forma_get_texto_dados(Forma* f, double* x, double* y, char* ancora, const char** texto) {
    if (f && f->tipo == 'x') {
        *x = f->dados.x.x;
        *y = f->dados.x.y;
        *ancora = f->dados.x.ancora;
        *texto = f->dados.x.texto;
    }
}

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

const char* forma_get_cor_borda(Forma* f) {
    return f->cor_borda;
}

const char* forma_get_cor_preenchimento(Forma* f) {
    return f->cor_preenchimento;
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
            #ifndef M_PI
                #define M_PI 3.14159265358979323846
            #endif
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

// =======================================================
// FUNÇÕES AUXILIARES ESTÁTICAS PARA DETECÇÃO DE COLISÃO
// =======================================================
// (essas funções podem ser usadas somente nesse arquivo) 

// função auxiliar genérica para clamping (limitação) de um valor entre um mínimo e um máximo
static double clamp(double valor, double min, double max) {
    if (valor < min) return min;
    if (valor > max) return max;
    return valor;
}

// verifica se um ponto (px, py) está dentro de um círculo
static int ponto_no_circulo(double px, double py, double cx, double cy, double raio) {
    double dx = px - cx;
    double dy = py - cy;
    return (dx * dx + dy * dy) < (raio * raio);
}

// verifica se um ponto (px, py) está dentro de um retângulo alinhado aos eixos
static int ponto_no_retangulo(double px, double py, double rx, double ry, double rw, double rh) {
    return px >= rx && px <= rx + rw && py >= ry && py <= ry + rh;
}

// verifica se um ponto está dentro de um triângulo
static int ponto_no_triangulo(double px, double py, double x1, double y1, double x2, double y2, double x3, double y3) {
    double d1 = (px - x2) * (y1 - y2) - (x1 - x2) * (py - y2);
    double d2 = (px - x3) * (y2 - y3) - (x2 - x3) * (py - y3);
    double d3 = (px - x1) * (y3 - y1) - (x3 - x1) * (py - y1);
    // o ponto está dentro se todos os sinais forem iguais (ou zero)
    int tem_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    int tem_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    return !(tem_neg && tem_pos);
}

// verifica se dois segmentos de reta se interceptam
static int linha_intercepta_linha(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
    double den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (den == 0) return 0; // linhas paralelas
    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
    double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
    return t > 0 && t < 1 && u > 0 && u < 1;
}

static int linha_vs_linha_wrapper(Forma* f1, Forma* f2) {
    double x1, y1, x2, y2, x3, y3, x4, y4;

    forma_get_linha_dados(f1, &x1, &y1, &x2, &y2);
    forma_get_linha_dados(f2, &x3, &y3, &x4, &y4);

    return linha_intercepta_linha(x1, y1, x2, y2, x3, y3, x4, y4);
}

// =======================================================
// IMPLEMENTAÇÕES DE SOBREPOSIÇÃO PARA CADA PAR DE FORMAS
// =======================================================

static int circulo_vs_circulo(Forma* f1, Forma* f2) {
    double x1, y1, r1, x2, y2, r2;
    forma_get_circulo_dados(f1, &x1, &y1, &r1);
    forma_get_circulo_dados(f2, &x2, &y2, &r2);
    double dx = x1 - x2;
    double dy = y1 - y2;
    return (dx * dx + dy * dy) < (r1 + r2) * (r1 + r2);
}

static int retangulo_vs_retangulo(Forma* f1, Forma* f2) {
    double x1, y1, w1, h1, x2, y2, w2, h2;
    // funciona pra texto também
    if (forma_get_tipo(f1) == 'r') forma_get_retangulo_dados(f1, &x1, &y1, &w1, &h1);
    else forma_get_texto_bbox(f1, &x1, &y1, &w1, &h1);
    
    if (forma_get_tipo(f2) == 'r') forma_get_retangulo_dados(f2, &x2, &y2, &w2, &h2);
    else forma_get_texto_bbox(f2, &x2, &y2, &w2, &h2);

    if (x1 + w1 < x2 || x1 > x2 + w2) return 0;
    if (y1 + h1 < y2 || y1 > y2 + h2) return 0;
    return 1;
}

static int circulo_vs_retangulo(Forma* circ, Forma* rect) {
    double cx, cy, raio, rx, ry, rw, rh;
    forma_get_circulo_dados(circ, &cx, &cy, &raio);
    if (forma_get_tipo(rect) == 'r') forma_get_retangulo_dados(rect, &rx, &ry, &rw, &rh);
    else forma_get_texto_bbox(rect, &rx, &ry, &rw, &rh);
    
    double ponto_prox_x = clamp(cx, rx, rx + rw);
    double ponto_prox_y = clamp(cy, ry, ry + rh);

    return ponto_no_circulo(ponto_prox_x, ponto_prox_y, cx, cy, raio);
}

static int linha_vs_circulo(Forma* linha, Forma* circ) {
    double x1, y1, x2, y2, cx, cy, r;
    forma_get_linha_dados(linha, &x1, &y1, &x2, &y2);
    forma_get_circulo_dados(circ, &cx, &cy, &r);

    if (ponto_no_circulo(x1, y1, cx, cy, r) || ponto_no_circulo(x2, y2, cx, cy, r)) return 1;
    
    double dx = x2 - x1, dy = y2 - y1;
    double t = ((cx - x1) * dx + (cy - y1) * dy) / (dx*dx + dy*dy);
    t = clamp(t, 0, 1);
    double prox_x = x1 + t * dx;
    double prox_y = y1 + t * dy;

    return ponto_no_circulo(prox_x, prox_y, cx, cy, r);
}

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

static int triangulo_vs_circulo(Forma* tri, Forma* circ) {
    double cx, cy, r, x1, y1, x2, y2, x3, y3;
    forma_get_circulo_dados(circ, &cx, &cy, &r);
    forma_get_triangulo_dados(tri, &x1, &y1, &x2, &y2, &x3, &y3);
    
    if (ponto_no_triangulo(cx, cy, x1, y1, x2, y2, x3, y3)) return 1;
    if (ponto_no_circulo(x1, y1, cx, cy, r) || ponto_no_circulo(x2, y2, cx, cy, r) || ponto_no_circulo(x3, y3, cx, cy, r)) return 1;

    // cria formas de linha temporárias para testar a colisão com as arestas
    Forma* aresta1 = criar_linha(0, x1, y1, x2, y2, "");
    Forma* aresta2 = criar_linha(0, x2, y2, x3, y3, "");
    Forma* aresta3 = criar_linha(0, x3, y3, x1, y1, "");
    int colide = linha_vs_circulo(aresta1, circ) || linha_vs_circulo(aresta2, circ) || linha_vs_circulo(aresta3, circ);
    destruir_forma(aresta1);
    destruir_forma(aresta2);
    destruir_forma(aresta3);
    
    return colide;
}

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

static int triangulo_vs_triangulo(Forma* t1, Forma* t2) {
    double t1x1, t1y1, t1x2, t1y2, t1x3, t1y3;
    double t2x1, t2y1, t2x2, t2y2, t2x3, t2y3;
    forma_get_triangulo_dados(t1, &t1x1, &t1y1, &t1x2, &t1y2, &t1x3, &t1y3);
    forma_get_triangulo_dados(t2, &t2x1, &t2y1, &t2x2, &t2y2, &t2x3, &t2y3);
    
    // CASO 1: algum vértice do Triângulo 1 está dentro do Triângulo 2?
    if (ponto_no_triangulo(t1x1, t1y1, t2x1, t2y1, t2x2, t2y2, t2x3, t2y3)) return 1;
    if (ponto_no_triangulo(t1x2, t1y2, t2x1, t2y1, t2x2, t2y2, t2x3, t2y3)) return 1;
    if (ponto_no_triangulo(t1x3, t1y3, t2x1, t2y1, t2x2, t2y2, t2x3, t2y3)) return 1;
    
    // CASO 2: algum vértice do Triângulo 2 está dentro do Triângulo 1?
    if (ponto_no_triangulo(t2x1, t2y1, t1x1, t1y1, t1x2, t1y2, t1x3, t1y3)) return 1;
    if (ponto_no_triangulo(t2x2, t2y2, t1x1, t1y1, t1x2, t1y2, t1x3, t1y3)) return 1;
    if (ponto_no_triangulo(t2x3, t2y3, t1x1, t1y1, t1x2, t1y2, t1x3, t1y3)) return 1;
    
    // CASO 3: as arestas dos dois triângulos se interceptam?

    // aresta 1 de T1 vs. todas as arestas de T2
    if (linha_intercepta_linha(t1x1, t1y1, t1x2, t1y2, t2x1, t2y1, t2x2, t2y2)) return 1;
    if (linha_intercepta_linha(t1x1, t1y1, t1x2, t1y2, t2x2, t2y2, t2x3, t2y3)) return 1;
    if (linha_intercepta_linha(t1x1, t1y1, t1x2, t1y2, t2x3, t2y3, t2x1, t2y1)) return 1;

    // aresta 2 de T1 vs. todas as arestas de T2
    if (linha_intercepta_linha(t1x2, t1y2, t1x3, t1y3, t2x1, t2y1, t2x2, t2y2)) return 1;
    if (linha_intercepta_linha(t1x2, t1y2, t1x3, t1y3, t2x2, t2y2, t2x3, t2y3)) return 1;
    if (linha_intercepta_linha(t1x2, t1y2, t1x3, t1y3, t2x3, t2y3, t2x1, t2y1)) return 1;

    // aresta 3 de T1 vs. todas as arestas de T2
    if (linha_intercepta_linha(t1x3, t1y3, t1x1, t1y1, t2x1, t2y1, t2x2, t2y2)) return 1;
    if (linha_intercepta_linha(t1x3, t1y3, t1x1, t1y1, t2x2, t2y2, t2x3, t2y3)) return 1;
    if (linha_intercepta_linha(t1x3, t1y3, t1x1, t1y1, t2x3, t2y3, t2x1, t2y1)) return 1;

    return 0;
}

// ================================================
// FUNÇÃO PRINCIPAL DE SOBREPOSIÇÃO (DESPACHANTE)
// ================================================

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