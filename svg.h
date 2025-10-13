#ifndef SVG_H
#define SVG_H

#include <stdio.h> 
#include "fila.h" 
#include "formas.h" 

/* SVG (Scalable Vector Graphics)
==========================================================
Módulo responsável por gerar a saída gráfica do programa.
Ele traduz as estruturas de dados de formas geométricas 
em um arquivo de texto formatado em SVG.
==========================================================
*/

/* Função que inicia um arquivo SVG para desenho.
Cria/abre o arquivo especificado e escreve o cabeçalho
padrão do SVG. Recebe o nome do arquivo e retorna
um ponteiro para o arquivo (FILE*) aberto em modo de escrita,
ou NULL em caso de erro ao abrir o arquivo.
*/
FILE* svg_iniciar(const char* nome_arquivo_svg);

/* Função que desenha todas as formas de uma fila em um arquivo SVG.
-> Ideal para desenhar o estado inicial (formas no chão) ou o estado
final (formas na arena). Recebe um ponteiro para o arquivo SVG já aberto
e uma fila contendo as formas a serem desenhadas.
*/
void svg_desenhar_fila_de_formas(FILE* svg_arquivo, Fila* formas);

/* Função que desenha uma única forma geométrica no arquivo SVG.
Esta é uma função auxiliar que pode ser chamada diretamente se necessário.
Ela identifica o tipo da forma (círculo, retângulo, etc.) e escreve
a tag SVG correspondente com seus atributos (cor, posição, etc.).
Recebe um ponteiro para o arquivo SVG já aberto e a forma a ser desenhada.
*/
void svg_desenhar_forma(FILE* svg_arquivo, Forma* forma);

/* Função que finaliza e fecha o arquivo SVG.
Escreve a tag de fechamento </svg> no arquivo e o fecha,
garantindo que o arquivo seja válido.
Recebe um ponteiro para o arquivo SVG que deve ser fechado.
*/
void svg_finalizar(FILE* svg_arquivo);

#endif