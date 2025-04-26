#include "game.h"
#include <math.h>

void spawnWhiteGhost(Enemy *whiteGhost, Texture2D whiteGhostSprite, Vector2 spawnPos) {
    whiteGhost->rect = {spawnPos.x, spawnPos.y, 64, 64}; // Adjust the size as needed
    whiteGhost->sprite = whiteGhostSprite;
    whiteGhost->speed = 30.0f; // Variable to edit speed
    whiteGhost->active = true;
    whiteGhost->currentHealth = 1000; // Killable but good luck
    whiteGhost->maxHealth = 1000; 
    whiteGhost->currentFrame = 0;
    whiteGhost->frameTime = 0.1f;
    whiteGhost->frameCounter = 0.0f;
    whiteGhost->lastCollisionTime = 0.0;
}

void spawnBlueGhost(Enemy *blueGhost, Texture2D blueGhostSprite, Vector2 spawnPos) {
    blueGhost->rect = {spawnPos.x, spawnPos.y, 80, 80}; // Adjust the size as needed (bigger than white ghost)
    blueGhost->sprite = blueGhostSprite;
    blueGhost->speed = 15.0f; // Variable to edit speed (slower than white ghost)
    blueGhost->active = true;
    blueGhost->currentHealth = 6; // Sword should 2 hit kill blue ghost
    blueGhost->maxHealth = 6; // Set max health for blue ghost
    blueGhost->currentFrame = 0;
    blueGhost->frameTime = 0.1f;
    blueGhost->frameCounter = 0.0f;
    blueGhost->lastCollisionTime = 0.0;
}

void spawnRedGhost(Enemy *redGhost, Texture2D redGhostSprite, Vector2 spawnPos) {
    redGhost->rect = {spawnPos.x, spawnPos.y, 32, 32}; // Adjust the size as needed (smaller than white ghost)
    redGhost->sprite = redGhostSprite;
    redGhost->speed = 45.0f; // Variable to edit speed (faster than white ghost)
    redGhost->active = true;
    redGhost->currentHealth = 3; // Sword should 1 shot red ghost
    redGhost->maxHealth = 3; // Set max health for red ghost
    redGhost->currentFrame = 0;
    redGhost->frameTime = 0.1f;
    redGhost->frameCounter = 0.0f;
    redGhost->lastCollisionTime = 0.0;
}

void moveGhost(Enemy* ghost, const Player* player) {
    if (!ghost->active) return;

    // Calculate direction vector from ghost to player
    Vector2 dir = {player->rect.x - ghost->rect.x, player->rect.y - ghost->rect.y};
    float length = sqrtf(dir.x * dir.x + dir.y * dir.y);

    // Normalize the direction vector and move the ghost
    if (length > 0) { 
        dir.x /= length;
        dir.y /= length; 
        ghost->rect.x += dir.x * ghost->speed * GetFrameTime();
        ghost->rect.y += dir.y * ghost->speed * GetFrameTime();
    }

    // Update animation frame
    ghost->frameCounter += GetFrameTime(); 
    if (ghost->frameCounter >= ghost->frameTime) {
        ghost->frameCounter = 0.0f;
        ghost->currentFrame++; 
        if (ghost->currentFrame > 3) {
            ghost->currentFrame = 0;
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

void drawBlueGhost(const Enemy *blueGhost) {
    if (blueGhost->active) {
        Rectangle source = {(float)(blueGhost->currentFrame * 32), 0.0f, 32.0f, 32.0f}; // Adjust the source rectangle based on the sprite sheet
        Rectangle dest = blueGhost->rect;
        Vector2 origin = {0, 0};
        float rotation = 0.0f;
        Color tint = WHITE;
        DrawTexturePro(blueGhost->sprite, source, dest, origin, rotation, tint);
    }
}

void drawRedGhost(const Enemy *redGhost) {
    if (redGhost->active) {
        Rectangle source = {(float)(redGhost->currentFrame * 32), 0.0f, 32.0f, 32.0f}; // Adjust the source rectangle based on the sprite sheet
        Rectangle dest = redGhost->rect;
        Vector2 origin = {0, 0};
        float rotation = 0.0f;
        Color tint = WHITE;
        DrawTexturePro(redGhost->sprite, source, dest, origin, rotation, tint);
    }
}

void checkWhiteGhostCollision(Enemy *whiteGhost, Player *player) {
    if (whiteGhost->active) {
        Rectangle whiteGhostHitbox = {whiteGhost->rect.x + 8, whiteGhost->rect.y + 8, 32, 32}; // Adjust the hitbox size as needed
        if (CheckCollisionRecs(whiteGhostHitbox, player->rect)) {
            player->currentHealth = 0; // Instakill player on collision with white ghost
        }
    }
}

void checkBlueGhostCollision(Enemy *blueGhost, Player *player) {
    if (blueGhost->active) {
        double currentTime = GetTime();
        if (currentTime - blueGhost->lastCollisionTime > 1.0) { // 1-second cooldown
            Rectangle blueGhostHitbox = {blueGhost->rect.x + 8, blueGhost->rect.y + 8, 48, 48};
            if (CheckCollisionRecs(blueGhostHitbox, player->rect)) {
                player->currentHealth -= 1; // Take half a heart of damage
                blueGhost->lastCollisionTime = currentTime; // Update last collision time
            }
        }
    }
}

void checkRedGhostCollision(Enemy *redGhost, Player *player) {
    if (redGhost->active) {
        double currentTime = GetTime();
        if (currentTime - redGhost->lastCollisionTime > 1.0) { // 1-second cooldown
            Rectangle redGhostHitbox = {redGhost->rect.x + 8, redGhost->rect.y + 8, 24, 24};
            if (CheckCollisionRecs(redGhostHitbox, player->rect)) {
                player->currentHealth -= 2; // Take a heart of damage
                redGhost->lastCollisionTime = currentTime; // Update last collision time
            }
        }
    }
}

void deactivateWhiteGhost(Enemy *whiteGhost) {
    whiteGhost->active = false;
}

void deactivateBlueGhost(Enemy *blueGhost) {
    blueGhost->active = false;
}

void deactivateRedGhost(Enemy *redGhost) {
    redGhost->active = false;
}

void handleWhiteGhostSpawn(Enemy *whiteGhost, Texture2D whiteGhostSprite) {
    double currentTime = GetTime();
    if (currentTime > 15 && !whiteGhost->active) {
        spawnWhiteGhost(whiteGhost, whiteGhostSprite, {2650, 500});
    }
}

void createTrap(Enemy *trap, Texture2D trapSprite, Vector2 spawnPos) {
    trap->rect = {spawnPos.x, spawnPos.y, 28, 28};
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