
#define RAYTMX_IMPLEMENTATION
#include "raytmx.h"
#include "game.h"

Vector2 torchPosition = {1500, 600};
std::vector<Enemy> traps;
Vector2 trapPositions[] = {
    {1984, 1884}, {1952, 1884}, {1920, 1884}, {1888, 1884}, {1856, 1884}, {1824, 1884},
    {2144, 1884}, {2112, 1916}, {2144, 1948}, {2176, 1980}, {2144, 2012}, {2176, 2044},
    {2144, 2076}, {2112, 2108}, {1984, 1916}, {1984, 1948}, {1984, 1980}, {1440, 2108},
    {1472, 2108}, {1504, 2108}, {1536, 2108}, {1728, 2108}, {1760, 2108}, {1792, 2108},
    {1824, 2108}, {1856, 2108}, {1888, 2108}, {1920, 2108}, {1728, 2076}, {1760, 2076},
    {1792, 2076}, {1824, 2044}, {1504, 1884}, {1472, 1916}, {1440, 1884}, {1536, 1916},
    {1536, 1948}, {1536, 1980}, {1568, 1948}, {1600, 1916}, {1632, 1948}, {1664, 1916},
    {1696, 1948}, {1728, 1916}, {1760, 1884},
};

int numTraps = sizeof(trapPositions) / sizeof(trapPositions[0]);

int main() {
    SetTraceLogLevel(LOG_DEBUG);
    TraceLog(LOG_DEBUG, "Opening window");
    InitWindow(W, H, "Crypt Escape");
    InitAudioDevice();

    Font noteFont = LoadFont("resources/alagard.png");
    const char* noteMsg = "I'm lost in this\ncrypt, passing by a\nlocked door\nrepeatedly.I found\na booby-trapped\nkey but got\ninjured.\n\nI managed to lift\nthe curse but\nghostly noises are\nmaking me\nparanoid.\n\nGet the key\nand GET OUT\nIMMEDIATELY!\n\n-Howard Carter";
    const char* noteGibberishMsg = "Lorem ipsum dolor\nsit amet consectet\nadipiscing elit\ndo eiusmod tempor\nincididunt ut labore\net dolore magna\n aliqua.\n\nUt enim ad minim\nveniam qui\nnostrud ullamco\nlaboris ni\naliquip\n\nAt vero eos\net IUSTO ODIO\nDIGNISSIMOS\n\n-Howard Carter";
    Font deathFont = LoadFontEx("resources/KAISG.ttf", 64, 0, 0);
    const char* deathMessage = "You have died!\n\n Try again?";

    const char* tmx = "resources/map.tmx";
    TmxMap* map = LoadTMX(tmx);
    if (map == nullptr) {
        TraceLog(LOG_ERROR, "couldn't load da map: %s", tmx);
        return EXIT_FAILURE;
    }

    Sound playerGruntSound = LoadSound("assets/Player_Grunt.mp3");
    Sound playerGroanSound = LoadSound("assets/Player_Groan.mp3");
    Texture2D hero = LoadTexture("assets/charactersheet.png");
    Texture2D ghostSprite = LoadTexture("assets/ghostsheet.png");
    Texture2D trapSprite = LoadTexture("assets/trapsheet.png");
    Texture2D lowLight = LoadTexture("assets/lowLight.png");
    Texture2D highLight = LoadTexture("assets/highLight.png");
    Texture2D torchSprite = LoadTexture("assets/Torch Animated.png");
    Texture2D hearts = LoadTexture("assets/heartsheet.png");
    Texture2D noteSprite = LoadTexture("assets/noteTiny.png");
    Texture2D noteItemSprite = LoadTexture("assets/noteBig.png");
    Texture2D keySprite = LoadTexture("assets/key.png");
    Texture2D doorSprite = LoadTexture("assets/DungeonDoor.png");
    Enemy ghost;

    deactivateGhost(&ghost);
    Door door;
    createDoor(&door, doorSprite, {2751, 710});
    Enemy traps[numTraps];

    for (int i = 0; i < numTraps; i++) {
        createTrap(&traps[i], trapSprite, trapPositions[i]);
    }

    Item torch;
    Rectangle torchPos = {1476, 635, 32, 32};
    createItem(&torch, torchSprite, torchPos, "Torch");

    Item note;
    Rectangle notePos = {2976, 2720, 32, 32};
    createItem(&note, noteSprite, notePos, "Note");

    Item key;
    Rectangle keyPos = {1440, 1980, 32, 32};
    createItem(&key, keySprite, keyPos, "Key");

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
                                                }},
                                                .maxHealth = 6,
                                                .currentHealth = 6,
                                                .inventory = std::vector<std::string>()};

    Camera2D camera = (Camera2D){.offset = (Vector2){.x = W / 2.0f, .y = H / 2.0f}, .target = (Vector2){.x = W / 2.0f, .y = H / 2.0f}, .rotation = 0.0f, .zoom = 1.0f};

    while (!WindowShouldClose()) {
        float previous_x = player.rect.x;
        float previous_y = player.rect.y;
        int prev_health = player.currentHealth;

        AnimateTMX(map);
        movePlayer(&player);
        moveRectByVel(&(player.rect), &(player.vel));

        if (isTileCollisions(map, &player)) {
            player.rect.x = previous_x;
            player.rect.y = previous_y;
        }

        checkDoorCollision(&door, &player);

        for (int i = 0; i < numTraps; i++) {
            if (checkTrapCollision(&player, &traps[i]) && traps[i].active) {
                player.currentHealth -= 1;
                traps[i].active = false;
                traps[i].deactivationTime = GetTime();
                PlaySound(playerGruntSound);
            }
            updateTrapState(&traps[i]);
        }

        update_animation(&(player.animations[player.state]));

        cameraFollow(&camera, &player);

        handleGhostSpawn(&ghost, ghostSprite);

        BeginDrawing();
        {
            ClearBackground(BLACK);
            BeginMode2D(camera);

            DrawTMX(map, &camera, 0, 0, WHITE);
            drawDoor(&door);
            drawGhost(&ghost);

            for (int i = 0; i < numTraps; i++) {
                drawTrap(&traps[i]);
            }

            moveGhost(&ghost, &player);
            checkGhostCollision(&ghost, &player);

            drawPlayer(&player);

            if (!torch.pickedUp) {
                drawTorch(&torch);
            }
            if (checkItemCollision(&torch, &player) && torch.pickedUp == false) {
                player.inventory.push_back("Torch");
                torch.pickedUp = true;
                torch.isUsing = true;
            }

            if (!note.pickedUp) {
                drawItem(&note);
            }
            if (checkItemCollision(&note, &player) && note.pickedUp == false) {
                player.inventory.push_back("Note");
                note.pickedUp = true;
                note.isUsing = true;
            }

            if (!key.pickedUp) {
                drawItem(&key);
            }
            if (checkItemCollision(&key, &player) && key.pickedUp == false) {
                player.inventory.push_back("Key");
                key.pickedUp = true;
            }

            EndMode2D();

            // TODO (Remove) Show player pos for debugging - whatever reason, std:: methods were not working for me
            char positionText[50]; 
            sprintf(positionText, "X: %.2f Y: %.2f", player.rect.x, player.rect.y);

            if (torch.pickedUp && torch.isUsing) {
                drawLight(highLight);
            } else {
                drawLight(lowLight);
            }

            if (IsKeyPressed(KEY_ONE) && player.inventory.size() > 0) {
                if (player.inventory[0] == "Note") note.isUsing = !note.isUsing;
                if (player.inventory[0] == "Torch") torch.isUsing = !torch.isUsing;
            }
            if (IsKeyPressed(KEY_TWO) && player.inventory.size() > 1) {
                if (player.inventory[1] == "Note") note.isUsing = !note.isUsing;
                if (player.inventory[1] == "Torch") torch.isUsing = !torch.isUsing;
            }
            if (IsKeyPressed(KEY_THREE) && player.inventory.size() > 2) {
                if (player.inventory[2] == "Note") note.isUsing = !note.isUsing;
                if (player.inventory[2] == "Torch") torch.isUsing = !torch.isUsing;
            }

            if (note.isUsing) {
                const char* msg;
                if (std::find(player.inventory.begin(), player.inventory.end(), "Torch") != player.inventory.end() && torch.isUsing) {
                    msg = noteMsg;
                } else {
                    msg = noteGibberishMsg;
                }
                DrawTexture(noteItemSprite, W / 2 - 160, H / 2 - 234, WHITE);
                DrawTextEx(noteFont, msg, {W / 2 - 100, H / 2 - 165}, 16.0f, 8, BLACK);
            }

            DrawFPS(5, 5);
            drawHearts(hearts, player.currentHealth);
            DrawInventoryHUD(&player);
            DrawText(positionText, 900, 10, 32, YELLOW);

            int finalHealth = player.currentHealth;
            if (prev_health != 0 && finalHealth == 0) {
                PlaySound(playerGroanSound);
            }

            if (finalHealth == 0) {
                DrawRectangleGradientV(W / 6, H / 6, W / 1.5, H / 1.5, (Color){120, 6, 6, 255}, BLACK);
                DrawTextEx(deathFont, deathMessage, {W / 2 - 125, H / 2 - 165}, 32.0f, 8, BLACK);
                Rectangle respawnBtn = {W / 2 - 125, H / 2 + 50, W / 4, H / 8};
                DrawRectangleRounded(respawnBtn, 1.0f, 1, (Color){70, 70, 80, 255});
                DrawTextEx(deathFont, "Respawn", (Vector2){W / 2 - 40, H / 2 + 75}, 36.0f, 2, WHITE);
                player.inventory.clear();
                torch.pickedUp = false;
                torch.isUsing = false;
                note.pickedUp = false;
                note.isUsing = false;
                key.pickedUp = false;
                key.isUsing = false;
                Vector2 mousePos = GetMousePosition();
                bool isMouseOver = CheckCollisionPointRec(mousePos, respawnBtn);
                if (isMouseOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    player.currentHealth = 6;
                    player.rect.x = startPosx;
                    player.rect.y = startPosy;
                    spawnGhost(&ghost, ghostSprite, {2650, 500});
                }
            }

        }
        EndDrawing();
    }

    UnloadSound(playerGruntSound);
    UnloadSound(playerGroanSound);
    UnloadTMX(map);
    UnloadTexture(hero);
    UnloadTexture(hearts);
    UnloadTexture(lowLight);
    UnloadTexture(highLight);
    UnloadTexture(ghostSprite);
    UnloadTexture(torchSprite);
    UnloadTexture(keySprite);
    UnloadTexture(noteSprite);
    UnloadTexture(noteItemSprite);
    UnloadTexture(doorSprite);
    CloseAudioDevice();
    UnloadFont(noteFont);
    UnloadFont(deathFont);
    CloseWindow();

    return 0;
}