#ifndef FORMAS_H
#define FORMAS_H

/* FORMAS UTILIZADAS NO PROJETO
===================================================
Serão utilizadas no projeto 5 formas:
- Retângulos
- Triângulos
- Círculos
- Linhas
- Textos
Todas possuem um id, cor de preenchimento 
e de borda, menos a linha que possui apenas uma cor.
===================================================
*/

typedef struct forma Forma;

/* RETÂNGULO 
------------------------------------------------------------
Um retângulo é uma figura geométrica plana com quatro lados, 
em que os lados opostos são paralelos e iguais em comprimento,
e todos os quatro ângulos internos são retos.
(POSSUI NO PROJETO: id, âncora (um dos pontos), largura, altura, 
cor de borda, cor de preenchimento).
------------------------------------------------------------
*/

// função que cria um retângulo
Forma* criar_retangulo(int id, double x, double y, double largura, double altura, 
const char* cor_borda, const char* cor_preenchimento);

/* TRIÂNGULO
-------------------------------------------------------------------
Um triângulo é uma figura geométrica plana que possui três lados
e três ângulos. É definido pela união de três pontos não colineares 
através de três segmentos de reta.
(POSSUI NO PROJETO: id, âncora (um dos pontos), outros dois pontos, 
cor de borda, cor de preenchimento).
-------------------------------------------------------------------
*/

// função que cria um triângulo
Forma* criar_triangulo(int id, double x1, double y1, double x2, double y2, double x3, double y3,
const char* cor_borda, const char* cor_preenchimento);

/* CÍRCULO
------------------------------------------------------------------
Um círculo é uma figura geométrica composta pelo conjunto 
de todos os pontos em um plano que estão a uma distância fixa (raio) 
de um ponto central.
(POSSUI NO PROJETO: id, âncora (centro), raio, cor de borda,
cor de preenchimento).
------------------------------------------------------------------
*/

// função que cria um círculo
Forma* criar_circulo(int id, double x, double y, double raio, 
const char* cor_borda, const char* cor_preenchimento);

/* LINHA
-------------------------------------------------------------------------------
A linha, nesse projeto, adere a definicão de um segmento de reta, estabelecido
como: uma parte finita de uma reta definida por dois pontos extremos,
que são seus início e fim.
Além disso, para sua implementação no trabalho, a linha também terá área,
estipulada como: 2 * comprimento.
(POSSUI NO PROJETO: id, duas âncoras (ínicio e fim), cor).
-------------------------------------------------------------------------------
*/

// função que cria uma linha
Forma* criar_linha(int id, double x1, double y1, double x2, double y2, 
const char* cor);

/* TEXTO
---------------------------------------------------------------------------
Um texto é um conjunto de letras, que podem formar palavras. No contexto
do projeto ele assume uma área, definida como: 20 * quantidade de caracteres.
(POSSUI NO PROJETO: id, âncora (como o texto ficará alinhado em relação à
coordenada), coordenada, conteúdo da mensagem, cor de borda, cor de
preenchimento).
---------------------------------------------------------------------------
*/

// função que cria um texto
Forma* criar_texto(int id, char ancora, double x, double y, const char* texto,
const char* cor_borda, const char* cor_preenchimento);


// FUNÇÕES GERAIS PARA FORMAS

// libera a memória alocada para uma forma 
void destruir_forma(Forma* forma);

// calcula a área da forma 
double forma_calcular_area(Forma* forma);

// retorna o ID da forma
int forma_get_id(Forma* forma);

// verifica se duas formas se sobrepõem
int formas_sobrepoem(Forma* forma1, Forma* forma2);

/* retorna o tipo da forma: 'r', 't', 'c', 'l', 'x'
(retângulo, triângulo, círculo, linha, texto)
*/
char forma_get_tipo(Forma* forma);

#endif