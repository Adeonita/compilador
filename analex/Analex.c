#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "Analex.h"
// #include "reconhecedores.h"

#define TAM_LEXEMA 50
#define TAM_NUM 20
#define ESTADO_LEU_BARRA 17
#define ESTADO_LEU_BARRA_ASTERISCO 20


void error(char msg[])
{
    printf("%s\n", msg);
    exit(1);
}

void mudaEstadoEIncrementaLexema(int *estado, int novoEstado, char *lexema, int *tamL, char c)
{
    *estado = novoEstado;
    lexema[*tamL] = c;
    lexema[++(*tamL)] = '\0';
}

void mudaEstadoEIncrementaDigito(int *estado, int novoEstado, char *digitos, int *tamD, char c)
{
    *estado = novoEstado;
    digitos[*tamD] = c;
    digitos[++(*tamD)] = '\0';
}

TOKEN reconhecedorBase(char categoria, char codigo)
{
    TOKEN t;
    t.cat = categoria;
    t.codigo = codigo;

    return t;
}

TOKEN reconheceOr(int *estado)
{
    *estado = 44;
    return reconhecedorBase(SN, OPERADOR_OR);
}

TOKEN reconheceAnd(int *estado)
{
    *estado = 47;
    return reconhecedorBase(SN, OPERADOR_AND);
}

TOKEN reconheceComparacao(int *estado)
{
    *estado = 28;
    return reconhecedorBase(SN, COMPARACAO);
}

TOKEN reconheceAtribuicao(int *estado, FILE *fd, const char c) {
    *estado = 29;
    ungetc(c, fd);

    return reconhecedorBase(SN, ATRIB);
}

TOKEN reconheceNegacao(int *estado, FILE *fd, const char c)
{
    *estado = 41;
    ungetc(c, fd);
    return reconhecedorBase(SN, OPERADOR_NEGACAO);
}

TOKEN reconheceDiferente(int *estado)
{
    *estado = 42;
    return reconhecedorBase(SN, OPERADOR_DIFERENTE);
}

TOKEN reconheceMenorQue(int *estado, FILE *fd, const char c)
{
    *estado = 26;
    ungetc(c, fd);

    return reconhecedorBase(SN, MENOR_QUE);
}


TOKEN reconheceMenorOuIgual(int *estado)
{
    *estado = 25;

    return reconhecedorBase(SN, MENOR_OU_IGUAL);
}

TOKEN reconheceMaiorQue(int *estado, FILE *fd, const char c)
{
    *estado = 23;
    ungetc(c, fd);

    return reconhecedorBase(SN, MAIOR_QUE);
}

TOKEN reconheceDivisao(int *estado, FILE *fd, const char c)
{
    *estado = 18;
    ungetc(c, fd);

    return reconhecedorBase(SN, DIVISAO);
}


TOKEN reconheceMaiorOuIgual(int *estado)
{
    *estado = 22;

    return reconhecedorBase(SN, MAIOR_OU_IGUAL);
}

TOKEN reconheceConstInt(int *estado, FILE *fd, const char *digitos, char c)
{
    TOKEN t;
    *estado = 4;
    ungetc(c, fd);
    t.cat = CT_I;
    t.valInt = atoi(digitos);
    return t;
}

TOKEN reconheceConstReal(int *estado, FILE *fd, const char *digitos, char c)
{
    TOKEN t;
    *estado = 48;
    ungetc(c, fd); //precisa do atof e do ungetc ?
    t.cat = CT_REAL;
    t.realVal = atof(digitos);
    return t;
}

TOKEN AnaLex(FILE *fd)
{
    int estado;
    char lexema[TAM_LEXEMA] = "";

    int tamL = 0;
    char digitos[TAM_NUM] = "";

    int tamD = 0;

    TOKEN t;
    estado = 0;

    while (true)
    {
        char c = fgetc(fd);
        bool leuLetra = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
        bool leuDigito = c >= '0' && c <= '9';
        bool leuSinalDeIgual = c == '=';
        bool leuFimDeLinha = c == EOF;
        bool leuLetraOuDigitoOuUnderscore = (leuLetra) || (leuDigito) || (c == '_');
        bool leuQuebraDeLinha = c == '\n';
        bool leuPonto = c == '.';
        bool LEU_E_COMERCIAL = c == '&';
        bool LEU_PIPE = c == '|';
        bool LEU_EXCLAMACAO = c == '!';
        bool LEU_ADICAO = c == '+';
        bool LEU_SUBTRACAO = c == '-';
        bool LEU_ASTERISCO = c == '*';
        bool LEU_BARRA = c == '/';

        bool LEU_MAIOR_QUE = c == '>';
        bool LEU_MENOR_QUE = c == '<';

        switch (estado)
        {
        case 0:
            if (c == ' ' || c == '\t')
            {
                estado = 0;
            }
            else if (leuLetra)
            {
                mudaEstadoEIncrementaLexema(&estado, 1, lexema, &tamL, c);
            }
            else if (leuDigito)
            {
                mudaEstadoEIncrementaDigito(&estado, 3, digitos, &tamD, c);
            }
            else if (leuSinalDeIgual)
            {
                mudaEstadoEIncrementaLexema(&estado, 27, lexema, &tamL, c);
            }
            else if (LEU_E_COMERCIAL)
            {
                mudaEstadoEIncrementaLexema(&estado, 45, lexema, &tamL, c);
            }
            else if (LEU_PIPE)
            {
                mudaEstadoEIncrementaLexema(&estado, 43, lexema, &tamL, c);
            }
            else if (LEU_EXCLAMACAO)
            {
                mudaEstadoEIncrementaLexema(&estado, 40, lexema, &tamL, c);
            }
            else if (LEU_MENOR_QUE) {
                mudaEstadoEIncrementaLexema(&estado, 24, lexema, &tamL, c);
            }
            else if (LEU_MAIOR_QUE){
                mudaEstadoEIncrementaLexema(&estado, 21, lexema, &tamL, c);
            }
            else if (LEU_BARRA) {
                mudaEstadoEIncrementaLexema(&estado, 17, lexema, &tamL, c);
            }

            else if (leuQuebraDeLinha)
            {
                estado = 0;
                t.cat = FIM_EXPR; // fim de linha (ou expressao) encontrado
                contLinha++;
                return t;
            }
            else if (leuFimDeLinha)
            {
                t.cat = FIM_ARQ;
                return t;
            }
            else
                error("Caracter invalido na expressao!");

            break;
        case 1:
            if (leuLetraOuDigitoOuUnderscore)
            {
                mudaEstadoEIncrementaLexema(&estado, 1, lexema, &tamL, c);
            }
            else
            {
                // transicao OUTRO* do estado 1 do AFD
                estado = 2;
                ungetc(c, fd);
                t.cat = ID;
                strcpy(t.lexema, lexema);
                return t;
            }

            break;

        case 3:
            if (leuDigito)
            {
                mudaEstadoEIncrementaDigito(&estado, 3, digitos, &tamD, c);
            }
            else if (leuPonto)
            {
                mudaEstadoEIncrementaDigito(&estado, 5, digitos, &tamD, c);
            }
            else
            {
                return reconheceConstInt(&estado, fd, digitos, c);
            }
            break;
        case 5:
            if (leuDigito)
            {
                mudaEstadoEIncrementaDigito(&estado, 6, digitos, &tamD, c);
            }
            else
            {
                error("Caractere invalido no ESTADO 3!");
            }
            break;
        case 6:
            if (leuDigito)
            {
                mudaEstadoEIncrementaDigito(&estado, 6, digitos, &tamD, c);
            }
            else
            {
                return reconheceConstReal(&estado, fd, digitos, c);
            }
            break;
        case 17:
            if (LEU_ASTERISCO) {
                mudaEstadoEIncrementaLexema(&estado, 20, lexema, &tamL, c);
            }
            return reconheceDivisao(&estado, fd, c);
        // case 20: TODO: TRATAR COMENTARIO
        //     if (leuLetra) {
        //         mudaEstadoEIncrementaLexema(&estado, 20, lexema, &tamL, c);
        //     }

            
        case 21:
            if (leuSinalDeIgual) {
                return reconheceMaiorOuIgual(&estado);
            }
            else {
                return reconheceMaiorQue(&estado, fd, c);
            }
            break;

        case 24:
            if (leuSinalDeIgual) {
                return reconheceMenorOuIgual(&estado);
            }
            else {
                return reconheceMenorQue(&estado, fd, c);
            }
            break;

        case 27:
            if (leuSinalDeIgual)
            {
                return reconheceComparacao(&estado);
            }
            else
            {
                return reconheceAtribuicao(&estado, fd, c);
            }
            break;
        case 40:
            if (leuSinalDeIgual)
            {
                return reconheceDiferente(&estado);
            }
            else
            {
                return reconheceNegacao(&estado, fd, c);
            }
        case 43:
            if (LEU_PIPE)
            {
                return reconheceOr(&estado);
            }
            else
            {
                error("Caractere invalido no ESTADO 43!");
            }

        case 45:
            if (LEU_E_COMERCIAL)
            {
                return reconheceAnd(&estado);
            }
            else
            {
                estado = 46;
                ungetc(c, fd);
                t.cat = SN;
                t.codigo = PONTEIRO;

                return t;
            }
            break;
        }
    }
}

int main()
{
    printf("inicio");
    FILE *fd;
    TOKEN tk;

    if ((fd = fopen("expressao.dat", "r")) == NULL)
        error("Arquivo de entrada da expressao nao encontrado!");

    printf("LINHA %d: ", contLinha);

    while (1)
    { // laço infinito para processar a expressão até o fim de arquivo
        tk = AnaLex(fd);

        switch (tk.cat)
        {
        case ID:
            printf("<ID, %s> ", tk.lexema);
            break;

        case SN:
            switch (tk.codigo)
            {
            case ADICAO:
                printf("<SN, ADICAO> ");
                break;
            case SUBTRACAO:
                printf("<SN, SUBTRACAO> ");
                break;
            case MULTIPLIC:
                printf("<SN, MULTIPLICACAO> ");
                break;
            case DIVISAO:
                printf("<SN, DIVISAO> ");
                break;
            case ATRIB:
                printf("<SN, ATRIBUICAO> ");
                break;

            case ABRE_PAR:
                printf("<SN, ABRE_PARENTESES> ");
                break;
            case FECHA_PAR:
                printf("<SN, FECHA_PARENTESES> ");
                break;
            case COMPARACAO:
                printf("<SN, COMPARACAO> ");
                break;
            case OPERADOR_OR:
                printf("<SN, OPERADOR_OR> ");
                break;
            case OPERADOR_AND:
                printf("<SN, OPERADOR_AND> ");
                break;
            case PONTEIRO:
                printf("<SN, PONTEIRO> ");
                break;
            case OPERADOR_NEGACAO:
                printf("<SN, OPERADOR_NEGACAO> ");
                break;
            case OPERADOR_DIFERENTE:
                printf("<SN, OPERADOR_DIFERENTE> ");
                break;
            case MENOR_QUE:
                printf("<SN, MENOR_QUE> ");
                break;
            case MENOR_OU_IGUAL:
                printf("<SN, MENOR_OU_IGUAL> ");
                break;
            case MAIOR_QUE:
                printf("<SN, MAIOR_QUE> ");
                break;
            case MAIOR_OU_IGUAL:
                printf("<SN, MAIOR_OU_IGUAL> ");
                break;
            }
            break;

        case CT_I:
            printf("<CT_I, %d> ", tk.valInt);
            break;

        case CT_REAL:
            printf("<CT_REAL, %f> ", tk.realVal);
            break;

        case FIM_EXPR:
            printf("<FIM_EXPR, %d>\n", 0);
            printf("LINHA %d: ", contLinha);
            break;

        case FIM_ARQ:
            printf(" <Fim do arquivo encontrado>\n");
            break;
        }

        if (tk.cat == FIM_ARQ)
            break;
    }

    fclose(fd);

    return 0;
}
