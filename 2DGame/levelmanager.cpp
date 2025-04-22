#include "game.h"

Vector2 entranceLocation[] = {
    { .x = 722.5f, .y = 126.0f }, // Start position
    { .x = 226.0f, .y = 1488.0f }, // Level 2 Entrance
    { .x = 290.0f, .y = 240.0f }, // Level 3 Entrance
};

Vector2 exitLocation[] = {
    { .x = 2658.0f, .y = 400.0f }, // Level 1 exit
    { .x = 378.0f, .y = 304.0f }, // Level 2 Exit ORDER MATTERS NEED TO UPDATE CURRENT LEVEL LOGIC
};

Vector2 returnToLevel2From3[] = {
    { .x = 3778.0f, .y = 304.0f },  // Level 2 TOP exit
    { .x = 3714.0f, .y = 1488.0f },   // Level 2 MIDDLE exit
    { .x = 3650.0f, .y = 2544.0f }    // Level 2 BOTTOM exit
};

const char* mapName[] = {
    "resources/level1.tmx",
    "resources/level2.tmx",
    "resources/level3.tmx",
    "resources/level4.tmx"
};

int lastLevel2ExitUsed = -1; // defaulted value

LevelTransition levelTransitions[] = {
    // Level 1 -> Level 2
    {1, 2, "resources/level2.tmx", {226.0f, 1488.0f}},

    // Level 2 -> Level 3 (3 unique exits)
    {2, 3, "resources/level3.tmx", {290.0f, 240.0f}},   // level2_1_exit

    // Level 2 -> Level 1
    {2, 1, "resources/level1.tmx", {2658.0f, 400.0f}},

    // Level 3 -> Level 4
    {3, 4, "resources/level4.tmx", {576.0f, 512.0f}},
};

bool HandleLevelTransition(int fromLevel, int toLevel, int transitionIndex, TmxMap*& map, Player& player) {
    int count = sizeof(levelTransitions) / sizeof(LevelTransition);
    int currentMatch = 0;
    for (int i = 0; i < count; i++) {
        if (levelTransitions[i].fromLevel == fromLevel && levelTransitions[i].toLevel == toLevel) {

            UnloadTMX(map);
            map = LoadTMX(levelTransitions[i].mapPath);
            if (!map) {
                TraceLog(LOG_ERROR, "Failed to load map: %s", levelTransitions[i].mapPath);
                return false;
            }
            TraceLog(LOG_DEBUG, "LOADED MAP: %s", levelTransitions[i].mapPath);
            player.currentLevel = toLevel;
            player.rect.x = levelTransitions[i].spawnPosition.x;
            player.rect.y = levelTransitions[i].spawnPosition.y;

            if (fromLevel == 2 && toLevel == 3) {
                lastLevel2ExitUsed = transitionIndex;  // Save which exit was used
            }

            return true;
        }
    }
    TraceLog(LOG_ERROR, "No matching transition found");
    return false;
}