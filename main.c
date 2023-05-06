#include <allegro5/allegro_font.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280   
#define SCREEN_HEIGHT 780   
//#define BULLET_SIZE 10     // do bullet size

typedef struct Enemy {
    float x;
    float y;
    float hp;
    float dmg;
    float speed;
} Enemy;

typedef struct Ship {
    float x;
    float y;
    float vx;
    float vy;
    float hp;
    float dmg;
} Ship;

typedef struct Bullet {
    float x;
    float vx;
    float y;
    float vy;
    int bullet_size;
    bool bullet_active;
} Bullet;

void startFrame(int* start_window, ALLEGRO_KEYBOARD_STATE* keyboard_state, ALLEGRO_FONT* font, ALLEGRO_COLOR bg_color);
void gameFrame(ALLEGRO_TIMER* timer, ALLEGRO_COLOR bg_color, ALLEGRO_KEYBOARD_STATE* keyboard_state, Ship* ship, Bullet* bullet, ALLEGRO_COLOR bullet_color, int* enemy_iter, Enemy* opponents, float* enemy_move, ALLEGRO_BITMAP* enemy_bitmap, ALLEGRO_BITMAP* bitmap);



int main() {
    // INICJACJA GŁÓWNYCH ELEMENTÓW ALLEGRO 5
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();


    // DEKLARACJA I INICJACJA WARTOŚCI STATKU KOSMICZNEGO KONTROLOWANEGO PRZEZ GRACZA
    Ship ship;
    ship.x = SCREEN_WIDTH / 2.0;
    ship.y = SCREEN_HEIGHT - 100;
    ship.vx = 0.5;
    ship.dmg = 1;


    // DEKLARACJA I INICJACJA WARTOŚCI POCISKU WYSTRZELIWANEGO PRZEZ GRACZA
    Bullet bullet;
    bullet.x = -10;
    bullet.y = -10;
    bullet.vx = 0;
    bullet.vy = -1;
    bullet.bullet_size = 10;
    bullet.bullet_active = false;


    // DYNAMICZNA ALOKACJA TABLICY JAKO KONTENER NA PRZECIWNIKÓW
    Enemy* opponents = (Enemy*)calloc(5, sizeof(Enemy));

    // INICJACJA WARTOŚCI KAŻDEGO Z PRZECIWNIKÓW
    opponents[0].x = 450;
    opponents[0].y = 300;
    opponents[0].dmg = 1;
    opponents[0].hp = 2;
    opponents[0].speed = 0.04;

    opponents[1].x = 600;
    opponents[1].y = 400;
    opponents[1].dmg = 1;
    opponents[1].hp = 2;
    opponents[1].speed = 0.04;

    opponents[2].x = 150;
    opponents[2].y = 300;
    opponents[2].dmg = 1;
    opponents[2].hp = 7;
    opponents[2].speed = 0.04;

    opponents[3].x = 23;
    opponents[3].y = 429;
    opponents[3].dmg = 1;
    opponents[3].hp = 2;
    opponents[3].speed = 0.04;

    opponents[4].x = 500;
    opponents[4].y = 100;
    opponents[4].dmg = 1;
    opponents[4].hp = 2;
    opponents[4].speed = 0.04;



    // DEKLARACJA I INICJACJA OKNA GRY
    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    al_set_window_title(display, "Space Invaders");

    // DEKLARACJA I INICJACJA KOLORÓW UŻYWANYCH DO RYSOWANIA OBIEKTÓW
    ALLEGRO_COLOR bg_color = al_map_rgb(0, 0, 0);
    ALLEGRO_COLOR bullet_color = al_map_rgb(0, 255, 0);

    // DEKLARACJA 
    ALLEGRO_KEYBOARD_STATE keyboard_state;
    ALLEGRO_FONT* font = al_create_builtin_font();

    // ŁADOWANIE GRAFIK REPREZENTUJĄCYCH STATKI KOSMICZNE
    ALLEGRO_BITMAP* bitmap = al_load_bitmap("C:/Users/PC/Desktop/space-invaders/player.png");
    ALLEGRO_BITMAP* enemy_bitmap = al_load_bitmap("C:/Users/PC/Desktop/space-invaders/ufo.png");

    // DEKLARACJA I INICJACJA "STOPERA"
    ALLEGRO_TIMER* timer = NULL;
    timer = al_create_timer(0.001); // 

    // SPRAWDZENIE POPRAWNOSCI ZMIENNYCH
    if (!timer) {
        printf("Nie udalo zainicjowac timera\n");
        return -1;
    }

    if (!bitmap) {
        printf("Nie udalo sie zaladowac obrazka statku kosmicznego gracza!\n");
        return -1;
    }

    if (!enemy_bitmap) {
        printf("Nie udalo sie zaladowac obrazka statku kosmicznego przeciwnikow!\n");
        return -1;
    }

    // DEKLARACJA I INICJACJA DODATKOWYCH ZMIENNYCH
    int enemy_iter;                 // Iterator tablicy wrogów
    float enemy_move = 0;           // Zmienna pomocnicza wykorzystywana do ruchu przeciwników


    // Ekran startowy gry
    int start_window = 1;
    startFrame(&start_window, &keyboard_state, font, bg_color);

    
    // WYSTARTOWANIE TIMERA
    al_start_timer(timer);


    // GŁÓWNA PĘTLA GRY
    gameFrame(timer, bg_color, &keyboard_state, &ship, &bullet, bullet_color, &enemy_iter, opponents, &enemy_move, enemy_bitmap, bitmap);
    //gameFrame(timer, bg_color, &keyboard_state, &ship, &bullet, bullet_color, &enemy_iter, opponents, &enemy_move, enemy_bitmap, bitmap);
    /*
    do {
        // SPRAWDZENIE 
        if (al_get_timer_count(timer) > 0) {
            al_set_timer_count(timer, 0);
            al_clear_to_color(bg_color);

            // Przetwarzanie zdarzeń klawiatury i myszy
            al_get_keyboard_state(&keyboard_state);
            if (al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT)) {
                ship.x -= ship.vx;
                if (ship.x < 0) {
                    ship.x = 0;
                }
            }

            if (al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT)) {
                ship.x += ship.vx;
                if (ship.x > SCREEN_WIDTH - 150) {
                    al_clear_to_color(bg_color);
                    ship.x = SCREEN_WIDTH - 150;
                }
            }
            if (al_key_down(&keyboard_state, ALLEGRO_KEY_SPACE) && !bullet.bullet_active) {
                puts("klikam spacje ");
                bullet.bullet_active = true;
                bullet.x = ship.x + 70;
                bullet.y = ship.y;
            }


            // Rysowanie pocisku
            if (bullet.bullet_active) {

                al_draw_filled_rectangle(bullet.x, bullet.y, bullet.x + bullet.bullet_size, bullet.y + bullet.bullet_size, bullet_color);

                // Aktualizacja pozycji pocisku
                bullet.bullet_active = true;
                if (bullet.bullet_active) {
                    bullet.y += bullet.vy;
                }
            }


            // Sprawdzenie, czy pocisk wyleciał poza ekran
            if (bullet.y < 0) {
                bullet.bullet_active = false;
            }



            for (enemy_iter = 0; enemy_iter < 5; enemy_iter++) {
                if (bullet.x > opponents[enemy_iter].x && bullet.x < opponents[enemy_iter].x + 80 && bullet.y < opponents[enemy_iter].y + 20) {
                    opponents[enemy_iter].hp -= 1;
                    //lista  z przeciwnikami 
                    if (!opponents[enemy_iter].hp) {
                        opponents[enemy_iter].x = -100;
                        opponents[enemy_iter].y = -100;
                    }

                    bullet.bullet_active = false;
                    bullet.x = -10;
                    bullet.y = -10;
                }
            }

            if (enemy_move > 100 || enemy_move < -100) {
                for (enemy_iter = 0; enemy_iter < 5; enemy_iter++) {
                    opponents[enemy_iter].speed *= -1;
                }
            }

            for (enemy_iter = 0; enemy_iter < 5; enemy_iter++) {
                opponents[enemy_iter].x += opponents[enemy_iter].speed;
                if (opponents[enemy_iter].hp) {
                    al_draw_bitmap(enemy_bitmap, opponents[enemy_iter].x, opponents[enemy_iter].y, 0);
                }
            }

            enemy_move += opponents[0].speed;
            enemy_move += opponents[1].speed;
            enemy_move += opponents[2].speed;
            enemy_move += opponents[3].speed;
            enemy_move += opponents[4].speed;


            al_draw_bitmap(bitmap, ship.x, ship.y, 0);
            al_flip_display();
        }
    } while (1);
    */

    //al_destroy_bitmap(bitmap);
    al_destroy_display(display);
    free(opponents);
}

void startFrame(int* start_window, ALLEGRO_KEYBOARD_STATE* keyboard_state, ALLEGRO_FONT* font, ALLEGRO_COLOR bg_color) {
    while (*start_window) {  // prostokat, jeżeli (MyszkaX > ButtonX) Oraz (MyszkaY > ButtonY) Oraz (MyszkaX < ButtonX+ButtonWidth) Oraz (MyszkaY < ButtonY+ButtonHeight) Oraz (LewyPrzyciskMyszkiNacisniety) wtedy ...
        al_get_keyboard_state(keyboard_state);
        al_clear_to_color(bg_color); //ustawia tlo
        //al_flip_display();
        al_draw_filled_rectangle(SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 2 - 50, SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 + 100, al_map_rgb(220, 200, 220));
        al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 + 30, ALLEGRO_ALIGN_CENTER, "Nacisnij ENTER by rozpoczac gre...");
        if (al_key_down(keyboard_state, ALLEGRO_KEY_ENTER)) {
            *start_window = 0;
        }
        al_flip_display();  //odswieza ekran
    }
}

void gameFrame(ALLEGRO_TIMER* timer, ALLEGRO_COLOR bg_color, ALLEGRO_KEYBOARD_STATE* keyboard_state, Ship* ship, Bullet* bullet, ALLEGRO_COLOR bullet_color, int* enemy_iter, Enemy* opponents, float* enemy_move, ALLEGRO_BITMAP* enemy_bitmap, ALLEGRO_BITMAP* bitmap){
//void gameFrame(ALLEGRO_TIMER *timer, ALLEGRO_COLOR bg_color, ALLEGRO_KEYBOARD_STATE* keyboard_state, Ship *ship, Bullet *bullet, ALLEGRO_COLOR bullet_color, int *enemy_iter, Enemy *opponents, float *enemy_move, ALLEGRO_BITMAP* enemy_bitmap, ALLEGRO_BITMAP* bitmap) {
    do {
        // SPRAWDZENIE 
        if (al_get_timer_count(timer) > 0) {
            al_set_timer_count(timer, 0);
            al_clear_to_color(bg_color);

            // Przetwarzanie zdarzeń klawiatury i myszy
            al_get_keyboard_state(&keyboard_state);
            if (al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT)) {
                ship->x -= ship->vx;
                if (ship->x < 0) {
                    ship->x = 0;
                }
            }

            if (al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT)) {
                ship->x += ship->vx;
                if (ship->x > SCREEN_WIDTH - 150) {
                    al_clear_to_color(bg_color);
                    ship->x = SCREEN_WIDTH - 150;
                }
            }
            if (al_key_down(&keyboard_state, ALLEGRO_KEY_SPACE) && !bullet->bullet_active) {
                puts("klikam spacje ");
                bullet->bullet_active = true;
                bullet->x = ship->x + 70;
                bullet->y = ship->y;
            }
            
            printf("%d", ship->dmg);
            // Rysowanie pocisku
            if ((bullet->bullet_active)) { //wyjatek ;)

                al_draw_filled_rectangle(bullet->x, bullet->y, bullet->x + bullet->bullet_size, bullet->y + bullet->bullet_size, bullet_color);

                // Aktualizacja pozycji pocisku
                bullet->bullet_active = true;
                if (bullet->bullet_active) {
                    bullet->y += bullet->vy;
                }
            }


            // Sprawdzenie, czy pocisk wyleciał poza ekran
            if (bullet->y < 0) {
                bullet->bullet_active = false;
            }


            for (*enemy_iter = 0; *enemy_iter < 5; *enemy_iter++) {
                if (bullet->x > opponents[*enemy_iter].x && bullet->x < opponents[*enemy_iter].x + 80 && bullet->y < opponents[*enemy_iter].y + 20) {
                    opponents[*enemy_iter].hp -= 1;
                    //lista  z przeciwnikami 
                    if (!opponents[*enemy_iter].hp) {
                        opponents[*enemy_iter].x = -100;
                        opponents[*enemy_iter].y = -100;
                    }

                    bullet->bullet_active = false;
                    bullet->x = -10;
                    bullet->y = -10;
                }
            }

            if (*enemy_move > 100 || *enemy_move < -100) {
                for (*enemy_iter = 0; *enemy_iter < 5; enemy_iter++) {
                    opponents[*enemy_iter].speed *= -1;
                }
            }

            for (enemy_iter = 0; enemy_iter < 5; enemy_iter++) {
                opponents[*enemy_iter].x += opponents[*enemy_iter].speed;
                if (opponents[*enemy_iter].hp) {
                    al_draw_bitmap(enemy_bitmap, opponents[*enemy_iter].x, opponents[*enemy_iter].y, 0);
                }
            }

            *enemy_move += opponents[0].speed;
            *enemy_move += opponents[1].speed;
            *enemy_move += opponents[2].speed;
            *enemy_move += opponents[3].speed;
            *enemy_move += opponents[4].speed;


            al_draw_bitmap(bitmap, ship->x, ship->y, 0);
            al_flip_display();
        }
    } while (1);
}
