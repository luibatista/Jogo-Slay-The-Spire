#ifndef RENDERER_H
#define RENDERER_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h> 
#include "jogador.h"
#include "inimigo.h"

typedef struct {
    ALLEGRO_DISPLAY* display;
    ALLEGRO_BITMAP* display_buffer;
    ALLEGRO_FONT* font;
} Renderer;

void DrawScaledText(ALLEGRO_FONT* font, ALLEGRO_COLOR color, float x, float y, float xscale, float yscale, int alignment, const char* text);
void FillRenderer(Renderer* renderer);
void RenderBackground(Renderer* renderer);
void RenderHealthBar(float x_begin, float x_end, float y_pos, int vida_atual, int vida_max, int escudo, ALLEGRO_FONT* font);
void RenderPlayer(Renderer* renderer, Jogador* jogador);
void RenderEnemies(Renderer* renderer, Inimigo* inimigos, int qtd, int alvo_selecionado);
void RenderHand(Renderer* renderer, Jogador* player, int selecionada);
void Render(Renderer* renderer, Jogador* jogador, Inimigo* inimigos, int qtd_inimigos, int carta_selecionada, int alvo_selecionado);

void DrawKeyVisual(Renderer* r, float x, float y, int w, const char* keyText, const char* descText);
void RenderMenu(Renderer* renderer);

void ClearRenderer(Renderer* renderer);

#endif