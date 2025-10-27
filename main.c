#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd.h"
#include "svg.h"
#include "formas.h"
#include "fila.h"
#include "pilha.h"
#include "processamento.h"

// -----------------------------
// FUNÇÃO AUXILIAR PARA STRINGS
// -----------------------------

// EXTRAIR_NOME_BASE
char* extrair_nome_base(char* path) {
    char* ultima_barra = strrchr(path, '/');
    if (ultima_barra == NULL) ultima_barra = strrchr(path, '\\');
    if (ultima_barra == NULL) ultima_barra = path - 1;
    char* ultimo_ponto = strrchr(path, '.');
    
    char* inicio_nome = ultima_barra + 1;
    int len;

    if (ultimo_ponto == NULL || ultimo_ponto < ultima_barra) {
        len = strlen(inicio_nome);
    } else {
        len = ultimo_ponto - inicio_nome;
    }

    char* base = (char*) malloc(len + 1);
    if (base == NULL) return NULL; 
 
    strncpy(base, inicio_nome, len);
    base[len] = '\0';

    return base;
}

// ==================
// FUNCAO PRINCIPAL
// ==================
// MAIN
int main(int argc, char *argv[]) {
    char* dir_entrada = NULL;
    char* arq_geo = NULL;
    char* dir_saida = NULL;
    char* arq_qry = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0) dir_entrada = argv[++i];
        else if (strcmp(argv[i], "-f") == 0) arq_geo = argv[++i];
        else if (strcmp(argv[i], "-o") == 0) dir_saida = argv[++i];
        else if (strcmp(argv[i], "-q") == 0) arq_qry = argv[++i];
    }

    if (arq_geo == NULL || dir_saida == NULL) {
        printf("Uso: ./programa -f <arq.geo> -o <dir_saida> [-e <dir_entrada>] [-q <arq.qry>]\n");
        return 1;
    }

    char path_geo[1024];
    if (dir_entrada != NULL) sprintf(path_geo, "%s/%s", dir_entrada, arq_geo);
    else strcpy(path_geo, arq_geo);
    
    char* nome_base_geo = extrair_nome_base(arq_geo);
    char path_svg_geo[1024];
    sprintf(path_svg_geo, "%s/%s.svg", dir_saida, nome_base_geo);
    
    char path_qry[1024];
    char path_svg_qry[1024];
    char path_log_txt[1024];
    
    if (arq_qry != NULL) {
        if (dir_entrada != NULL) sprintf(path_qry, "%s/%s", dir_entrada, arq_qry);
        else strcpy(path_qry, arq_qry);
        
        char* nome_base_qry = extrair_nome_base(arq_qry);
        sprintf(path_svg_qry, "%s/%s-%s.svg", dir_saida, nome_base_geo, nome_base_qry);
        sprintf(path_log_txt, "%s/%s-%s.txt", dir_saida, nome_base_geo, nome_base_qry);
        free(nome_base_qry);
    }

    Fila* formas_chao = criar_fila(500);
    Fila* arena = criar_fila(500);
    Fila* formas_derrotadas = criar_fila(100);
    
    int num_max_disparadores = 10;
    int num_max_carregadores = 10;
    Disparador** disparadores = inicializar_disparadores(num_max_disparadores);
    Carregador** carregadores = inicializar_carregadores(num_max_carregadores);
    
    int proxima_id_global = 1;
    FILE* arquivo_log = NULL;

    printf("iniciando programa! :D\n");

    printf("lendo arquivo GEO: %s\n", path_geo);
    int maior_id_lida = ler_arquivo_geo(path_geo, formas_chao);
    proxima_id_global = maior_id_lida + 1;

    printf("gerando SVG inicial: %s\n", path_svg_geo);
    svg_desenhar(path_svg_geo, formas_chao, NULL);

    if (arq_qry != NULL) {
        printf("processando arquivo QRY: %s\n", path_qry);
        
        arquivo_log = fopen(path_log_txt, "w");
        if (arquivo_log == NULL) {
            printf("ERRO: nao foi possivel criar o arquivo de Log: %s\n", path_log_txt);
        }

        ler_arquivo_qry(path_qry, path_svg_qry, disparadores, num_max_disparadores,
        carregadores, num_max_carregadores, formas_chao, arena, formas_derrotadas,
        &proxima_id_global, arquivo_log);
        
        printf("gerando SVG final: %s\n", path_svg_qry);
        svg_desenhar(path_svg_qry, formas_chao, arena);
        
        if (arquivo_log != NULL) {
            fclose(arquivo_log);
        }
    }

    printf("limpando memoria...\n");

    destruir_disparadores(disparadores, num_max_disparadores);
    destruir_carregadores(carregadores, num_max_carregadores); 
    
    while(!fila_vazia(formas_chao)) {
        destruir_forma((Forma*) desenfileirar(formas_chao));
    }
    destruir_fila(formas_chao);
    
    while(!fila_vazia(arena)) {
        destruir_forma((Forma*) desenfileirar(arena));
    }
    destruir_fila(arena);
    
    while(!fila_vazia(formas_derrotadas)) {
        destruir_forma((Forma*) desenfileirar(formas_derrotadas));
    }
    destruir_fila(formas_derrotadas);

    free(nome_base_geo);
    printf("programa finalizado\n");

    return 0;
}