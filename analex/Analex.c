#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "Analex.h"

#define TAM_LEXEMA 50
#define TAM_NUM 20

void error(char msg[])
{
    printf("%s\n", msg);
    exit(1);
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
        
        switch (estado)
        {
            case 0:
                if (c == ' ' || c == '\t'){
                    estado = 0;
                }
                else if (leuLetra) { //inicializa lexema
                    estado = 1;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                } else if (leuDigito) { // inicio de constante inteira - inicializa digitos
                    estado = 3;
                    digitos[tamD] = c;
                    digitos[++tamD] = '\0';
                } else if (leuSinalDeIgual) {
                    estado = 27;
                } else if (leuQuebraDeLinha) {
                    estado = 0;
                    t.cat = FIM_EXPR; // fim de linha (ou expressao) encontrado
                    contLinha++;
                    return t;
                } else if (leuFimDeLinha){
                    t.cat = FIM_ARQ;
                    return t;
                } else
                    error("Caracter invalido na expressao!");

                break;
            case 1:
                if (leuLetraOuDigitoOuUnderscore) {
                    estado = 1;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                } else {
                    // transicao OUTRO* do estado 1 do AFD

                    estado = 2;
                    ungetc(c, fd);
                    t.cat = ID;
                    strcpy(t.lexema, lexema);
                    return t;
                }

                break;

            case 3:
                if (leuDigito) {
                    estado = 3;
                    digitos[tamD] = c;
                    digitos[++tamD] = '\0';
                } else if (leuPonto) {
                    estado = 5;
                    digitos[tamD] = c;
                    digitos[++tamD] = '\0';
                } else {
                    estado = 4;
                    ungetc(c, fd);
                    t.cat = CT_I;
                    t.valInt = atoi(digitos);
                    return t;
                }
                break;
            case 5:
            if (leuDigito) {
                estado = 6;
                digitos[tamD] = c;
                digitos[++tamD] = '\0';
            } else {
                printf("Caractere invalido no ESTADO 3!");
                exit(1);
            }
                break;
            case 6:
                if (leuDigito) {
                    estado = 6;
                    digitos[tamD] = c;
                    digitos[++tamD] = '\0';
                } else {
                    estado = 48;
                    ungetc(c, fd);
                    t.cat = CT_REAL;
                    t.realVal = atof(digitos);
                    //strcpy(t.lexema, lexema);
                    return t;
                }
                break;

            case 27: 
                if (leuSinalDeIgual) {
                    estado = 29;
                    t.cat = SN;
                    t.codigo = COMPARACAO;

                    return t;
                } else {
                    estado = 28;
                    t.cat  = SN;
                    t.codigo = ATRIB;

                    return t;
                }
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
        }

        if (tk.cat == FIM_ARQ)
            break;
    }

    fclose(fd);

    return 0;
}
