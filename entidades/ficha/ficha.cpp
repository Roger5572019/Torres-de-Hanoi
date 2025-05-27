#include <allegro5/allegro5.h>
#include "alambre.hpp"
#include "ficha.hpp"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

void crearFichas(int num, float altura, Ficha *misFichas)
{
  int ancho = 40, count = 0;
  float alto = altura / num;
  float centroX = 100;
  float x_inicial;

  for (int i = 0; i < num; i++)
  {
    Ficha *fichaI = &misFichas[i];
    fichaI->height = alto;
    fichaI->width = ancho;
    fichaI->color = static_cast<Color>(i % 7); // Ciclador de colores
    fichaI->numeroFicha = count;
    fichaI->idPalo = 0;
    count++;
    if (i == 0)
      fichaI->y = 100 + 20;
    else
      fichaI->y = misFichas[i - 1].y + alto;
    x_inicial = centroX - (ancho / 2);
    fichaI->x = x_inicial;
    ancho += 20; // crecer cada ficha
  }
}

void moverFicha(int num, Ficha *misFichas, Alambre *misAlambres, int idFicha, int idPalo)
{
  for (int i = 0; i < num; i++)
  {
    if (misFichas[i].numeroFicha == idFicha)
    {
      int paloAnterior = misFichas[i].idPalo;

      for (int j = 0; j < 3; j++)
      {
        if (misAlambres[j].id == idPalo)
        {
          misAlambres[j].count++;
          misFichas[i].x = misAlambres[j].x + (misAlambres[j].width / 2) - (misFichas[i].width / 2);
          misFichas[i].y = (misAlambres[j].y + misAlambres[j].height + 20) - (misFichas[i].height * misAlambres[j].count);
          misAlambres[paloAnterior].count--;
          misFichas[i].idPalo = misAlambres[j].id;
          break;
        }
      }
    }
  }
}

void renderizarFichas(Ficha *fichas, int tam)
{
  for (int i = 0; i < tam; i++)
  {
    Ficha *fichaI = &fichas[i];
    al_draw_filled_rounded_rectangle(
        fichaI->x, fichaI->y,
        fichaI->x + fichaI->width, fichaI->y + fichaI->height,
        10, 10,
        obtenerColor(fichaI->color));
  }
}

ALLEGRO_COLOR obtenerColor(Color color)
{
  switch (color)
  {
  case ROJO:
    return al_map_rgb(255, 0, 0);
  case NARANJA:
    return al_map_rgb(255, 165, 0);
  case AMARILLO:
    return al_map_rgb(255, 255, 0);
  case VERDE:
    return al_map_rgb(0, 255, 0);
  case AZUL:
    return al_map_rgb(0, 0, 255);
  case MORADO:
    return al_map_rgb(128, 0, 128);
  case ROSA:
    return al_map_rgb(255, 105, 180);
  default:
    return al_map_rgb(255, 255, 255);
  }
}
