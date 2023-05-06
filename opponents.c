#include <stdio.h>
#include <stdlib.h>
#include "opponents.h"

void initializeOpponents(Enemy* opponents, Enemy* opponents1, Enemy* opponents2){
    
    opponents1 = (Enemy*)calloc(5, sizeof(Enemy));
    opponents2 = (Enemy*)calloc(5, sizeof(Enemy));

    // INICJACJA WARTOŒCI KA¯DEGO Z PRZECIWNIKÓW
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

    // INICJACJA WARTOŒCI KA¯DEGO Z PRZECIWNIKÓW
    opponents1[0].x = 450;
    opponents1[0].y = 300;
    opponents1[0].dmg = 1;
    opponents1[0].hp = 1;
    opponents1[0].speed = 0.04;

    opponents1[1].x = 600;
    opponents1[1].y = 400;
    opponents1[1].dmg = 1;
    opponents1[1].hp = 1;
    opponents1[1].speed = 0.04;

    opponents1[2].x = 150;
    opponents1[2].y = 300;
    opponents1[2].dmg = 1;
    opponents1[2].hp = 1;
    opponents1[2].speed = 0.04;

    opponents1[3].x = 23;
    opponents1[3].y = 429;
    opponents1[3].dmg = 1;
    opponents1[3].hp = 1;
    opponents1[3].speed = 0.04;

    opponents1[4].x = 500;
    opponents1[4].y = 100;
    opponents1[4].dmg = 1;
    opponents1[4].hp = 1;
    opponents2[4].speed = 0.04;

    // INICJACJA WARTOŒCI KA¯DEGO Z PRZECIWNIKÓW
    opponents2[0].x = 450;
    opponents2[0].y = 300;
    opponents2[0].dmg = 1;
    opponents2[0].hp = 1;
    opponents2[0].speed = 0.04;

    opponents2[1].x = 600;
    opponents2[1].y = 400;
    opponents2[1].dmg = 1;
    opponents2[1].hp = 1;
    opponents2[1].speed = 0.04;

    opponents2[2].x = 150;
    opponents2[2].y = 300;
    opponents2[2].dmg = 1;
    opponents2[2].hp = 1;
    opponents2[2].speed = 0.04;

    opponents2[3].x = 23;
    opponents2[3].y = 429;
    opponents2[3].dmg = 1;
    opponents2[3].hp = 1;
    opponents2[3].speed = 0.04;

    opponents2[4].x = 500;
    opponents2[4].y = 100;
    opponents2[4].dmg = 1;
    opponents2[4].hp = 1;
    opponents2[4].speed = 0.04;
}


