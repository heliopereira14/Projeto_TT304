## Estrutura do Projeto

- `main.c`: validação dos argumentos de linha de comando, alocação dinâmica inicial das estruturas, criação/sincronização das threads (`pthread_create` e `pthread_join`) e liberação final de memória.
- `tarefa.h` e `tarefa.c`: definição da estrutura de tarefa, alocação dinâmica da tarefa e de seu buffer zerado (`tempo_execucao * 10` bytes), criação das tarefas especiais de encerramento (`id = -1`) e liberação de memória.
- `fila_prioridade.h` e `fila_prioridade.c`: fila de prioridade implementada como lista encadeada ordenada com capacidade máxima `MAX_TASKS`, protegida por `pthread_mutex_t` e variáveis de condição (`nao_cheia` e `nao_vazia`). A prioridade segue o menor `tempo_execucao`, com desempate pela ordem de chegada (FIFO).
- `threads.h` e `threads.c`: rotina da thread produtora (leitura do arquivo texto, enfileiramento e envio de `N` tarefas de encerramento) e rotina das `N` threads consumidoras (remoção por prioridade, simulação com `sleep` e liberação de memória).
- `Makefile`: automação da compilação com flags estritas (`-std=c11`, `-Wall`, `-Wextra`, `-Werror`, `-pedantic`, `-pthread`) e definição de `MAX_TASKS`.

## Formato do Arquivo de Entrada

Cada linha do arquivo de entrada deve conter dois inteiros separados por espaço: `<id> <tempo_execucao>` (em segundos). Exemplo (`tarefas.txt`):

text
101 4
102 2
103 2
104 1
105 3


## Compilação

Compilar com o valor padrão de `MAX_TASKS` (20):

bash
make

Recompilar definindo outro limite para `MAX_TASKS` em tempo de compilação (exemplo com `MAX_TASKS=4`):

bash
make clean && make MAX_TASKS=4


## Execução

Passe o número `N` de threads consumidoras e o caminho do arquivo de entrada pela linha de comando:

bash
./escalonador 3 tarefas.txt


## Verificação de Memória com Valgrind

Para conferir se não há vazamentos de memória:

bash
valgrind --leak-check=full --show-leak-kinds=all ./escalonador 3 tarefas.txt
