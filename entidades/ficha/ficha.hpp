#pragma once
#include "alambre.hpp"
enum Color {ROJO = 0, NARANJA = 1, AMARILLO = 2, VERDE = 3, AZUL = 4, MORADO = 5, ROSA = 6};

struct Ficha
{
    float x, y;
    float width, height;
    int numeroFicha;
    int idPalo;
    Color color;

};

void moverFicha(int num, Ficha *misFichas, Alambre *misAlambres, int idFicha, int idPalo);
void renderizarFichas(Ficha* , int);
void crearFichas(int num, float altura, Ficha *misFichas);
ALLEGRO_COLOR obtenerColor(Color);
