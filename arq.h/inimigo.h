#ifndef _INIMIGO_H_
#define _INIMIGO_H_

#include "baralho.h"

typedef enum {
    INTENCAO_ATACAR,
    INTENCAO_DEFENDER
} Intencao;

typedef struct{
    int vida;
    int vida_max;    
    int escudo;
    Intencao intencao_atual; 
    int valor_acao;          
} Inimigo;

#endif