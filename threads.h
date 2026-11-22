#ifndef THREADS_H
#define THREADS_H

#include "fila_prioridade.h"

/* Parâmetros enviados dinamicamente para a thread produtora */
typedef struct ArgsProdutor {
    FilaPrioridade *fila;
    const char *nome_arquivo;
    int num_consumidores;
} ArgsProdutor;

/* Parâmetros enviados dinamicamente para cada thread consumidora */
typedef struct ArgsConsumidor {
    FilaPrioridade *fila;
    int id_consumidor;
} ArgsConsumidor;

/* Função executada pela thread produtora */
void *rotina_produtor(void *arg);

/* Função executada pelas N threads consumidoras */
void *rotina_consumidor(void *arg);

#endif /* THREADS_H */