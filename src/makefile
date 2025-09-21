# =========================
# INSTRUÇÕES DE COMPILAÇÃO
# =========================

PROJ_NAME = ted
ALUNO = juliagruara
LIBS =  
OBJETOS = main.o pilha.o fila.o formas.o leitura.o svg.o processamento.o

# compilador
CC = gcc

# flags
CFLAGS= -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration
LDFLAGS=-O0

# ---------------------
#  INSTRUÇÃO PRINCIPAL - como fazer o programa FINAL
# ---------------------
$(PROJ_NAME): $(OBJETOS)
	$(CC) -o $(PROJ_NAME) $(LDFLAGS) $(OBJETOS) $(LIBS)

# ---------------------
#  INSTRUÇÃO GENÉRICA - como fazer qualquer arquivo .o
# ---------------------
%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

# dependências extras (para cada módulo)
# orienta sobre a dependência de um arquivo .o em relação a um .h

pilha.o: pilha.h
fila.o: fila.h
formas.o: formas.h

leitura.o: leitura.h formas.h
svg.o: svg.h formas.h
processamento.o: processamento.h pilha.h fila.h formas.h
main.o: pilha.h fila.h formas.h leitura.h svg.h processamento.h

# --------------------
#  LIMPEZA (opcional)
# --------------------
clean:
	rm -f $(OBJETOS) $(PROJ_NAME)
