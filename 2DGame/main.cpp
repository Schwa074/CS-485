#include <raylib.h>
#include <vector>
#include <format>
#define RAYTMX_IMPLEMENTATION
#include "raytmx.h"

const int W = 1200;
const int H = 800;
const float MAX_GRAV = 300.0f;
const float startPosx = 722.56f;
const float startPosy = 126.01f;

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
  int maxHealth = 6;
  int currentHealth = 6;
};

struct Enemy {
  Rectangle rect;
  Texture2D sprite;
  float speed;
  bool active;
};

void spawnGhost(Enemy *ghost, Texture2D ghostSprite, Vector2 spawnPos) {
  ghost->rect = {spawnPos.x, spawnPos.y, 48, 68};
  ghost->sprite = ghostSprite;
  ghost->speed = 5.0f; // We can change this speed as needed
  ghost->active = true;
}

void moveGhost(Enemy *ghost, const Player *player) {
  if (!ghost->active) return;

  Vector2 dir = {player->rect.x - ghost->rect.x, player->rect.y - ghost->rect.y};
  float length = sqrtf(dir.x * dir.x + dir.y * dir.y);

  if (length > 0) {
    dir.x /= length;
    dir.y /= length;
    ghost->rect.x += dir.x * ghost->speed * GetFrameTime();
    ghost->rect.y += dir.y * ghost->speed * GetFrameTime();
  }
}

void drawGhost(const Enemy *ghost) {
  if (ghost->active) {
    DrawTexturePro(ghost->sprite, {0, 0, 48, 68}, ghost->rect, {0, 0}, 0.0f, WHITE);
  }
}

void checkGhostCollision(Enemy *ghost, Player *player) {
  if (ghost->active && CheckCollisionRecs(ghost->rect, player->rect)) {
    player->currentHealth = 0; // It should insta kill the player
  }
}

void handleGhostSpawn(Enemy *ghost, Texture2D ghostSprite) {
  if (GetTime() > 180.0 && !ghost->active) { // 3 minutes = 180 seconds
    spawnGhost(ghost, ghostSprite, {600, 400}); // We can change this spawn location as needed
  }
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
        // Repeating
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

void drawPlayer(const Player *player) {
  Rectangle source = animation_frame(&(player->animations[player->state]));
  DrawTexturePro(player->sprite,
                 source,
                 player->rect,
                 {0, 0},
                 0.0f,
                 WHITE);
}

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

void moveRectByVel(Rectangle *rect, const Vector2 *vel) {
  rect->x += vel->x * GetFrameTime();
  rect->y += vel->y * GetFrameTime();
}

void keepPlayerInScreen(Player *player) {
  if (player->rect.y > (H - player->rect.height)) {
    player->vel.y = 0.0;
    player->rect.y = (H - player->rect.height);
  }
}

bool isTileCollisions(TmxMap *map, Player *player) {
  for (unsigned int i = 0; i < map->layersLength; i++) {
    if (strcmp(map->layers[i].name, "collisions") == 0 && map->layers[i].type == LAYER_TYPE_OBJECT_GROUP) {
      TmxObject col;
      if (CheckCollisionTMXObjectGroupRec(map->layers[i].exact.objectGroup,
                                          player->rect,
                                          &col)) {
        return true;
      }
    }
  }
  return false;
}

void cameraFollow(Camera2D *camera, const Player *player) {
  camera->target = (Vector2) {
    .x = player->rect.x,
    .y = player->rect.y,
  };
}

void drawHearts(Texture2D heartsheet, int currentHealth) {
  // Draw hit points in top left corner based on health
  switch (currentHealth)
  {
    // Hearts: Full, Full, Full
    case 6:
    {
      DrawTexturePro(heartsheet, {0, 0, 32, 32}, {32, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Leftmost heart
      DrawTexturePro(heartsheet, {0, 0, 32, 32}, {64, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Middle heart
      DrawTexturePro(heartsheet, {0, 0, 32, 32}, {96, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Rightmost heart
      break;
    }
    // Hearts: Full, Full, Half
    case 5:
    {
      DrawTexturePro(heartsheet, {0, 0, 32, 32}, {32, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Leftmost heart
      DrawTexturePro(heartsheet, {0, 0, 32, 32}, {64, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Middle heart
      DrawTexturePro(heartsheet, {32, 0, 32, 32}, {96, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Rightmost heart
      break;
    }
    // Hearts: Full, Full, Empty
    case 4:
    {
      DrawTexturePro(heartsheet, {0, 0, 32, 32}, {32, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Leftmost heart
      DrawTexturePro(heartsheet, {0, 0, 32, 32}, {64, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Middle heart
      DrawTexturePro(heartsheet, {64, 0, 32, 32}, {96, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Rightmost heart
      break;
    }
    // Hearts: Full, Half, Empty
    case 3:
    {
      DrawTexturePro(heartsheet, {0, 0, 32, 32}, {32, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Leftmost heart
      DrawTexturePro(heartsheet, {32, 0, 32, 32}, {64, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Middle heart
      DrawTexturePro(heartsheet, {64, 0, 32, 32}, {96, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Rightmost heart
      break;
    }
    // Hearts: Full, Empty, Empty
    case 2:
    {
      DrawTexturePro(heartsheet, {0, 0, 32, 32}, {32, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Leftmost heart
      DrawTexturePro(heartsheet, {64, 0, 32, 32}, {64, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Middle heart
      DrawTexturePro(heartsheet, {64, 0, 32, 32}, {96, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Rightmost heart
      break;
    }
    // Hearts: Half, Empty, Empty
    case 1:
    {
      DrawTexturePro(heartsheet, {32, 0, 32, 32}, {32, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Leftmost heart
      DrawTexturePro(heartsheet, {64, 0, 32, 32}, {64, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Middle heart
      DrawTexturePro(heartsheet, {64, 0, 32, 32}, {96, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Rightmost heart
      break;
    }
    // Hearts: Empty, Empty, Empty
    case 0:
    {
      DrawTexturePro(heartsheet, {64, 0, 32, 32}, {32, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Leftmost heart
      DrawTexturePro(heartsheet, {64, 0, 32, 32}, {64, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Middle heart
      DrawTexturePro(heartsheet, {64, 0, 32, 32}, {96, 32, 32, 32}, {0, 0}, 0.0f, WHITE); // Rightmost heart
      break;
    }

  }
}

int main() {
  SetTraceLogLevel(LOG_DEBUG);  // Enable debug-level logs
  TraceLog(LOG_DEBUG, "Opening window");
  InitWindow(W, H, "Crypt Escape");

  const char* tmx = "resources/map.tmx";
  TmxMap* map = LoadTMX(tmx);
  if (map == nullptr) {
    TraceLog(LOG_ERROR, "couldn't load da map: %s", tmx);
    return EXIT_FAILURE;
  }

  Texture2D hero = LoadTexture("assets/charactersheet.png");
  Texture2D ghostSprite = LoadTexture("assets/ghostsheet.png");
  Enemy ghost;
  spawnGhost(&ghost, ghostSprite, {600, 400});
  Texture2D hearts = LoadTexture("assets/heartsheet.png");

  Player player = Player{.rect = (Rectangle){.x = startPosx,
                                            .y = startPosy,
                                            .width = 64.0f,
                                            .height = 64.0f},
                                            .vel = (Vector2){.x = 0.0f, .y = 0.0f},
                                            .sprite = hero,
                                            .dir = RIGHT,
                                            .state = IDLE,
                                            .animations = {
                                              (Animation){
                                              .fst = 0,
                                              .lst = 2,
                                              .cur = 0,
                                              .offset = 0,
                                              .spd = 0.1f,
                                              .rem = 0.1f,
                                              .type = REPEATING,
                                              },
                                              (Animation){
                                                  .fst = 0,
                                                  .lst = 2,
                                                  .cur = 0,
                                                  .offset = 1,
                                                  .spd = 0.1f,
                                                  .rem = 0.1f,
                                                  .type = REPEATING,
                                              },
                                              (Animation){
                                                .fst = 0,
                                                .lst = 2,
                                                .cur = 0,
                                                .offset = 2,
                                                .spd = 0.1f,
                                                .rem = 0.1f,
                                                .type = REPEATING,
                                              },
                                              (Animation){
                                                .fst = 0,
                                                .lst = 2,
                                                .cur = 0,
                                                .offset = 3,
                                                .spd = 0.1f,
                                                .rem = 0.1f,
                                                .type = REPEATING,
                                              },
                                              (Animation){
                                                  .fst = 1,
                                                  .lst = 1,
                                                  .cur = 1,
                                                  .offset = 0,
                                                  .spd = 0.1f,
                                                  .rem = 0.1f,
                                                  .type = ONESHOT,
                                              }}};

  Camera2D camera = (Camera2D){
    .offset = (Vector2){
      .x = W / 2.0f,
      .y = H / 2.0f,
    },
    .target = (Vector2){
      .x = W / 2.0f,
      .y = H / 2.0f,
    },
    .rotation = 0.0f,
    .zoom = 1.0f,
  };

  while (!WindowShouldClose()) {
    // Update
    float previous_x = player.rect.x;
    float previous_y = player.rect.y;
    
    AnimateTMX(map);
    movePlayer(&player);
    moveRectByVel(&(player.rect), &(player.vel));
    if (isTileCollisions(map, &player)) {
      player.rect.x = previous_x;
      player.rect.y = previous_y;
    }

    //keepPlayerInScreen(&player);
    update_animation(&(player.animations[player.state]));
    cameraFollow(&camera, &player);


    // Drawing
    BeginDrawing();
    {
      ClearBackground(BLACK);
      BeginMode2D(camera);
      DrawTMX(map, &camera, 0, 0, WHITE);
      drawGhost(&ghost);
      moveGhost(&ghost, &player);
      checkGhostCollision(&ghost, &player);
      drawPlayer(&player);
      EndMode2D();
      DrawFPS(5, 5);
      // DrawText(std::format("(x: {:0.0f}, y: {:0.0f})",
      //                      player.rect.x, player.rect.y)
      //          .c_str(),
      //          5, 550, 32, RED);

      // Draw hit points in top left corner
      drawHearts(hearts, player.currentHealth);
    }
    EndDrawing();
  }

  UnloadTMX(map);
  UnloadTexture(hero);
  UnloadTexture(hearts);
  CloseWindow();
  return 0;
}
