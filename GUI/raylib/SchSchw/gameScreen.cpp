#include "classes.h"
#include "constants.h"
#include "gameObjects.h"
#include <raylib.h>

Screen GameScreen(Vector2 windowDeltaScales) {
  InitWindow(windowSize.x * windowDeltaScales.x, windowSize.y * windowDeltaScales.y, "SchSchwGame");
  SetTargetFPS(fpsc);
  bool shouldClose = false;

  while (!shouldClose) {
    windowDeltaScales.x = GetMonitorWidth(0) / 2560.0f;
    windowDeltaScales.y = GetMonitorHeight(0) / 1440.0f;

    if (IsMouseButtonDown(0)) {
      Vector2 mousePos = GetMousePosition();
      Rectangle mouseRec = (Rectangle){mousePos.x, mousePos.y, 1, 1};
      if (CheckCollisionRecs(settingsB.getRect(), mouseRec)) {
        shouldClose = true;
      }
    }

    BeginDrawing();
    ClearBackground(backgroundColor);
    DrawRectangleRec(settingsB.getRect(), RED);
    EndDrawing();

    if (WindowShouldClose()) {
      shouldClose = true;
    }
  }
  CloseWindow();
  return INTROSCREEN;
}
