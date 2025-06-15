#ifndef TABELA_SIMBOLOS_H
#define TABELA_SIMBOLOS_H

#define TAM_TAB 100

typedef struct {
    char nome[TAM_MAX_LEXEMA];
} EntradaTS;

void inicializaTabela();
int buscaNaTabela(const char *nome);
int insereNaTabela(const char *nome);
void imprimeTabela();

#endif
