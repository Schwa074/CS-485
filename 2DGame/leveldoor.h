#ifndef LEVELDOOR_H
#define LEVELDOOR_H

#include "leveldoor.h"

struct LevelDoor {
    Rectangle rect;
    int toLevel;
};

void createLevelDoor(LevelDoor *door, Rectangle pos);
bool checkLevelDoorCollision(LevelDoor *door, Player *player);

#endif // LEVELDOOR_H