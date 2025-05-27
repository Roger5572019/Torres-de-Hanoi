#include <allegro5/allegro5.h>
#include "alambre.hpp"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

void renderizarAlambres(int num)
{
    Alambre alambres[num];
    int ancho = 10;
    int alto = 220;
    float centroX = 100;
    float x_inicial;

    for (int i = 0; i < num; i++)
    {
        Alambre *alambresI = &alambres[i];
        alambresI->height = alto;
        alambresI->width = ancho;

        // Siempre calcular x_inicial en el centro
        x_inicial = centroX - (ancho / 2);

        // Posición Y base
        alambresI->y = 100;
        alambres->x = x_inicial;
        // Dibuja el alambre (como torre)
        al_draw_filled_rounded_rectangle(
            x_inicial, alambresI->y,                  // parte superior
            alambres->x + ancho, alambresI->y + alto + 20, // parte inferior
            5, 5,
            al_map_rgb(77, 19, 6)); // marrón

        centroX += 305; // separa las torres
    }
}
