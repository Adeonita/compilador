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

TOKEN reconheceAtribuicao(int *estado, FILE *fd, const char c)
{
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
    ungetc(c, fd);

    return reconhecedorBase(SN, DIVISAO);
}

TOKEN reconheceMaiorOuIgual(int *estado)
{
    *estado = 22;

    return reconhecedorBase(SN, MAIOR_OU_IGUAL);
}

TOKEN reconheceAdicao(int *estado, char *lexema, int *tamL, char c)
{
    // *estado = 39;

    mudaEstadoEIncrementaLexema(estado, 39, lexema, tamL, c);

    return reconhecedorBase(SN, ADICAO);
}

TOKEN reconheceSubtracao(int *estado, char *lexema, int *tamL, char c)
{
    mudaEstadoEIncrementaLexema(estado, 38, lexema, tamL, c);

    return reconhecedorBase(SN, SUBTRACAO);
}

TOKEN reconheceMultiplicacao(int *estado, char *lexema, int *tamL, char c)
{
    mudaEstadoEIncrementaLexema(estado, 37, lexema, tamL, c);

    return reconhecedorBase(SN, MULTIPLIC);
}

TOKEN reconheceAbreParenteses(int *estado, char *lexema, int *tamL, char c)
{
    mudaEstadoEIncrementaLexema(estado, 36, lexema, tamL, c);

    return reconhecedorBase(SN, ABRE_PAR);
}

TOKEN reconheceFechaParenteses(int *estado, char *lexema, int *tamL, char c)
{
    mudaEstadoEIncrementaLexema(estado, 35, lexema, tamL, c);

    return reconhecedorBase(SN, FECHA_PAR);
}

TOKEN reconheceAbreColchetes(int *estado, char *lexema, int *tamL, char c)
{
    mudaEstadoEIncrementaLexema(estado, 33, lexema, tamL, c);

    return reconhecedorBase(SN, ABRE_COLCHETE);
}

TOKEN reconheceFechaColchetes(int *estado, char *lexema, int *tamL, char c)
{
    mudaEstadoEIncrementaLexema(estado, 34, lexema, tamL, c);

    return reconhecedorBase(SN, FECHA_COLCHETE);
}

TOKEN reconheceAbreChaves(int *estado, char *lexema, int *tamL, char c)
{
    mudaEstadoEIncrementaLexema(estado, 49, lexema, tamL, c);

    return reconhecedorBase(SN, ABRE_CHAVES);
}

TOKEN reconheceVirgula(int *estado, char *lexema, int *tamL, char c)
{
    mudaEstadoEIncrementaLexema(estado, 32, lexema, tamL, c);

    return reconhecedorBase(SN, VIRGULA);
}

TOKEN reconhecePontoEVirgula(int *estado, char *lexema, int *tamL, char c)
{
    mudaEstadoEIncrementaLexema(estado, 31, lexema, tamL, c);

    return reconhecedorBase(SN, PONTO_E_VIRGULA);
}

TOKEN reconheceFechaChaves(int *estado, char *lexema, int *tamL, char c)
{
    mudaEstadoEIncrementaLexema(estado, 50, lexema, tamL, c);

    return reconhecedorBase(SN, FECHA_CHAVES);
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
    ungetc(c, fd); // precisa do atof e do ungetc ?
    t.cat = CT_REAL;
    t.realVal = atof(digitos);
    return t;
}

TOKEN reconheceCaractere(int *estado, FILE *fd, char *lexema, int *tamL, char c)
{
    TOKEN t;
    ungetc(c, fd); // devolve o próximo caractere da leitura
    t.cat = CARACTERE;   // categoria constante caractere
    strcpy(t.lexema, lexema);
    return t;
}

TOKEN reconheceString(int *estado, FILE *fd, char *lexema, int *tamL, char c)
{
    TOKEN t;
    ungetc(c, fd);
    t.cat = STRINGCON;
    strcpy(t.lexema, lexema);
    return t;
}

TOKEN reconheceQuebraDeLinha(int *estado, FILE *fd, char *lexema, int *tamL, char c)
{
    TOKEN t;
    ungetc(c, fd);
    t.cat = QUEBRA_DE_LINHA;
    strcpy(t.lexema, lexema);
    return t;
}

TOKEN reonheceFimExpr(int *estado, FILE *fd, char *lexema, int *tamL, char c)
{
    TOKEN t;
    ungetc(c, fd);
    t.cat = FIM_EXPR;
    strcpy(t.lexema, lexema);
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
        bool LEU_CONTRA_BARRA = '\\';
        bool LEU_ABRE_PAR = c == '(';
        bool LEU_FECHA_PAR = c == ')';
        bool LEU_ABRE_COLCHETE = c == '[';
        bool LEU_FECHA_COLCHETE = c == ']';
        bool LEU_ABRE_CHAVE = c == '{';
        bool LEU_FECHA_CHAVE = c == '}';
        bool LEU_VIRGULA = c == ',';
        bool LEU_PONTO_E_VIRGULA = c == ';';
        bool LEU_ASPAS_SIMPLES = c == '\'';
        bool LEU_ASPAS_DUPLAS = c == '"';
        bool LEU_CARACTERE = c != '\'' && c != '\\';
        bool LEU_QUEBRA_DE_LINHA = c =='n';
        bool LEU_BARRA_ZERO = c == '0';

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
            else if (LEU_MENOR_QUE)
            {
                mudaEstadoEIncrementaLexema(&estado, 24, lexema, &tamL, c);
            }
            else if (LEU_MAIOR_QUE)
            {
                mudaEstadoEIncrementaLexema(&estado, 21, lexema, &tamL, c);
            }
            else if (LEU_BARRA)
            {
                mudaEstadoEIncrementaLexema(&estado, 17, lexema, &tamL, c);
            }
            else if (LEU_ADICAO)
            {
                return reconheceAdicao(&estado, lexema, &tamL, c);
            }
            else if (LEU_SUBTRACAO)
            {
                return reconheceSubtracao(&estado, lexema, &tamL, c);
            }
            else if (LEU_ASTERISCO)
            {
                return reconheceMultiplicacao(&estado, lexema, &tamL, c);
            }
            else if (LEU_ABRE_PAR)
            {
                return reconheceAbreParenteses(&estado, lexema, &tamL, c);
            }
            else if (LEU_FECHA_PAR)
            {
                return reconheceFechaParenteses(&estado, lexema, &tamL, c);
            }
            else if (LEU_ABRE_COLCHETE)
            {
                return reconheceAbreColchetes(&estado, lexema, &tamL, c);
            }
            else if (LEU_FECHA_COLCHETE)
            {
                return reconheceFechaColchetes(&estado, lexema, &tamL, c);
            }
            else if (LEU_ABRE_CHAVE)
            {
                return reconheceAbreChaves(&estado, lexema, &tamL, c);
            }
            else if (LEU_FECHA_CHAVE)
            {
                return reconheceFechaChaves(&estado, lexema, &tamL, c);
            }
            else if (LEU_VIRGULA)
            {
                return reconheceVirgula(&estado, lexema, &tamL, c);
            }
            else if (LEU_PONTO_E_VIRGULA)
            {
                return reconhecePontoEVirgula(&estado, lexema, &tamL, c);
            }
            else if (LEU_ASPAS_SIMPLES) {
                mudaEstadoEIncrementaLexema(&estado, 7, lexema, &tamL, c);
            }
            else if (LEU_ASPAS_DUPLAS) {
                mudaEstadoEIncrementaLexema(&estado, 15, lexema, &tamL, c);
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
        case 7:
            if (LEU_CARACTERE)
            {
                mudaEstadoEIncrementaLexema(&estado, 8, lexema, &tamL, c);
            }
            else if (LEU_CONTRA_BARRA)
            {
                mudaEstadoEIncrementaLexema(&estado, 10, lexema, &tamL, c);
            }
            else {
                error("Caractere invalido no ESTADO 7!");
            }
            break;
        case 8 :
            if (LEU_ASPAS_SIMPLES){
                mudaEstadoEIncrementaLexema(&estado, 9, lexema, &tamL, c);
            } 
            else
            {
                error("Caractere inválido no Estado 8");
            }
            break;
        case 9: 
            return reconheceCaractere(&estado, fd, lexema, &tamL, c);
            break;
        case 10:
            if (LEU_QUEBRA_DE_LINHA){
                mudaEstadoEIncrementaLexema(&estado, 11, lexema, &tamL, c);
            }
            else if (LEU_BARRA_ZERO) {
                mudaEstadoEIncrementaLexema(&estado, 13, lexema, &tamL, c);
            }
            else {
                error("Caractere inválido no Estado 10");
            }
            break;
        case 11:
            if (LEU_ASPAS_SIMPLES){
                mudaEstadoEIncrementaLexema(&estado, 12, lexema, &tamL, c);
            }
            break;
        case 12:
            return reconheceQuebraDeLinha(&estado, fd, lexema, &tamL, c);
            break;
        case 13:
            if (LEU_ASPAS_SIMPLES) {
                mudaEstadoEIncrementaLexema(&estado, 14, lexema, &tamL, c);
            }
            else {
                error("Caractere inválido no Estado 13");
            }
            break;
        case 14:
            return reonheceFimExpr(&estado, fd, lexema, &tamL, c);
        case 15:
            if (LEU_CARACTERE && !LEU_ASPAS_DUPLAS) {
                mudaEstadoEIncrementaLexema(&estado, 15, lexema, &tamL, c);
            }
            else if (LEU_ASPAS_DUPLAS){
                mudaEstadoEIncrementaLexema(&estado, 16, lexema, &tamL, c);
            }
            else {
                error("Caractere inválido no Estado 15");
            }
            break;
        case 16:
            return reconheceString(&estado, fd, lexema, &tamL, c);
            break;
        case 17:
            if (LEU_ASTERISCO)
            {
                mudaEstadoEIncrementaLexema(&estado, 20, lexema, &tamL, c);
            }
            else {
                mudaEstadoEIncrementaLexema(&estado, 18, lexema, &tamL, c);

                return reconheceDivisao(&estado, fd, c);
            }
            break;
        case 19:
            if (LEU_BARRA) {
                estado = 0;
            }
            else if (LEU_ASTERISCO) {
                mudaEstadoEIncrementaLexema(&estado, 19, lexema, &tamL, c);
            }
            else if (leuFimDeLinha){
                error("Comentário de bloco não fechado.");
            } 
            else {
                mudaEstadoEIncrementaLexema(&estado, 20, lexema, &tamL, c);
            }
            break;
        case 20:
            if (LEU_ASTERISCO) {
                mudaEstadoEIncrementaLexema(&estado, 19, lexema, &tamL, c);
            }
            else if (leuFimDeLinha){
                error("Comentário de bloco não fechado.");
            }
            else {
                mudaEstadoEIncrementaLexema(&estado, 20, lexema, &tamL, c);
            }
            break;

        case 21:
            if (leuSinalDeIgual)
            {
                return reconheceMaiorOuIgual(&estado);
            }
            else
            {
                return reconheceMaiorQue(&estado, fd, c);
            }
            break;

        case 24:
            if (leuSinalDeIgual)
            {
                return reconheceMenorOuIgual(&estado);
            }
            else
            {
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
            case ABRE_COLCHETE:
                printf("<SN, ABRE_COLCHETE> ");
                break;
            case FECHA_COLCHETE:
                printf("<SN, FECHA_COLCHETE> ");
                break;
            case ABRE_CHAVES:
                printf("<SN, ABRE_CHAVES> ");
                break;
            case FECHA_CHAVES:
                printf("<SN, FECHA_CHAVES> ");
                break;
            case VIRGULA:
                printf("<SN, VIRGULA> ");
                break;
            case PONTO_E_VIRGULA:
                printf("<SN, PONTO_E_VIRGULA> ");
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
        
        case CARACTERE:
            printf("<CARACTERE, %s> ", tk.lexema);
            break;
        
        case STRINGCON:
            printf("<STRINGCON, %s> ", tk.lexema);
            break;
        case QUEBRA_DE_LINHA:
            printf("<QUEBRA_DE_LINHA, %s> ", tk.lexema);
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
