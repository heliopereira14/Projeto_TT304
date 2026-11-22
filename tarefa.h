#ifndef TAREFA_H
#define TAREFA_H

/* Estrutura de cada tarefa lida do arquivo */
typedef struct Tarefa {
    int id;                 /* Identificador da tarefa (ou -1 para encerrar) */
    int tempo_execucao;     /* Duração em segundos */
    unsigned char *buffer;  /* Buffer de dados zerado (tempo_execucao * 10 bytes) */
    struct Tarefa *prox;    /* Próximo elemento na fila encadeada */
} Tarefa;

/* Aloca dinamicamente uma tarefa e seu buffer preenchido com zeros */
Tarefa *criar_tarefa(int id, int tempo_execucao);

/* Cria a tarefa especial (id = -1) usada para avisar os consumidores para parar */
Tarefa *criar_tarefa_encerramento(void);

/* Libera o buffer interno e a struct da tarefa */
void liberar_tarefa(Tarefa *tarefa);

#endif /* TAREFA_H */