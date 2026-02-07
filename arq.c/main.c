#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "renderer.h"
#include "utils.h"
#include "baralho.h"
#include "carta.h"
#include "jogador.h"
#include "inimigo.h"

// ADICIONADO "MENU" AQUI
typedef enum { MENU, TURN_PLAYER, TURN_ENEMY, GAME_OVER, VICTORY } GameState;

void comprar_cartas(Jogador* p, int qtd) {
    for(int i=0; i<qtd; i++) {
        if (p->baralho_jogador.qtd_compra == 0) {
            if (p->baralho_jogador.qtd_descarte == 0) break;
            for(int j=0; j < p->baralho_jogador.qtd_descarte; j++) {
                p->baralho_jogador.compra[j] = p->baralho_jogador.descarte[j];
            }
            p->baralho_jogador.qtd_compra = p->baralho_jogador.qtd_descarte;
            p->baralho_jogador.qtd_descarte = 0;
            for(int k=0; k<p->baralho_jogador.qtd_compra; k++) {
                int r = rand() % p->baralho_jogador.qtd_compra;
                Card temp = p->baralho_jogador.compra[k];
                p->baralho_jogador.compra[k] = p->baralho_jogador.compra[r];
                p->baralho_jogador.compra[r] = temp;
            }
        }
        if (p->baralho_jogador.qtd_compra > 0) {
            int topo = p->baralho_jogador.qtd_compra - 1;
            p->baralho_jogador.mao[p->baralho_jogador.qtd_mao] = p->baralho_jogador.compra[topo];
            p->baralho_jogador.qtd_mao++;
            p->baralho_jogador.qtd_compra--;
        }
    }
}

void descartar_mao(Jogador* p) {
    while(p->baralho_jogador.qtd_mao > 0) {
        int idx = p->baralho_jogador.qtd_mao - 1;
        if (p->baralho_jogador.cap_descarte == 0) {
             p->baralho_jogador.cap_descarte = 50;
             p->baralho_jogador.descarte = (Card*)malloc(50 * sizeof(Card));
        }
        p->baralho_jogador.descarte[p->baralho_jogador.qtd_descarte] = p->baralho_jogador.mao[idx];
        p->baralho_jogador.qtd_descarte++;
        p->baralho_jogador.qtd_mao--;
    }
}

void definir_intencoes(Inimigo* inimigos, int qtd) {
    for(int i=0; i<qtd; i++) {
        if (rand() % 2 == 0) {
            inimigos[i].intencao_atual = INTENCAO_ATACAR;
            if (inimigos[i].vida_max > 40)
                 inimigos[i].valor_acao = 10 + (rand() % 10); 
            else
                 inimigos[i].valor_acao = 5 + (rand() % 6);   
        } else {
            inimigos[i].intencao_atual = INTENCAO_DEFENDER;
            inimigos[i].valor_acao = 5 + (rand() % 10);
        }
    }
}

void gerar_inimigo(Inimigo* inimigo, int dificuldade_extra) {
    if (rand() % 2 == 0) {
        inimigo->vida = 10 + (rand() % 21) + dificuldade_extra;
        if (inimigo->vida > 35) inimigo->vida = 35;
    } else {
        inimigo->vida = 40 + (rand() % 61) + (dificuldade_extra * 2);
    }
    inimigo->vida_max = inimigo->vida;
    inimigo->escudo = 0;
}

int main() {
    int combate_atual = 1;
    srand(time(NULL));
    must_init(al_init(), "allegro");
    must_init(al_init_image_addon(), "allegro");
    must_init(al_init_primitives_addon(), "primitives");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));

    Renderer renderer;
    FillRenderer(&renderer);
    al_register_event_source(queue, al_get_display_event_source(renderer.display));

    // ---------------------------------------------- JOGADOR ----------------------------------------------------------
    // MUDANÇA: COMEÇA NO MENU
    GameState estado_atual = MENU;

    Jogador player;
    player.vida_max = 100;
    player.vida = 100;
    player.energia_max = 3;
    player.energia = 3;
    player.escudo = 0;
    inicio_baralho(&player.baralho_jogador);
    // ---------------------------------------------- FIM JOGADOR -----------------------------------------------------

    // ---------------------------------------------- CARTAS ----------------------------------------------------------
    Card todas_cartas[20];
    preencher_baralho(todas_cartas);
    cria_pilha_compras(&player.baralho_jogador, todas_cartas);

    player.baralho_jogador.mao = (Card*)malloc(10 * sizeof(Card));
    player.baralho_jogador.cap_mao = 10;
    comprar_cartas(&player, 5);
    // ---------------------------------------------- FIM CARTAS -----------------------------------------------------

    // ---------------------------------------------- INIMIGOS ----------------------------------------------------------
    int qtd_inimigos = 2;
    Inimigo inimigos[2];

    for(int i=0; i<qtd_inimigos; i++) {
        gerar_inimigo(&inimigos[i], 0);
    }
    definir_intencoes(inimigos, qtd_inimigos);
    // ---------------------------------------------- FIM INIMIGOS ----------------------------------------------------------

    int carta_selecionada = 0;
    int alvo_selecionado = 0;

    al_start_timer(timer);
    int done = 0;
    int redraw = 1;
    ALLEGRO_EVENT event;

    while (!done) {
        al_wait_for_event(queue, &event);

        if (inimigos[alvo_selecionado].vida <= 0) {
            for(int i = 0; i < qtd_inimigos; i++) {
                if(inimigos[i].vida > 0) {
                    alvo_selecionado = i;
                    break;
                }
            }
        }

        if (event.type == ALLEGRO_EVENT_TIMER) {
            redraw = 1;
            if (estado_atual != MENU && player.vida <= 0) estado_atual = GAME_OVER;

            int vivos = 0;
            for(int i=0; i<qtd_inimigos; i++) if(inimigos[i].vida > 0) vivos++;

            if(vivos == 0 && estado_atual != MENU) {
                if (combate_atual < 10) {
                    combate_atual++;
                    for(int i=0; i<qtd_inimigos; i++) {
                        gerar_inimigo(&inimigos[i], combate_atual);
                    }

                    player.energia = player.energia_max;
                    player.escudo = 0;
                    descartar_mao(&player);
                    comprar_cartas(&player, 5);

                    definir_intencoes(inimigos, qtd_inimigos);

                    estado_atual = TURN_PLAYER;
                    carta_selecionada = 0;
                    alvo_selecionado = 0;
                } else {
                    estado_atual = VICTORY;
                }
            }

        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            done = 1;
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            
            if (estado_atual == MENU) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    estado_atual = TURN_PLAYER;
                }
            }
            else if (estado_atual == TURN_PLAYER) {
                switch(event.keyboard.keycode) {

                    //-------------------------- MOVIMENTAÇÃO DE CARTAS E ALVOS --------------------------------
                    case ALLEGRO_KEY_LEFT:
                        carta_selecionada--;
                        if(carta_selecionada < 0) carta_selecionada = 4;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        carta_selecionada++;
                        if(carta_selecionada > 4) carta_selecionada = 0;
                        if(carta_selecionada >= player.baralho_jogador.qtd_mao)
                            carta_selecionada = player.baralho_jogador.qtd_mao - 1;
                        break;
                    case ALLEGRO_KEY_UP:
                    case ALLEGRO_KEY_DOWN:
                    {
                        int tentativa = alvo_selecionado;
                        for(int i = 0; i < qtd_inimigos; i++) {
                            tentativa++;
                            if (tentativa >= qtd_inimigos) tentativa = 0;
                            if (inimigos[tentativa].vida > 0) {
                                alvo_selecionado = tentativa;
                                break;
                            }
                        }
                    }
                    break;
                    //------------------------------- FIM MOVIMENTAÇÃO DE CARTAS E ALVOS------------------------

                    //------------------------------- MECANICA DE JOGO ----------------------------------------------
                    case ALLEGRO_KEY_ENTER:
                        if (player.baralho_jogador.qtd_mao > 0) {
                            Card c = player.baralho_jogador.mao[carta_selecionada];
                            if (player.energia >= c.custo) {
                                player.energia -= c.custo;
                                if (c.tipo == ATTACK) {
                                    if (inimigos[alvo_selecionado].vida > 0) {
                                        int dano = c.efeito;
                                        if(inimigos[alvo_selecionado].escudo > 0) {
                                            int temp = inimigos[alvo_selecionado].escudo;
                                            inimigos[alvo_selecionado].escudo -= dano;
                                            dano -= temp;
                                        }
                                        if(dano > 0) inimigos[alvo_selecionado].vida -= dano;
                                    }
                                }
                                else if (c.tipo == DEFENSE) {
                                    player.escudo += c.efeito;
                                }
                                else if (c.tipo == SPECIAL) {
                                    descartar_mao(&player);
                                    comprar_cartas(&player, 5);

                                    carta_selecionada = 0;
                                    continue;
                                }

                                if (player.baralho_jogador.cap_descarte == 0) {
                                    player.baralho_jogador.cap_descarte = 50;
                                    player.baralho_jogador.descarte = (Card*)malloc(50 * sizeof(Card));
                                }

                                player.baralho_jogador.descarte[player.baralho_jogador.qtd_descarte] = c;
                                player.baralho_jogador.qtd_descarte++;
                                for(int k=carta_selecionada; k < player.baralho_jogador.qtd_mao - 1; k++) {
                                    player.baralho_jogador.mao[k] = player.baralho_jogador.mao[k+1];
                                }
                                player.baralho_jogador.qtd_mao--;
                                if(carta_selecionada >= player.baralho_jogador.qtd_mao) carta_selecionada--;
                                if(carta_selecionada < 0) carta_selecionada = 0;
                            }
                        }
                        break;

                    case ALLEGRO_KEY_C:
                        if (player.vida<100){
                            player.vida += 5 + (rand() % 6);
                            if (player.vida>player.vida_max){
                                player.vida=player.vida_max;
                            }
                        }
                        break;

                    case ALLEGRO_KEY_E:
                        estado_atual = TURN_ENEMY;
                        break;

                    case ALLEGRO_KEY_SPACE:
                        for(int i=0; i<2;i++){
                            inimigos[i].vida = 0;
                        }
                        break;

                    default:
                        break;
                } 
            } 
        } 

        if (estado_atual == TURN_ENEMY) {

            for(int i=0; i<qtd_inimigos; i++) inimigos[i].escudo = 0;

            for(int i=0; i<qtd_inimigos; i++) {
                if(inimigos[i].vida > 0) {
                    if (inimigos[i].intencao_atual == INTENCAO_ATACAR) {
                        int dano = inimigos[i].valor_acao;
                        if(player.escudo > 0) {
                            int temp = player.escudo;
                            player.escudo -= dano;
                            dano -= temp;
                        }
                        if(dano > 0) player.vida -= dano;
                    } else {
                        inimigos[i].escudo += inimigos[i].valor_acao;
                    }
                }
            }

            player.energia = player.energia_max;
            player.escudo = 0;

            descartar_mao(&player);
            comprar_cartas(&player, 5);

            definir_intencoes(inimigos, qtd_inimigos);

            estado_atual = TURN_PLAYER;
        }

        if (redraw && al_is_event_queue_empty(queue)) {
            if (estado_atual == MENU) {
                RenderMenu(&renderer);
            }
            // -----------------------
            else if (estado_atual == GAME_OVER) {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                DrawScaledText(renderer.font, al_map_rgb(255, 0, 0), (DISPLAY_WIDTH/2)/4.0, (DISPLAY_HEIGHT/2)/4.0, 4, 4, ALLEGRO_ALIGN_CENTER, "GAME OVER");
                al_flip_display();
            }
            else if (estado_atual == VICTORY) {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                DrawScaledText(renderer.font, al_map_rgb(0, 255, 0), (DISPLAY_WIDTH/2)/4.0, (DISPLAY_HEIGHT/2)/4.0, 4, 4, ALLEGRO_ALIGN_CENTER, "VITORIA!");
                al_flip_display();
            }
            else {
                Render(&renderer, &player, inimigos, qtd_inimigos, carta_selecionada, alvo_selecionado);
            }
            redraw = 0;
        }
    } 

    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    ClearRenderer(&renderer);
    return 0;
}