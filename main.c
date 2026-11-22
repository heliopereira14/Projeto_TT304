#include "fila_prioridade.h"
#include "threads.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <pthread.h>

/* Converte e valida o número de consumidores passado via terminal */
static int ler_inteiro_positivo(const char *texto, int *saida) {
    char *fim_ptr = NULL;
    long valor = 0;

    if (texto == NULL || *texto == '\0') {
        return 0;
    }

    errno = 0;
    valor = strtol(texto, &fim_ptr, 10);

    if (errno != 0 || *fim_ptr != '\0' || valor <= 0 || valor > INT_MAX) {
        return 0;
    }

    *saida = (int)valor;
    return 1;
}

int main(int argc, char **argv) {
    int num_consumidores = 0;
    int i = 0;
    FilaPrioridade *fila = NULL;
    ArgsProdutor *args_prod = NULL;
    ArgsConsumidor *args_cons = NULL;
    pthread_t *thread_prod = NULL;
    pthread_t *threads_cons = NULL;

    if (argc != 3 || !ler_inteiro_positivo(argv[1], &num_consumidores)) {
        fprintf(stderr, "Uso: %s <num_consumidores> <arquivo_tarefas>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Aloca todas as structs e vetores dinamicamente em etapa única */
    fila = criar_fila(MAX_TASKS);
    args_prod = (ArgsProdutor *)malloc(sizeof(ArgsProdutor));
    args_cons = (ArgsConsumidor *)calloc((size_t)num_consumidores, sizeof(ArgsConsumidor));
    thread_prod = (pthread_t *)malloc(sizeof(pthread_t));
    threads_cons = (pthread_t *)malloc((size_t)num_consumidores * sizeof(pthread_t));

    if (fila == NULL || args_prod == NULL || args_cons == NULL ||
        thread_prod == NULL || threads_cons == NULL) {
        fprintf(stderr, "Erro: falha na alocacao dinamica de memoria.\n");
        free(threads_cons);
        free(thread_prod);
        free(args_cons);
        free(args_prod);
        destruir_fila(fila);
        return EXIT_FAILURE;
    }

    args_prod->fila = fila;
    args_prod->nome_arquivo = argv[2];
    args_prod->num_consumidores = num_consumidores;

    if (pthread_create(thread_prod, NULL, rotina_produtor, args_prod) != 0) {
        fprintf(stderr, "Erro: falha ao criar a thread produtora.\n");
        free(threads_cons);
        free(thread_prod);
        free(args_cons);
        free(args_prod);
        destruir_fila(fila);
        return EXIT_FAILURE;
    }

    for (i = 0; i < num_consumidores; i++) {
        args_cons[i].fila = fila;
        args_cons[i].id_consumidor = i + 1;

        if (pthread_create(&threads_cons[i], NULL, rotina_consumidor, &args_cons[i]) != 0) {
            int j = 0;
            fprintf(stderr, "Erro: falha ao criar a thread consumidora %d.\n", i + 1);
            pthread_join(*thread_prod, NULL);
            for (j = 0; j < i; j++) {
                pthread_join(threads_cons[j], NULL);
            }
            free(threads_cons);
            free(thread_prod);
            free(args_cons);
            free(args_prod);
            destruir_fila(fila);
            return EXIT_FAILURE;
        }
    }

    /* Aguarda o término do produtor e de todos os consumidores */
    pthread_join(*thread_prod, NULL);

    for (i = 0; i < num_consumidores; i++) {
        pthread_join(threads_cons[i], NULL);
    }

    /* Libera toda a memória alocada antes de finalizar o processo */
    free(threads_cons);
    free(thread_prod);
    free(args_cons);
    free(args_prod);
    destruir_fila(fila);

    return EXIT_SUCCESS;
}