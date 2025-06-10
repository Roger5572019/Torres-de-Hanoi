#include <allegro5/allegro5.h>
#include "entidades/ficha/ficha.hpp"
#include "entidades/ficha/alambre.hpp"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

using namespace std;

void hanoi(int num, int origen, int destino, int auxiliar, Ficha *misFichas, Alambre *misAlambres, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font);
void gestionar_pausa_y_eventos(Ficha *misFichas, Alambre *misAlambres, int numFichas, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font);
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
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_FONT *font = al_load_font("fuentes\\arial.ttf", 24, 0);
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    // Crear pantalla
    ALLEGRO_DISPLAY *disp = al_create_display(SIZE_W, SIZE_H);
    // Crear temporizador para controlar los FPS (30 FPS en este caso)

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    // Cargar fuente

    if (!font)
    {
        printf("Error: no se pudo cargar la fuente.\n");
        return -1;
    }

    // Registrar eventos a la cola de eventos
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    ALLEGRO_EVENT event;
    bool redraw = true;
    // Iniciar temporizador
    al_start_timer(timer);
    cout << "Fichas: ";
    cin >> numFichasGlobal;
    Ficha *misFichas = new Ficha[numFichasGlobal];
    Alambre misAlambres[3];
    do
    {
        cout << "Ingresa la velocidad: ";
        cin >> vel;

    } while (vel <= 0 || vel > 50);

    crearFichas(numFichasGlobal, 220, misFichas);
    crearAlambres(3, numFichasGlobal, misAlambres);
    int fichasVistas = numFichasGlobal;
    bool finHanoi = false;
    cin.ignore();
    cin.clear();

    cout << "Numero del archivo: " << leerNumFichasFile();

    if (numFichasGlobal != leerNumFichasFile())
    {
        prtFileUltimo = fopen("resultados.txt", "w");

        if (!prtFileUltimo)
        {
            cout << "Error File";
            return -1;
        }
        fprintf(prtFileUltimo, "%d\n", numFichasGlobal);
    }
    else
    {
        char opcion = ' ';
        do
        {

            cout << "Se detecto que el archivo es compatible para tu numero de fichas" << endl;
            cout << "Quieres usarlo? (s/n)" << endl;
            cin >> opcion;
            switch (opcion)
            {
            case 's':
                usoDeFile = true;
                prtFileUltimo = fopen("resultados.txt", "r");
                if (!prtFileUltimo)
                {
                    cout << "Error File";
                    return -1;
                }
                break;
            case 'n':
                usoDeFile = false;
                prtFileUltimo = fopen("resultados.txt", "w");
                if (!prtFileUltimo)
                {
                    cout << "Error File";
                    return -1;
                }
                fprintf(prtFileUltimo, "%d\n", numFichasGlobal);
                break;
            default:
                cout << "opcion invalida, intente de nuevo";
                break;
            }
        } while (opcion != 's' && opcion != 'n');
    }
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

        // Si se presiona una tecla o se cierra la ventana, salir del bucle
        else if ((event.type == ALLEGRO_EVENT_KEY_DOWN))
        {
            if (!usoDeFile)
            {
                if (!finHanoi)
                {
                    hanoi(fichasVistas, 0, 2, 1, misFichas, misAlambres, queue, font);
                    finHanoi = true;
                }
            }
            else
            {
                if (!finHanoi)
                {
                    rewind(prtFileUltimo);
                    fscanf(prtFileUltimo, "%*d\n");

                    while (!feof(prtFileUltimo))
                    {
                        if (terminado)
                            break;
                        fscanf(prtFileUltimo, "%d\t%d\n", &idFichaFile, &idPaloFile);
                        actualizarEstados(numFichasGlobal, misFichas, misAlambres, idFichaFile, idPaloFile);
                        reiniciarMovFichas(idFichaFile, misFichas);
                        int fichaMovida;
                        do
                        {
                            fichaMovida = false;
                            if (terminado)
                                break;
                            gestionar_pausa_y_eventos(misFichas, misAlambres, numFichasGlobal, queue, font);
                            fichaMovida = moverFicha(numFichasGlobal, misFichas, misAlambres, idFichaFile, idPaloFile, vel);
                            // Redibujar la pantalla para mostrar el movimiento realizado
                            al_clear_to_color(al_map_rgb(0, 0, 0));
                            renderizarAlambres(3, misAlambres);
                            renderizarFichas(misFichas, numFichasGlobal);
                            al_flip_display();

                            // La pausa de 1 segundo para poder ver el resultado
                            al_rest(1.0 / 120.0);
                        } while (!fichaMovida);
                    }
                    finHanoi = true;
                }
            }
        }

        else if ((event.type == ALLEGRO_EVENT_DISPLAY_CLOSE))
        {
            terminado = false;
            break;
        }

        // Si se debe redibujar y no hay más eventos pendientes
        if (redraw && al_is_event_queue_empty(queue))
        {
            // Limpiar pantalla a color negro
            al_clear_to_color(al_map_rgb(0, 0, 0));
            // Dibujar Alambres
            renderizarAlambres(3, misAlambres);
            // Dibujar Fichas
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
                vel = vel + 0.5f;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
            {
                if (vel > 0)
                {
                    vel = vel - 0.5f;
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
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 600, ALLEGRO_ALIGN_CENTER, "PAUSA (Presiona 'P' para continuar)");
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

// Asegúrate de que las variables globales sean accesibles
extern int numFichasGlobal;

void hanoi(int num, int origen, int destino, int auxiliar, Ficha *misFichas, Alambre *misAlambres, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font)
{
    // Si el usuario quiere salir, detenemos la recursión
    if (terminado)
        return;
    int fichaMovida = false;
    if (num == 1)
    {
        cout << "Mueva el bloque " << num << " desde " << origen << " hasta " << destino << endl;
        fprintf(prtFileUltimo, "%d\t%d\n", num, destino);
        actualizarEstados(numFichasGlobal, misFichas, misAlambres, num, destino);
        reiniciarMovFichas(num, misFichas);
        do
        {
            if (terminado)
                return;
            gestionar_pausa_y_eventos(misFichas, misAlambres, numFichasGlobal, event_queue, font);
            fichaMovida = moverFicha(numFichasGlobal, misFichas, misAlambres, num, destino, vel);
            // Redibujar la pantalla para mostrar el movimiento realizado
            al_clear_to_color(al_map_rgb(0, 0, 0));
            renderizarAlambres(3, misAlambres);
            renderizarFichas(misFichas, numFichasGlobal);
            al_flip_display();

            // La pausa de 1 segundo para poder ver el resultado
            al_rest(1.0 / 60.0);

        } while (!fichaMovida);
    }
    else
    {
        hanoi(num - 1, origen, auxiliar, destino, misFichas, misAlambres, event_queue, font);

        cout << "Mueva el bloque " << num << " desde " << origen << " hasta " << destino << endl;
        fprintf(prtFileUltimo, "%d\t%d\n", num, destino);
        actualizarEstados(numFichasGlobal, misFichas, misAlambres, num, destino);
        reiniciarMovFichas(num, misFichas);
        do
        {
            if (terminado)
                return;
            gestionar_pausa_y_eventos(misFichas, misAlambres, numFichasGlobal, event_queue, font);
            fichaMovida = moverFicha(numFichasGlobal, misFichas, misAlambres, num, destino, vel);
            // Redibujar la pantalla para mostrar el movimiento realizado
            al_clear_to_color(al_map_rgb(0, 0, 0));
            renderizarAlambres(3, misAlambres);
            renderizarFichas(misFichas, numFichasGlobal);
            al_flip_display();

            // La pausa de 1 segundo para poder ver el resultado
            al_rest(1.0 / 120.0);
        } while (!fichaMovida);

        hanoi(num - 1, auxiliar, destino, origen, misFichas, misAlambres, event_queue, font);
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
