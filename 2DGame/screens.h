#ifndef SCREENS_H
#define SCREENS_H

#include "game.h"

void drawPauseScreen(Rectangle resumeBtn, Font textFont);

void drawRespawnScreen(Rectangle respawnBtn, Font textFont);

void drawStartScreen(Rectangle respawnBtn, Rectangle quitButton, Font textFont);

void drawWinScreen(Rectangle backToStartButton, Font textFont);

extern bool isPaused;
extern bool inStartScreen;

#endif