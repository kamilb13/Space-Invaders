#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct Enemy {
    float x;
    float y;
    float hp;
    float dmg;
    float speed;
    short type;
} Enemy;

Enemy* initializeOpponents(int lvl);
