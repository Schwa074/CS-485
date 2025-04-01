#include "game.h"

void createItem(Item *item, Texture2D itemSprite, Rectangle pos, std::string itemName) {
    item->itemName = itemName;
    item->rect = pos;
    item->sprite = itemSprite;
    item->currentFrame = 0;
    item->frameTime = 0.0f;
    item->frameCounter = 0.0f;
}

void drawItem(Item* item) {
    Rectangle dest = item->rect;
    Rectangle source = {0.0f, 0.0f, 32.0f, 32.0f};
    Vector2 origin = {0, 0};
    float rotation = 0.0f;
    Color tint = WHITE;
    DrawTexturePro(item->sprite, source, dest, origin, rotation, tint);
}

bool checkItemCollision(Item* item, Player* player) {
    return CheckCollisionRecs(item->rect, player->rect);
}