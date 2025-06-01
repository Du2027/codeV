#include <raylib.h>

#include "classes.h"
#include "constants.h"
#include "gameObjects.h"

int main() {
  InitWindow(0, 0, "TEMP_WINDOW");
  Vector2 monitorSizes = (Vector2){static_cast<float>(GetMonitorWidth(0)), static_cast<float>(GetMonitorHeight(0))}; // seems to be safer than (float)
  Vector2 windowDeltaScale = (Vector2){monitorSizes.x / 2560, monitorSizes.y / 1440};
  CloseWindow();

  InitWindow(windowSize.x * windowDeltaScale.x, windowSize.y * windowDeltaScale.y, "SchSchwGame");
  SetTargetFPS(fpsc);

  while (!WindowShouldClose()) {
    windowDeltaScale.x = GetMonitorWidth(0) / 2560.0f;
    windowDeltaScale.y = GetMonitorHeight(0) / 1440.0f;

    BeginDrawing();
    ClearBackground(backgroundColor);
    EndDrawing();
  }
  return 0;
}
