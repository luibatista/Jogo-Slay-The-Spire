#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include "jogador.h"
#include "inimigo.h"

typedef struct {
  ALLEGRO_DISPLAY* display;
  ALLEGRO_BITMAP* display_buffer;
  ALLEGRO_FONT* font;
} Renderer;

void FillRenderer(Renderer* renderer);

void DrawScaledText(ALLEGRO_FONT* font, ALLEGRO_COLOR color, float x, float y,
                    float xscale, float yscale, int alignment, const char* text);

void Render(Renderer* renderer, Jogador* jogador, Inimigo* inimigos, int qtd_inimigos, int carta_selecionada, int alvo_selecionado);

void ClearRenderer(Renderer* renderer);

#endif