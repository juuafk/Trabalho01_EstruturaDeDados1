# =========================
# INSTRUÇÕES DE COMPILAÇÃO
# =========================

PROJ_NAME = ted
ALUNO = juliagruara
LIBS =  
OBJETOS = main.o pilha.o fila.o formas.o cmd.o svg.o processamento.o campo.o

# compilador
CC = gcc

# flags
CFLAGS= -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration
LDFLAGS=-O0 -fstack-protector-all

# ---------------------
#  INSTRUÇÃO PRINCIPAL
# ---------------------
ted: $(OBJETOS)
	$(CC) -o ted $(LDFLAGS) $(OBJETOS) $(LIBS)

# ---------------------
#  INSTRUÇÃO GENÉRICA
# ---------------------
%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

# ---------------------
#  DEPENDÊNCIAS
# ---------------------
pilha.o: pilha.h
fila.o: fila.h
formas.o: formas.h
campo.o: campo.h fila.h

cmd.o: cmd.h formas.h fila.h processamento.h pilha.h campo.h
svg.o: svg.h formas.h fila.h
processamento.o: processamento.h pilha.h fila.h formas.h campo.h
main.o: pilha.h fila.h formas.h cmd.h svg.h processamento.h campo.h

# --------------------
#  LIMPEZA
# --------------------
clean:
	rm -f $(OBJETOS) ted
