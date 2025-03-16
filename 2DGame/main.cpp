#include <raylib.h>
#include <vector>
#include <format>
#define RAYTMX_IMPLEMENTATION
#include "raytmx.h"

const int W = 1200;
const int H = 720;
const float startPosx = 722.56f;
const float startPosy = 126.01f;
// const float startPosx = 1722.56f;
// const float startPosy = 1985.01f;

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
  std::string inventory[2];
};

struct Enemy {
  Rectangle rect;
  Texture2D sprite;
  float speed;
  bool active;
  int currentFrame;
  float frameTime;
  float frameCounter;
  float deactivationTime;
  float resetTime;
};

struct Item {
  Rectangle rect;
  Texture2D sprite;
  int currentFrame;
  float frameTime;
  float frameCounter;
  bool pickedUp = false;
};

void spawnGhost(Enemy *ghost, Texture2D ghostSprite, Vector2 spawnPos) {
  ghost->rect = {spawnPos.x, spawnPos.y, 64, 64};
  ghost->sprite = ghostSprite;
  ghost->speed = 15.0f; // We can change this speed as needed
  ghost->active = true;
  ghost->currentFrame = 0;
  ghost->frameTime = 0.1f; // Time per frame in seconds
  ghost->frameCounter = 0.0f;
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

  // Update animation frame
  ghost->frameCounter += GetFrameTime();
  if (ghost->frameCounter >= ghost->frameTime) {
    ghost->frameCounter = 0.0f;
    ghost->currentFrame++;
    if (ghost->currentFrame > 3) { // Assuming 4 frames (0, 1, 2, 3)
      ghost->currentFrame = 0;
    }
  }
}

void drawGhost(const Enemy *ghost) {
  if (ghost->active) {
    Rectangle source = {(float)(ghost->currentFrame * 32), 0.0f, 32.0f, 32.0f}; // Each frame is 32x32 pixels
    Rectangle dest = ghost->rect;
    Vector2 origin = {0, 0};
    float rotation = 0.0f;
    Color tint = WHITE;
    DrawTexturePro(ghost->sprite, source, dest, origin, rotation, tint);
  }
}

void checkGhostCollision(Enemy *ghost, Player *player) {
  if (ghost->active) {
    Rectangle ghostHitbox = {ghost->rect.x + 8, ghost->rect.y + 8, 32, 32}; // Adjust the offset and size as needed
    if (CheckCollisionRecs(ghostHitbox, player->rect)) {
      player->currentHealth = 0; // It should insta kill the player
    }
  }
}

void handleGhostSpawn(Enemy *ghost, Texture2D ghostSprite) {
  if (GetTime() > 180.0 && !ghost->active) { // 3 minutes = 180 seconds
    spawnGhost(ghost, ghostSprite, {600, 400}); // We can change this spawn location as needed
  }
}

std::vector<Enemy> traps;

Vector2 trapPositions[] = {
  {1984, 1884}, // Top Wall
  {1952, 1884}, // Top Wall
  {1920, 1884}, // Top Wall
  {1888, 1884}, // Top Wall
  {1856, 1884}, // Top Wall
  {1824, 1884}, // Top Wall

  {2144, 1884}, // Top Entrance Squiggle
  {2112, 1916}, // Top Entrance Squiggle
  {2144, 1948}, // Top Entrance Squiggle
  {2176, 1980}, // Top Entrance Squiggle
  {2144, 2012}, // Top Entrance Squiggle
  
  {2176, 2044}, // Top Entrance Squiggle Corner
  {2144, 2076}, // Top Entrance Squiggle Corner
  {2112, 2108}, // Top Entrance Squiggle Corner

  {1984, 1916}, // Top Entrance
  {1984, 1948}, // Top Entrance
  {1984, 1980}, // Top Entrance

  {1440, 2108}, // Bottom Wall
  {1472, 2108}, // Bottom Wall
  {1504, 2108}, // Bottom Wall
  {1536, 2108}, // Bottom Wall

  {1728, 2108}, // Bottom Wall
  {1760, 2108}, // Bottom Wall
  {1792, 2108}, // Bottom Wall
  {1824, 2108}, // Bottom Wall
  {1856, 2108}, // Bottom Wall
  {1888, 2108}, // Bottom Wall
  {1920, 2108}, // Bottom Wall

  {1728, 2076}, // Bottom Wall Second Row
  {1760, 2076}, // Bottom Wall Second Row
  {1792, 2076}, // Bottom Wall Second Row
  {1824, 2044}, // Bottom Wall Second Row

  {1504, 1884}, // Around the key
  {1472, 1916}, // Around the key
  {1440, 1884}, // Around the key

  {1536, 1916}, // Around the key
  {1536, 1948}, // Around the key
  {1536, 1980}, // Around the key

  {1568, 1948}, // Middle Squiggle
  {1600, 1916}, // Middle Squiggle
  {1632, 1948}, // Middle Squiggle
  {1664, 1916}, // Middle Squiggle
  {1696, 1948}, // Middle Squiggle
  {1728, 1916}, // Middle Squiggle
  {1760, 1884}, // Middle Squiggle
};

Vector2 torchPosition = {1500, 600};

void createTorch(Item *torch, Texture2D torchSprite, Vector2 spawnPos)
{
  torch->rect = {spawnPos.x, spawnPos.y, 32, 32};
  torch->sprite = torchSprite;
  torch->currentFrame = 0;
  torch->frameTime = 0.1f; // Time per frame in seconds
  torch->frameCounter = 0.0f;
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

bool checkItemCollision(Item* item, Player* player)
{
  return CheckCollisionRecs(item->rect, player->rect);
}

int numTraps = sizeof(trapPositions) / sizeof(trapPositions[0]);

void createTrap(Enemy *trap, Texture2D trapSprite, Vector2 spawnPos) {
  trap->rect = {spawnPos.x, spawnPos.y, 32, 32};
  trap->sprite = trapSprite;
  trap->speed = 0.0f; // We can change this speed as needed
  trap->active = true;
  trap->currentFrame = 0;
  trap->frameTime = 0.1f; // Time per frame in seconds
  trap->frameCounter = 0.0f;
  trap->resetTime = 3.0f;
  trap->deactivationTime = 0.0f;
}


void drawTrap(Enemy *trap) {
  if(trap->active) {
    trap->frameCounter += GetFrameTime();
    if (trap->frameCounter >= trap->frameTime) {
      trap->frameCounter = 0.0f;
      trap->currentFrame++;
      if (trap->currentFrame > 3) { // Assuming 4 frames (0, 1, 2, 3)
        trap->currentFrame = 0;
      }
    }
  }
  else {
    trap->currentFrame = 3;
  }

  Rectangle source = {(float)(trap->currentFrame * 32), 0.0f, 32.0f, 32.0f}; // Each frame is 32x32 pixels
  Rectangle dest = trap->rect;
  Vector2 origin = {0, 0};
  float rotation = 0.0f;
  Color tint = WHITE;
  DrawTexturePro(trap->sprite, source, dest, origin, rotation, tint);
}

void updateTrapState(Enemy* trap) {
  if (!trap->active && GetTime() - trap->deactivationTime > trap->resetTime) {
      trap->active = true;  // Reactivate trap after reset time
  }
}

bool checkTrapCollision(Player* player, Enemy* trap) {
  if(trap->active) {
    return CheckCollisionRecs(player->rect, trap->rect);
  }
  return false;
}

void createNote(Item *note, Texture2D noteSprite)
{
  note->rect = {2976, 2720, 32, 32};
  note->sprite = noteSprite;
  note->currentFrame = 0;
  note->frameTime = 0.0f;
  note->frameCounter = 0.0f;
}

void drawNote(Item* note) {
  Rectangle dest = note->rect;
  Rectangle source = {0.0f, 0.0f, 32.0f, 32.0f};
  Vector2 origin = {0, 0};
  float rotation = 0.0f;
  Color tint = WHITE;
  DrawTexturePro(note->sprite, source, dest, origin, rotation, tint);
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

void drawLight(Texture2D light)
{
  DrawTexture(light, 0, 0, WHITE);
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
  Texture2D trapSprite = LoadTexture("assets/trapsheet.png");
  Texture2D lowLight = LoadTexture("assets/lowLight.png");
  Texture2D highLight = LoadTexture("assets/highLight.png");
  Texture2D torchSprite = LoadTexture("assets/Torch Animated.png");
  Texture2D hearts = LoadTexture("assets/heartsheet.png");
  Texture2D noteSprite = LoadTexture("assets/noteTiny.png");
  Enemy ghost;
  spawnGhost(&ghost, ghostSprite, {600, 400});


  Enemy traps[numTraps];
  for (int i = 0; i < numTraps; i++) {
      createTrap(&traps[i], trapSprite, trapPositions[i]);
  }
  
  Item torch;
  createTorch(&torch, torchSprite, torchPosition);

  Item note;
  createNote(&note, noteSprite);
  

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

    for (int i = 0; i < numTraps; i++) {
      if (checkTrapCollision(&player, &traps[i]) && traps[i].active) {
          player.currentHealth -= 1;
          traps[i].active = false;
          traps[i].deactivationTime = GetTime();
          // Maybe play a sound or trigger a visual effect here
      }
      updateTrapState(&traps[i]);  // Ensure trap is reactivated after reset time
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

      for (int i = 0; i < numTraps; i++) {
        drawTrap(&traps[i]);
      }

      moveGhost(&ghost, &player);
      checkGhostCollision(&ghost, &player);
      drawPlayer(&player);

      if (!torch.pickedUp)
      {
        drawTorch(&torch);
      }
      if (checkItemCollision(&torch, &player) && torch.pickedUp == false)
      {
        player.inventory[0] = "Torch";
        torch.pickedUp = true;
      }

      if (!note.pickedUp)
      {
        drawNote(&note);
      }
      if (checkItemCollision(&note, &player) && note.pickedUp == false)
      {
        player.inventory[1] = "Note";
        note.pickedUp = true;
      }

      EndMode2D();

      // TODO (Remove) Show player pos for debugging - whatever reason, std:: methods were not working for me
      char positionText[50]; 
      sprintf(positionText, "X: %.2f Y: %.2f", player.rect.x, player.rect.y);

      if (torch.pickedUp)
      {
        drawLight(highLight);
      }
      else
      {
        drawLight(lowLight);
      }

      DrawFPS(5, 5);
      drawHearts(hearts, player.currentHealth);
      DrawText(positionText, 900, 10, 32, YELLOW);
    }
    EndDrawing();
  }

  UnloadTMX(map);
  UnloadTexture(hero);
  UnloadTexture(hearts);
  UnloadTexture(lowLight);
  UnloadTexture(highLight);
  UnloadTexture(ghostSprite);
  UnloadTexture(torchSprite);
  CloseWindow();
  return 0;
}
