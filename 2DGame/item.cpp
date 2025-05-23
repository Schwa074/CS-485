#include "game.h"

void createItem(Item *item, Texture2D itemSprite, Rectangle pos, std::string itemName) {
    item->itemName = itemName;
    item->rect = pos;
    item->sprite = itemSprite;
    item->currentFrame = 0;
    item->frameTime = 0.1f;
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

void drawSword(Item* sword) {
  sword->frameCounter += GetFrameTime();
  if (sword->frameCounter >= sword->frameTime) {
      sword->frameCounter = 0.0f;
      sword->currentFrame++;
      if (sword->currentFrame > 7) { // Assuming 8 frames (0, 1, 2, 3, 4, 5, 6, 7)
          sword->currentFrame = 0;
      }
  }

  // All frames are on the same line unlike torch
  Rectangle source = {(float)(sword->currentFrame * 32), 0.0f, 32.0f, 32.0f}; // Each frame is 32x32 pixels
  Rectangle dest = sword->rect;
  Vector2 origin = {0, 0};
  float rotation = 0.0f;
  Color tint = WHITE;
  DrawTexturePro(sword->sprite, source, dest, origin, rotation, tint);
}

void swingSword(Item* sword, Player* player, std::vector<Enemy>& ghosts, Texture2D swordSwingSprite, Sound ghostDeathSound) {
  // Update animation frame
  sword->frameCounter += GetFrameTime();
  if (sword->frameCounter >= sword->frameTime) {
      sword->frameCounter = 0.0f;
      sword->currentFrame++;
      TraceLog(LOG_DEBUG, std::to_string(sword->currentFrame).c_str());
      if (sword->currentFrame > 8) { // 9 frames (0 to 8)
          sword->currentFrame = 0; // Reset animation
      }
  }

  // Calculate source and destination rectangles
  Rectangle source = {(float)(sword->currentFrame * 96), 0.0f, 96.0f, 96.0f}; // Each frame is 96x96 pixels

  // Scalable sword size
  float scale = 2.0f; // Scale factor (2.0x larger)
  float scaledWidth = 96.0f * scale;
  float scaledHeight = 96.0f * scale;

  // Adjust the position to center the sprite on the player
  Rectangle dest = {
      player->rect.x + (player->rect.width / 2) - (scaledWidth / 2),  // Center X
      player->rect.y + (player->rect.height / 2) - (scaledHeight / 2), // Center Y
      scaledWidth,  // Width scaled by the factor
      scaledHeight  // Height scaled by the factor
  };
  Vector2 origin = {0, 0};
  float rotation = 0.0f;
  Color tint = WHITE;

  // Draw the sword swing animation
  DrawTexturePro(swordSwingSprite, source, dest, origin, rotation, tint);

  // Sword hitbox (same as the destination rectangle)
  Rectangle swordHitbox = dest;

  // Check collision with all ghosts
  for (auto& ghost : ghosts) {
      if (ghost.active && CheckCollisionRecs(swordHitbox, ghost.rect)) {
          ghost.currentHealth -= 3; // Reduce health by 3
          if (ghost.currentHealth <= 0) {
            PlaySound(ghostDeathSound);
            ghost.active = false; // Deactivate ghost if health is 0 or less
          }
      }
  }
}

bool checkItemCollision(Item* item, Player* player) {
    return CheckCollisionRecs(item->rect, player->rect);
}