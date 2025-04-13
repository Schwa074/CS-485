#ifndef GAMEUTILS_H
#define GAMEUTILS_H

#include "game.h"
#include "raytmx.h"

void moveRectByVel(Rectangle *rect, const Vector2 *vel);
bool isTileCollisions(TmxMap *map, Player *player);
void cameraFollow(Camera2D *camera, const Player *player);
void drawHearts(Texture2D heartsheet, int currentHealth);
void drawLight(Texture2D light);
void DrawInventoryHUD(const Player *player, int selectedSlot);

#endif // GAMEUTILS_H