#ifndef RECONHECEDORES_H
#define RECONHECEDORES_H

#include "Analex.h"

TOKEN reconheceOr(int *estado);
TOKEN reconheceAnd(int *estado);
TOKEN reconheceComparacao(int *estado);
TOKEN reconheceAtribuicao(int *estado);
// TOKEN reconheceConstInt(int *estado, FILE *fd, const char *digitos, char c);
// TOKEN reconheceConstReal(int *estado, FILE *fd, const char *digitos, char c);

#endif
