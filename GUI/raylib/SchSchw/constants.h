#pragma once
#include "classes.h"
#include <raylib.h>

enum Screen {
  LEAVE = -2,
  ERRORSCREEN = -1,
  INTROSCREEN = 0,
  SETTINGSSCREEN = 1,
  GAME = 2
};

// settings
extern int fpsc;
extern Vector2 windowSize;

// color-related
extern const Color backgroundColor;
extern const Color errorSBackgroundColor;