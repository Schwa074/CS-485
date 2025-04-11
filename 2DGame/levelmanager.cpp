#include "game.h"

Vector2 playerTeleportPosition[] = {
    { .x = 2658.00, .y = 400.00}, // Level 1 exit
    { .x = 226.00, .y = 1488.00}, // Level 2 Entrance
    {}, // Level 2 Exit
};

const char* mapName[] = {
    "resources/level1.tmx",
    "resources/level2.tmx",
    "resources/level3.tmx"
};