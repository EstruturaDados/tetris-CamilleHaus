#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5   // Tamanho fixo da fila

/* ============================
   Estrutura que representa uma peça
   ============================ */
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único
} Peca;

/* ============================
   Estrutura da Fila Circular
   ============================ */
typedef struct {
    Peca itens[TAMANHO_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

/* ============================
   Variável global para gerar IDs únicos
   ============================ */
int contadorId = 0;

/* ============================
   Função para gerar uma nova peça automaticamente
   ============================ */
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    
    nova.nome = tipos[rand() % 4];  // Escolhe tipo aleatório
    nova.id = contadorId++;         // ID sequencial único
    
    return nova;
}

/* ============================
   Inicializa a fila
   ============================ */
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->quantidade = 0;
}

/* ============================
   Verifica se a fila está cheia
   ============================ */
int filaCheia(Fila *f) {
    return f->quantidade == TAMANHO_FILA;
}

/* ============================
   Verifica se a fila está vazia
   ============================ */
int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

/* ============================
   Enqueue - Inserir peça na fila
   ============================ */
void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("⚠ Fila cheia! Não é possível inserir nova peça.\n");
        return;
    }

    f->tras = (f->tras + 1) % TAMANHO_FILA;
    f->itens[f->tras] = p;
    f->quantidade++;

    printf("✔ Peça [%c %d] inserida na fila.\n", p.nome, p.id);
}

/* ============================
   Dequeue - Remover peça da frente
   ============================ */
void dequeue(Fila *f) {
    if (filaVazia(f)) {
        printf("⚠ Fila vazia! Não há peças para jogar.\n");
        return;
    }

    Peca removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAMANHO_FILA;
    f->quantidade--;

    printf("✔ Peça [%c %d] jogada!\n", removida.nome, removida.id);
}

/* ============================
   Exibir estado atual da fila
   ============================ */
void exibirFila(Fila *f) {
    printf("\nFila de peças:\n");

    if (filaVazia(f)) {
        printf("[Fila vazia]\n");
        return;
    }

    int i;
    int indice = f->frente;

    for (i = 0; i < f->quantidade; i++) {
        printf("[%c %d] ", f->itens[indice].nome, f->itens[indice].id);
        indice = (indice + 1) % TAMANHO_FILA;
    }

    printf("\n");
}

/* ============================
   Função principal
   ============================ */
int main() {
    srand(time(NULL));  // Inicializa gerador aleatório

    Fila fila;
    inicializarFila(&fila);

    // Inicializa fila com 5 peças
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    int opcao;

    do {
        exibirFila(&fila);

        printf("\nOpções:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                dequeue(&fila);
                break;

            case 2:
                enqueue(&fila, gerarPeca());
                break;

            case 0:
                printf("Encerrando o jogo...\n");
                break;

            default:
                printf("⚠ Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}
