#include "game.h"
#include <math.h>

void spawnWhiteGhost(Enemy *whiteGhost, Texture2D whiteGhostSprite, Vector2 spawnPos) {
    whiteGhost->rect = {spawnPos.x, spawnPos.y, 64, 64}; // Adjust the size as needed
    whiteGhost->sprite = whiteGhostSprite;
    whiteGhost->speed = 30.0f; // Variable to edit speed
    whiteGhost->active = true;
    whiteGhost->currentFrame = 0;
    whiteGhost->frameTime = 0.1f;
    whiteGhost->frameCounter = 0.0f;
}

void moveWhiteGhost(Enemy *whiteGhost, const Player *player) {
    if (!whiteGhost->active) return;
    Vector2 dir = {player->rect.x - whiteGhost->rect.x, player->rect.y - whiteGhost->rect.y};
    float length = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (length > 0) { 
        dir.x /= length;
        dir.y /= length; 
        whiteGhost->rect.x += dir.x * whiteGhost->speed * GetFrameTime();
        whiteGhost->rect.y += dir.y * whiteGhost->speed * GetFrameTime();
    }
    whiteGhost->frameCounter += GetFrameTime(); 
    if (whiteGhost->frameCounter >= whiteGhost->frameTime) {
        whiteGhost->frameCounter = 0.0f;
        whiteGhost->currentFrame++; 
        if (whiteGhost->currentFrame > 3) {
            whiteGhost->currentFrame = 0;
        }
    }
}

void drawWhiteGhost(const Enemy *whiteGhost) {
    if (whiteGhost->active) {
        Rectangle source = {(float)(whiteGhost->currentFrame * 32), 0.0f, 32.0f, 32.0f}; // Adjust the source rectangle based on the sprite sheet
        Rectangle dest = whiteGhost->rect;
        Vector2 origin = {0, 0};
        float rotation = 0.0f;
        Color tint = WHITE;
        DrawTexturePro(whiteGhost->sprite, source, dest, origin, rotation, tint);
    }
}

void checkWhiteGhostCollision(Enemy *whiteGhost, Player *player) {
    if (whiteGhost->active) {
        Rectangle whiteGhostHitbox = {whiteGhost->rect.x + 8, whiteGhost->rect.y + 8, 32, 32}; // Adjust the hitbox size as needed
        if (CheckCollisionRecs(whiteGhostHitbox, player->rect)) {
            player->currentHealth = 0;
        }
    }
}

void deactivateWhiteGhost(Enemy *whiteGhost) {
    whiteGhost->active = false;
}

void handleWhiteGhostSpawn(Enemy *whiteGhost, Texture2D whiteGhostSprite) {
    double currentTime = GetTime();
    if (currentTime > 15 && !whiteGhost->active) {
        spawnWhiteGhost(whiteGhost, whiteGhostSprite, {2650, 500});
    }
}

void createTrap(Enemy *trap, Texture2D trapSprite, Vector2 spawnPos) {
    trap->rect = {spawnPos.x, spawnPos.y, 32, 32};
    trap->sprite = trapSprite;
    trap->speed = 0.0f;
    trap->active = true;
    trap->currentFrame = 0;
    trap->frameTime = 0.1f;
    trap->frameCounter = 0.0f;
    trap->resetTime = 3.0f;
    trap->deactivationTime = 0.0f;
}

void drawTrap(Enemy *trap) {
    if (trap->active) {
        trap->frameCounter += GetFrameTime();
        if (trap->frameCounter >= trap->frameTime) {
            trap->frameCounter = 0.0f;
            trap->currentFrame++;
            if (trap->currentFrame > 3) {
                trap->currentFrame = 0;
            }
        }
    } else {
        trap->currentFrame = 3;
    }
    Rectangle source = {(float)(trap->currentFrame * 32), 0.0f, 32.0f, 32.0f};
    Rectangle dest = trap->rect;
    Vector2 origin = {0, 0};
    float rotation = 0.0f;
    Color tint = WHITE;
    DrawTexturePro(trap->sprite, source, dest, origin, rotation, tint);
}

void updateTrapState(Enemy* trap) {
    if (!trap->active && GetTime() - trap->deactivationTime > trap->resetTime) {
        trap->active = true;
    }
}

bool checkTrapCollision(Player* player, Enemy* trap) {
    if (trap->active) {
        return CheckCollisionRecs(player->rect, trap->rect);
    }
    return false;
}