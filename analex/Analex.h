#ifndef ANALEX
#define ANALEX 
#define TAM_MAX_LEXEMA 310 

#define PR_IF 1
#define PR_ELSE 2
#define PR_WHILE 3
#define PR_FOR 4
#define PR_RETURN 5
#define PR_INT 6
#define PR_FLOAT 7
#define PR_CHAR 8
#define PR_VOID 9


enum TOKEN_CAT { ID=1, PR, SN, CT_I, CT_REAL, CARACTERE, STRINGCON, FIM_EXPR, FIM_ARQ, QUEBRA_DE_LINHA }; 

/* Onde: ID: Identificador, SN: Sinal; CT_I: Constante numérica inteira */ 
 
enum SINAIS {   
                ATRIB = 1, ADICAO, SUBTRACAO, MULTIPLIC, DIVISAO, ABRE_PAR, FECHA_PAR, 
                ABRE_COLCHETE, FECHA_COLCHETE, ABRE_CHAVES, FECHA_CHAVES, VIRGULA, PONTO_E_VIRGULA,
                COMPARACAO, PONTEIRO, OPERADOR_AND, OPERADOR_OR, OPERADOR_DIFERENTE, OPERADOR_NEGACAO,
                MENOR_QUE, MENOR_OU_IGUAL, MAIOR_QUE, MAIOR_OU_IGUAL
            }; // Sinais válidos da linguagem

typedef
    struct {
        enum TOKEN_CAT cat;

        union {
            int codigo; 
            char lexema[TAM_MAX_LEXEMA];
            int valInt;
            float realVal;
            char *string;
        }; 
    } TOKEN;

#endif

extern int contLinha;
