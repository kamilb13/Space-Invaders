#include <allegro5/allegro_font.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280   
#define SCREEN_HEIGHT 780   
#define BALL_SPEED 0.05     
#define PADDLE_SPEED 0.5
#define BULLET_SIZE 10

typedef struct Enemy {
    float x;
    float y;
    float hp;   
    float dmg;
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
    bool bullet_active;
} Bullet;

int main(){
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();
    al_install_mouse();
    Ship ship;
    Bullet bullet;

    bullet.x = SCREEN_WIDTH / 2.0;
    bullet.y = SCREEN_HEIGHT / 2.0;
    bullet.vx = BALL_SPEED;
    bullet.vy = BALL_SPEED;
    ship.x = 0;
    ship.y = SCREEN_HEIGHT - 100;
    bullet.x = 0;
    bullet.y = 0;
    bullet.vx = 0;
    bullet.vy = -1;
    bullet.bullet_active = false;



    Enemy enemy;
    enemy.x = 150;
    enemy.y = 300;
    enemy.dmg = 1;
    enemy.hp = 2;

    
    Enemy* opponents = (Enemy*)calloc(5, sizeof(Enemy));   //Tablica z przeciwnikami
    Enemy* enemy_p = (Enemy*)malloc(sizeof(Enemy));
    Enemy* null_enemy = NULL;
    opponents[0].x = 450;
    opponents[0].y = 300;
    opponents[0].dmg = 1;
    opponents[0].hp = 2;

    opponents[1].x = 600;
    opponents[1].y = 400;
    opponents[1].dmg = 1;
    opponents[1].hp = 2;

    opponents[2].x = 150;
    opponents[2].y = 300;
    opponents[2].dmg = 1;
    opponents[2].hp = 7;

    opponents[3].x = 23;
    opponents[3].y = 429;
    opponents[3].dmg = 1;
    opponents[3].hp = 2;

    opponents[4].x = 500;
    opponents[4].y = 100;
    opponents[4].dmg = 1;
    opponents[4].hp = 2;

	enemy_p->x = 450;
	enemy_p->y = 300;
	enemy_p->dmg = 1;
	enemy_p->hp = 2;

    //float ball_x = SCREEN_WIDTH / 2.0;
    //float ball_y = SCREEN_HEIGHT / 2.0;
    //float ball_vx = BALL_SPEED;
    //float ball_vy = BALL_SPEED;
    //float paddle_x = 0;
    //float paddle_y = SCREEN_HEIGHT - 100;
    //float bullet_x = 0;
    //float bullet_y = 0;
    //float bullet_vx = 0;
    //float bullet_vy = -1;
    //bool bullet_active = false;



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
    ALLEGRO_BITMAP* enemy_bitmap = al_load_bitmap("C:/Users/kamil/Desktop/ufo.png");
    if (!bitmap){
        printf("Nie udalo sie zaladowac obrazka!\n");
        return -1;
    }

	int enemy_iter; // ITERATOR TABLICY WROGOW

    //Ekran startowy gry 
    int kiszka = 1;
    while (kiszka) {  // prostokat, jeżeli (MyszkaX > ButtonX) Oraz (MyszkaY > ButtonY) Oraz (MyszkaX < ButtonX+ButtonWidth) Oraz (MyszkaY < ButtonY+ButtonHeight) Oraz (LewyPrzyciskMyszkiNacisniety) wtedy ...
        al_get_keyboard_state(&keyboard_state);
        al_clear_to_color(bg_color); //ustawia tlo
        al_draw_filled_rectangle(SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 2 - 50, SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 + 100, al_map_rgb(220, 200, 220));
        al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 + 30, ALLEGRO_ALIGN_CENTER, "Nacisnij ENTER by rozpoczac gre...");
        if (al_key_down(&keyboard_state, ALLEGRO_KEY_ENTER)) {
            kiszka = 0;
        }

        al_flip_display();  //odswieza ekran
    }
	
    while (1){

        al_clear_to_color(bg_color);

        // Przetwarzanie zdarzeń klawiatury i myszy
        al_get_keyboard_state(&keyboard_state);
        //al_get_mouse_state(&mouse_state);
        if (al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT)) {
            //paddle_x -= PADDLE_SPEED;
            ship.x -= PADDLE_SPEED;
            if (ship.x < 0) {
                ship.x = 0;
            }
        }

        if (al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT)){
            //al_clear_to_color(bg_color);
            ship.x += PADDLE_SPEED;
            if (ship.x > SCREEN_WIDTH - 150) {
                al_clear_to_color(bg_color);
                ship.x = SCREEN_WIDTH - 150;
            }
        }
        if (al_key_down(&keyboard_state, ALLEGRO_KEY_SPACE) && !bullet.bullet_active){
            puts("klikam spacje ");
            bullet.bullet_active = true;
            bullet.x = ship.x + 70;
            bullet.y = ship.y;

        }
		

        // Rysowanie pocisku
        if (bullet.bullet_active) {

            al_draw_filled_rectangle(bullet.x, bullet.y, bullet.x + BULLET_SIZE, bullet.y + BULLET_SIZE, bullet_color);
            if (bullet.y == SCREEN_HEIGHT) {
                printf("kiszka");
            }
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

        if (bullet.x > enemy.x && bullet.x < enemy.x + 80 && bullet.y < enemy.y + 20) {
            enemy.hp -= 1;
            bullet.bullet_active = false;
            bullet.x = -1;
            bullet.y = -1;
        }

		if (bullet.x > enemy_p->x && bullet.x < enemy_p->x + 80 && bullet.y < enemy_p->y + 20) { // CUdenko dynamiczne
            free(enemy_p);
            //enemy_p = NULL;
			bullet.bullet_active = false;
            bullet.x = -10;
            bullet.y = -10;
		}

        if (enemy.hp > 0) {     // słabe, do wyrzucenia 
            al_draw_bitmap(enemy_bitmap, enemy.x, enemy.y, 0);
        }
        else {
            enemy.x = -10;
            enemy.y = -10;
        }
            

        if(enemy_p) {
			al_draw_bitmap(enemy_bitmap, enemy_p->x, enemy_p->y, 0);
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
        
        for (enemy_iter = 0; enemy_iter < 5 ; enemy_iter++) {
			if (opponents[enemy_iter].hp) {
				al_draw_bitmap(enemy_bitmap, opponents[enemy_iter].x, opponents[enemy_iter].y, 0);
			}
        }
        
        
        al_draw_bitmap(bitmap, ship.x, ship.y, 0);
        al_flip_display();
    }
    
    al_destroy_bitmap(bitmap);
    al_destroy_display(display);
    free(opponents);
}
