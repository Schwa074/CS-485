#include "game.h"
#include "raytmx.h"

void moveRectByVel(Rectangle *rect, const Vector2 *vel)
{
    rect->x += vel->x * GetFrameTime();
    rect->y += vel->y * GetFrameTime();
}

bool isTileCollisions(TmxMap *map, Player *player)
{
    for (unsigned int i = 0; i < map->layersLength; i++)
    {
        if (strcmp(map->layers[i].name, "collisions") == 0 && map->layers[i].type == LAYER_TYPE_OBJECT_GROUP)
        {
            TmxObject col;
            if (CheckCollisionTMXObjectGroupRec(map->layers[i].exact.objectGroup, player->rect, &col))
            {
                return true;
            }
        }
    }
    return false;
}

void cameraFollow(Camera2D *camera, const Player *player)
{
    camera->target = (Vector2){
        .x = player->rect.x,
        .y = player->rect.y,
    };
}

void drawHearts(Texture2D heartsheet, int currentHealth)
{
    switch (currentHealth)
    {
    case 6:
        DrawTexturePro(heartsheet, {0, 0, 32, 32}, {32, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        DrawTexturePro(heartsheet, {0, 0, 32, 32}, {64, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        DrawTexturePro(heartsheet, {0, 0, 32, 32}, {96, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        break;
    case 5:
        DrawTexturePro(heartsheet, {0, 0, 32, 32}, {32, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        DrawTexturePro(heartsheet, {0, 0, 32, 32}, {64, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        DrawTexturePro(heartsheet, {32, 0, 32, 32}, {96, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        break;
    case 4:
        DrawTexturePro(heartsheet, {0, 0, 32, 32}, {32, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        DrawTexturePro(heartsheet, {0, 0, 32, 32}, {64, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        DrawTexturePro(heartsheet, {64, 0, 32, 32}, {96, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        break;
    case 3:
        DrawTexturePro(heartsheet, {0, 0, 32, 32}, {32, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        DrawTexturePro(heartsheet, {32, 0, 32, 32}, {64, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        DrawTexturePro(heartsheet, {64, 0, 32, 32}, {96, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        break;
    case 2:
        DrawTexturePro(heartsheet, {0, 0, 32, 32}, {32, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        DrawTexturePro(heartsheet, {64, 0, 32, 32}, {64, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        DrawTexturePro(heartsheet, {64, 0, 32, 32}, {96, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        break;
    case 1:
        DrawTexturePro(heartsheet, {32, 0, 32, 32}, {32, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        DrawTexturePro(heartsheet, {64, 0, 32, 32}, {64, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        DrawTexturePro(heartsheet, {64, 0, 32, 32}, {96, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        break;
    case 0:
        DrawTexturePro(heartsheet, {64, 0, 32, 32}, {32, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        DrawTexturePro(heartsheet, {64, 0, 32, 32}, {64, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        DrawTexturePro(heartsheet, {64, 0, 32, 32}, {96, 32, 32, 32}, {0, 0}, 0.0f, WHITE);
        break;
    }
}

void drawLight(Texture2D light)
{
    DrawTexture(light, 0, 0, WHITE);
}

// --- Inventory HUD ---
// Draws the inventory HUD at the bottom of the screen
//Extern to use textures loaded in main.cpp
extern Texture2D torchStillSprite;
extern Texture2D noteSprite;
extern Texture2D keySprite;

void DrawInventoryHUD(const Player *player, int selectedSlot)
{
    const int slotSize = 60;
    const int spacing = 10;
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    const int numSlots = 4;
    const int barWidth = numSlots * slotSize + (numSlots - 1) * spacing;
    const int barX = (screenWidth - barWidth) / 2;
    const int barY = screenHeight - slotSize - 20;

    DrawText("Inventory:", 10, 60, 20, WHITE);
    DrawText("Press 1-4 to use items", 10, 80, 20, WHITE);

    for (int i = 0; i < numSlots; i++) {
        int x = barX + i * (slotSize + spacing);
        int y = barY;
        Rectangle slot = { (float)x, (float)y, (float)slotSize, (float)slotSize };

        DrawRectangleRec(slot, Fade(BLACK, 0.4f));
        DrawRectangleLinesEx(slot, 2, RAYWHITE);

        // Draw key label (1, 2, 3, 4)
        DrawText(TextFormat("%d", i + 1), x + 5, y + 5, 10, WHITE);

        if (i < player->inventory.size()) {
            const std::string &item = player->inventory[i];

            Texture2D itemTexture;
            if (item == "Torch")      itemTexture = torchStillSprite;
            else if (item == "Note")  itemTexture = noteSprite;
            else if (item == "Key")   itemTexture = keySprite;
            else continue;

            // Draw item icon
            DrawTexturePro(
                itemTexture,
                Rectangle{ 0, 0, (float)itemTexture.width, (float)itemTexture.height },
                Rectangle{ (float)x + 10, (float)y + 10, (float)slotSize - 20, (float)slotSize - 20 },
                Vector2{ 0, 0 },
                0.0f,
                WHITE
            );
        }
    }
}
// --- End Inventory HUD ---