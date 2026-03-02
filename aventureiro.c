#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

/* ============================
   Estrutura da Peça
   ============================ */
typedef struct {
    char nome;  // Tipo da peça
    int id;     // ID único
} Peca;

/* ============================
   Fila Circular
   ============================ */
typedef struct {
    Peca itens[TAMANHO_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

/* ============================
   Pilha de Reserva (LIFO)
   ============================ */
typedef struct {
    Peca itens[TAMANHO_PILHA];
    int topo;
} Pilha;

/* ============================
   Variável global de ID
   ============================ */
int contadorId = 0;

/* ============================
   Geração de peças aleatórias
   ============================ */
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};

    nova.nome = tipos[rand() % 4];
    nova.id = contadorId++;

    return nova;
}

/* ============================
   FUNÇÕES DA FILA
   ============================ */
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->quantidade = 0;
}

int filaCheia(Fila *f) {
    return f->quantidade == TAMANHO_FILA;
}

int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;

    f->tras = (f->tras + 1) % TAMANHO_FILA;
    f->itens[f->tras] = p;
    f->quantidade++;
}

Peca dequeue(Fila *f) {
    Peca vazia = {'-', -1};

    if (filaVazia(f)) {
        return vazia;
    }

    Peca removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAMANHO_FILA;
    f->quantidade--;

    return removida;
}

/* ============================
   FUNÇÕES DA PILHA
   ============================ */
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAMANHO_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

void push(Pilha *p, Peca peça) {
    if (pilhaCheia(p)) {
        printf("⚠ Pilha cheia! Não é possível reservar mais peças.\n");
        return;
    }

    p->topo++;
    p->itens[p->topo] = peça;
    printf("✔ Peça [%c %d] enviada para a reserva.\n", peça.nome, peça.id);
}

Peca pop(Pilha *p) {
    Peca vazia = {'-', -1};

    if (pilhaVazia(p)) {
        printf("⚠ Pilha vazia! Não há peças reservadas.\n");
        return vazia;
    }

    Peca removida = p->itens[p->topo];
    p->topo--;

    printf("✔ Peça [%c %d] usada da reserva!\n", removida.nome, removida.id);
    return removida;
}

/* ============================
   EXIBIÇÃO DO ESTADO
   ============================ */
void exibirFila(Fila *f) {
    printf("Fila de peças:\t");

    if (filaVazia(f)) {
        printf("[vazia]");
        return;
    }

    int i, indice = f->frente;

    for (i = 0; i < f->quantidade; i++) {
        printf("[%c %d] ", f->itens[indice].nome, f->itens[indice].id);
        indice = (indice + 1) % TAMANHO_FILA;
    }
}

void exibirPilha(Pilha *p) {
    printf("\nPilha de reserva\t(Topo -> Base): ");

    if (pilhaVazia(p)) {
        printf("[vazia]");
        return;
    }

    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
}

/* ============================
   MAIN
   ============================ */
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    /* Preenche a fila inicial */
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    int opcao;

    do {
        printf("\n==============================\n");
        printf("Estado atual:\n\n");

        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("\n\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {

            /* Jogar peça (remove da fila) */
            case 1: {
                Peca jogada = dequeue(&fila);
                if (jogada.id != -1) {
                    printf("✔ Peça [%c %d] jogada!\n", jogada.nome, jogada.id);
                    enqueue(&fila, gerarPeca()); // mantém fila cheia
                }
                break;
            }

            /* Reservar peça (fila -> pilha) */
            case 2: {
                if (pilhaCheia(&pilha)) {
                    printf("⚠ Pilha cheia! Não é possível reservar.\n");
                    break;
                }

                Peca reservada = dequeue(&fila);
                if (reservada.id != -1) {
                    push(&pilha, reservada);
                    enqueue(&fila, gerarPeca()); // mantém fila cheia
                }
                break;
            }

            /* Usar peça da reserva */
            case 3: {
                Peca usada = pop(&pilha);
                // Não retorna para fila, apenas remove do jogo
                break;
            }

            case 0:
                printf("Encerrando o jogo...\n");
                break;

            default:
                printf("⚠ Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}