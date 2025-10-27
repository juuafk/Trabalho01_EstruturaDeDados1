#ifndef CMD_H
#define CMD_H

#include <stdio.h> 
#include "fila.h"
#include "processamento.h"

/* CMD
-----------------------------------------
FUNCÃO DO ARQUIVO: Ler arquivos texto 
(.geo, .qry) e traduzir para o programa.
-----------------------------------------
*/

// LER_ARQUIVO_GEO
// lê o arquivo .geo (descreve o estado inicial do jogo) e cria todas as formas
int ler_arquivo_geo(const char* arquivo, Fila* formas_chao);


// LER_ARQUIVO_QRY
// lê o arquivo .qry (descreve as ações que irão acontecer no jogo) e processa todos os comandos
void ler_arquivo_qry(const char* path_qry, const char* path_svg_qry,Disparador** disparadores, int num_d, 
Carregador** carregadores, int num_c,Fila* formas_chao, Fila* arena, Fila* formas_derrotadas,
int* proxima_id_global, FILE* log);

#endif