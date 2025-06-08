#include "reconhecedores.h"

TOKEN reconhecedorBase(char categoria, char codigo){
    TOKEN t;
    t.cat = categoria;
    t.codigo = codigo;

    return t;
}

TOKEN reconheceOr(int *estado) {
    *estado = 44;
    return reconhecedorBase(SN, OPERADOR_OR );
}

TOKEN reconheceAnd(int *estado) {
    *estado = 47;
    return reconhecedorBase(SN, OPERADOR_AND);
}

TOKEN reconheceComparacao(int *estado) {
    *estado = 29;
    return reconhecedorBase(SN, COMPARACAO);
}

TOKEN reconheceAtribuicao(int *estado) {
    *estado = 28;
    return reconhecedorBase(SN, ATRIB);
}

// TOKEN reconheceConstInt(int *estado, FILE *fd, const char *digitos, char c) {
//     TOKEN t;
//     *estado = 4;
//     ungetc(c, fd);
//     t.cat = CT_I;
//     t.valInt = atoi(digitos);
//     return t;
// }

// TOKEN reconheceConstReal(int *estado, FILE *fd, const char *digitos, char c) {
//     TOKEN t;
//     *estado = 48;
//     ungetc(c, fd);
//     t.cat = CT_REAL;
//     t.realVal = atof(digitos);
//     return t;
// }