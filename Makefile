CC        = gcc
MAX_TASKS ?= 20
CFLAGS    = -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Werror -pedantic -O2 -DMAX_TASKS=$(MAX_TASKS)
LDFLAGS   = -pthread

BINARIO   = escalonador
FONTES    = main.c tarefa.c fila_prioridade.c threads.c
OBJETOS   = $(FONTES:.c=.o)

all: $(BINARIO)

$(BINARIO): $(OBJETOS)
	$(CC) $(CFLAGS) -o $(BINARIO) $(OBJETOS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJETOS) $(BINARIO)

.PHONY: all clean
