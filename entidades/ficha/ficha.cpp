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
    fichaI->numeroFicha = count + 1;
    fichaI->idPalo = 0;
    fichaI->movAbajo = false;
    fichaI->movArriba = false;
    fichaI->movHorizontal = false;
    count++;
    if (i == 0)
      fichaI->y = 300 + 20;
    else
      fichaI->y = misFichas[i - 1].y + alto;
    x_inicial = centroX - (ancho / 2);
    fichaI->x = x_inicial;
    ancho += 20; // crecer cada ficha
  }
}

void reiniciarMovFichas(int num, Ficha *misFichas)
{

  for (int i = 0; i < num; i++)
  {
    Ficha *fichaI = &misFichas[i];

    fichaI->movAbajo = false;
    fichaI->movArriba = false;
    fichaI->movHorizontal = false;
  }
}

bool moverFicha(int num, Ficha *misFichas, Alambre *misAlambres, int idFicha, int idPalo, float vel)
{
  const float velocidad = vel;
  bool fichaMovida = false;

  for (int i = 0; i < num; i++)
  {
    if (misFichas[i].numeroFicha == idFicha)
    {
      int paloAnterior = misFichas[i].idPalo;

      for (int j = 0; j < 3; j++)
      {
        if (misAlambres[j].id == idPalo)
        {
          // --- FASE 1: MOVER HACIA ARRIBA ---
          if (!misFichas[i].movArriba)
          {
            // 1. Calculamos la posición Y de destino
            float destinoY = misAlambres[paloAnterior].y - 150;

            // 2. Comprobamos si AÚN no hemos llegado
            if (misFichas[i].y > destinoY)
            {
              // 3. Calculamos la distancia que nos falta
              float distanciaFaltante = misFichas[i].y - destinoY;

              // 4. Movemos la ficha la cantidad correcta
              if (distanciaFaltante > velocidad)
              {
                // Si falta más que nuestra velocidad, movemos a toda velocidad
                misFichas[i].y -= velocidad;
              }
              else
              {
                misFichas[i].y -= distanciaFaltante;
              }
            }
            else // Si ya llegamos o nos pasamos, corregimos y terminamos esta fase
            {
              misFichas[i].y = destinoY; 
              misFichas[i].movArriba = true;
            }
          }
          // --- FASE 2: MOVER HORIZONTALMENTE ---
          else if (!misFichas[i].movHorizontal)
          {
            // 1. Calculamos la posición X de destino (para que quede centrada)
            float destinoX = misAlambres[j].x + (misAlambres[j].width / 2) - (misFichas[i].width / 2);

            // 2. Calculamos la distancia (puede ser positiva o negativa)
            float distanciaX = misFichas[i].x - destinoX;

            // 3. Si la distancia (en valor absoluto) es mayor que cero, nos movemos
            if (abs(distanciaX) > 0.1f) // Usamos un umbral pequeño para evitar problemas con floats
            {
              if (abs(distanciaX) > velocidad)
              {
                // Mover a toda velocidad hacia la izquierda o derecha
                if (distanciaX > 0)
                  misFichas[i].x -= velocidad; // Mover a la izquierda
                else
                  misFichas[i].x += velocidad; // Mover a la derecha
              }
              else
              {
                // Mover la distancia exacta que falta
                misFichas[i].x -= distanciaX;
              }
            }
            else // Si ya llegamos, corregimos y terminamos
            {
              misFichas[i].x = destinoX; // Asegura la posición exacta
              misFichas[i].movHorizontal = true;
            }
          }
          // --- FASE 3: MOVER HACIA ABAJO ---
          else if (!misFichas[i].movAbajo)
          {
            // 1. Calculamos la posición Y de destino
            float destinoY = (misAlambres[j].y + misAlambres[j].height + 20) - (misFichas[i].height * misAlambres[j].count);

            // 2. Comprobamos si AÚN no hemos llegado
            if (misFichas[i].y < destinoY)
            {
              // 3. Calculamos la distancia
              float distanciaFaltante = destinoY - misFichas[i].y;

              // 4. Movemos la cantidad correcta
              if (distanciaFaltante > velocidad)
              {
                misFichas[i].y += velocidad;
              }
              else
              {
                misFichas[i].y += distanciaFaltante;
              }
            }
            else // Si ya llegamos, corregimos y terminamos
            {
              misFichas[i].y = destinoY; // Asegura la posición exacta
              misFichas[i].movAbajo = true;
              fichaMovida = true; // El movimiento completo ha terminado
            }
          }

          break; // Salimos del bucle de alambres una vez encontrado el destino
        }
      }
    }
  }
  return fichaMovida;
}

void actualizarEstados(int num, Ficha *misFichas, Alambre *misAlambres, int idFicha, int idPalo)
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
