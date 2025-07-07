#include <cstdlib>
#include <cstring>
#include <raylib.h>
#include <string>
#define RAYGUI_IMPLEMENTATION
#include "errorMessage.h"
#include "raygui.h"

//---------------------------------------------------------------------------------------------------------------------------
//Doesnt really make sense, bc Smb. would have to draw millions of pixels, too inacurate
//---------------------------------------------------------------------------------------------------------------------------

#define DEBUG 1

enum state {
  INTRO,
  APP,
};

enum error {
  NONE,
  PATH,
  VIEW,
  FILEEXT
};

int main() {
  InitWindow(0, 0, "TEMP_WINDOW");
  Vector2 monitorSizes = (Vector2){static_cast<float>(GetMonitorWidth(0)), static_cast<float>(GetMonitorHeight(0))}; // seems to be safer than (float)
  Vector2 windowSize = (Vector2){monitorSizes.x / 2, monitorSizes.y / 2};
  Vector2 windowDeltaScale = (Vector2){monitorSizes.x / 2560, monitorSizes.y / 1440};
  CloseWindow();

  InitWindow(windowSize.x, windowSize.y, "PoHM-helper");
  SetTargetFPS(144);

  Image workingImage;
  Texture workingTexture;
  bool invalidImageError = false;
  bool shouldClose = false;
  char *path = (char *)calloc(200, sizeof(char)); // auto-0's memory
  if (DEBUG) {
    path = std::strcpy(path, "example.png");
  }

  // @intro screen
  state currentState = INTRO;
  error continuationError = NONE;

  Color viewRecColor = (Color){100, 100, 100, 255};
  Color closeColor;
  Color continueRecColor;
  Color continueTextColor;
  
  Rectangle viewRec = (Rectangle){windowSize.x / 6, 180 * windowDeltaScale.x, (windowSize.x / 3) * 2, windowSize.y / 6};
  Rectangle checkBoxMap = (Rectangle){viewRec.x + viewRec.width / 2, viewRec.y + 70 * windowDeltaScale.y, 20, 20};
  Rectangle checkBoxImg = (Rectangle){viewRec.x + 30 * windowDeltaScale.x, viewRec.y + 70 * windowDeltaScale.y, 20, 20};
  Rectangle inputRec = (Rectangle){(windowSize.x / 4) / 2, 400 * windowDeltaScale.y, (windowSize.x / 4) * 3, 40 * windowDeltaScale.y};
  Rectangle continueRec = (Rectangle){inputRec.x, inputRec.y + 150 * windowDeltaScale.y, inputRec.width, inputRec.height + 10 * windowDeltaScale.y};
  
  int xFontSize = 32;
  int spacing = 10;
  Vector2 xSizes = MeasureTextEx(GetFontDefault(), "X", xFontSize, 0);
  Rectangle xRec = (Rectangle){windowSize.x - xSizes.x - spacing, static_cast<float>(spacing), xSizes.x, xSizes.y};

  bool viewBefore[2] = {false, false}; // Image, Map
  bool viewCurrent[2] = {false, false};
  if (DEBUG) {
    viewCurrent[0] = true;
  }

  // @app screen
  float menuBarMargin = 5;
  Rectangle menuBarRec = (Rectangle){menuBarMargin, menuBarMargin, monitorSizes.x /6, monitorSizes.y - menuBarMargin*2};
  
  Color menuBarColor = (Color){130,130,130,255};

  while (!shouldClose) {
    BeginDrawing();
    ClearBackground(DARKGRAY);
    Vector2 mousePos = GetMousePosition();

    // x-closing
    if (CheckCollisionRecs(xRec, (Rectangle){mousePos.x, mousePos.y, 1, 1})) {
      closeColor = RED;
      if (IsMouseButtonPressed(0)) {
        shouldClose = true;
      }
    } else {
      closeColor = WHITE;
    }
    DrawText("X", xRec.x, xRec.y, xFontSize, closeColor);

    switch (currentState) {
    case INTRO:
      // colorchanges on hover
      if (CheckCollisionRecs(continueRec, (Rectangle){mousePos.x, mousePos.y, 1, 1})) {
        continueRecColor = (Color){60, 60, 60, 255};
        continueTextColor = (Color){180, 180, 180, 255};
      } else {
        continueRecColor = LIGHTGRAY;
        continueTextColor = BLACK;
      }

      // headings
      DrawText("PoHM-helper", (windowSize.x / 2) - (MeasureText("PoHM-helper", 64) / 2), 50 * windowDeltaScale.y, 64, WHITE);
      DrawText("(PixelOrientedHeightMaps)", (windowSize.x / 2) - (MeasureText("(PixelOrientedHeightMaps)", 24) / 2), 120 * windowDeltaScale.y, 24, WHITE);
      DrawText("Path:", inputRec.x, inputRec.y - 45 * windowDeltaScale.y, 32, WHITE);

      // uncheck other checkBox if second is pressed and both would be true
      if (viewCurrent[0] == true && viewCurrent[1] == true) {
        if (viewBefore[0] == true) {
          viewCurrent[0] = false;
          viewCurrent[1] = true;
        }
        if (viewBefore[1] == true) {
          viewCurrent[1] = false;
          viewCurrent[0] = true;
        }
      } else {
        viewBefore[0] = viewCurrent[0];
        viewBefore[1] = viewCurrent[1];
      }

      // view checkBoxes
      DrawRectangleRec(viewRec, viewRecColor);
      DrawText("what to view:", viewRec.x + 20 * windowDeltaScale.x, viewRec.y + 20 * windowDeltaScale.y, 22, WHITE);
      GuiSetStyle(CHECKBOX, TEXT_COLOR_NORMAL, 0xFFFFFFFF);
      GuiSetStyle(CHECKBOX, BORDER_COLOR_NORMAL, 0xFFFFFFFF);
      GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, 0xFFFFFFFF);
      GuiCheckBox(checkBoxImg, "empty Image", &viewCurrent[0]);
      GuiCheckBox(checkBoxMap, "predef Map with src Image", &viewCurrent[1]);

      // textBox
      GuiSetStyle(TEXTBOX, BASE_COLOR_PRESSED, 0x999999FF);
      GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, 0x999999FF);
      GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, 0xFFFFFFFF);
      GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, 0xFFFFFFFF);
      GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
      int maxPathSize;
      if (DEBUG) {
        maxPathSize = 12;
      } else {
        maxPathSize = 200;
      }
      GuiTextBox(inputRec, path, maxPathSize, true);

      // check if continuation is valid
      if (CheckCollisionRecs(continueRec, (Rectangle){mousePos.x, mousePos.y, 1, 1}) && IsMouseButtonPressed(0)) {
        if (!FileExists(path)) {
          continuationError = PATH;
        } else if (!IsFileExtension(path, ".png")) {
          continuationError = FILEEXT;
        } else if (viewCurrent[0] == viewCurrent[1]) {
          continuationError = VIEW;
        } else {
          currentState = APP;
          workingImage = LoadImage(path);
        }
      }

      DrawRectangleRec(continueRec, continueRecColor);
      DrawText("Continue -->", inputRec.x + (inputRec.width / 2) - MeasureText("Continue -->", 22) / 2, inputRec.y + 165 * windowDeltaScale.y, 22, continueTextColor);

      // draw Error
      if (continuationError != NONE) {
        switch (continuationError) {
        case PATH:
          DrawText("NO VALID PATH ENTERED", (windowSize.x / 2) - MeasureText("NO VALID PATH ENTERED", 20) / 2, 620 * windowDeltaScale.y, 20, RED);
          break;
        case FILEEXT:
          DrawText("NO .PNG FILE ENTERED", (windowSize.x / 2) - MeasureText("NO .PNG FILE ENTERED", 20) / 2, 620 * windowDeltaScale.y, 20, RED);
          break;
        case VIEW:
          DrawText("NO VIEW-METHOD SELECTED", (windowSize.x / 2) - MeasureText("NO VIEW-METHOD SELECTED", 20) / 2, 620 * windowDeltaScale.y, 20, RED);
          break;
        }
      }
      break;

    case APP:
      if (windowSize.x == monitorSizes.x / 2) {
        ToggleFullscreen();
        windowSize.x = monitorSizes.x;
        windowSize.y = monitorSizes.y;
      }

      xRec.x = windowSize.x - xSizes.x - spacing;
      xRec.y = static_cast<float>(spacing);

      if (!IsImageValid(workingImage)) {
        shouldClose = true;
        invalidImageError = true;
        ToggleFullscreen();
      }
      
      DrawRectangleRounded(menuBarRec, 0.04, 50, menuBarColor);
      break;
    }
    EndDrawing();

    if (WindowShouldClose()) {
      shouldClose = true;
    }
  }
  CloseWindow();
  if (invalidImageError) {
    showErrorScreen(windowDeltaScale, "Image is Invalid!");
  }
}
