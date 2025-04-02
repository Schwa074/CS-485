#include "game.h"

void createItem(Item *item, Texture2D itemSprite, Rectangle pos, std::string itemName) {
    item->itemName = itemName;
    item->rect = pos;
    item->sprite = itemSprite;
    item->currentFrame = 0;
    item->frameTime = 0.0f;
    item->frameCounter = 0.0f;
    item->pickedUp = false;
    item->isUsing = false;
}

void drawItem(Item* item) {
    Rectangle dest = item->rect;
    Rectangle source = {0.0f, 0.0f, 32.0f, 32.0f};
    Vector2 origin = {0, 0};
    float rotation = 0.0f;
    Color tint = WHITE;
    DrawTexturePro(item->sprite, source, dest, origin, rotation, tint);
}

void drawTorch(Item* torch)
{
  torch->frameCounter += GetFrameTime();
    if (torch->frameCounter >= torch->frameTime) {
      torch->frameCounter = 0.0f;
      torch->currentFrame++;
      if (torch->currentFrame > 7) { // Assuming 8 frames (0, 1, 2, 3, 4, 5, 6, 7)
        torch->currentFrame = 0;
      }
    }

  Rectangle source;
  // First 4 sprites on top half of sheet
  if (torch->currentFrame > 3)
  {
    source = {(float)(torch->currentFrame * 64), 0.0f, 64.0f, 64.0f}; // Each frame is 32x32 pixels
  }
  // Next 4 sprites on bottom half of sheet
  else
  {
    source = {(float)(torch->currentFrame * 64), 64.0f, 64.0f, 64.0f}; // Each frame is 32x32 pixels
  }
  
  Rectangle dest = torch->rect;
  Vector2 origin = {0, 0};
  float rotation = 0.0f;
  Color tint = WHITE;
  DrawTexturePro(torch->sprite, source, dest, origin, rotation, tint);

}

bool checkItemCollision(Item* item, Player* player) {
    return CheckCollisionRecs(item->rect, player->rect);
}