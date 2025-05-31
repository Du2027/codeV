#include "constants.h"
#include <raylib.h>

enum currentScreen {
  INTROSCREEN = 0,
  SETTINGSSCREEN = -1,
  GAME = 1
};

const int fpsc = 144;
const Vector2 windowSize = (Vector2){600, 800};

const Color backgroundColor = (Color){255, 0, 255, 255};
