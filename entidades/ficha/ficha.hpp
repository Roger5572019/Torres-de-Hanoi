#pragma once

enum Color {AMARILLO, ROJO, AZUL, VERDE, MORADO, ROSA};

struct Ficha
{
    float x, y;
    float width, height;
    Color color;
};

void renderizarFichas(int , float);