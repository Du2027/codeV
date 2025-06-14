#include "classes.h"
#include "constants.h"
#include "gameObjects.h"
#include <raylib.h>

Screen IntroScreen(Vector2 windowDeltaScale) {
  InitWindow(windowSize.x * windowDeltaScale.x, windowSize.y * windowDeltaScale.y, "SchSchwGame");
  SetTargetFPS(fpsc);
  bool shouldClose = false;
  Screen nextScreen = LEAVE;

  while (!shouldClose) {
    windowDeltaScale.x = GetMonitorWidth(0) / 2560.0f;
    windowDeltaScale.y = GetMonitorHeight(0) / 1440.0f;

    if (IsMouseButtonDown(0)) {
      Vector2 mousePos = GetMousePosition();
      Rectangle mouseRec = (Rectangle){mousePos.x, mousePos.y, 1, 1};
      if (CheckCollisionRecs(settingsB.getRect(), mouseRec)) {
        nextScreen = SETTINGSSCREEN;
        shouldClose = true;
      }
    }

    BeginDrawing();
    ClearBackground(backgroundColor);
    DrawRectangleRec(settingsB.getRect(), RED);
    DrawText("Settings", settingsB.x, settingsB.y, 20, BLACK);
    EndDrawing();

    if (WindowShouldClose()) {
      shouldClose = true;
    }
  }
  return nextScreen;
}
