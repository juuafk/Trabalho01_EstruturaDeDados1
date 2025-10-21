#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cmd.h"   
#include "formas.h"
#include "fila.h"
#include "processamento.h"

#define MAX_LINHA 1024 

void ler_arquivo_geo(const char* nome_arquivo_geo, Fila* formas_chao) {
    FILE* arquivo = fopen(nome_arquivo_geo, "r");
    if (arquivo == NULL) {
        printf("ERRO: não foi possivel abrir o arquivo .geo: %s\n :(", nome_arquivo_geo);
        return;
    }

    char linha[MAX_LINHA];
    char comando[8];

    // lê o arquivo linha por linha
    while (fgets(linha, sizeof(linha), arquivo)) {
        // ignora linhas vazias ou comentários
        if (linha[0] == '\n' || linha[0] == '#') {
            continue;
        }

        sscanf(linha, "%s", comando);

        if (strcmp(comando, "c") == 0) {
            int id;
            double x, y, r;
            char corb[32], corp[32];
            sscanf(linha, "c %d %lf %lf %lf %s %s", &id, &x, &y, &r, corb, corp);
            Forma* f = criar_circulo(id, x, y, r, corb, corp);
            enfileirar(formas_chao, f);

        } else if (strcmp(comando, "r") == 0) {
            int id;
            double x, y, w, h;
            char corb[32], corp[32];
            sscanf(linha, "r %d %lf %lf %lf %lf %s %s", &id, &x, &y, &w, &h, corb, corp);
            Forma* f = criar_retangulo(id, x, y, w, h, corb, corp);
            enfileirar(formas_chao, f);

        } else if (strcmp(comando, "t") == 0) {
            int id;
            double x1, y1, x2, y2, x3, y3;
            char corb[32], corp[32];
            sscanf(linha, "t %d %lf %lf %lf %lf %lf %lf %s %s", &id, &x1, &y1, &x2, &y2, &x3, &y3, corb, corp);
            Forma* f = criar_triangulo(id, x1, y1, x2, y2, x3, y3, corb, corp);
            enfileirar(formas_chao, f);

        } else if (strcmp(comando, "l") == 0) {
            int id;
            double x1, y1, x2, y2;
            char cor[32];
            sscanf(linha, "l %d %lf %lf %lf %lf %s", &id, &x1, &y1, &x2, &y2, cor);
            Forma* f = criar_linha(id, x1, y1, x2, y2, cor);
            enfileirar(formas_chao, f);

        } else if (strcmp(comando, "x") == 0) {
            int id;
            double x, y;
            char ancora;
            char corb[32], corp[32], texto[MAX_LINHA];
            sscanf(linha, "x %d %c %lf %lf %s %s %[^\n]", &id, &ancora, &x, &y, corb, corp, texto);
            Forma* f = criar_texto(id, ancora, x, y, texto, corb, corp);
            enfileirar(formas_chao, f);
        }
    }

    fclose(arquivo);
}

//FALTA COISAAA!!