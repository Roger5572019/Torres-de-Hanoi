#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

int main()
{
    al_init();
    al_install_keyboard();

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue(); // inicializar cola de eventos
    ALLEGRO_DISPLAY *disp = al_create_display(320, 500);  // pantalla
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_FONT *font = al_create_builtin_font(); // fuente
    // Registrar eventos a queque (cola de eventos)
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    ALLEGRO_EVENT event;
    bool redraw = true;
    al_start_timer(timer);
    while (1)
    {
        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_TIMER)
            redraw = true;
        else if ((event.type == ALLEGRO_EVENT_KEY_DOWN) || (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE))
            break;

        if (redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}