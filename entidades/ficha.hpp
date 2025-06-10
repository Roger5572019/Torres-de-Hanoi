#pragma once
#include "alambre.hpp"
enum Color {ROJO = 0, NARANJA = 1, AMARILLO = 2, VERDE = 3, AZUL = 4, MORADO = 5, ROSA = 6};

struct Ficha
{
    float x, y;
    float width, height;
    int numeroFicha;
    int idPalo;
    bool movArriba, movAbajo, movHorizontal;
    Color color;

};

bool moverFicha(int num, Ficha *misFichas, Alambre *misAlambres, int idFicha, int idPalo, float vel);
void renderizarFichas(Ficha* , int);
void crearFichas(int num, float altura, Ficha *misFichas);
void actualizarEstados(int num, Ficha *misFichas, Alambre *misAlambres, int idFicha, int idPalo);
void reiniciarMovFichas(int num, Ficha *misFichas);
ALLEGRO_COLOR obtenerColor(Color);
