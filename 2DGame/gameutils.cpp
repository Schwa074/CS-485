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

void DrawInventoryHUD(const Player *player)
{
    DrawText("Inventory:", 10, 60, 20, WHITE);
    DrawText("Press the number to use the item", 10, 80, 20, WHITE);

    bool hasTorch = false;
    bool hasNote = false;
    bool hasKey = false;
    for (size_t i = 0; i < player->inventory.size(); i++)
    {
        std::string itemText = std::to_string(i + 1) + " - " + player->inventory[i];
        DrawText(itemText.c_str(), 10, 110 + (int)i * 20, 18, YELLOW);
        if (player->inventory[i] == "Torch")
        {
            hasTorch = true;
        }
        if (player->inventory[i] == "Note")
        {
            hasNote = true;
        }
        if (player->inventory[i] == "Key")
        {
            hasKey = true;
        }
    }
}