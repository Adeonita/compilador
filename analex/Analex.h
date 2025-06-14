#ifndef ANALEX
#define ANALEX 
#define TAM_MAX_LEXEMA 31 

enum TOKEN_CAT { ID=1, SN, CT_I, CT_REAL, CARACTERE, STRINGCON, FIM_EXPR, FIM_ARQ, QUEBRA_DE_LINHA }; 

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
        }; 
    } TOKEN;

#endif
/* Contador de linhas do código fonte */ 

int contLinha = 1; 