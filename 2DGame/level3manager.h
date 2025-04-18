#ifndef LEVEL3MANAGER_H
#define LEVEL3MANAGER_H

#include "game.h"
#include "raytmx.h"


struct ResetEncounters {
    Vector2 spawnPosition;
    Rectangle rect;
};

extern ResetEncounters hiddenCollisions[];
void CreateEncounterCollision();
void CheckEncounterCollision(Player* player);
void HandleEncounterCollision(Player* player);

#endif // LEVEL3MANAGER_H