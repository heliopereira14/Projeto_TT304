#include "tarefa.h"
#include <stdlib.h>
#include <limits.h>

Tarefa *criar_tarefa(int id, int tempo_execucao) {
    Tarefa *nova = (Tarefa *)malloc(sizeof(Tarefa));
    if (nova == NULL) {
        return NULL;
    }

    nova->id = id;
    nova->tempo_execucao = tempo_execucao;
    nova->prox = NULL;

    if (tempo_execucao > 0) {
        /* calloc já aloca e zera todos os bytes de uma vez */
        size_t tam_buffer = (size_t)tempo_execucao * 10U;
        nova->buffer = (unsigned char *)calloc(tam_buffer, sizeof(unsigned char));
        if (nova->buffer == NULL) {
            free(nova);
            return NULL;
        }
    } else {
        nova->buffer = NULL;
    }

    return nova;
}

Tarefa *criar_tarefa_encerramento(void) {
    Tarefa *fim = (Tarefa *)malloc(sizeof(Tarefa));
    if (fim == NULL) {
        return NULL;
    }

    /* Colocamos INT_MAX no tempo para garantir que ela fique no fim da fila
     * e nenhuma thread saia antes de terminar todas as tarefas válidas */
    fim->id = -1;
    fim->tempo_execucao = INT_MAX;
    fim->buffer = NULL;
    fim->prox = NULL;

    return fim;
}

void liberar_tarefa(Tarefa *tarefa) {
    if (tarefa == NULL) {
        return;
    }
    free(tarefa->buffer);
    free(tarefa);
}