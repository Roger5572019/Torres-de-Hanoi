#include <allegro5/allegro5.h>
#include "entidades/ficha.hpp"
#include "entidades/alambre.hpp"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

using namespace std;

void hanoi(int num, int origen, int destino, int auxiliar, Ficha *misFichas, Alambre *misAlambres, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font, bool reiniciado);
void gestionar_pausa_y_eventos(Ficha *misFichas, Alambre *misAlambres, int numFichas, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font);
void animarMovimientoFicha(int idFicha, int paloDestino, Ficha *fichas, Alambre *alambres, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font);
void configurarPartida(float &velocidad, Ficha *&fichas, Alambre *alambres);
FILE *gestionarArchivoResultados(int numFichas, bool &usarArchivo);
int leerNumFichasFile();

int const SIZE_W = 800;
int const SIZE_H = 800;

int numFichasGlobal = 0;
float vel = 20.0f;
Ficha misFichas;

bool usoDeFile = false;
bool terminado = false;
bool pausa = false;

FILE *prtFileUltimo;

int main()
{

    // Inicializar librería principal de Allegro
    al_init();
    // Inicializar tecladoFichasFichas
    al_install_keyboard();
    // Inicializar addons de fuentes
    al_init_font_addon();
    al_init_ttf_addon();
    // Inicializar addons de figuras
    al_init_primitives_addon();

    // Antialiasing para las figuras
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    // Crear pantalla
    ALLEGRO_DISPLAY *disp = al_create_display(SIZE_W, SIZE_H);

    // Crear temporizador para controlar los FPS (60 FPS en este caso)
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);

    // Crear cola de eventos
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_EVENT event;

    // Cargar fuente
    ALLEGRO_FONT *font = al_load_font("fuentes\\arial.ttf", 24, 0);

    if (!font)
    {
        printf("Error: no se pudo cargar la fuente.\n");
        return -1;
    }

    // Registrar eventos a la cola de eventos
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    // --- CONFIGURACIÓN DE LA PARTIDA ---
    Ficha *misFichas = nullptr;
    Alambre misAlambres[3];
    configurarPartida(vel, misFichas, misAlambres);

    // --- GESTIÓN DEL ARCHIVO ---
    prtFileUltimo = gestionarArchivoResultados(numFichasGlobal, usoDeFile);
    if (!prtFileUltimo)
    {
        delete[] misFichas;
        return -1;
    }

    // --- BUCLE PRINCIPAL DEL JUEGO ---
    al_start_timer(timer);
    bool redraw = true;
    int fichasVistas = numFichasGlobal;
    bool iniHanoi = false;
    bool finHanoi = false;
    bool reiniciado = false;
    int idFichaFile;
    int idPaloFile;

    while (!terminado)
    {
        // Esperar evento
        al_wait_for_event(queue, &event);

        // Si es un evento del temporizador, marcar para redibujar
        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;
        }

        else if ((event.type == ALLEGRO_EVENT_KEY_DOWN))
        {
            if (!finHanoi)
            {
                if (event.keyboard.keycode == ALLEGRO_KEY_E)
                {

                    if (!usoDeFile)
                    {
                        if (!iniHanoi)
                        {
                            iniHanoi = true;
                            hanoi(fichasVistas, 0, 2, 1, misFichas, misAlambres, queue, font, reiniciado);

                            fclose(prtFileUltimo);

                            if (terminado)
                            {
                                cout << "\nAnimacion interrumpida. Cambios descartados." << endl;
                                remove("resultadosTmp.txt");
                            }
                            else
                            {
                                cout << "\nAnimacion completada. Guardando resultados." << endl;
                                remove("resultados.txt");                   
                                rename("resultadosTmp.txt", "resultados.txt"); 
                            }

                            finHanoi = true;
                        }
                    }
                    else
                    {
                        if (!iniHanoi)
                        {
                            iniHanoi = true;
                            rewind(prtFileUltimo);
                            fscanf(prtFileUltimo, "%*d\n");

                            while (fscanf(prtFileUltimo, "%d\t%d\n", &idFichaFile, &idPaloFile) == 2)
                            {
                                if (terminado)
                                    break;

                                actualizarEstados(numFichasGlobal, misFichas, misAlambres, idFichaFile, idPaloFile);
                                reiniciarMovFichas(idFichaFile, misFichas);
                                animarMovimientoFicha(idFichaFile, idPaloFile, misFichas, misAlambres, queue, font);
                            }
                            finHanoi = true;
                        }
                    }
                }
            }
            else
            {
                if (event.keyboard.keycode == ALLEGRO_KEY_R)
                {
                    iniHanoi = false;
                    finHanoi = false;
                    reiniciado = true;
                    // Crear los objetos del juego
                    crearFichas(numFichasGlobal, 220, misFichas);
                    crearAlambres(3, numFichasGlobal, misAlambres);
                }
            }

            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                terminado = false;
                break;
            }
        }

        else if ((event.type == ALLEGRO_EVENT_DISPLAY_CLOSE))
        {
            terminado = false;
            break;
        }

        if (redraw && al_is_event_queue_empty(queue))
        {

            al_clear_to_color(al_map_rgb(0, 0, 0));
            if (!iniHanoi)
            {
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 600, ALLEGRO_ALIGN_CENTER, "Presiona 'E' para Empezar");
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 650, ALLEGRO_ALIGN_CENTER, "Presiona 'ESC' para Salir");
            }
            if (finHanoi)
            {
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 600, ALLEGRO_ALIGN_CENTER, "Presiona 'R' para Reiniciar");
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 650, ALLEGRO_ALIGN_CENTER, "Presiona 'ESC' para Salir");
            }
            renderizarAlambres(3, misAlambres);
            renderizarFichas(misFichas, numFichasGlobal);
            al_flip_display();

            redraw = false;
        }
    }

    // Destruir recursos y cerrar
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    delete (misFichas);
    fclose(prtFileUltimo);
    return 0;
}

void gestionar_pausa_y_eventos(Ficha *misFichas, Alambre *misAlambres, int numFichas, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font)
{

    ALLEGRO_EVENT event;
    while (al_get_next_event(event_queue, &event))
    {
        if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (event.keyboard.keycode == ALLEGRO_KEY_P)
            {
                pausa = !pausa;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
            {
                vel = vel + 5.0f;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
            {
                if (vel > 5)
                {
                    vel = vel - 5.0f;
                }
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                terminado = true;
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            terminado = true;
        }
    }

    while (pausa && !terminado)
    {

        al_clear_to_color(al_map_rgb(0, 0, 0));
        renderizarAlambres(3, misAlambres);
        renderizarFichas(misFichas, numFichas);
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 650, ALLEGRO_ALIGN_CENTER, "PAUSA (Presiona 'P' para continuar)");
        al_flip_display();

        al_wait_for_event(event_queue, &event);
        if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (event.keyboard.keycode == ALLEGRO_KEY_P)
            {
                pausa = false;
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                terminado = true;
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            terminado = true;
        }
    }
}

void hanoi(int num, int origen, int destino, int auxiliar, Ficha *misFichas, Alambre *misAlambres, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font, bool reiniciado)
{
    if (terminado)
        return;

    if (num == 1)
    {
        cout << "Mueva el bloque " << num << " desde " << origen << " hasta " << destino << endl;
        if (!reiniciado)
            fprintf(prtFileUltimo, "%d\t%d\n", num, destino);

        actualizarEstados(numFichasGlobal, misFichas, misAlambres, num, destino);
        reiniciarMovFichas(num, misFichas);
        animarMovimientoFicha(num, destino, misFichas, misAlambres, event_queue, font);
    }
    else
    {
        hanoi(num - 1, origen, auxiliar, destino, misFichas, misAlambres, event_queue, font, reiniciado);
        if (terminado)
            return;
        cout << "Mueva el bloque " << num << " desde " << origen << " hasta " << destino << endl;
        if (!reiniciado)
            fprintf(prtFileUltimo, "%d\t%d\n", num, destino);

        actualizarEstados(numFichasGlobal, misFichas, misAlambres, num, destino);
        reiniciarMovFichas(num, misFichas);

        animarMovimientoFicha(num, destino, misFichas, misAlambres, event_queue, font);

        hanoi(num - 1, auxiliar, destino, origen, misFichas, misAlambres, event_queue, font, reiniciado);
    }
}

int leerNumFichasFile()
{
    FILE *primerLinea = fopen("resultados.txt", "r");
    if (!primerLinea)
    {
        cout << "No se encontro el archivo, Error";
        return -1;
    }
    int numFichas = 0;
    fscanf(primerLinea, "%d\n", &numFichas);
    fclose(primerLinea);
    return numFichas;
}

// Animar el movimiento de una ficha hasta que se complete.
void animarMovimientoFicha(int idFicha, int paloDestino, Ficha *fichas, Alambre *alambres, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font)
{
    bool fichaMovida = false;
    do
    {
        if (terminado)
            break; // Salir si el usuario presiona ESC

        // Gestiona eventos como pausa, velocidad, etc.
        gestionar_pausa_y_eventos(fichas, alambres, numFichasGlobal, queue, font);

        // Mueve la ficha un pequeño paso
        fichaMovida = moverFicha(numFichasGlobal, fichas, alambres, idFicha, paloDestino, vel);

        // Dibuja el estado actual de la escena
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 600, ALLEGRO_ALIGN_CENTER, "Aumenta o Disminuye Velocidad con < >");
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 650, ALLEGRO_ALIGN_CENTER, "Presiona 'P' para pausar");
        renderizarAlambres(3, alambres);
        renderizarFichas(fichas, numFichasGlobal);
        al_flip_display();

        // Pequeña pausa para controlar la velocidad de la animación
        al_rest(1.0 / 120.0);

    } while (!fichaMovida);
}

void configurarPartida(float &velocidad, Ficha *&fichas, Alambre *alambres)
{

    cout << "Fichas: ";
    cin >> numFichasGlobal;
    fichas = new Ficha[numFichasGlobal];

    do
    {
        cout << "Ingresa la velocidad pixeles por frame (1-50): ";
        cin >> velocidad;
    } while (velocidad <= 0 || velocidad > 50);

    // Crear los objetos del juego
    crearFichas(numFichasGlobal, 220, fichas);
    crearAlambres(3, numFichasGlobal, alambres);
}

FILE *gestionarArchivoResultados(int numFichas, bool &usarArchivo)
{
    FILE *archivo = nullptr;
    int fichasEnArchivo = leerNumFichasFile();
    cout << "Numero de fichas en archivo previo: " << fichasEnArchivo << endl;

    if (numFichas != fichasEnArchivo)
    {
        cout << "El numero de fichas es diferente. Creando nuevo archivo de resultados..." << endl;
        archivo = fopen("resultadosTmp.txt", "w");
        if (archivo)
        {
            fprintf(archivo, "%d\n", numFichas);
        }
    }
    else
    {
        char opcion = ' ';
        do
        {
            cout << "Se detecto un archivo compatible. Quieres usarlo para ver la solucion? (s/n)" << endl;
            cin >> opcion;

            if (opcion == 's')
            {
                usarArchivo = true;
                archivo = fopen("resultados.txt", "r");
            }
            else if (opcion == 'n')
            {
                usarArchivo = false;
                archivo = fopen("resultadosTmp.txt", "w");
                if (archivo)
                {
                    fprintf(archivo, "%d\n", numFichas);
                }
            }
            else
            {
                cout << "Opcion invalida, intente de nuevo." << endl;
            }
        } while (opcion != 's' && opcion != 'n');
    }

    if (!archivo)
    {
        cout << "Error: No se pudo abrir el archivo de resultados." << endl;
    }

    return archivo;
}