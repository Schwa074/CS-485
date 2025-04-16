#ifndef LEVEL2MANAGER_H
#define LEVEL2MANAGER_H

#include "game.h"
#include "raytmx.h"

struct ResetHallwayCollision {
    Vector2 spawnPosition;
    Rectangle rect;
};

extern ResetHallwayCollision hiddenCollisions[];
void CreateHiddenCollisions();
void CheckHiddenCollsions(Player* player);
void HandleHiddenCollision(Player* player);

#endif // LEVEL2MANAGER_H