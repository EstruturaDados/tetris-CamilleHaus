#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

/* ============================
   Estrutura da Peça
   ============================ */
typedef struct {
    char nome;
    int id;
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
   Pilha (Reserva)
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
   Geração automática de peça
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

    if (filaVazia(f)) return vazia;

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

void push(Pilha *p, Peca peca) {
    if (pilhaCheia(p)) {
        printf("⚠ Pilha cheia!\n");
        return;
    }

    p->itens[++p->topo] = peca;
}

Peca pop(Pilha *p) {
    Peca vazia = {'-', -1};

    if (pilhaVazia(p)) return vazia;

    return p->itens[p->topo--];
}

/* ============================
   EXIBIÇÃO
   ============================ */
void exibirFila(Fila *f) {
    printf("\nFila de peças\t");

    if (filaVazia(f)) {
        printf("[vazia]");
        return;
    }

    int indice = f->frente;
    for (int i = 0; i < f->quantidade; i++) {
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
   TROCAR PEÇA ATUAL
   ============================ */
void trocarPecaAtual(Fila *fila, Pilha *pilha) {

    if (filaVazia(fila) || pilhaVazia(pilha)) {
        printf("⚠ Não é possível trocar. Fila ou pilha vazia.\n");
        return;
    }

    int indiceFrente = fila->frente;

    Peca temp = fila->itens[indiceFrente];
    fila->itens[indiceFrente] = pilha->itens[pilha->topo];
    pilha->itens[pilha->topo] = temp;

    printf("✔ Troca realizada entre frente da fila e topo da pilha!\n");
}

/* ============================
   TROCA MÚLTIPLA (3 por 3)
   ============================ */
void trocaMultipla(Fila *fila, Pilha *pilha) {

    if (fila->quantidade < 3 || pilha->topo < 2) {
        printf("⚠ Troca múltipla impossível. Necessário 3 peças em cada.\n");
        return;
    }

    int indice = fila->frente;

    for (int i = 0; i < 3; i++) {
        int indiceFila = (indice + i) % TAMANHO_FILA;
        int indicePilha = pilha->topo - i;

        Peca temp = fila->itens[indiceFila];
        fila->itens[indiceFila] = pilha->itens[indicePilha];
        pilha->itens[indicePilha] = temp;
    }

    printf("✔ Troca múltipla realizada com sucesso!\n");
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

    /* Preenche fila inicial */
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    int opcao;

    do {
        printf("\n==============================\n");
        printf("Estado atual:\n");

        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("\n\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("4 - Trocar peça atual\n");
        printf("5 - Troca múltipla (3 por 3)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {

            case 1: {
                Peca jogada = dequeue(&fila);
                if (jogada.id != -1) {
                    printf("✔ Peça [%c %d] jogada!\n", jogada.nome, jogada.id);
                    enqueue(&fila, gerarPeca());
                }
                break;
            }

            case 2: {
                if (pilhaCheia(&pilha)) {
                    printf("⚠ Pilha cheia!\n");
                    break;
                }

                Peca reservada = dequeue(&fila);
                if (reservada.id != -1) {
                    push(&pilha, reservada);
                    enqueue(&fila, gerarPeca());
                }
                break;
            }

            case 3: {
                Peca usada = pop(&pilha);
                if (usada.id != -1)
                    printf("✔ Peça [%c %d] usada da reserva!\n", usada.nome, usada.id);
                break;
            }

            case 4:
                trocarPecaAtual(&fila, &pilha);
                break;

            case 5:
                trocaMultipla(&fila, &pilha);
                break;

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("⚠ Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}