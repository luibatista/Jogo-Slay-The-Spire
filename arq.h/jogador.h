#ifndef _JOGADOR_H_
#define _JOGADOR_H_

#include "baralho.h"
#include "carta.h"

typedef struct{
    Baralho baralho_jogador; 
    int vida;
    int vida_max;
    int escudo; 
    int energia;
    int energia_max;
} Jogador;

#endif