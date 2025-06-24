#include <stdio.h>
#include <stdlib.h>
#include "Analex.h"
#include "tabela_simbolos.h"


void testaAnalex() {

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

        case PR:
            switch (tk.codigo) {
                case PR_IF: printf("<PR, IF> "); break;
                case PR_ELSE: printf("<PR, ELSE> "); break;
                case PR_WHILE: printf("<PR, WHILE> "); break;
                case PR_FOR: printf("<PR, FOR> "); break;
                case PR_RETURN: printf("<PR, RETURN> "); break;
                case PR_INT: printf("<PR, INT> "); break;
                case PR_FLOAT: printf("<PR, FLOAT> "); break;
                case PR_CHAR: printf("<PR, CHAR> "); break;
                case PR_VOID: printf("<PR, VOID> "); break;

            }
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
            printf("<STRINGCON, %s> ", tk.string);
            free(tk.string);
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
}

int main() {
    inicializaTabela();

    testaAnalex();
    imprimeTabela();
    return 0;
}
