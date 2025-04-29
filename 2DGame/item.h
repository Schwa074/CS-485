#ifndef ITEM_H
#define ITEM_H

#include "game.h"

struct Item {
    std::string itemName;
    Rectangle rect;
    Texture2D sprite;
    int currentFrame;
    float frameTime;
    float frameCounter;
    bool pickedUp;
    bool isUsing;
};

void createItem(Item *item, Texture2D itemSprite, Rectangle pos, std::string itemName);
void drawItem(Item* item);
void drawTorch(Item* torch);
void drawSword(Item* sword);
void swingSword(Item* sword, Player* player, std::vector<Enemy>& ghosts, Texture2D swordSwingSprite, Sound ghostDeathSound);
bool checkItemCollision(Item* item, Player* player);

#endif // ITEM_H