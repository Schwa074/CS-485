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

void spawnWhiteGhost(Enemy *whiteGhost, Texture2D whiteGhostSprite, Vector2 spawnPos);
void moveWhiteGhost(Enemy *whiteGhost, const Player *player);
void drawWhiteGhost(const Enemy *whiteGhost);
void checkWhiteGhostCollision(Enemy *whiteGhost, Player *player);
void deactivateWhiteGhost(Enemy *whiteGhost);
void handleWhiteGhostSpawn(Enemy *whiteGhost, Texture2D whiteGhostSprite);
void createTrap(Enemy *trap, Texture2D trapSprite, Vector2 spawnPos);
void drawTrap(Enemy *trap);
void updateTrapState(Enemy* trap);
bool checkTrapCollision(Player* player, Enemy* trap);

#endif // ENEMY_H