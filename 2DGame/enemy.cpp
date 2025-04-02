#include "game.h"
#include <math.h>

void spawnGhost(Enemy *ghost, Texture2D ghostSprite, Vector2 spawnPos) {
    ghost->rect = {spawnPos.x, spawnPos.y, 64, 64};
    ghost->sprite = ghostSprite;
    ghost->speed = 30.0f;
    ghost->active = true;
    ghost->currentFrame = 0;
    ghost->frameTime = 0.1f;
    ghost->frameCounter = 0.0f;
}

void moveGhost(Enemy *ghost, const Player *player) {
    if (!ghost->active) return;
    Vector2 dir = {player->rect.x - ghost->rect.x, player->rect.y - ghost->rect.y};
    float length = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (length > 0) {
        dir.x /= length;
        dir.y /= length;
        ghost->rect.x += dir.x * ghost->speed * GetFrameTime();
        ghost->rect.y += dir.y * ghost->speed * GetFrameTime();
    }
    ghost->frameCounter += GetFrameTime();
    if (ghost->frameCounter >= ghost->frameTime) {
        ghost->frameCounter = 0.0f;
        ghost->currentFrame++;
        if (ghost->currentFrame > 3) {
            ghost->currentFrame = 0;
        }
    }
}

void drawGhost(const Enemy *ghost) {
    if (ghost->active) {
        Rectangle source = {(float)(ghost->currentFrame * 32), 0.0f, 32.0f, 32.0f};
        Rectangle dest = ghost->rect;
        Vector2 origin = {0, 0};
        float rotation = 0.0f;
        Color tint = WHITE;
        DrawTexturePro(ghost->sprite, source, dest, origin, rotation, tint);
    }
}

void checkGhostCollision(Enemy *ghost, Player *player) {
    if (ghost->active) {
        Rectangle ghostHitbox = {ghost->rect.x + 8, ghost->rect.y + 8, 32, 32};
        if (CheckCollisionRecs(ghostHitbox, player->rect)) {
            player->currentHealth = 0;
        }
    }
}

void deactivateGhost(Enemy *ghost) {
    ghost->active = false;
}

void handleGhostSpawn(Enemy *ghost, Texture2D ghostSprite) {
    double currentTime = GetTime();
    if (currentTime > 15 && !ghost->active) {
        spawnGhost(ghost, ghostSprite, {2650, 500});
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