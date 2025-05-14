#include <allegro5/allegro5.h>
#include "ficha.hpp"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

void renderizarFichas(int num, float altura)
{
  Ficha fichas[num];
  int ancho = 40;
  int alto = 20;
  float centroX = 300;
  float x_inicial;

  for (int i = 0; i < num; i++)
  {
    Ficha *fichaI = &fichas[i];
    fichaI->height = alto;
    fichaI->width = ancho;

    if (i == 0)
      fichaI->y = 100;
    else
      fichaI->y = fichas[i - 1].y + alto;

    x_inicial = centroX - (ancho / 2);
    al_draw_filled_rounded_rectangle(
        x_inicial, fichaI->y,
        x_inicial + ancho, fichaI->y + alto,
        10, 10,
        al_map_rgb(255, 255, 255));

    ancho += 20; // crecer cada ficha
  }
}
