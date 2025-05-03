#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"

enum Direction {
    LEFT = -1,
    RIGHT = 1,
};

enum CurrentState {
    RUNNING_DOWN = 0,
    RUNNING_LEFT = 1,
    RUNNING_RIGHT = 2,
    RUNNING_UP = 3,
    IDLE = 4
};

enum AnimationType {
    REPEATING,
    ONESHOT
};

struct Animation {
    int fst;
    int lst;
    int cur;
    int offset;
    float spd;
    float rem;
    AnimationType type;
};

struct Player {
    Rectangle rect;
    Vector2 vel;
    Texture2D sprite;
    Direction dir;
    CurrentState state;
    std::vector<Animation> animations;
    int maxHealth;
    int currentHealth;
    std::vector<std::string> inventory;
    int currentLevel;
    // Add these for footstep sound control
    float footstepTimer = 0.0f;
    float footstepCooldown = 0.25f;
};

void movePlayer(Player *player);
void drawPlayer(const Player *player);
void update_animation(Animation *self);
Rectangle animation_frame(const Animation *self);
void keepPlayerInScreen(Player *player);
void updatePlayerCurrentLevel(Player *player);


#endif // PLAYER_H