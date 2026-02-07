#include "renderer.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include "constants.h"

#define COLOR_BG al_map_rgb(30, 25, 40)
#define COLOR_GOLD al_map_rgb(255, 215, 0)
#define COLOR_KEY_FACE al_map_rgb(220, 220, 230)
#define COLOR_KEY_SHADOW al_map_rgb(100, 100, 110)
#define COLOR_KEY_TEXT al_map_rgb(40, 40, 50)
#define COLOR_DESC_TEXT al_map_rgb(200, 200, 255)

void DrawScaledText(ALLEGRO_FONT* font, ALLEGRO_COLOR color, float x, float y,
                    float xscale, float yscale, int alignment, const char* text) {
  ALLEGRO_TRANSFORM transform;
  al_identity_transform(&transform);
  al_scale_transform(&transform, xscale, yscale);
  al_use_transform(&transform);
  al_draw_text(font, color, x, y, alignment, text);
  al_identity_transform(&transform);
  al_use_transform(&transform);
}

void FillRenderer(Renderer* renderer) {
  al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
  
  renderer->display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  renderer->display_buffer = al_create_bitmap(DISPLAY_BUFFER_WIDTH, DISPLAY_BUFFER_HEIGHT);
  renderer->font = al_create_builtin_font();
}

void RenderBackground(Renderer* renderer) {
  al_clear_to_color(al_map_rgb(20, 20, 30)); 
}

void RenderHealthBar(float x_begin, float x_end, float y_pos, int vida_atual, int vida_max, int escudo, ALLEGRO_FONT* font) {

  al_draw_filled_rectangle(x_begin, y_pos, x_end, y_pos + 20, al_map_rgb(50, 0, 0));
  
  float porcentagem = (float)vida_atual / (float)vida_max;
  if(porcentagem < 0) porcentagem = 0;
  float largura_vida = (x_end - x_begin) * porcentagem;
  
  al_draw_filled_rectangle(x_begin, y_pos, x_begin + largura_vida, y_pos + 20, al_map_rgb(200, 0, 0));

  char text[50];
  if (escudo > 0)
      sprintf(text, "%d/%d (+%d)", vida_atual, vida_max, escudo);
  else
      sprintf(text, "%d/%d", vida_atual, vida_max);

  DrawScaledText(font, al_map_rgb(255, 255, 255), (x_begin + x_end)/2.0/2.0, (y_pos + 25)/2.0, 2.0, 2.0, ALLEGRO_ALIGN_CENTRE, text);
}

void RenderPlayer(Renderer* renderer, Jogador* jogador) {
    al_draw_filled_circle(PLAYER_BEGIN_X, PLAYER_BEGIN_Y, 50, al_map_rgb(0, 0, 200));
    
    RenderHealthBar(PLAYER_BEGIN_X - 50, PLAYER_BEGIN_X + 50, PLAYER_BEGIN_Y + 60, 
                    jogador->vida, jogador->vida_max, jogador->escudo, renderer->font);
    
    char info[50];
    
    sprintf(info, "Energia: %d/%d", jogador->energia, jogador->energia_max);
    DrawScaledText(renderer->font, al_map_rgb(255, 255, 0), PLAYER_BEGIN_X/3.0, 50, 3.0, 3.0, ALLEGRO_ALIGN_CENTRE, info);

    float deck_x = 150.0; 

    sprintf(info, "Compra: %d", jogador->baralho_jogador.qtd_compra);
    DrawScaledText(renderer->font, al_map_rgb(255, 255, 255), deck_x/2.0, 880/2.0, 2.0, 2.0, ALLEGRO_ALIGN_LEFT, info);
    
    sprintf(info, "Descarte: %d", jogador->baralho_jogador.qtd_descarte);
    DrawScaledText(renderer->font, al_map_rgb(150, 150, 150), deck_x/2.0, 940/2.0, 2.0, 2.0, ALLEGRO_ALIGN_LEFT, info);
}

void RenderEnemies(Renderer* renderer, Inimigo* inimigos, int qtd, int alvo_selecionado) {
    int enemy_width = 100;
    int gap = 300;
    
    int fim_area_inimigos = DISPLAY_BUFFER_WIDTH - PLAYER_BEGIN_X;
    int largura_total = ((qtd - 1) * gap) + enemy_width;
    int start_x = fim_area_inimigos - largura_total;

    for(int i = 0; i < qtd; i++) {
        if(inimigos[i].vida > 0) {
            int x = start_x + (i * gap);
            int y = PLAYER_BEGIN_Y;
            
            if(i == alvo_selecionado) {
                al_draw_circle(x + 50, y, 55, al_map_rgb(255, 255, 0), 5);
            }

            ALLEGRO_COLOR cor_inimigo;
            char tipo_txt[10];
            
            if (inimigos[i].vida_max > 40) {
                cor_inimigo = al_map_rgb(100, 0, 0); 
                sprintf(tipo_txt, "FORTE");
            } else {
                cor_inimigo = al_map_rgb(200, 50, 50); 
                sprintf(tipo_txt, "FRACO");
            }
            
            al_draw_filled_circle(x + 50, y, 50, cor_inimigo);
            
            DrawScaledText(renderer->font, al_map_rgb(255, 255, 255), (x + 50)/1.5, y/1.5 - 5, 1.5, 1.5, ALLEGRO_ALIGN_CENTRE, tipo_txt);

            RenderHealthBar(x, x + 100, y + 60, inimigos[i].vida, inimigos[i].vida_max, inimigos[i].escudo, renderer->font);

            char intencao_txt[20];
            ALLEGRO_COLOR cor_intencao;
            
            if (inimigos[i].intencao_atual == INTENCAO_ATACAR) {
                sprintf(intencao_txt, "ATK %d", inimigos[i].valor_acao);
                cor_intencao = al_map_rgb(255, 100, 100);
            } else {
                sprintf(intencao_txt, "DEF %d", inimigos[i].valor_acao);
                cor_intencao = al_map_rgb(100, 100, 255);
            }
            
            DrawScaledText(renderer->font, cor_intencao, (x + 50)/2.0, (y - 80)/2.0, 2.0, 2.0, ALLEGRO_ALIGN_CENTRE, intencao_txt);
        }
    }
}

void RenderHand(Renderer* renderer, Jogador* player, int selecionada) {
    int start_x = HAND_BEGIN_X;
    int gap = 220; 

    for(int i = 0; i < player->baralho_jogador.qtd_mao; i++) {
        Card c = player->baralho_jogador.mao[i];
        
        int x = start_x + (i * gap);
        int y = HAND_BEGIN_Y;
        
        if(i == selecionada) y -= 50;

        ALLEGRO_COLOR cor_carta = al_map_rgb(200, 200, 200);
        if(c.tipo == ATTACK) cor_carta = al_map_rgb(255, 150, 150);
        if(c.tipo == DEFENSE) cor_carta = al_map_rgb(150, 150, 255);
        if(c.tipo == SPECIAL) cor_carta = al_map_rgb(200, 100, 255);

        al_draw_filled_rounded_rectangle(x, y, x + 200, y + 300, 10, 10, cor_carta);
        al_draw_rounded_rectangle(x, y, x + 200, y + 300, 10, 10, al_map_rgb(0,0,0), 3);

        char buffer[30];
        sprintf(buffer, "E: %d", c.custo);
        DrawScaledText(renderer->font, al_map_rgb(0,0,0), x/2 + 10, y/2 + 10, 2.0, 2.0, ALLEGRO_ALIGN_LEFT, buffer);
        
        char* tipoStr = (c.tipo == ATTACK) ? "ATK" : (c.tipo == DEFENSE ? "DEF" : "ESP");
        sprintf(buffer, "%s %d", tipoStr, c.efeito);
        DrawScaledText(renderer->font, al_map_rgb(0,0,0), x/2 + 50, y/2 + 70, 2.0, 2.0, ALLEGRO_ALIGN_CENTER, buffer);
    }
}

void Render(Renderer* renderer, Jogador* jogador, Inimigo* inimigos, int qtd_inimigos, int carta_selecionada, int alvo_selecionado) {
  al_set_target_bitmap(renderer->display_buffer);
  
  RenderBackground(renderer);
  RenderPlayer(renderer, jogador);
  RenderEnemies(renderer, inimigos, qtd_inimigos, alvo_selecionado);
  RenderHand(renderer, jogador, carta_selecionada);

  al_set_target_backbuffer(renderer->display);
  al_draw_scaled_bitmap(renderer->display_buffer, 0, 0, DISPLAY_BUFFER_WIDTH,
                        DISPLAY_BUFFER_HEIGHT, 0, 0, DISPLAY_WIDTH,
                        DISPLAY_HEIGHT, 0);
  al_flip_display();
}

void DrawKeyVisual(Renderer* r, float x, float y, int w, const char* keyText, const char* descText) {
    int h = 60;
    int shadow_offset = 5;
    int text_scale = 2.0;

    al_draw_filled_rounded_rectangle(x, y + shadow_offset, x + w, y + h + shadow_offset, 8, 8, COLOR_KEY_SHADOW);

    al_draw_filled_rounded_rectangle(x, y, x + w, y + h, 8, 8, COLOR_KEY_FACE);
    
    al_draw_rounded_rectangle(x, y, x + w, y + h, 8, 8, al_map_rgb(0,0,0), 3);

    DrawScaledText(r->font, COLOR_KEY_TEXT, (x + w/2)/text_scale, (y + h/4)/text_scale, text_scale, text_scale, ALLEGRO_ALIGN_CENTRE, keyText);

    DrawScaledText(r->font, COLOR_DESC_TEXT, (x + w + 30)/text_scale, (y + h/4)/text_scale, text_scale, text_scale, ALLEGRO_ALIGN_LEFT, descText);
}

void RenderMenu(Renderer* renderer) {
    al_set_target_bitmap(renderer->display_buffer);
    al_clear_to_color(COLOR_BG);

    DrawScaledText(renderer->font, COLOR_GOLD, 
                   DISPLAY_BUFFER_WIDTH/2.0/5.0, 15, 
                   5.0, 5.0, ALLEGRO_ALIGN_CENTRE, "SLAY THE SPIRE (CLONE)");

    float start_y_nav = 320; 
    float col1_x = 150;
    int key_size_std = 70;

    DrawScaledText(renderer->font, COLOR_GOLD, col1_x/2.5, (start_y_nav - 60)/2.5, 2.5, 2.5, ALLEGRO_ALIGN_LEFT, "NAVEGACAO");

    DrawKeyVisual(renderer, col1_x, start_y_nav, key_size_std, "<", "Carta Anterior");
    DrawKeyVisual(renderer, col1_x, start_y_nav + 80, key_size_std, ">", "Proxima Carta");
    DrawKeyVisual(renderer, col1_x, start_y_nav + 160, key_size_std, "^", "Trocar Alvo (Inimigo)");
    
    float col2_x = DISPLAY_BUFFER_WIDTH/2.0 + 50;
    float start_y_act = 320;

    DrawScaledText(renderer->font, COLOR_GOLD, col2_x/2.5, (start_y_act - 60)/2.5, 2.5, 2.5, ALLEGRO_ALIGN_LEFT, "ACOES");

    DrawKeyVisual(renderer, col2_x, start_y_act, 160, "ENTER", "Jogar Carta Selecionada");
    DrawKeyVisual(renderer, col2_x, start_y_act + 80, key_size_std, "E", "Encerrar Turno");
    
    DrawScaledText(renderer->font, al_map_rgb(150,150,150), col2_x/2.0, (start_y_act + 180)/2.0, 2.0, 2.0, ALLEGRO_ALIGN_LEFT, "Cheats (Debug):");
    DrawKeyVisual(renderer, col2_x, start_y_act + 220, key_size_std, "C", "Curar Vida");
    DrawKeyVisual(renderer, col2_x, start_y_act + 300, 160, "SPACE", "Matar Todos Inimigos");

    float footer_y = DISPLAY_BUFFER_HEIGHT - 80;
    float text_scale_footer = 1.5;

    DrawScaledText(renderer->font, al_map_rgb(255, 255, 255), 
                   DISPLAY_BUFFER_WIDTH/2.0/text_scale_footer, footer_y/text_scale_footer, 
                   text_scale_footer, text_scale_footer, ALLEGRO_ALIGN_CENTRE, "PRESSIONE [ ENTER ] PARA INICIAR");

    al_set_target_backbuffer(renderer->display);
    al_draw_scaled_bitmap(renderer->display_buffer, 0, 0, DISPLAY_BUFFER_WIDTH,
                        DISPLAY_BUFFER_HEIGHT, 0, 0, DISPLAY_WIDTH,
                        DISPLAY_HEIGHT, 0);
    al_flip_display();
}

void ClearRenderer(Renderer* renderer) {
  al_destroy_display(renderer->display);
  al_destroy_bitmap(renderer->display_buffer);
  al_destroy_font(renderer->font);
}