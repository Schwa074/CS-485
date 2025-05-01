#include "game.h"

void createDoor(Door *door, Texture2D doorSprite, Vector2 spawnPos) {
    door->rect = {spawnPos.x, spawnPos.y, 196, 32};
    door->sprite = doorSprite;
    door->isOpen = false;
}

void drawDoor(const Door *door) {
    if (!door->isOpen) {
        Rectangle source = {0.0f, 0.0f, 196.0f, 32.0f};
        Rectangle dest = door->rect;
        Vector2 origin = {0, 0};
        float rotation = 0.0f;
        Color tint = WHITE;
        DrawTexturePro(door->sprite, source, dest, origin, rotation, tint);
    }
}

void checkDoorCollision(Door *door, Player *player, Sound doorSound) {
    if (!door->isOpen && CheckCollisionRecs(door->rect, player->rect)) {
        auto it = std::find(player->inventory.begin(), player->inventory.end(), "Key");
        if (it != player->inventory.end()) {
            door->isOpen = true;
            PlaySound(doorSound);
        } else {
            player->rect.x -= player->vel.x * GetFrameTime();
            player->rect.y -= player->vel.y * GetFrameTime();
        }
    }
}