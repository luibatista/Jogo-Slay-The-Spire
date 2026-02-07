#ifndef _CARTA_H_
#define _CARTA_H_

typedef enum {
    ATTACK,
    DEFENSE,
    SPECIAL
}CardType;

typedef struct {
    CardType tipo;
    int custo;
    int efeito;
}Card;

Card criar_card(CardType tipo, int custo);

#endif
