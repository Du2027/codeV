#pragma once
#include <raylib.h>

enum Screen {
  LEAVE = -2,
  ERRORSCREEN = -1,
  INTROSCREEN = 0,
  SETTINGSSCREEN = 1,
  GAME = 2
};

extern const int fpsc;
extern const Vector2 windowSize;

extern const Color backgroundColor;
extern const Color errorSBackgroundColor;
