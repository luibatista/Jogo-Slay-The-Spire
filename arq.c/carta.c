

#include "carta.h"
#include "baralho.h"
#include <stdlib.h>

Card criar_card(CardType tipo, int custo){
    Card c;
    c.tipo=tipo;
    c.custo=custo;
    if (tipo == SPECIAL) {
        c.custo = 0;
        c.efeito = 0;
        return c;
    }
    switch(custo){
        case 0:
            c.efeito=rand() % (5-1+1)+1;
            break;
        case 1:
            c.efeito=rand() % (10-5+1)+5;
            break;
        case 2:
            c.efeito=rand() % (15-10+1)+10;
            break;
        case 3:
            c.efeito=rand() % (30-15+1)+15;
            break;      
    }
    return c;
}