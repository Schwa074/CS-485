#define RAYTMX_IMPLEMENTATION
#include "raytmx.h"
#include "game.h"

// --- Global Sprites Variables ---
Texture2D torchStillSprite;
Texture2D noteSprite;
Texture2D keySprite;
Texture2D swordStillSprite;
// --- End Global Sprites Variables ---

int main() {
    bool firstGame = true;
    double bestTime = 0.0;
    time_t startTime = time(0);
    double elapsedTime;
    bool isSwingingSword = false; // Tracks whether the sword swing animation is active
    bool invisibleZonesCreated = false; // Tracks if invisible zones have been created
    SetTraceLogLevel(LOG_DEBUG);
    TraceLog(LOG_DEBUG, "Opening window");
    InitWindow(W, H, "Crypt Escape");
    InitAudioDevice();

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    CreateHiddenCollisions();

    // Start Screen if statement

    Font noteFont = LoadFont("resources/alagard.png");
    const char* noteMsg = "I'm lost in this\ncrypt, passing by a\nlocked door\nrepeatedly.I found\na booby-trapped\nkey but got\ninjured.\n\nI managed to lift\nthe curse but\nghostly noises are\nmaking me\nparanoid.\n\nGet the key\nand GET OUT\nIMMEDIATELY!\n\n-Howard Carter";
    const char* noteGibberishMsg = "Lorem ipsum dolor\nsit amet consectet\nadipiscing elit\ndo eiusmod tempor\nincididunt ut labore\net dolore magna\n aliqua.\n\nUt enim ad minim\nveniam qui\nnostrud ullamco\nlaboris ni\naliquip\n\nAt vero eos\net IUSTO ODIO\nDIGNISSIMOS\n\n-Howard Carter";


    const char* tmx = "resources/level1.tmx";
    TmxMap* map = LoadTMX(tmx);
    if (map == nullptr) {
        TraceLog(LOG_ERROR, "couldn't load da map: %s", tmx);
        return EXIT_FAILURE;
    }

    //Load Music Section for Background Music
    Music musicStartScreen = LoadMusicStream("assets/Start_Screen.mp3");
    Music musicBackground = LoadMusicStream("assets/Background_Music.mp3");
    Music musicVictory = LoadMusicStream("assets/ffvii_victory_music.mp3");

    //Sound FX Section
    Sound playerGruntSound = LoadSound("assets/Player_Grunt.mp3");
    Sound playerGroanSound = LoadSound("assets/Player_Groan.mp3");
    // Add sword sound

    Font textFont = LoadFontEx("resources/alagard.png", 64, 0, 0);
    Texture2D hero = LoadTexture("assets/charactersheet.png");
    Texture2D whiteGhostSprite = LoadTexture("assets/whiteghostsheet.png");
    Texture2D redGhostSprite = LoadTexture("assets/redghostsheet.png");
    Texture2D blueGhostSprite = LoadTexture("assets/blueghostsheet.png");
    Texture2D trapSprite = LoadTexture("assets/trapsheet.png");
    Texture2D lowLight = LoadTexture("assets/lowLight.png");
    Texture2D highLight = LoadTexture("assets/highLight.png");
    Texture2D torchSprite = LoadTexture("assets/Torch Animated.png");
    Texture2D swordSprite = LoadTexture("assets/sword animated.png");
    Texture2D hearts = LoadTexture("assets/heartsheet.png");
    torchStillSprite = LoadTexture("assets/Torch.png");
    swordStillSprite = LoadTexture("assets/sword.png");
    noteSprite = LoadTexture("assets/noteTiny.png");
    keySprite = LoadTexture("assets/key.png");
    Texture2D noteItemSprite = LoadTexture("assets/noteBig.png");
    Texture2D doorSprite = LoadTexture("assets/DungeonDoor.png");
    Texture2D swordSwingSprite = LoadTexture("assets/swordswingsheet.png");
    Enemy whiteGhost;
    Enemy blueGhost;
    Enemy redGhost;

    deactivateWhiteGhost(&whiteGhost);
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

    Item sword;
    Rectangle swordPos = {2432, 466, 48, 48};
    createItem(&sword, swordSprite, swordPos, "Sword");

    LevelDoor level1_exit;
    level1_exit.toLevel = 2;
    Rectangle level1_exitPos = {2656, 320, 64, 32};
    createLevelDoor(&level1_exit, level1_exitPos);

    LevelDoor level2_entrance;
    level2_entrance.toLevel = 2;
    Rectangle level2_entrancePos = {224, 1408, 64, 32};
    createLevelDoor(&level2_entrance, level2_entrancePos);

    // Level 2 have 3 exits, only one valid - TOP EXIT
    LevelDoor level2_1_exit;
    level2_1_exit.toLevel = 3;
    Rectangle level2_1_exitPos = {3776, 224, 64, 32};
    createLevelDoor(&level2_1_exit, level2_1_exitPos);

    // Level 2 have 3 exits, only one valid - MIDDLE EXIT
    LevelDoor level2_2_exit;
    level2_2_exit.toLevel = 3;
    Rectangle level2_2_exitPos = {3712, 1408, 64, 32};
    createLevelDoor(&level2_2_exit, level2_2_exitPos);

    // Level 2 have 3 exits, only one valid - BOTTOM EXIT
    LevelDoor level2_3_exit;
    level2_3_exit.toLevel = 3;
    Rectangle level2_3_exitPos = {3648, 2464, 64, 32};
    createLevelDoor(&level2_3_exit, level2_3_exitPos);

    LevelDoor level3_entrance;
    level3_entrance.toLevel = 2;
    Rectangle level3_entrancePos = {288, 160, 64, 32};
    createLevelDoor(&level3_entrance, level3_entrancePos);

    LevelDoor level3_exit;
    level3_exit.toLevel = 4;
    Rectangle level3_exitPos = {2144, 1376, 64, 32};
    createLevelDoor(&level3_exit, level3_exitPos);

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
                                                .inventory = std::vector<std::string>(),
                                                .currentLevel = 1
                                                };

    Camera2D camera = (Camera2D){.offset = (Vector2){.x = W / 2.0f, .y = H / 2.0f}, .target = (Vector2){.x = W / 2.0f, .y = H / 2.0f}, .rotation = 0.0f, .zoom = 1.0f};

    //--- Boolean and Int values for Switching Background Music ---
    bool hasStartedGame = false;
    bool hasStartedMenuMusic = false;
    bool hasStartedGameMusic = false;
    bool hasStartedVictoryMusic = false;
    //int previousLevel = player.currentLevel;
    SetMusicVolume(musicBackground, 0.1f);
    SetMusicVolume(musicStartScreen, 0.1f);
    SetMusicVolume(musicVictory, 0.1f);

    while (!WindowShouldClose()) {
        //Keep Music Stream Active
        UpdateMusicStream(musicStartScreen);
        UpdateMusicStream(musicBackground);
        UpdateMusicStream(musicVictory);

// --- Update elapsed time ---
        if (!inStartScreen)
        {
            elapsedTime = difftime(time(0), startTime);
            //std::cout << "Time: " << elapsedTime << std::endl;
        }
        
// --- End update elapsed time ---
        

// --- Check if quit was pressed
        if (pressedQuit)
        {
            std::cout << "Quit was pressed\n";
            break;
        }
// --- End of quit check
  
// --- Check for pause key ---
        if (IsKeyPressed(KEY_P))
        {
            isPaused = !isPaused;
        }
// --- End of check for pause key ---

        // When not paused update game
        if (!isPaused && !inStartScreen)
        {
            // --- Update game ---
            if (!hasStartedGameMusic) {
                StopMusicStream(musicStartScreen);        // Stop menu music
                hasStartedMenuMusic = false;              // Reset menu music flag
                PlayMusicStream(musicBackground);         // Start background music
                hasStartedGameMusic = true;
            }
        
            float previous_x = player.rect.x;
            float previous_y = player.rect.y;
            int prev_health = player.currentHealth;
    
            AnimateTMX(map);
            movePlayer(&player);
    
            // Replace with pause screen 
            if(player.currentHealth != 0) {
                moveRectByVel(&(player.rect), &(player.vel));
            }
    
            if (isTileCollisions(map, &player)) {
                player.rect.x = previous_x;
                player.rect.y = previous_y;
            }
    
            checkDoorCollision(&door, &player);
    
            if(player.currentLevel == 2) {
                whiteGhost.active = false;
                CheckHiddenCollsions(&player);
            }

            // player.currentHealth != 0) prevents bug when player is hurt while dead
            if(player.currentLevel == 1 && player.currentHealth != 0) {
                for (int i = 0; i < numTraps; i++) {
                    if (checkTrapCollision(&player, &traps[i]) && traps[i].active) {
                        player.currentHealth -= 1;
                        traps[i].active = false;
                        traps[i].deactivationTime = GetTime();
                        PlaySound(playerGruntSound);
                    }
                    updateTrapState(&traps[i]);
                }
            }
    
            update_animation(&(player.animations[player.state]));
    
            cameraFollow(&camera, &player);
    
            handleWhiteGhostSpawn(&whiteGhost, whiteGhostSprite);
    
            BeginDrawing();
            {
                ClearBackground(BLACK);
                BeginMode2D(camera);
                DrawTMX(map, &camera, 0, 0, WHITE);
    
    
                drawPlayer(&player);

                if(player.currentLevel == 1) {
                    drawDoor(&door);
                    drawWhiteGhost(&whiteGhost);
        
                    moveGhost(&whiteGhost, &player);
                    checkWhiteGhostCollision(&whiteGhost, &player);

                    for (const auto& ghost : ghosts) {
                        if (ghost.active) {
                            DrawTexture(ghost.sprite, ghost.rect.x, ghost.rect.y, WHITE);
                        }
                    }

                    for (int i = 0; i < numTraps; i++) {
                        drawTrap(&traps[i]);
                    }
    
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
                    if (!sword.pickedUp) {
                        drawSword(&sword);
                    }
                    if (checkItemCollision(&sword, &player) && sword.pickedUp == false) {
                        player.inventory.push_back("Sword");
                        sword.pickedUp = true;
                    }
                }
                if (isSwingingSword) {
                    swingSword(&sword, &player, ghosts, swordSwingSprite);
                    
                    // Check if the animation has completed
                    if (sword.currentFrame == 0 && sword.frameCounter == 0.0f) {
                        isSwingingSword = false; // Animation has finished
                    }
                }
// --- Level Transitions ---
    
                // Level 1 → Level 2
                if (player.currentLevel == 1 && checkLevelDoorCollision(&level1_exit, &player)) {
                    HandleLevelTransition(1, 2, 0, map, player); // Only one path from 1 → 2
                }
    
                // Level 2 → Level 1
                if (player.currentLevel == 2 && checkLevelDoorCollision(&level2_entrance, &player)) {
                    HandleLevelTransition(2, 1, 0, map, player); // Only one path from 2 → 1
                }
    
                // Level 2 → Level 3 (multiple exits)
                if (player.currentLevel == 2) {
                    if (checkLevelDoorCollision(&level2_1_exit, &player)) {
                        HandleLevelTransition(2, 3, 0, map, player); // First way into level 3
                        CreateInvisibleZones(); // Ghost spawning zones for level 3
                        if (!invisibleZonesCreated) {
                            CreateInvisibleZones();
                            invisibleZonesCreated = true;
                        }
                    } else if (checkLevelDoorCollision(&level2_2_exit, &player)) {
                        HandleLevelTransition(2, 3, 1, map, player);
                        CreateInvisibleZones(); // Ghost spawning zones for level 3
                        if (!invisibleZonesCreated) {
                            CreateInvisibleZones();
                            invisibleZonesCreated = true;
                        }
                    } else if (checkLevelDoorCollision(&level2_3_exit, &player)) {
                        HandleLevelTransition(2, 3, 2, map, player);
                        CreateInvisibleZones(); // Ghost spawning zones for level 3
                        if (!invisibleZonesCreated) {
                            CreateInvisibleZones();
                            invisibleZonesCreated = true;
                        }
                    }
                }
    
                // Level 3 -> CORRECT level 2 door
                if (player.currentLevel == 3 && checkLevelDoorCollision(&level3_entrance, &player)) {
                    UnloadTMX(map);
                    player.currentLevel = 2;
                    const char* tmx = mapName[player.currentLevel - 1];
                    map = LoadTMX(tmx);
                
                    if (map == nullptr) {
                        TraceLog(LOG_ERROR, "Failed to load map on return from Level 3");
                        return EXIT_FAILURE;
                    }
                    
                    // Reset the invisible zones flag
                    invisibleZonesCreated = false;
                    // Send player back to the correct return spot based on the last exit used
                    Vector2 returnPos = returnToLevel2From3[lastLevel2ExitUsed];
                    player.rect.x = returnPos.x;
                    player.rect.y = returnPos.y;
                }

                // Level 3 -> 4
                if(player.currentLevel == 3 && checkLevelDoorCollision(&level3_exit, &player)) {
                    HandleLevelTransition(3, 4, 0, map, player);
                    inWinScreen = true;
                    if (firstGame) // if they beat the game the first time
                    {
                        bestTime = elapsedTime;
                        firstGame = false;
                    }
                    else if (elapsedTime < bestTime) // if they beat their best time
                    {
                        bestTime = elapsedTime;
                    }
                }

                // Level 4 -> 1
                if (player.currentLevel == 4 && pressedPlayAgain)
                {
                    pressedPlayAgain = false;
                    inWinScreen = false;
                    HandleLevelTransition(4, 1, 0, map, player);
                }
    
// --- End Level Transitions ---

                if (player.currentLevel == 3) {
                    whiteGhost.active = false;

                    // Check invisible zones and spawn ghosts
                    CheckInvisibleZones(&player, ghosts, redGhostSprite, blueGhostSprite);

                    // Update all ghosts in the ghosts vector
                    for (auto& ghost : ghosts) {
                        if (ghost.active) {
                            // Move the ghost
                            moveGhost(&ghost, &player);

                            DrawTexturePro(ghost.sprite, 
                                {ghost.currentFrame * 32.0f, 0.0f, 32.0f, 32.0f}, // Source rectangle
                                ghost.rect, // Destination rectangle
                                {0, 0},     // Origin
                                0.0f,       // Rotation
                                WHITE);     // Tint

                            // Check for collisions with the player
                            if (CheckCollisionRecs(ghost.rect, player.rect)) {
                                player.currentHealth -= 1; // Example: Reduce player health on collision
                                ghost.active = false;     // Deactivate the ghost after collision
                            }
                        }
                    }
                }

                EndMode2D();
    
// --- Inventory Management ---
                int slotNum = 0;
    
                if (torch.pickedUp && torch.isUsing) {
                    if (!inWinScreen)
                    {
                        drawLight(highLight);
                    }
                } else {
                    if (!inWinScreen)
                    {
                        drawLight(lowLight);
                    }
                }
    
                if (IsKeyPressed(KEY_ONE) && player.inventory.size() > 0) {
                    slotNum = 0;
                    if (player.inventory[0] == "Note") note.isUsing = !note.isUsing;
                    if (player.inventory[0] == "Torch") torch.isUsing = !torch.isUsing;
                    if (player.inventory[0] == "Sword") isSwingingSword = true;
                }
                if (IsKeyPressed(KEY_TWO) && player.inventory.size() > 1) {
                    slotNum = 1;
                    if (player.inventory[1] == "Note") note.isUsing = !note.isUsing;
                    if (player.inventory[1] == "Torch") torch.isUsing = !torch.isUsing;
                    if (player.inventory[1] == "Sword") isSwingingSword = true; 
                }
                if (IsKeyPressed(KEY_THREE) && player.inventory.size() > 2) {
                    slotNum = 2;
                    if (player.inventory[2] == "Note") note.isUsing = !note.isUsing;
                    if (player.inventory[2] == "Torch") torch.isUsing = !torch.isUsing;
                    if (player.inventory[2] == "Sword") isSwingingSword = true; 
                }
                if (IsKeyPressed(KEY_FOUR) && player.inventory.size() > 3) {
                    slotNum = 3;
                    if (player.inventory[3] == "Note") note.isUsing = !note.isUsing;
                    if (player.inventory[3] == "Torch") torch.isUsing = !torch.isUsing;
                    if (player.inventory[3] == "Sword") isSwingingSword = true; 
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
                
    
// --- End Inventory Management ---
    
// --- Misc HUD ---
    
                // TODO (Remove) Show player pos for debugging - whatever reason, std:: methods were not working for me
                if (!inWinScreen)
                {
                    //std::cout << "Time: " << elapsedTime << std::endl;
                    char positionText[50]; 
                    char timeText[50];
                    sprintf(positionText, "X: %.2f Y: %.2f", player.rect.x, player.rect.y);
                    sprintf(timeText, "Time: %.0f", elapsedTime);
                    DrawFPS(5, 5);
                    drawHearts(hearts, player.currentHealth);
                    DrawInventoryHUD(&player, slotNum);
                    DrawText(timeText, 900, 10, 32, RAYWHITE);
                    DrawText(positionText, 900, 42, 32, YELLOW);
                }
                
                
// --- End Misc HUD ---

// --- Win Screen ---
                if (inWinScreen)
                {
                    // --- Update Music to Victory Music ---
                    if (!hasStartedVictoryMusic) {
                        StopMusicStream(musicBackground);        // Stop ingame background music
                        PlayMusicStream(musicVictory);         // Start victory music
                        hasStartedVictoryMusic = true;
                    }
                    Rectangle playAgainBtn = {W / 2 - 150, H / 2 + 100, W / 4, H / 8};
                    Rectangle quitBtn = {W / 2 - 150, H / 2 + 200, W / 4, H / 8};
                    drawWinScreen(playAgainBtn, quitBtn, textFont, bestTime);
                    Vector2 mousePos = GetMousePosition();
                    bool isMouseOverPlayAgain = CheckCollisionPointRec(mousePos, playAgainBtn);
                    bool isMouseOverQuit = CheckCollisionPointRec(mousePos, quitBtn);
                    if (isMouseOverPlayAgain && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        StopMusicStream(musicVictory); // Stop victory music
                        hasStartedGameMusic = false; // Reset game music flag
                        hasStartedVictoryMusic = false; // Reset victory music flag
                        startTime = time(0); // reset start time
                        player.inventory.clear();
                        torch.pickedUp = false;
                        torch.isUsing = false;
                        note.pickedUp = false;
                        note.isUsing = false;
                        key.pickedUp = false;
                        key.isUsing = false;
                        sword.pickedUp = false;
                        sword.isUsing = false;
                        Vector2 mousePos = GetMousePosition();
                        player.currentHealth = 6;
                        player.rect.x = startPosx;
                        player.rect.y = startPosy;
                        spawnWhiteGhost(&whiteGhost, whiteGhostSprite, {2650, 500});
                        pressedPlayAgain = true;
                    }
                    if (isMouseOverQuit && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
                    {
                        pressedQuit = true;
                    }
                }
// --- End of Win Screen
    
// --- Respawn Screen ---
    
                int finalHealth = player.currentHealth;
                if (prev_health != 0 && finalHealth == 0) {
                    PlaySound(playerGroanSound);
                }
                
                if (finalHealth == 0) {
                    Rectangle respawnBtn = {W / 2 - 125, H / 2 + 50, W / 4, H / 8};
                    drawRespawnScreen(respawnBtn, textFont);
                    player.inventory.clear();
                    torch.pickedUp = false;
                    torch.isUsing = false;
                    note.pickedUp = false;
                    note.isUsing = false;
                    key.pickedUp = false;
                    key.isUsing = false;
                    sword.pickedUp = false;
                    sword.isUsing = false;
                    Vector2 mousePos = GetMousePosition();
                    bool isMouseOver = CheckCollisionPointRec(mousePos, respawnBtn);
                    if (isMouseOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        player.currentHealth = 6;
                        player.rect.x = startPosx;
                        player.rect.y = startPosy;
                        spawnWhiteGhost(&whiteGhost, whiteGhostSprite, {2650, 500});
                    }
                }
    
// --- End Respawn Screen ---
            }
            EndDrawing();
        }
// --- Pause Screen ---
        else if(isPaused && !inStartScreen)
        {
            BeginDrawing();
            Rectangle resumeBtn = {W / 2 - 125, H / 2 + 50, W / 4, H / 8};
            drawPauseScreen(resumeBtn, textFont);
            Vector2 mousePos = GetMousePosition();
            bool isMouseOver = CheckCollisionPointRec(mousePos, resumeBtn);
            if (isMouseOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
            {
                isPaused = false;
            }
            EndDrawing();
        }
// --- End of Pause Screen ---
// --- Start Screen ---
        else if(!isPaused && inStartScreen) 
        {
            BeginDrawing();
            ClearBackground(BLACK);
    
            // Create start and quit buttons
            Rectangle startBtn = {W / 2 - 125, H / 2 - 50, W / 4, H / 8};
            Rectangle quitBtn = {W / 2 - 125, H / 2 + 50, W / 4, H / 8};
    
            drawStartScreen(startBtn, quitBtn, textFont);

            static bool hasStartedMenuMusic = false;
            if (!hasStartedMenuMusic) {
                PlayMusicStream(musicStartScreen);
                hasStartedMenuMusic = true;
            }
    
            Vector2 mousePos = GetMousePosition();
            bool isStartOver = CheckCollisionPointRec(mousePos, startBtn);
            bool isQuitOver = CheckCollisionPointRec(mousePos, quitBtn);
    
            if (isStartOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                inStartScreen = false; // Start the game
                hasStartedGame = true;
                StopMusicStream(musicStartScreen);
            }
    
            if (isQuitOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                break; // Quit the game
            }
    
            EndDrawing();
            
        // Do nothing, wait for user to click start or quit
        }
// --- End of Start Screen ---
// --- Win Screen --- Freezes the program
//         if (inWinScreen)
//         {
//             BeginDrawing();
//             DrawTMX(map, &camera, 0, 0, WHITE);
//             drawPlayer(&player);
//             Rectangle quitBtn = {W / 2, H / 2, W / 4, H / 8};
//             drawWinScreen(quitBtn, textFont);
//             Vector2 mousePos = GetMousePosition();
//             bool isMouseOver = CheckCollisionPointRec(mousePos, quitBtn);
//             if (isMouseOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
//             {
//                 std::cout << "Pressed quit\n";
//                 break;
//             }
// }
// --- End of Win Screen

    } // End of while loop
    // else for pause screen goes here

// --- Unload Assets ---

    UnloadFont(textFont);
    UnloadTMX(map);
    UnloadMusicStream(musicStartScreen);
    UnloadMusicStream(musicBackground);
    UnloadMusicStream(musicVictory);
    UnloadSound(playerGruntSound);
    UnloadSound(playerGroanSound);
    UnloadTexture(hero);
    UnloadTexture(hearts);
    UnloadTexture(lowLight);
    UnloadTexture(highLight);
    UnloadTexture(whiteGhostSprite);
    UnloadTexture(redGhostSprite);
    UnloadTexture(blueGhostSprite);
    UnloadTexture(torchSprite);
    UnloadTexture(swordSprite);
    UnloadTexture(keySprite);
    UnloadTexture(noteSprite);
    UnloadTexture(noteItemSprite);
    UnloadTexture(doorSprite);
    CloseAudioDevice();
    UnloadFont(noteFont);
    CloseWindow();

// --- End Unload Assets ---

    return 0;
}
// --- End of main.cpp ---