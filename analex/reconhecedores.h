#ifndef RECONHECEDORES_H
#define RECONHECEDORES_H

#include "Analex.h"

TOKEN reconheceOr(int *estado);
TOKEN reconheceAnd(int *estado);
TOKEN reconheceComparacao(int *estado);
TOKEN reconheceAtribuicao(int *estado);

#endif
