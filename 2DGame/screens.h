#ifndef SCREENS_H
#define SCREENS_H

#include "game.h"

void drawPauseScreen(Rectangle resumeBtn, Font textFont);

void drawRespawnScreen(Rectangle respawnBtn, Font textFont);

void drawStartScreen(Rectangle respawnBtn, Rectangle quitButton, Font textFont);

void drawWinScreen(Rectangle playAgainButton, Rectangle backToStartButton, Font textFont, double bestTime);

extern bool isPaused;
extern bool inStartScreen;
extern bool inWinScreen;
extern bool pressedQuit;
extern bool pressedPlayAgain;

#endif