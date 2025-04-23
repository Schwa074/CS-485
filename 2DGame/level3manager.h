#ifndef LEVEL3MANAGER_H
#define LEVEL3MANAGER_H

#include "game.h"

// Constants for invisible collision zones
const int INVISIBLE_ZONE_WIDTH = 400;
const int INVISIBLE_ZONE_HEIGHT = 400;

// Predefined spawn locations for the collision zones
const Vector2 INVISIBLE_ZONE_SPAWN_LOCATIONS[] = {
    {280, 2660},  // Spawn 1
    {928, 1540},  // Spawn 2
    {2140, 1670}, // Spawn 3
    {2000, 320},  // Spawn 4
    {2850, 1500}, // Spawn 5
    {2000, 2660}  // Spawn 6
};

// Struct for invisible collision zones
struct InvisibleCollisionZone {
    Rectangle rect;
    bool triggered; // To ensure the zone only triggers once
};

// Function declarations
void CreateInvisibleZones();
void CheckInvisibleZones(Player* player, std::vector<Enemy>& ghosts, Texture2D redGhostSprite, Texture2D blueGhostSprite);
void SpawnRandomGhosts(std::vector<Enemy>& ghosts, Texture2D redGhostSprite, Texture2D blueGhostSprite, Rectangle zone);

#endif // LEVEL3MANAGER_H