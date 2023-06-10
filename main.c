#include <allegro5/allegro_font.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>
#include "opponents.h"
#include <time.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>


#define SCREEN_WIDTH 1280   
#define SCREEN_HEIGHT 720
#define MAX_LEVEL 3
//#define BULLET_SIZE 10     // do bullet size

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
//void gameFrame(ALLEGRO_TIMER* timer, ALLEGRO_COLOR bg_color, ALLEGRO_KEYBOARD_STATE* keyboard_state, Ship* ship, Bullet* bullet, ALLEGRO_COLOR bullet_color, int* enemy_iter, Enemy* opponents, float* enemy_move, ALLEGRO_BITMAP* enemy_bitmap, ALLEGRO_BITMAP* bitmap);
void drawBullet(Bullet* bullet, ALLEGRO_COLOR bullet_color);
void drawBulletEnemy(Bullet* bullet_enemy, ALLEGRO_COLOR bullet_color_enemy);
void checkCollision(Bullet* bullet, Enemy* enemy, ALLEGRO_SAMPLE* sample, int* points);
void shipMove(Ship* ship, Bullet* bullet, ALLEGRO_KEYBOARD_STATE* keyboard_state, ALLEGRO_COLOR bg_color, ALLEGRO_SAMPLE* sample);
int checkEnemy(Enemy* opponents);
void endFrame(int* start_window, ALLEGRO_MOUSE_STATE* mouse_state, ALLEGRO_FONT* font, ALLEGRO_FONT* shadowFont, ALLEGRO_FONT* menuFont, ALLEGRO_BITMAP* bg_bitmap, int* points);
void checkEnemyBulletColision(Bullet* bullet_enemy, Ship* ship);
void myStartFrame(int* start_window, ALLEGRO_MOUSE_STATE* mouse_state, ALLEGRO_FONT* font, ALLEGRO_FONT* shadowFont, ALLEGRO_FONT* menuFont, ALLEGRO_BITMAP* bg_bitmap, ALLEGRO_SAMPLE* sample);

int main() {
    // INICJACJA GŁÓWNYCH ELEMENTÓW ALLEGRO 5
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_mouse();
    al_init_image_addon();
    al_init_ttf_addon();
    al_install_audio();
    al_init_acodec_addon();

    al_reserve_samples(3);

    // DEKLARACJA I INICJACJA WARTOŚCI STATKU KOSMICZNEGO KONTROLOWANEGO PRZEZ GRACZA
    Ship ship;
    ship.x = SCREEN_WIDTH / 2.0;
    ship.y = SCREEN_HEIGHT - 100;
    ship.vx = 0.5;
    ship.dmg = 1;
    ship.hp = 3;


    // DEKLARACJA I INICJACJA WARTOŚCI POCISKU WYSTRZELIWANEGO PRZEZ GRACZA
    Bullet bullet;
    bullet.x = -10;
    bullet.y = -10;
    bullet.vx = 0;
    bullet.vy = -1;
    bullet.bullet_size = 10;
    bullet.bullet_active = false;

    // DODAC METEORYTY :)
    Bullet bullet_enemy;
    bullet_enemy.x = 100;
    bullet_enemy.y = -10;
    bullet_enemy.vx = 0;
    bullet_enemy.vy = 0.5;
    bullet_enemy.bullet_size = 10;
    bullet_enemy.bullet_active = false;

    int level = 1;
    int points = 0;



    Enemy* opponents = initializeOpponents(level);

    // DEKLARACJA I INICJACJA OKNA GRY
    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    al_set_window_title(display, "Space Invaders");

    // DEKLARACJA I INICJACJA KOLORÓW UŻYWANYCH DO RYSOWANIA OBIEKTÓW
    ALLEGRO_COLOR bg_color = al_map_rgb(0, 0, 0);
    ALLEGRO_COLOR bullet_color = al_map_rgb(0, 255, 0);
    ALLEGRO_COLOR bullet_color_enemy = al_map_rgb(255, 0, 0);


    // DEKLARACJA 
    ALLEGRO_KEYBOARD_STATE keyboard_state;
    ALLEGRO_MOUSE_STATE mouse_state;
    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_FONT* vv = al_load_ttf_font("C:/Users/kamil/desktop/Space-Invaders/fonts/vermin_vibes_1989/Vermin Vibes 1989.ttf", 140, NULL);
    ALLEGRO_FONT* vvMenu = al_load_ttf_font("C:/Users/kamil/desktop/Space-Invaders/fonts/vermin_vibes_1989/Vermin Vibes 1989.ttf", 80, NULL);
    ALLEGRO_FONT* vvPoints = al_load_ttf_font("C:/Users/kamil/desktop/Space-Invaders/fonts/vermin_vibes_1989/Vermin Vibes 1989.ttf", 30, NULL);
    ALLEGRO_FONT* vvLevel = al_load_ttf_font("C:/Users/kamil/desktop/Space-Invaders/fonts/vermin_vibes_1989/Vermin Vibes 1989.ttf", 40, NULL);
    ALLEGRO_SAMPLE* menu_sample = al_load_sample("C:/Users/kamil/desktop/Space-Invaders/sounds/start_sound.ogg");;
    ALLEGRO_SAMPLE* shoot_sample = al_load_sample("C:/Users/kamil/desktop/Space-Invaders/sounds/shoot.ogg");
    ALLEGRO_SAMPLE* dead_sample = al_load_sample("C:/Users/kamil/desktop/Space-Invaders/sounds/blast.ogg");

    // ŁADOWANIE GRAFIK REPREZENTUJĄCYCH STATKI KOSMICZNE
    ALLEGRO_BITMAP* bitmap = al_load_bitmap("C:/Users/kamil/desktop/Space-Invaders/graphics/player.png");
    ALLEGRO_BITMAP* enemy_bitmap_1 = al_load_bitmap("C:/Users/kamil/desktop/Space-Invaders/graphics/my_ufo.png");
    ALLEGRO_BITMAP* enemy_bitmap_2 = al_load_bitmap("C:/Users/kamil/desktop/Space-Invaders/graphics/ufo_2.png"); 
    ALLEGRO_BITMAP* enemy_bitmap_3 = al_load_bitmap("C:/Users/kamil/desktop/Space-Invaders/graphics/ufo.png"); 
    ALLEGRO_BITMAP* heart_bitmap = al_load_bitmap("C:/Users/kamil/desktop/Space-Invaders/graphics/heart.png"); 

    ALLEGRO_BITMAP* background_bitmap = al_load_bitmap("C:/Users/kamil/desktop/Space-Invaders/graphics/space-background.png");
    ALLEGRO_BITMAP* game_background_bitmap = al_load_bitmap("C:/Users/kamil/desktop/Space-Invaders/graphics/game-background.png");

    // DEKLARACJA I INICJACJA "STOPERA"
    ALLEGRO_TIMER* timer = NULL;
    timer = al_create_timer(0.001); // 

    /*
    if (!mcFont) {
        printf("Nie udalo sie zainicjowac czcionki!");
        return -1;
    }*/

    // SPRAWDZENIE POPRAWNOSCI ZMIENNYCH
    if (!timer) {
        printf("Nie udalo zainicjowac timera\n");
        return -1;
    }

    if (!bitmap) {
        printf("Nie udalo sie zaladowac obrazka statku kosmicznego gracza!\n");
        return -1;
    }

    if (!enemy_bitmap_1) {
        printf("Nie udalo sie zaladowac obrazka statku kosmicznego przeciwnikow!\n");
        return -1;
    }

    srand(time(NULL));

    // DEKLARACJA I INICJACJA DODATKOWYCH ZMIENNYCH
    int enemy_iter;                 // Iterator tablicy wrogów
    float enemy_move = 0;           // Zmienna pomocnicza wykorzystywana do ruchu przeciwników


    // Ekran startowy gry
    int start_window = 1;
    int gameLoop = 1;
    //startFrame(&start_window, &keyboard_state, font, bg_color);
    myStartFrame(&start_window, &mouse_state, vv, vv, vvMenu, background_bitmap, menu_sample);


    // WYSTARTOWANIE TIMERA
    al_start_timer(timer);

    int shoot = 0;


    do {
        // SPRAWDZENIE
        if (al_get_timer_count(timer) > 0) {
            al_set_timer_count(timer, 0);
            //al_clear_to_color(bg_color);
            al_draw_bitmap(background_bitmap, 0, 0, NULL);
            // Przetwarzanie zdarzeń klawiatury i myszy

            shipMove(&ship, &bullet, &keyboard_state, bg_color, shoot_sample);

            // Rysowanie pocisku
            drawBullet(&bullet, bullet_color);

            shoot = rand() % 10;
            if (!bullet_enemy.bullet_active) {
                bullet_enemy.x = opponents[shoot].x;
                bullet_enemy.y = opponents[shoot].y;
                bullet_enemy.bullet_active = true;
            }

            drawBulletEnemy(&bullet_enemy, bullet_color_enemy);
            checkEnemyBulletColision(&bullet_enemy, &ship);

            for (int hearts = 0; hearts < ship.hp; hearts++)
            {
                al_draw_bitmap(heart_bitmap, 20 + 65 * hearts, 20, 0);
            }
            
            al_draw_textf(vvPoints, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, 20, ALLEGRO_ALIGN_CENTER, "Points: %d", points);
            al_draw_textf(vvLevel, al_map_rgb(255, 255, 255), SCREEN_WIDTH - 10, 20, ALLEGRO_ALIGN_RIGHT, "LEVEL %d", level);


            if (checkEnemy(opponents)) {
                checkCollision(&bullet, opponents, dead_sample, &points);
            }


            if (enemy_move > 200 || enemy_move < -200) {
                for (enemy_iter = 0; enemy_iter < 10; enemy_iter++) {
                    opponents[enemy_iter].speed *= -1;
                }
            }

            for (enemy_iter = 0; enemy_iter < 10; enemy_iter++) {
                opponents[enemy_iter].x += opponents[enemy_iter].speed;
                if (opponents[enemy_iter].hp > 0) {
                    switch (opponents[enemy_iter].type) {
                    case 1:
                        al_draw_bitmap(enemy_bitmap_1, opponents[enemy_iter].x, opponents[enemy_iter].y, 0);
                        break;
                    case 2:
                        al_draw_bitmap(enemy_bitmap_2, opponents[enemy_iter].x, opponents[enemy_iter].y, 0);
                        break;
                    case 3:
                        al_draw_bitmap(enemy_bitmap_3, opponents[enemy_iter].x, opponents[enemy_iter].y, 0);
                        break;
                    }
                   
                }
            }


           

            enemy_move += opponents[0].speed * 10;

            /*
            enemy_move += opponents[0].speed;
            enemy_move += opponents[1].speed;
            enemy_move += opponents[2].speed;
            enemy_move += opponents[3].speed;
            enemy_move += opponents[4].speed;          
            enemy_move += opponents[5].speed;
            enemy_move += opponents[6].speed;
            enemy_move += opponents[7].speed;
            enemy_move += opponents[8].speed;
            enemy_move += opponents[9].speed;*/

            if (checkEnemy(opponents) == 0 && level == MAX_LEVEL) {
                opponents = initializeOpponents(1);
                gameLoop = 0;
                break;
            }
            else if (checkEnemy(opponents) == 0) {
                level++;
                opponents = initializeOpponents(level);
            }

            if (ship.hp > 0) {
                al_draw_bitmap(bitmap, ship.x, ship.y, 0);
            }
            else {
                gameLoop = 0;
                break;
            }
            
            al_flip_display();
        }
    } while (gameLoop);

    endFrame(&start_window, &mouse_state, vv, vv, vvMenu, background_bitmap, &points);

	
    al_destroy_bitmap(bitmap);
    al_destroy_display(display);
    free(opponents);

    return 0;
}

void startFrame(int* start_window, ALLEGRO_KEYBOARD_STATE* keyboard_state, ALLEGRO_FONT* font, ALLEGRO_COLOR bg_color) {
    while (*start_window) {  // prostokat, jeżeli (MyszkaX > ButtonX) Oraz (MyszkaY > ButtonY) Oraz (MyszkaX < ButtonX+ButtonWidth) Oraz (MyszkaY < ButtonY+ButtonHeight) Oraz (LewyPrzyciskMyszkiNacisniety) wtedy ...
        al_get_keyboard_state(keyboard_state);
        al_clear_to_color(bg_color); //ustawia tlo
        al_draw_filled_rectangle(SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 2 - 50, SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 + 100, al_map_rgb(220, 200, 220));
        al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 + 30, ALLEGRO_ALIGN_CENTER, "Nacisnij ENTER by rozpoczac gre...");
        if (al_key_down(keyboard_state, ALLEGRO_KEY_ENTER)) {
            *start_window = 0;
        }
        al_flip_display();  //odswieza ekran
    }
}

void myStartFrame(int* start_window, ALLEGRO_MOUSE_STATE* mouse_state, ALLEGRO_FONT* font, ALLEGRO_FONT* shadowFont, ALLEGRO_FONT* menuFont, ALLEGRO_BITMAP* bg_bitmap, ALLEGRO_SAMPLE* sample) {
    al_play_sample(sample, 0.2, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
    while (*start_window) {
        
        al_play_sample(sample, 0.01, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
        al_get_mouse_state(mouse_state);
        al_draw_bitmap(bg_bitmap, 0, 0, NULL);
        al_draw_filled_rectangle(SCREEN_WIDTH / 2 - 150, 300, SCREEN_WIDTH / 2 + 150, 400, al_map_rgba(40, 0, 100, 0));
        al_draw_text(shadowFont, al_map_rgb(100, 0, 255), SCREEN_WIDTH / 2 + 5, 25, ALLEGRO_ALIGN_CENTER, "SPACE  INVADERS");
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, 20, ALLEGRO_ALIGN_CENTER, "SPACE  INVADERS");
        al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, 320, ALLEGRO_ALIGN_CENTER, "START");
       
        if (mouse_state->buttons & 1 && mouse_state->x > SCREEN_WIDTH / 2 - 150 && mouse_state->x < SCREEN_WIDTH / 2 + 150 && mouse_state->y > 300 && mouse_state->y < 400) {
            *start_window = 0;
        }
        al_flip_display();  
    }

    al_stop_sample(sample);
    al_destroy_sample(sample);

    return;
}

void endFrame(int* start_window, ALLEGRO_MOUSE_STATE* mouse_state, ALLEGRO_FONT* font, ALLEGRO_FONT* shadowFont, ALLEGRO_FONT* menuFont, ALLEGRO_BITMAP* bg_bitmap, int* points) {
    while (1) {
        al_get_mouse_state(mouse_state);
        al_draw_bitmap(bg_bitmap, 0, 0, NULL);
        al_draw_filled_rectangle(SCREEN_WIDTH / 2 - 150, 320, SCREEN_WIDTH / 2 + 150, 420, al_map_rgba(40, 0, 100, 0));
        al_draw_text(shadowFont, al_map_rgb(100, 0, 255), SCREEN_WIDTH / 2 + 5, 25, ALLEGRO_ALIGN_CENTER, "SPACE  INVADERS");
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, 20, ALLEGRO_ALIGN_CENTER, "SPACE  INVADERS");
        al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, 340, ALLEGRO_ALIGN_CENTER, "KONIEC");
        al_draw_textf(menuFont, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, 200, ALLEGRO_ALIGN_CENTER, "Points: %d", *points);
        al_flip_display();

        if (mouse_state->buttons & 1 && mouse_state->x > SCREEN_WIDTH / 2 - 150 && mouse_state->x < SCREEN_WIDTH / 2 + 150 && mouse_state->y > 300 && mouse_state->y < 400) {
            return;
        }
        
    }
    
}

int checkEnemy(Enemy* opponents) {
    int check = 0;
    for (int i = 0; i < 10; i++) check = opponents[i].hp > 0 ? ++check : check;
    return check;
}

void drawBullet(Bullet* bullet, ALLEGRO_COLOR bullet_color) {
    // Rysowanie pocisku
    if (bullet->bullet_active) {

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
}

void drawBulletEnemy(Bullet* bullet_enemy, ALLEGRO_COLOR bullet_color_enemy) {
    // Rysowanie pocisku
    if (bullet_enemy->bullet_active) {

        al_draw_filled_rectangle(bullet_enemy->x + 30, bullet_enemy->y + 20, bullet_enemy->x + bullet_enemy->bullet_size + 30, bullet_enemy->y + bullet_enemy->bullet_size + 20, bullet_color_enemy);

        // Aktualizacja pozycji pocisku przeciwnika
        bullet_enemy->bullet_active = true;
        if (bullet_enemy->bullet_active) {
            bullet_enemy->y += bullet_enemy->vy;
        }
    }

    // Sprawdzenie, czy pocisk przeciwnika wyleciał poza ekran
    if (bullet_enemy->y > SCREEN_HEIGHT) {
        bullet_enemy->bullet_active = false;
    }
}

void checkEnemyBulletColision(Bullet* bullet_enemy, Ship* ship) {

    if (bullet_enemy->x < ship->x + 72 && bullet_enemy->x > ship->x  && bullet_enemy->y > ship->y + 45) {
        printf("trafiony zostalem");
        ship->hp--;
        bullet_enemy->bullet_active = 0;
    }
}

void checkCollision(Bullet* bullet, Enemy* enemy, ALLEGRO_SAMPLE* sample, int* points) {
    int iter;
    for (iter = 0; iter < 10; iter++) {
        if (bullet->x > enemy[iter].x && bullet->x < enemy[iter].x + 80 && bullet->y < enemy[iter].y + 20 && bullet->y > enemy[iter].y) {
            enemy[iter].hp -= 1;
            //lista  z przeciwnikami
            if (!enemy[iter].hp) {
                al_play_sample(sample, 0.5, 0, 1.5, ALLEGRO_PLAYMODE_ONCE, NULL);
                enemy[iter].x = -100;
                enemy[iter].y = -100;
                *points += 10 * enemy->type;
            }

            printf("%d\n", *points);
            bullet->bullet_active = false;
            bullet->x = -10;
            bullet->y = -10;
        }
    }
}

void shipMove(Ship* ship, Bullet* bullet, ALLEGRO_KEYBOARD_STATE* keyboard_state, ALLEGRO_COLOR bg_color, ALLEGRO_SAMPLE* sample) {
    al_get_keyboard_state(keyboard_state);
    if (al_key_down(keyboard_state, ALLEGRO_KEY_LEFT)) {
        ship->x -= ship->vx;
        if (ship->x < 0) {
            ship->x = 0;
        }
    }

    if (al_key_down(keyboard_state, ALLEGRO_KEY_RIGHT)) {
        ship->x += ship->vx;
        if (ship->x > SCREEN_WIDTH - 150) {
            ship->x = SCREEN_WIDTH - 150;
        }
    }
    if (al_key_down(keyboard_state, ALLEGRO_KEY_SPACE) && !bullet->bullet_active) {
        al_play_sample(sample, 0.1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        puts("klikam spacje ");
        bullet->bullet_active = true;
        bullet->x = ship->x + 70;
        bullet->y = ship->y;
    }
}
