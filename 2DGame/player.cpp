#include "game.h"

void movePlayer(Player *player) {
    player->vel.x = 0.0f;
    player->vel.y = 0.0f;
    player->state = IDLE;
    if (IsKeyDown(KEY_A)) {
        player->vel.x = -200.0f;
        player->dir = LEFT;
        player->state = RUNNING_LEFT;
    } else if (IsKeyDown(KEY_D)) {
        player->vel.x = 200.0f;
        player->dir = RIGHT;
        player->state = RUNNING_RIGHT;
    } else if (IsKeyDown(KEY_W)) {
        player->vel.y = -200.0f;
        player->state = RUNNING_UP;
    } else if (IsKeyDown(KEY_S)) {
        player->vel.y = 200.0f;
        player->state = RUNNING_DOWN;
    }
    if (IsKeyDown(KEY_A) && IsKeyDown(KEY_W)) {
        player->vel.x = -200;
        player->vel.y = -200;
        player->dir = LEFT;
        player->state = RUNNING_LEFT;
    } else if (IsKeyDown(KEY_A) && IsKeyDown(KEY_S)) {
        player->vel.x = -200;
        player->vel.y = 200;
        player->dir = LEFT;
        player->state = RUNNING_LEFT;
    } else if (IsKeyDown(KEY_D) && IsKeyDown(KEY_W)) {
        player->vel.x = 200;
        player->vel.y = -200;
        player->dir = RIGHT;
        player->state = RUNNING_RIGHT;
    } else if (IsKeyDown(KEY_D) && IsKeyDown(KEY_S)) {
        player->vel.x = 200;
        player->vel.y = 200;
        player->dir = RIGHT;
        player->state = RUNNING_RIGHT;
    }
}

void drawPlayer(const Player *player) {
    Rectangle source = animation_frame(&(player->animations[player->state]));
    DrawTexturePro(player->sprite, source, player->rect, {0, 0}, 0.0f, WHITE);
}

void update_animation(Animation *self) {
    float dt = GetFrameTime();
    self->rem -= dt;
    if (self->rem < 0) {
        self->rem = self->spd;
        self->cur++;
        if (self->cur > self->lst) {
            switch (self->type) {
                case REPEATING:
                    self->cur = self->fst;
                    break;
                case ONESHOT:
                    self->cur = self->lst;
                    break;
            }
        }
    }
}

Rectangle animation_frame(const Animation *self) {
    int x = (self->cur % (self->lst + 1)) * 48;
    int y = self->offset * 68;
    return (Rectangle) {
        .x = (float)x, .y = (float)y, .width = 48.0f, .height = 68.0f
    };
}

void keepPlayerInScreen(Player *player) {
    if (player->rect.y > (H - player->rect.height)) {
        player->vel.y = 0.0;
        player->rect.y = (H - player->rect.height);
    }
}