#include "game.h"

ResetHallwayCollision hiddenCollisions[2];

void CreateHiddenCollisions() {
    int random_number = std::rand() % 3 + 1;
    TraceLog(LOG_DEBUG, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
    TraceLog(LOG_DEBUG, "@@@@ Random number is: %d @@@@", random_number);
    TraceLog(LOG_DEBUG, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");


    switch(random_number) {
        case 1:
            hiddenCollisions[0] = (ResetHallwayCollision){ .rect = {2816, 1440, 32, 352} }; // Middle
            hiddenCollisions[1] = (ResetHallwayCollision){ .rect = {2816, 2496, 32, 352} }; // Bottom
            break;
        case 2:
            hiddenCollisions[0] = (ResetHallwayCollision){ .rect = {2816,  272, 32, 352} }; // Top
            hiddenCollisions[1] = (ResetHallwayCollision){ .rect = {2816, 2496, 32, 352} }; // Bottom
            break;
        case 3:
            hiddenCollisions[0] = (ResetHallwayCollision){ .rect = {2816,  272, 32, 352} }; // Top
            hiddenCollisions[1] = (ResetHallwayCollision){ .rect = {2816, 1440, 32, 352} }; // Middle
            break;
    }
}

void CheckHiddenCollsions(Player* player) {
    for(int i = 0; i < 3; i++) {
        ResetHallwayCollision currentHallway = hiddenCollisions[i];
        if(CheckCollisionRecs(player->rect, currentHallway.rect)) {
            TraceLog(LOG_DEBUG, "Hitting hidden collision at: x - %f and y - %f", player->rect.x, player->rect.y);
            HandleHiddenCollision(player);
        }
    }
}

void HandleHiddenCollision(Player* player) {
    if(player->rect.y < 1000) {
        // Hit top hallway collision
        player->rect.x = 900;
        player->rect.y = player->rect.y + 1184;
    } else if (player->rect.y > 1000 && player->rect.y < 2000)
    {
        // Hit middle hallway collision
        player->rect.x = 900;
    } else if (player->rect.y > 2000)
    {
        // Hit bottom hallway collision
        player->rect.x = 900;
        player->rect.y = player->rect.y - 1056;
    }
    
    
}