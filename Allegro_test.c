#include <allegro5/allegro_font.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>
#include "opponents.h"
#include <time.h>

#define SCREEN_WIDTH 1280   
#define SCREEN_HEIGHT 780   
//#define BULLET_SIZE 10     // do bullet size

/*
typedef struct Enemy {
    float x;
    float y;
    float hp;
    float dmg;
    float speed;
} Enemy;
*/

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
void checkCollision(Bullet* bullet, Enemy* enemy);
void shipMove(Ship* ship, Bullet* bullet, ALLEGRO_KEYBOARD_STATE* keyboard_state, ALLEGRO_COLOR bg_color);
int checkEnemy(Enemy* opponents);
void endFrame(ALLEGRO_FONT* font, ALLEGRO_KEYBOARD_STATE* keyboard_state);
void checkEnemyBulletColision(Bullet* bullet_enemy, Ship* ship);

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
    ALLEGRO_FONT* font = al_create_builtin_font();

    // ŁADOWANIE GRAFIK REPREZENTUJĄCYCH STATKI KOSMICZNE
    ALLEGRO_BITMAP* bitmap = al_load_bitmap("C:/Users/PC/Desktop/space-invaders/player.png");
    ALLEGRO_BITMAP* enemy_bitmap = al_load_bitmap("C:/Users/PC/Desktop/space-invaders/ufo.png"); //C:\Users\PC\Desktop\space-invaders

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

    srand(time(NULL));

    // DEKLARACJA I INICJACJA DODATKOWYCH ZMIENNYCH
    int enemy_iter;                 // Iterator tablicy wrogów
    float enemy_move = 0;           // Zmienna pomocnicza wykorzystywana do ruchu przeciwników


    // Ekran startowy gry
    int start_window = 1;
    int gameLoop = 1;
    startFrame(&start_window, &keyboard_state, font, bg_color);


    // WYSTARTOWANIE TIMERA
    al_start_timer(timer);

    int shoot = 0;

    
    do {
        // SPRAWDZENIE
        if (al_get_timer_count(timer) > 0) {
            al_set_timer_count(timer, 0);
            al_clear_to_color(bg_color);

            // Przetwarzanie zdarzeń klawiatury i myszy

            shipMove(&ship, &bullet, &keyboard_state, bg_color);

            // Rysowanie pocisku
            drawBullet(&bullet, bullet_color);

            shoot = rand() % 5;
            if(!bullet_enemy.bullet_active){
                bullet_enemy.x = opponents[shoot].x;
                bullet_enemy.y = opponents[shoot].y;
                bullet_enemy.bullet_active = true;
            }

            drawBulletEnemy(&bullet_enemy, bullet_color_enemy);
            checkEnemyBulletColision(&bullet_enemy, &ship);

       
            if(checkEnemy(opponents)){
                checkCollision(&bullet, opponents);
            }
            

            if (enemy_move > 100 || enemy_move < -100) {
                for (enemy_iter = 0; enemy_iter < 5; enemy_iter++) {
                    opponents[enemy_iter].speed *= -1;
                }
            }

            for (enemy_iter = 0; enemy_iter < 5; enemy_iter++) {
                opponents[enemy_iter].x += opponents[enemy_iter].speed;
                if (opponents[enemy_iter].hp > 0) {
                    al_draw_bitmap(enemy_bitmap, opponents[enemy_iter].x, opponents[enemy_iter].y, 0);
                }
            }



            enemy_move += opponents[0].speed;
            enemy_move += opponents[1].speed;
            enemy_move += opponents[2].speed;
            enemy_move += opponents[3].speed;
            enemy_move += opponents[4].speed;

            if (checkEnemy(opponents) == 0 && level == 3) {
                opponents = initializeOpponents(1);
                gameLoop = 0;
                break;
            }
            else if (checkEnemy(opponents) == 0) {
                level++;
                opponents = initializeOpponents(level);
            }
            al_draw_bitmap(bitmap, ship.x, ship.y, 0);
            al_flip_display();
        }
    } while (gameLoop);

    endFrame(font, &keyboard_state);

    al_destroy_bitmap(bitmap);
    al_destroy_display(display);
    free(opponents);

    return 0;
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

void endFrame(ALLEGRO_FONT* font, ALLEGRO_KEYBOARD_STATE* keyboard_state) {
    while (1) {
        //puts("bambaryla");
        al_get_keyboard_state(keyboard_state);
        al_draw_filled_rectangle(SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 2 - 50, SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 + 100, al_map_rgb(220, 200, 220));
        al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 + 30, ALLEGRO_ALIGN_CENTER, "Koniec gry...");
        al_flip_display();
        if (al_key_down(keyboard_state, ALLEGRO_KEY_ENTER)) {
            puts("Koniec ;)");
            return;
        }
    }
}

int checkEnemy(Enemy* opponents) {
    int check = 0;
    for (int i = 0; i < 5; i++) check = opponents[i].hp > 0 ? ++check : check;
    return check;

    /*
    int i = 0;
    if (opponents[i].hp == 0 && opponents[i+1].hp == 0 && opponents[i+2].hp == 0 && opponents[i+3].hp == 0 && opponents[i+4].hp == 0) {
        return -1;
    }
    */
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

        al_draw_filled_rectangle(bullet_enemy->x, bullet_enemy->y, bullet_enemy->x + bullet_enemy->bullet_size, bullet_enemy->y + bullet_enemy->bullet_size, bullet_color_enemy);

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
    //printf("%d", );
    if (bullet_enemy->x < ship->x + 30 && bullet_enemy->x > ship->x && bullet_enemy->y < ship->y + 30 && bullet_enemy->y > ship->y) {
        printf("trafiony zostalem");
        // ship->hp--;
    }
}

void checkCollision(Bullet* bullet, Enemy* enemy) {
    int iter;
    for (iter = 0; iter < 5; iter++) {
        if (bullet->x > enemy[iter].x && bullet->x < enemy[iter].x + 80 && bullet->y < enemy[iter].y + 20 && bullet->y > enemy[iter].y) {
            enemy[iter].hp -= 1;
            //lista  z przeciwnikami
            if (!enemy[iter].hp) {
                enemy[iter].x = -100;
                enemy[iter].y = -100;
            }

            bullet->bullet_active = false;
            bullet->x = -10;
            bullet->y = -10;
        }
    }
}

void shipMove(Ship* ship, Bullet* bullet, ALLEGRO_KEYBOARD_STATE* keyboard_state, ALLEGRO_COLOR bg_color) {
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
            al_clear_to_color(bg_color);
            ship->x = SCREEN_WIDTH - 150;
        }
    }
    if (al_key_down(keyboard_state, ALLEGRO_KEY_SPACE) && !bullet->bullet_active) {
        puts("klikam spacje ");
        bullet->bullet_active = true;
        bullet->x = ship->x + 70;
        bullet->y = ship->y;
    }
}
