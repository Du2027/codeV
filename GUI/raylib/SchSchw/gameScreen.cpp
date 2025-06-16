#include "classes.h"
#include "constants.h"
#include "gameObjects.h"
#include <raylib.h>
#include <string>

Screen GameScreen(Vector2 windowDeltaScales) {
  SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
  InitWindow(windowSize.x * windowDeltaScales.x, windowSize.y * windowDeltaScales.y, "SchSchwGame");
  SetTargetFPS(fpsc);

  int waterC = 1;

  bool shouldClose = false;
  Screen nextScreen = INTROSCREEN;

  while (!shouldClose) {
    windowDeltaScales.x = GetMonitorWidth(0) / 2560.0f;
    windowDeltaScales.y = GetMonitorHeight(0) / 1440.0f;

    if (IsMouseButtonPressed(0)) {
      Vector2 mousePos = GetMousePosition();
      Rectangle mouseRec = (Rectangle){mousePos.x, mousePos.y, 1, 1};
      if (CheckCollisionRecs(settingsB.getRect(), mouseRec)) {
        shouldClose = true;
        nextScreen = SETTINGSSCREEN;
      } else {
        waterC++;
      }
    }

    BeginDrawing();
    ClearBackground(backgroundColor);

    DrawRectangle(0, (windowSize.y - waterC) * windowDeltaScales.x, (windowSize.x) * windowDeltaScales.x, 1 + waterC, BLUE);

    // Draw waterC as Text
    std::string waterCstr = std::to_string(waterC);
    DrawText(waterCstr.data(), (windowSize.x / 2 - MeasureText(waterCstr.data(), 64) / 2) * windowDeltaScales.x, 90, 64, WHITE);

    DrawRectangleRec(settingsB.getRect(), RED);
    EndDrawing();

    if (WindowShouldClose()) {
      shouldClose = true;
      nextScreen = LEAVE;
    }
  }
  CloseWindow();
  return nextScreen;
}
