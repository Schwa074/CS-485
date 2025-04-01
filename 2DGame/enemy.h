#ifndef ENEMY_H
#define ENEMY_H

#include "game.h"

struct Enemy {
    Rectangle rect;
    Texture2D sprite;
    float speed;
    bool active;
    int currentFrame;
    float frameTime;
    float frameCounter;
    float deactivationTime;
    float resetTime;
};

void spawnGhost(Enemy *ghost, Texture2D ghostSprite, Vector2 spawnPos);
void moveGhost(Enemy *ghost, const Player *player);
void drawGhost(const Enemy *ghost);
void checkGhostCollision(Enemy *ghost, Player *player);
void deactivateGhost(Enemy *ghost);
void handleGhostSpawn(Enemy *ghost, Texture2D ghostSprite);
void createTrap(Enemy *trap, Texture2D trapSprite, Vector2 spawnPos);
void drawTrap(Enemy *trap);
void updateTrapState(Enemy* trap);
bool checkTrapCollision(Player* player, Enemy* trap);

#endif // ENEMY_H