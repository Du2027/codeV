#include "errorScreen.h"
#include "constants.h"
#include "raylib.h"

Screen errorScreen(Vector2 windowDeltaScales, const char* message) {
  InitWindow(300 * windowDeltaScales.x, 170 * windowDeltaScales.y, "SchSchwGame");
  SetTargetFPS(fpsc);
  bool shouldClose = false;
  
  while (!shouldClose) {
    windowDeltaScales.x = GetMonitorWidth(0) / 2560.0f;
    windowDeltaScales.y = GetMonitorHeight(0) / 1440.0f;

    if (IsMouseButtonDown(0)) {
      Vector2 mousePos = GetMousePosition();
      Rectangle mouseRec = (Rectangle){mousePos.x, mousePos.y, 1, 1};
    }

    BeginDrawing();
    DrawText(message, 20 * windowDeltaScales.x, 20 * windowDeltaScales.y, 25, BLACK);
    DrawText("X", 220 * windowDeltaScales.x, 100 * windowDeltaScales.y, 45, WHITE);
    ClearBackground(errorSBackgroundColor);
    EndDrawing();

    if (WindowShouldClose()) {
      shouldClose = true;
    }
  }
  CloseWindow();
  return LEAVE;
};
