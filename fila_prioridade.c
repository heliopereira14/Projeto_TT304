#include "fila_prioridade.h"
#include <stdlib.h>

FilaPrioridade *criar_fila(int capacidade) {
    FilaPrioridade *fila = (FilaPrioridade *)malloc(sizeof(FilaPrioridade));
    if (fila == NULL) {
        return NULL;
    }

    fila->inicio = NULL;
    fila->qtd = 0;
    fila->capacidade = capacidade;

    if (pthread_mutex_init(&fila->mutex, NULL) != 0) {
        free(fila);
        return NULL;
    }

    if (pthread_cond_init(&fila->nao_cheia, NULL) != 0) {
        pthread_mutex_destroy(&fila->mutex);
        free(fila);
        return NULL;
    }

    if (pthread_cond_init(&fila->nao_vazia, NULL) != 0) {
        pthread_cond_destroy(&fila->nao_cheia);
        pthread_mutex_destroy(&fila->mutex);
        free(fila);
        return NULL;
    }

    return fila;
}

void inserir_fila(FilaPrioridade *fila, Tarefa *nova) {
    Tarefa *anterior = NULL;
    Tarefa *atual = NULL;

    pthread_mutex_lock(&fila->mutex);

    /* Laço while para evitar problema com acordada espúria (spurious wakeup) */
    while (fila->qtd == fila->capacidade) {
        pthread_cond_wait(&fila->nao_cheia, &fila->mutex);
    }

    atual = fila->inicio;

    if (nova->id < 0) {
        /* Tarefa especial de encerramento vai sempre para o final da fila */
        while (atual != NULL) {
            anterior = atual;
            atual = atual->prox;
        }
    } else {
        /* Ordena pelo menor tempo_execucao.
         * Usar '<=' garante que empates fiquem em ordem de chegada (FIFO). */
        while (atual != NULL && atual->id >= 0 && atual->tempo_execucao <= nova->tempo_execucao) {
            anterior = atual;
            atual = atual->prox;
        }
    }

    if (anterior == NULL) {
        nova->prox = fila->inicio;
        fila->inicio = nova;
    } else {
        nova->prox = atual;
        anterior->prox = nova;
    }

    fila->qtd++;

    pthread_cond_signal(&fila->nao_vazia);
    pthread_mutex_unlock(&fila->mutex);
}

Tarefa *remover_fila(FilaPrioridade *fila) {
    Tarefa *removida = NULL;

    pthread_mutex_lock(&fila->mutex);

    while (fila->qtd == 0) {
        pthread_cond_wait(&fila->nao_vazia, &fila->mutex);
    }

    /* Como a lista já está ordenada, a maior prioridade está sempre no início */
    removida = fila->inicio;
    fila->inicio = removida->prox;
    removida->prox = NULL;
    fila->qtd--;

    pthread_cond_signal(&fila->nao_cheia);
    pthread_mutex_unlock(&fila->mutex);

    return removida;
}

void destruir_fila(FilaPrioridade *fila) {
    Tarefa *atual = NULL;
    Tarefa *proximo = NULL;

    if (fila == NULL) {
        return;
    }

    atual = fila->inicio;
    while (atual != NULL) {
        proximo = atual->prox;
        liberar_tarefa(atual);
        atual = proximo;
    }

    pthread_cond_destroy(&fila->nao_vazia);
    pthread_cond_destroy(&fila->nao_cheia);
    pthread_mutex_destroy(&fila->mutex);
    free(fila);
}