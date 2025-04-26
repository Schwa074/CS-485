#include "level3manager.h"
#include <cstdlib> // For std::rand()

// Array to hold the invisible collision zones
InvisibleCollisionZone invisibleZones[6];
std::vector<Enemy> ghosts;

// Function to create the invisible collision zones
void CreateInvisibleZones() {
    for (int i = 0; i < 6; i++) {
        invisibleZones[i] = {
            .rect = {INVISIBLE_ZONE_SPAWN_LOCATIONS[i].x, INVISIBLE_ZONE_SPAWN_LOCATIONS[i].y, INVISIBLE_ZONE_WIDTH, INVISIBLE_ZONE_HEIGHT},
            .triggered = false
        };

        // Debug log for each invisible zone
        TraceLog(LOG_DEBUG, "Invisible Zone %d: x=%f, y=%f, width=%f, height=%f",
            i, invisibleZones[i].rect.x, invisibleZones[i].rect.y,
            invisibleZones[i].rect.width, invisibleZones[i].rect.height);
    }
}

// Function to check if the player collides with any invisible zone
void CheckInvisibleZones(Player* player, std::vector<Enemy>& ghosts, Texture2D redGhostSprite, Texture2D blueGhostSprite) {
    for (int i = 0; i < 6; i++) {
        if (!invisibleZones[i].triggered && CheckCollisionRecs(player->rect, invisibleZones[i].rect)) {
            TraceLog(LOG_DEBUG, "Player collided with invisible zone %d at x=%f, y=%f", i, invisibleZones[i].rect.x, invisibleZones[i].rect.y);
            invisibleZones[i].triggered = true; // Mark the zone as triggered
            SpawnRandomGhosts(ghosts, redGhostSprite, blueGhostSprite, invisibleZones[i].rect);
        }
    }
}

// Function to spawn a random set of ghosts in the triggered zone
void SpawnRandomGhosts(std::vector<Enemy>& ghosts, Texture2D redGhostSprite, Texture2D blueGhostSprite, Rectangle zone) {
    // Log the function call and the zone details
    TraceLog(LOG_DEBUG, "SpawnRandomGhosts called. Zone: x=%f, y=%f, width=%f, height=%f",
             zone.x, zone.y, zone.width, zone.height);

    // Randomly choose 1 to 3 ghosts to spawn
    int spawnCount = std::rand() % 3 + 1;
    TraceLog(LOG_DEBUG, "Number of ghosts to spawn: %d", spawnCount);

    for (int i = 0; i < spawnCount; i++) {
        // Calculate a random spawn position within the zone
        Vector2 spawnPos = {
            zone.x + std::rand() % (int)zone.width,
            zone.y + std::rand() % (int)zone.height
        };
        TraceLog(LOG_DEBUG, "Ghost %d spawn position: x=%f, y=%f", i, spawnPos.x, spawnPos.y);

        // Create a new ghost
        Enemy newGhost;
        if (std::rand() % 2 == 0) {
            spawnRedGhost(&newGhost, redGhostSprite, spawnPos); // Spawn a red ghost
            TraceLog(LOG_DEBUG, "Spawned a red ghost at x=%f, y=%f", spawnPos.x, spawnPos.y);
        } else {
            spawnBlueGhost(&newGhost, blueGhostSprite, spawnPos); // Spawn a blue ghost
            TraceLog(LOG_DEBUG, "Spawned a blue ghost at x=%f, y=%f", spawnPos.x, spawnPos.y);
        }

        // Add the new ghost to the vector
        ghosts.push_back(newGhost);
        TraceLog(LOG_DEBUG, "Ghost added to vector. Total ghosts: %d", ghosts.size());
    }
}