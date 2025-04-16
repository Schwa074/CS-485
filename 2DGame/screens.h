#ifndef SCREENS_H
#define SCREENS_H

#include "game.h"

void drawPauseScreen(Rectangle resumeBtn);

void drawRespawnScreen(Rectangle respawnBtn);

void drawStartScreen(Rectangle respawnBtn, Rectangle quitButton);

extern bool isPaused;

#endif