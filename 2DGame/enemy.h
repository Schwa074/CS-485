#ifndef ENEMY_H
#define ENEMY_H

#include "game.h"

struct Enemy {
    Rectangle rect;
    Texture2D sprite;
    float speed;
    bool active;
    int currentFrame;
    int maxHealth;
    int currentHealth;
    float frameTime;
    float frameCounter;
    float deactivationTime;
    float resetTime;
    double lastCollisionTime; // For mimicking i-frames on collision
};

void spawnWhiteGhost(Enemy *whiteGhost, Texture2D whiteGhostSprite, Vector2 spawnPos);
void moveWhiteGhost(Enemy *whiteGhost, const Player *player);
void drawWhiteGhost(const Enemy *whiteGhost);
void checkWhiteGhostCollision(Enemy *whiteGhost, Player *player);
void deactivateWhiteGhost(Enemy *whiteGhost);
void handleWhiteGhostSpawn(Enemy *whiteGhost, Texture2D whiteGhostSprite);
void spawnRedGhost(Enemy *redGhost, Texture2D redGhostSprite, Vector2 spawnPos);
void moveRedGhost(Enemy *redGhost, const Player *player);
void drawRedGhost(const Enemy *redGhost);
void checkRedGhostCollision(Enemy *redGhost, Player *player);
void deactivateRedGhost(Enemy *redGhost);
void spawnBlueGhost(Enemy *blueGhost, Texture2D blueGhostSprite, Vector2 spawnPos);
void moveBlueGhost(Enemy *blueGhost, const Player *player);
void drawBlueGhost(const Enemy *blueGhost);
void checkBlueGhostCollision(Enemy *blueGhost, Player *player);
void deactivateBlueGhost(Enemy *blueGhost);
void createTrap(Enemy *trap, Texture2D trapSprite, Vector2 spawnPos);
void drawTrap(Enemy *trap);
void updateTrapState(Enemy* trap);
bool checkTrapCollision(Player* player, Enemy* trap);

#endif // ENEMY_H