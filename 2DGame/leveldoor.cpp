#include "game.h"

void createLevelDoor(LevelDoor *door, Rectangle pos) {
    door->rect = pos;
}

bool checkLevelDoorCollision(LevelDoor *door, Player *player) {
    return CheckCollisionRecs(door->rect, player->rect);
}