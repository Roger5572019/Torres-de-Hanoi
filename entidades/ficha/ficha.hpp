#pragma once

enum Color {ROJO = 0, NARANJA = 1, AMARILLO = 2, VERDE = 3, AZUL = 4, MORADO = 5, ROSA = 6};

struct Ficha
{
    float x, y;
    float width, height;
    Color color;
};

void renderizarFichas(int , float);
ALLEGRO_COLOR obtenerColor(Color);
