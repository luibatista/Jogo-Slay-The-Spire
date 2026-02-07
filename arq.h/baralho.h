#ifndef _BARALHO_H_
#define _BARALHO_H_

#include <stdlib.h>
#include "carta.h"

typedef struct {
    Card *compra;     
    int qtd_compra;
    int cap_compra;

    Card *descarte;
    int qtd_descarte;
    int cap_descarte;

    Card *mao;
    int qtd_mao;
    int cap_mao;
} Baralho;

void inicio_baralho(Baralho *b);

void preencher_baralho(Card baralho[20]);

void ShuffleBaralho(Card array[20], int size);

void cria_pilha_compras(Baralho *b, Card baralho[20]);

#endif