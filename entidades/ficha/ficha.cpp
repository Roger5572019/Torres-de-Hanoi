#include <allegro5/allegro5.h>
#include "ficha.hpp"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

void renderizarFichas(int num, float altura)
{
  Ficha fichas[num];
  int ancho = 40;
  float alto = altura / num;
  float centroX = 300;
  float x_inicial;

  for (int i = 0; i < num; i++)
  {
    Ficha *fichaI = &fichas[i];
    fichaI->height = alto;
    fichaI->width = ancho;
    fichaI->color = static_cast<Color>(i % 7);//Ciclador de colores
    if (i == 0)
      fichaI->y = 300 + 10;
    else
      fichaI->y = fichas[i - 1].y + alto;

    x_inicial = centroX - (ancho / 2);

    al_draw_filled_rounded_rectangle(
        x_inicial, fichaI->y,
        x_inicial + fichaI->width, fichaI->y + fichaI->height,
        10, 10,
        obtenerColor(fichaI->color)
    );
    
    ancho += 20; // crecer cada ficha
  }
}

ALLEGRO_COLOR obtenerColor(Color color)
{
    switch (color)
    {
    case ROJO:    return al_map_rgb(255, 0, 0);
    case NARANJA: return al_map_rgb(255, 165, 0);
    case AMARILLO:return al_map_rgb(255, 255, 0);
    case VERDE:   return al_map_rgb(0, 255, 0);
    case AZUL:    return al_map_rgb(0, 0, 255);
    case MORADO:  return al_map_rgb(128, 0, 128);
    case ROSA:    return al_map_rgb(255, 105, 180);
    default:      return al_map_rgb(255, 255, 255);
    }
}
