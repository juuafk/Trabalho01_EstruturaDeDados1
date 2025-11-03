#ifndef CMD_H
#define CMD_H

#include <stdio.h> 
#include "fila.h"
#include "processamento.h"
#include "campo.h" 

// ==================================
// CMD: lê arquivos texto (.geo .qry)
// e traduz para o programa.
// ==================================

// LER_ARQUIVO_GEO
// lê o arquivo .geo (descreve o estado inicial do jogo) e cria todas as formas
int ler_arquivo_geo(const char* arquivo_geo, Fila* formas_chao);

// LER_ARQUIVO_QRY
// lê o arquivo .qry (descreve as ações que irão acontecer no jogo) e processa todos os comandos
void ler_arquivo_qry(char* path_qry, char* path_svg_qry, Disparador** disparadores, int num_max_disparadores,
Carregador** carregadores, int num_max_carregadores, Campo* campo_jogo,  Fila* formas_derrotadas,
int* proxima_id_global, FILE* arquivo_log);

#endif