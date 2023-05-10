#include <stdio.h>
#include <stdlib.h>
#include "opponents.h"

Enemy* initializeOpponents(int lvl) {

    Enemy* opponents = NULL;

    if(lvl == 1){
        opponents = (Enemy*)calloc(5, sizeof(Enemy));
        opponents[0].x = 450;
        opponents[0].y = 300;
        opponents[0].dmg = 1;
        opponents[0].hp = 1;
        opponents[0].speed = 0.04;

        opponents[1].x = 600;
        opponents[1].y = 400;
        opponents[1].dmg = 1;
        opponents[1].hp = 1;
        opponents[1].speed = 0.04;

        opponents[2].x = 150;
        opponents[2].y = 300;
        opponents[2].dmg = 1;
        opponents[2].hp = 1;
        opponents[2].speed = 0.04;
    
        opponents[3].x = 23;
        opponents[3].y = 429;
        opponents[3].dmg = 1;
        opponents[3].hp = 1;
        opponents[3].speed = 0.04;

        opponents[4].x = 500;
        opponents[4].y = 100;
        opponents[4].dmg = 1;
        opponents[4].hp = 1;
        opponents[4].speed = 0.04;
    }

    if (lvl == 2) {
        opponents = (Enemy*)calloc(5, sizeof(Enemy));
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
        opponents[2].hp = 2;
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
    }

    if (lvl == 3) {
        opponents = (Enemy*)calloc(5, sizeof(Enemy));
        opponents[0].x = 450;
        opponents[0].y = 300;
        opponents[0].dmg = 1;
        opponents[0].hp = 3;
        opponents[0].speed = 0.04;

        opponents[1].x = 600;
        opponents[1].y = 400;
        opponents[1].dmg = 1;
        opponents[1].hp = 3;
        opponents[1].speed = 0.04;

        opponents[2].x = 150;
        opponents[2].y = 300;
        opponents[2].dmg = 1;
        opponents[2].hp = 3;
        opponents[2].speed = 0.04;

        opponents[3].x = 23;
        opponents[3].y = 429;
        opponents[3].dmg = 1;
        opponents[3].hp = 3;
        opponents[3].speed = 0.04;

        opponents[4].x = 500;
        opponents[4].y = 100;
        opponents[4].dmg = 1;
        opponents[4].hp = 3;
        opponents[4].speed = 0.04;
    }

    return opponents;
}

