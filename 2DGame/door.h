#ifndef DOOR_H
#define DOOR_H

#include "game.h"

struct Door {
    Rectangle rect;
    Texture2D sprite;
    bool isOpen;
};

void createDoor(Door *door, Texture2D doorSprite, Vector2 spawnPos);
void drawDoor(const Door *door);
void checkDoorCollision(Door *door, Player *player, Sound doorSound);

#endif // DOOR_H