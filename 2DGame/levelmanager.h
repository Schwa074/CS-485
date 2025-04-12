#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "game.h"
#include "raytmx.h"

struct LevelTransition {
    int fromLevel;
    int toLevel;
    const char* mapPath;
    Vector2 spawnPosition;
};

extern Vector2 entranceLocation[];
extern Vector2 exitLocation[];
extern const char* mapName[];
extern int lastLevel2ExitUsed; // Index of where returnToLevel2From3 point to
extern Vector2 returnToLevel2From3[];
bool HandleLevelTransition(int fromLevel, int toLevel, int transitionIndex, TmxMap*& map, Player& player);

#endif // LEVELMANAGER_H