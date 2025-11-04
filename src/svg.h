#ifndef SVG_H
#define SVG_H

#include <stdio.h> 
#include "fila.h" 
#include "formas.h"
#include "campo.h"

// SVG
// ==========================================================
// Módulo responsável por gerar a saída gráfica do programa.
// Ele traduz as estruturas de dados de formas geométricas 
// em um arquivo de texto formatado em SVG.
// ==========================================================


// =========================
// FUNÇÃO PÚBLICA PRINCIPAL
// =========================

// SVG_DESENHAR
// desenha o estado atual do jogo em um arquivo SVG
void svg_desenhar(const char* nome_arquivo_svg, Fila* formas_chao, Fila* formas_derrotadas, Campo* campo);

#endif