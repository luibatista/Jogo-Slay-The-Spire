
#include "baralho.h"

void inicio_baralho(Baralho *b){
    b->compra=NULL;
    b->qtd_compra=0;
    b->cap_compra=0;

    b->descarte=NULL;
    b->qtd_descarte=0;
    b->cap_descarte=0;

    b->mao=NULL;
    b->qtd_mao=0;
    b->cap_mao=0;
}


void preencher_baralho(Card baralho[20]){
    int count=0;
        baralho[count] = criar_card(ATTACK, 0);
        count++;
        baralho[count] = criar_card(ATTACK, 2);
        count++;
        baralho[count] = criar_card(ATTACK, 3);
        count++;
        baralho[count] = criar_card(DEFENSE, 0);
        count++;
        baralho[count] = criar_card(DEFENSE, 2);
        count++;
        baralho[count] = criar_card(DEFENSE, 3);
        count++;
    for(int i=0; i<3; i++){
        baralho[count] = criar_card(ATTACK, 1);
        count++;
        baralho[count] = criar_card(DEFENSE, 1);
        count++;
    }
        
    for(int i=0; i<4; i++){
        baralho[count] = criar_card(ATTACK, rand() % (3-0+1)+0);
        count++;
    }
    for(int i=0; i<2; i++){
        baralho[count] = criar_card(DEFENSE, rand() % (3-0+1)+0);
        count++;
    }
    for(int i=0; i<2; i++){
        baralho[count] = criar_card(SPECIAL, 0);
        count++;
    }
}

void ShuffleBaralho(Card array[20], int size) {
    int i;
    for (i = 0; i < size - 1; i++) {
      int j = i + rand() / (RAND_MAX / (size - i) + 1);
      if (j == i) {
        continue;
      }
      Card int_j = array[j]; 

      array[j] = array[i];

      array[i] = int_j;
    }
}

void cria_pilha_compras(Baralho *b, Card baralho[20]){
    b->cap_compra=20;
    b->qtd_compra=20;
    b->compra = (Card*) malloc(b->cap_compra * sizeof(Card));
    for(int i=0; i<20; i++){
        b->compra[i]=baralho[i];
    }
}

