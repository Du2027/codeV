#include "errorMessage.h"
#include <raylib.h>
#include <string>

void showErrorScreen(Vector2 windowDeltaScales, const std::string message) {
  InitWindow(300 * windowDeltaScales.x, 170 * windowDeltaScales.y, "ERROR");
  SetTargetFPS(60);
  bool shouldClose = false;

  while (!WindowShouldClose()) {
    windowDeltaScales.x = GetMonitorWidth(0) / 2560.0f;
    windowDeltaScales.y = GetMonitorHeight(0) / 1440.0f;

    if (IsMouseButtonDown(0)) {
      Vector2 mousePos = GetMousePosition();
      Rectangle mouseRec = (Rectangle){mousePos.x, mousePos.y, 1, 1};
    }

    BeginDrawing();
    DrawText(message.c_str(), 20 * windowDeltaScales.x, 20 * windowDeltaScales.y, 25, BLACK);
    DrawText("X", 220 * windowDeltaScales.x, 100 * windowDeltaScales.y, 45, WHITE);
    ClearBackground((Color){200, 0, 0, 255});
    EndDrawing();
  }
  CloseWindow();
}
