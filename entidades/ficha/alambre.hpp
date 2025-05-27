#pragma once

struct Alambre
{
    int id;
    float x, y;
    float width, height;
    int count;
};

void renderizarAlambres(int, Alambre *);
void crearAlambres(int, int, Alambre *);
