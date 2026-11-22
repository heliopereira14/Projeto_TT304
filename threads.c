#include "threads.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *rotina_produtor(void *arg) {
    ArgsProdutor *args = (ArgsProdutor *)arg;
    FILE *arq = fopen(args->nome_arquivo, "r");
    int id = 0;
    int tempo_execucao = 0;
    int i = 0;

    if (arq == NULL) {
        perror("Erro ao abrir arquivo de entrada");
    } else {
        while (fscanf(arq, "%d %d", &id, &tempo_execucao) == 2) {
            if (id < 0 || tempo_execucao < 0) {
                fprintf(stderr, "Aviso: tarefa ignorada por ter valores negativos (%d, %d).\n",
                        id, tempo_execucao);
                continue;
            }

            Tarefa *tarefa = criar_tarefa(id, tempo_execucao);
            if (tarefa == NULL) {
                fprintf(stderr, "Erro: falha ao alocar memoria para tarefa %d.\n", id);
                break;
            }

            inserir_fila(args->fila, tarefa);
            printf("Produtor: tarefa %d alocada e enfileirada.\n", tarefa->id);
            fflush(stdout);
        }
        fclose(arq);
    }

    /* Ao final do arquivo, insere N tarefas especiais (id = -1) para parar os consumidores */
    for (i = 0; i < args->num_consumidores; i++) {
        Tarefa *fim = criar_tarefa_encerramento();
        if (fim != NULL) {
            inserir_fila(args->fila, fim);
        }
    }

    return NULL;
}

void *rotina_consumidor(void *arg) {
    ArgsConsumidor *args = (ArgsConsumidor *)arg;

    while (1) {
        Tarefa *tarefa = remover_fila(args->fila);

        /* Se recebeu tarefa especial com id < 0, libera e encerra a thread */
        if (tarefa->id < 0) {
            liberar_tarefa(tarefa);
            break;
        }

        printf("Consumidor %d: iniciando tarefa %d (duração %d s).\n",
               args->id_consumidor, tarefa->id, tarefa->tempo_execucao);
        fflush(stdout);

        sleep((unsigned int)tarefa->tempo_execucao);

        printf("Consumidor %d: tarefa %d concluída.\n",
               args->id_consumidor, tarefa->id);
        fflush(stdout);

        /* Libera o buffer e a struct da tarefa assim que conclui */
        liberar_tarefa(tarefa);
    }

    return NULL;
}