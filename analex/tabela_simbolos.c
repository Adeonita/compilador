#include <string.h>
#include <stdio.h>
#include "Analex.h"
#include "tabela_simbolos.h"

static EntradaTS tabela[TAM_TAB];
static int proxPosLivre = 0;

void inicializaTabela() {
    proxPosLivre = 0;
}

int buscaNaTabela(const char *nome) {
    for (int i = 0; i < proxPosLivre; i++) {
        if (strcmp(tabela[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

int insereNaTabela(const char *nome) {
    int pos = buscaNaTabela(nome);
    if (pos == -1 && proxPosLivre < TAM_TAB) {
        strcpy(tabela[proxPosLivre].nome, nome);
        return proxPosLivre++;
    }
    return pos;
}

void imprimeTabela() {
    printf("\nTabela de Símbolos:\n");
    for (int i = 0; i < proxPosLivre; i++) {
        printf("%d: %s\n", i, tabela[i].nome);
    }
}
