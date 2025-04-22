#include "screens.h"

bool isPaused = false;
bool inStartScreen = true;
bool inWinScreen = false;
bool pressedQuit = false;
bool pressedPlayAgain = false;

void drawPauseScreen(Rectangle resumeBtn, Font textFont)
{
    const char* pauseMessage = "Game is currently Paused.\nClick resume or press \nP to continue playing.";
    DrawRectangleGradientV(W / 6, H / 6, W / 1.5, H / 1.5, (Color){185, 185, 185, 255}, (Color){100, 100, 100, 100});
    DrawTextEx(textFont, pauseMessage, {W / 2 - 300, H / 2 - 165}, 32.0f, 8, BLACK);
    DrawRectangleRounded(resumeBtn, 1.0f, 1, (Color){70, 70, 80, 255});
    DrawTextEx(textFont, "Resume", (Vector2){W / 2 - 40, H / 2 + 75}, 36.0f, 2, WHITE);
}

void drawRespawnScreen(Rectangle respawnBtn, Font deathFont)
{
    const char* deathMessage = "You have died!\n\n Try again?";
    DrawRectangleGradientV(W / 6, H / 6, W / 1.5, H / 1.5, (Color){120, 6, 6, 255}, BLACK);
    DrawTextEx(deathFont, deathMessage, {W / 2 - 125, H / 2 - 165}, 32.0f, 8, BLACK);
    DrawRectangleRounded(respawnBtn, 1.0f, 1, (Color){70, 70, 80, 255});
    DrawTextEx(deathFont, "Respawn", (Vector2){W / 2 - 40, H / 2 + 75}, 36.0f, 2, WHITE);
}

void drawStartScreen(Rectangle startButton, Rectangle quitButton, Font textFont)
{
    const char* StartTitleMessage = "          Crypt Escape\n  Press Start to Begin Game\n    Press Quit to Exit Game";
    DrawRectangleGradientV(0,0, W, H, (Color){185, 185, 185, 255}, (Color){185, 185, 185, 255});
    DrawTextEx(textFont, StartTitleMessage, {W / 2 - 300, H / 2 - 165}, 32.0f, 8, BLACK);
    DrawRectangleRounded(startButton, 1.0f, 1, (Color){70, 70, 80, 255});
    DrawRectangleRounded(quitButton, 1.0f, 1, (Color){70, 70, 80, 255});
    DrawTextEx(textFont, "Start", (Vector2){W / 2 - 23, H / 2 - 20}, 36.0f, 2, WHITE);
    DrawTextEx(textFont, "Quit", (Vector2){W / 2 - 7, H / 2 + 85}, 36.0f, 2, WHITE);
}

void drawWinScreen(Rectangle playAgainButton, Rectangle quitButton, Font textFont)
{
    const char* winMessage = "Congratulations, you won!\nYou escaped the crypt!";
    //DrawRectangleGradientV(W / 6, H / 6, W / 1.5, H / 1.5, (Color){185, 185, 185, 255}, (Color){100, 100, 100, 100});
    DrawTextEx(textFont, winMessage, {W / 2 - 175, H / 2 - 165}, 32.0f, 8, RAYWHITE);
    DrawRectangleRounded(playAgainButton, 1.0f, 1, (Color){70, 70, 80, 255});
    DrawTextEx(textFont, "Play Again", (Vector2){W / 2 - 100, H / 2 + 130}, 36.0f, 2, WHITE);
    DrawRectangleRounded(quitButton, 1.0f, 1, (Color){70, 70, 80, 255});
    DrawTextEx(textFont, "Quit", (Vector2){W / 2 - 40, H / 2 + 230}, 36.0f, 2, WHITE);
}