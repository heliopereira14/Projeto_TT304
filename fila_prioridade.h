#ifndef FILA_PRIORIDADE_H
#define FILA_PRIORIDADE_H

#include "tarefa.h"
#include <pthread.h>

#ifndef MAX_TASKS
#define MAX_TASKS 20
#endif

#if MAX_TASKS <= 0
#error "MAX_TASKS precisa ser maior que zero."
#endif

/* Fila de prioridade limitada com mutex e variáveis de condição */
typedef struct FilaPrioridade {
    Tarefa *inicio;             /* Aponta para a tarefa de maior prioridade (menor tempo) */
    int qtd;                    /* Número atual de tarefas na fila */
    int capacidade;             /* Limite máximo (MAX_TASKS) */
    pthread_mutex_t mutex;      /* Trava para exclusão mútua */
    pthread_cond_t nao_cheia;   /* Sinaliza quando abre vaga na fila */
    pthread_cond_t nao_vazia;   /* Sinaliza quando entra uma tarefa na fila */
} FilaPrioridade;

/* Aloca e inicializa a fila de prioridade no heap */
FilaPrioridade *criar_fila(int capacidade);

/* Insere mantendo a ordenação por menor tempo (bloqueia se estiver cheia) */
void inserir_fila(FilaPrioridade *fila, Tarefa *nova);

/* Retira a primeira tarefa da fila (bloqueia se estiver vazia) */
Tarefa *remover_fila(FilaPrioridade *fila);

/* Limpa possíveis restos da fila, destrói mutex/conds e dá free */
void destruir_fila(FilaPrioridade *fila);

#endif /* FILA_PRIORIDADE_H */