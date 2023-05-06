#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct Enemy {
    float x;
    float y;
    float hp;
    float dmg;
    float speed;
} Enemy;

void initializeOpponents(Enemy* opponents, Enemy* opponents1, Enemy* opponents2);