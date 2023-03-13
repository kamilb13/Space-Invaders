#include <allegro5/allegro_font.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>

#define SCREEN_WIDTH 1280   
#define SCREEN_HEIGHT 780   
#define BALL_SPEED 0.05     
#define PADDLE_SPEED 0.5    // jest w strukturze Player
#define BULLET_SIZE 10      


int main(){
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();
    al_install_mouse();

    float ball_x = SCREEN_WIDTH / 2.0;
    float ball_y = SCREEN_HEIGHT / 2.0; 
    float ball_vx = BALL_SPEED;
    float ball_vy = BALL_SPEED;
    float paddle_x = 0;
    float paddle_y = SCREEN_HEIGHT - 100;

    float bullet_x = 0;
    float bullet_y = 0;
    float bullet_vx = 0;
    float bullet_vy = -1;
    bool bullet_active = false;



    //Tworzę zmienną display typu ALLEGRO_DISPLAY
    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    al_set_window_title(display, "Space Invaders");

    ALLEGRO_COLOR bg_color = al_map_rgb(0, 0, 0);
    ALLEGRO_COLOR ball_color = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR paddle_color = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR bullet_color = al_map_rgb(0, 255, 0);
    ALLEGRO_COLOR object_color = al_map_rgb(255, 255, 255);

    ALLEGRO_KEYBOARD_STATE keyboard_state;
    ALLEGRO_MOUSE_STATE mouse_state;

    ALLEGRO_BITMAP* bitmap = al_load_bitmap("C:/Users/kamil/Desktop/player.png");
    if (!bitmap) {
        printf("Nie udalo sie zaladowac obrazka!\n");
        return -1;
    }

    while (1) {
        //al_flip_display();
        puts(" ");
        //printf("%lf", paddle_x);
        puts(" ");
        // Przetwarzanie zdarzeń klawiatury i myszy
        al_get_keyboard_state(&keyboard_state);
        //al_get_mouse_state(&mouse_state);
        if (al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT)) {
            //al_clear_to_color(bg_color);
            paddle_x -= PADDLE_SPEED;
            if (paddle_x < 0) {
                //al_clear_to_color(bg_color);
                paddle_x = 0;
            }
        }
        /*
        // Rysowanie obiektów
        for (int i = 0; i < NUM_OBJECTS; i++) {
            if (objects[i].active) {
                al_draw_filled_rectangle(objects[i].x, objects[i].y, objects[i].x + objects[i].width, objects[i].y + objects[i].height, al_map_rgb(255, 255, 0));
            }
        }
        */
        if (al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT)){
            //al_clear_to_color(bg_color);
            paddle_x += PADDLE_SPEED;
            if (paddle_x > SCREEN_WIDTH - 150) {
                al_clear_to_color(bg_color);
                paddle_x = SCREEN_WIDTH - 150;
            }
        }
        if (al_key_down(&keyboard_state, ALLEGRO_KEY_SPACE) && !bullet_active){
            //printf("kiszka");
            //al_clear_to_color(bg_color);
            bullet_active = true;
            bullet_x = paddle_x + 25;
            bullet_y = paddle_y;
        }

        // Aktualizacja pozycji pocisku
        if (bullet_active) {
            bullet_x += bullet_vx;
            bullet_y += bullet_vy;

            // Sprawdzenie, czy pocisk wyleciał poza ekran
            if (bullet_y < 0) {
                printf("wylecial");
                bullet_active = false;
            }
        }

        // Rysowanie pocisku
        if (bullet_active){
            //al_clear_to_color(bg_color);
            al_draw_filled_rectangle(bullet_x+45, bullet_y, bullet_x + BULLET_SIZE+45, bullet_y + BULLET_SIZE, bullet_color);
            //printf("%lf", bullet_y);
            if (bullet_y == SCREEN_HEIGHT) {
                printf("kiszka");
            }
            // Sprawdzenie kolizji z obiektami
            /*
            for (int i = 0; i < NUM_OBJECTS; i++) {
                if (objects[i].active && bullet_x + BULLET_SIZE >= objects[i].x && bullet_x <= objects[i].x + objects[i].width && bullet_y <= objects[i].y + objects[i].height) {
                    objects[i].active = false;
                    bullet_active = false;
                }
            }
            */
            al_flip_display();
            //al_clear_to_color(bg_color);

        }

        al_clear_to_color(bg_color);
        al_draw_bitmap(bitmap, paddle_x, paddle_y, 0);
        al_flip_display();
    }

    al_destroy_bitmap(bitmap);
    al_destroy_display(display);
}
