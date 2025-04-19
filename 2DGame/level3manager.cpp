#include "level3manager.h"
#include <cstdlib> // For std::rand()

// Array to hold the invisible collision zones
InvisibleCollisionZone invisibleZones[6];

// Function to create the invisible collision zones
void CreateInvisibleZones() {
    for (int i = 0; i < 6; i++) {
        invisibleZones[i] = {
            .rect = {INVISIBLE_ZONE_SPAWN_LOCATIONS[i].x, INVISIBLE_ZONE_SPAWN_LOCATIONS[i].y, INVISIBLE_ZONE_WIDTH, INVISIBLE_ZONE_HEIGHT},
            .triggered = false
        };
    }
}

// Function to check if the player collides with any invisible zone
void CheckInvisibleZones(Player* player, std::vector<Enemy>& ghosts, Texture2D redGhostSprite, Texture2D blueGhostSprite) {
    for (int i = 0; i < 6; i++) {
        if (!invisibleZones[i].triggered && CheckCollisionRecs(player->rect, invisibleZones[i].rect)) {
            invisibleZones[i].triggered = true; // Mark the zone as triggered
            SpawnRandomGhosts(ghosts, redGhostSprite, blueGhostSprite, invisibleZones[i].rect);
        }
    }
}

// Function to spawn a random set of ghosts in the triggered zone
void SpawnRandomGhosts(std::vector<Enemy>& ghosts, Texture2D redGhostSprite, Texture2D blueGhostSprite, Rectangle zone) {
    int spawnCount = std::rand() % 3 + 1; // Randomly choose 1 to 3 ghosts

    for (int i = 0; i < spawnCount; i++) {
        Vector2 spawnPos = {
            zone.x + std::rand() % (int)zone.width,
            zone.y + std::rand() % (int)zone.height
        };

        Enemy newGhost;
        if (std::rand() % 2 == 0) {
            spawnRedGhost(&newGhost, redGhostSprite, spawnPos); // Spawn a red ghost
        } else {
            spawnBlueGhost(&newGhost, blueGhostSprite, spawnPos); // Spawn a blue ghost
        }
        ghosts.push_back(newGhost); // Add the new ghost to the vector
    }
}