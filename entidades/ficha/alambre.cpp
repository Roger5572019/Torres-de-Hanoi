#include <allegro5/allegro5.h>
#include "alambre.hpp"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

void crearAlambres(int num, int fichas, Alambre *alambres)
{
    int ancho = 10, count = 0;
    int alto = 220;
    float centroX = 100;
    float x_inicial;

    for (int i = 0; i < num; i++)
    {
        Alambre *alambresI = &alambres[i];
        if (i == 0)
        {
            alambresI->count = fichas;
        }
        else
        {
            alambresI->count = 0;
        }

        alambresI->height = alto;
        alambresI->width = ancho;

        // Siempre calcular x_inicial en el centro
        x_inicial = centroX - (ancho / 2);

        // Posición Y base
        alambresI->y = 300;
        alambresI->x = x_inicial;
        alambresI->id = count;
        count++;
        centroX += 305; // separa las torres
    }
}

void renderizarAlambres(int num, Alambre *alambresI)
{
    for (int i = 0; i < num; i++)
    {
        al_draw_filled_rounded_rectangle(
            alambresI[i].x, alambresI[i].y,
            alambresI[i].x + alambresI[i].width, alambresI[i].y + alambresI[i].height + 20,
            5, 5,
            al_map_rgb(77, 19, 6)); // marrón
    }
}
