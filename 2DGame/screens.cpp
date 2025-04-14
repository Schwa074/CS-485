#include "screens.h"

void drawPauseScreen(Rectangle resumeBtn)
{

}

void drawRespawnScreen(Rectangle respawnBtn)
{
    std::cout << "In draw respawn screen method\n";
    Font deathFont = LoadFontEx("resources/KAISG.ttf", 64, 0, 0);
    const char* deathMessage = "You have died!\n\n Try again?";
    DrawRectangleGradientV(W / 6, H / 6, W / 1.5, H / 1.5, (Color){120, 6, 6, 255}, BLACK);
    DrawTextEx(deathFont, deathMessage, {W / 2 - 125, H / 2 - 165}, 32.0f, 8, BLACK);
    DrawRectangleRounded(respawnBtn, 1.0f, 1, (Color){70, 70, 80, 255});
    DrawTextEx(deathFont, "Respawn", (Vector2){W / 2 - 40, H / 2 + 75}, 36.0f, 2, WHITE);
    UnloadFont(deathFont);
}

void drawStartScreen(Rectangle startButton, Rectangle quitButton)
{
    
}