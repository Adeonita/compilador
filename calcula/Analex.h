#ifndef ANALEX
#define ANALEX 
#define TAM_MAX_LEXEMA 31 

enum TOKEN_CAT { ID=1, SN, CT_I, FIM_EXPR, FIM_ARQ }; 

/* Onde: ID: Identificador, SN: Sinal; CT_I: Constante numérica inteira */ 
 
enum SINAIS {ATRIB = 1, ADICAO, SUBTRACAO, MULTIPLIC, DIVISAO, ABRE_PAR, FECHA_PAR, COMPARACAO}; // Sinais válidos da linguagem

typedef
    struct {  

        enum TOKEN_CAT cat;

        union {
            int codigo; 
            char lexema[TAM_MAX_LEXEMA];
            int valInt;
        }; 
    } TOKEN;

#endif
/* Contador de linhas do código fonte */ 

int contLinha = 1; 