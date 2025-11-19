#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Estruturas de Dados ---
typedef struct Comodo {
    char nome[50];
    char pista[100]; 
    struct Comodo *esq; 
    struct Comodo *dir; 
} Comodo;

typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esq; 
    struct PistaNode *dir;
} PistaNode;

// --- Protótipos das Funções ---
Comodo* criarSala(const char *nome, const char *pista, Comodo *e, Comodo *d);
PistaNode* inserirPista(PistaNode *raiz, const char *pista);
void explorarSalasComPistas(Comodo *atual, PistaNode **raizPistas);
void exibirPistas(PistaNode *raiz);
void liberarMemoriaComodos(Comodo *raiz);
void liberarMemoriaPistas(PistaNode *raiz);

// --- Funções de Estrutura de Dados ---

Comodo* criarSala(const char *nome, const char *pista, Comodo *e, Comodo *d) {
    Comodo *novo = (Comodo*)malloc(sizeof(Comodo));
    if (novo == NULL) exit(1);
    strcpy(novo->nome, nome);
    strcpy(novo->pista, pista);
    novo->esq = e;
    novo->dir = d;
    return novo;
}

PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*)malloc(sizeof(PistaNode));
        if (novo == NULL) exit(1);
        strcpy(novo->pista, pista);
        novo->esq = novo->dir = NULL;
        return novo;
    }
    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0) {
        raiz->esq = inserirPista(raiz->esq, pista);
    } else if (cmp > 0) {
        raiz->dir = inserirPista(raiz->dir, pista);
    }
    return raiz;
}

void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf("  - %s\n", raiz->pista);
        exibirPistas(raiz->dir);
    }
}

// --- Funções de Lógica do Jogo ---

void explorarSalasComPistas(Comodo *atual, PistaNode **raizPistas) {
    char escolha;
    
    printf("\n--- EXPLORACAO INICIADA ---\n");
    
    while (atual != NULL) {
        printf("\nVoce esta em: **%s**\n", atual->nome);
        
        if (strlen(atual->pista) > 0) {
            printf(">>> 🔍 Pista Encontrada: '%s' <<<\n", atual->pista);
            *raizPistas = inserirPista(*raizPistas, atual->pista);
            printf("Pista registrada.\n");
        } else {
            printf("Nao ha pistas relevantes neste comodo.\n");
        }
        
        printf("\nPara onde ir? (e)squerda, (d)ireita, ou (s)air: ");
        
        // CORREÇÃO: Lê a escolha e limpa o buffer do teclado
        if (scanf(" %c", &escolha) != 1) break; 
        while (getchar() != '\n');

        if (escolha == 's') {
            printf("\nSaindo da mansao...\n");
            break;
        } else if (escolha == 'e') {
            atual = atual->esq != NULL ? atual->esq : (printf("Caminho bloqueado!\n"), atual);
        } else if (escolha == 'd') {
            atual = atual->dir != NULL ? atual->dir : (printf("Caminho bloqueado!\n"), atual);
        } else {
            printf("Opcao invalida. Tente 'e', 'd' ou 's'.\n");
        }
    }
}

// --- Funções de Limpeza de Memória ---

void liberarMemoriaComodos(Comodo *raiz) {
    if (raiz != NULL) {
        liberarMemoriaComodos(raiz->esq);
        liberarMemoriaComodos(raiz->dir);
        free(raiz);
    }
}

void liberarMemoriaPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        liberarMemoriaPistas(raiz->esq);
        liberarMemoriaPistas(raiz->dir);
        free(raiz);
    }
}

// ----------------------------------------------------------------------
// Função Principal (main)
// ----------------------------------------------------------------------

int main() {
    Comodo *mansaoRaiz = NULL;
    PistaNode *pistasRaiz = NULL; 

    printf("--- Detective Quest: Coleta de Pistas ---\n");

    // Montagem da Mansão
    Comodo *cozinha = criarSala("Cozinha", "Peca de xadrez branca", NULL, NULL);
    Comodo *biblioteca = criarSala("Biblioteca", "Po de giz nos livros", NULL, cozinha);
    Comodo *escritorio = criarSala("Escritorio", "Taca de cristal quebrada", NULL, NULL); 
    Comodo *salaMusica = criarSala("Sala de Musica", "Luva de seda preta", escritorio, NULL);
    mansaoRaiz = criarSala("Hall de Entrada", "Jornal antigo rasgado", biblioteca, salaMusica);
    printf("Mapa pronto! Mansao montada.\n");
    
    // Explorar e Coletar
    explorarSalasComPistas(mansaoRaiz, &pistasRaiz);
    
    // Exibir o Resultado Final
    printf("\n=========================================\n");
    printf("        CADERNETA DE PISTAS FINAIS       \n");
    printf("=========================================\n");
    
    if (pistasRaiz == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(pistasRaiz);
    }
    
    // Limpeza de Memória
    liberarMemoriaComodos(mansaoRaiz);
    liberarMemoriaPistas(pistasRaiz);
    printf("\nMemoria liberada. Fim do programa.\n");
    
    return 0;
}