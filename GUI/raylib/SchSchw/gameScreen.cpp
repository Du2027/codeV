#include "classes.h"
#include "constants.h"
#include <cmath>
#include <cstdio>
#include <raylib.h>
#include <string>

Screen GameScreen(Vector2 windowDeltaScales) {
  SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
  InitWindow(windowSize.x * windowDeltaScales.x, windowSize.y * windowDeltaScales.y, "SchSchwGame");
  SetTargetFPS(fpsc);

  int waterC = 1;

  bool shouldClose = false;
  bool inShop = false;
  Screen nextScreen = INTROSCREEN;

  int waterFrameC = 0;
  int waterTextureC = 0;
  double waterTexturePx = 80.0;
  const int waterTextureOffsetX = 30;
  const int waterTextureOffsetY = 5;
  int waterTextureOffsetYSum = 0;
  const int waterTextureOffsetMax = 10;
  int waterTextureOffsetC = 0;
  char waterTextureOffsetTendenz = 'u'; // u/d
  Image waterImages[5] = {LoadImage("assets/water_ani/80/1.png"),
                          LoadImage("assets/water_ani/80/2.png"),
                          LoadImage("assets/water_ani/80/3.png"),
                          LoadImage("assets/water_ani/80/4.png"),
                          LoadImage("assets/water_ani/80/5.png")};

  Texture waterTextures[5];
  for (int i = 0; i < 5; i++) {
    waterTextures[i] = LoadTextureFromImage(waterImages[i]);
  }

  Image background = LoadImage("assets/background.png");
  Texture backgroundTexture = LoadTextureFromImage(background);

  Image settings = LoadImage("assets/settings_icon.png");
  ImageResize(&settings, (windowSize.x / 10), windowSize.y / 12);
  Texture settingsTexture = LoadTextureFromImage(settings);

  Image bookmark = LoadImage("assets/bookmark.png");
  Texture bookmarkTexture = LoadTextureFromImage(bookmark);

  Image store = LoadImage("assets/store_icon.png");
  ImageResize(&store, (windowSize.x / 14), windowSize.y / 20);
  Texture storeTexture = LoadTextureFromImage(store);

  Image pump = LoadImage("assets/pump.png");
  ImageResize(&pump, (windowSize.x / 8), windowSize.y / 6);
  Texture pumpTexture = LoadTextureFromImage(pump);
  
  PRect settingsB(500 * windowDeltaScales.x, 700 * windowDeltaScales.y, settings.width, settings.height);
  PRect bookmarkRec((windowSize.x / 8) * 6, -50 * windowDeltaScales.y, bookmark.width, bookmark.height);

  int shopMargin = 50;
  int shopFrameC = 0;
  PRect shopRec(shopMargin * windowDeltaScales.x, shopMargin * windowDeltaScales.y, windowSize.x - shopMargin * 2 * windowDeltaScales.x, windowSize.y - shopMargin * 2 * windowDeltaScales.y);
  int shopStartPos = -shopRec.height - shopMargin;
  int shopRelPos = shopRec.height + shopMargin;
  int shopAniStep = 0;

  Image storeBg = LoadImage("assets/store_background.png");
  ImageResize(&storeBg, shopRec.width, shopRec.height);
  Texture storeBgTexture = LoadTextureFromImage(storeBg);

  shopRec.y = shopStartPos;
  
  while (!shouldClose) {
    windowDeltaScales.x = GetMonitorWidth(0) / 2560.0f;
    windowDeltaScales.y = GetMonitorHeight(0) / 1440.0f;

    if (IsMouseButtonPressed(0)) {
      Vector2 mousePos = GetMousePosition();
      Rectangle mouseRec = (Rectangle){mousePos.x, mousePos.y, 1, 1};
      if (CheckCollisionRecs(settingsB.getRect(), mouseRec)) {
        shouldClose = true;
        nextScreen = SETTINGSSCREEN;
      } else if (CheckCollisionRecs(mouseRec, bookmarkRec.getRect())) {
        (inShop == true) ? inShop=false : inShop=true;
      } else if (!inShop) {
        waterC++;
      }
    }

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexture(backgroundTexture, 0, 0 - background.height + windowSize.y, (Color){255, 255, 255, 200});

    // water texture & animation
    int spritesY = ceil((waterC + 10) / waterTexturePx * 1.0);
    for (int i = 0; i < spritesY; i++) {
      int spritesX = ceil(windowSize.x / waterTexturePx);
      waterTextureOffsetYSum = 0;
      for (int n = 0; n < spritesX + ceil((waterTextureOffsetX * i) / waterTexturePx * 1.0); n++) {

        // y offset pendling between OffsetMax and -OffsetMax
        if (waterTextureOffsetTendenz == 'u' && waterTextureOffsetYSum < waterTextureOffsetMax) {
          waterTextureOffsetYSum += waterTextureOffsetY;
        } else if (waterTextureOffsetTendenz == 'd' && waterTextureOffsetYSum > -waterTextureOffsetMax) {
          waterTextureOffsetYSum -= waterTextureOffsetY;
        }
        if (waterTextureOffsetYSum == waterTextureOffsetMax) {
          waterTextureOffsetTendenz = 'd';
        } else if (waterTextureOffsetYSum == -waterTextureOffsetMax) {
          waterTextureOffsetTendenz = 'u';
        }

        DrawTexture(waterTextures[waterTextureC], waterTexturePx * n /*-waterTextureOffsetX * i*/, (windowSize.y - waterC + waterTexturePx * i) - waterTextureOffsetYSum, WHITE);
      }
    }

    // Draw waterC as Text
    std::string waterCstr = std::to_string(waterC);
    DrawText(waterCstr.data(), (windowSize.x / 2 - MeasureText(waterCstr.data(), 64) / 2) * windowDeltaScales.x, 90, 64, WHITE);

    if (inShop) {
      if (shopAniStep < 8) {
        if (shopFrameC <= fpsc / 160) {
          shopFrameC++;
        } else {
          shopFrameC = 0;
          shopAniStep++;
        }
        shopRec.y = shopStartPos + shopMargin + (shopRelPos / 8.0) * shopAniStep;
      }
      
    } else if (shopAniStep > 0) {
      if (shopFrameC <= fpsc / 160) {
        shopFrameC++;
      } else {
        shopFrameC = 0;
        shopAniStep--;
      }
      shopRec.y = shopStartPos + shopMargin + (shopRelPos / 8.0) * shopAniStep;
    }
    
    //DrawTexture(storeBgTexture, shopRec.x, shopRec.y, WHITE); 
    DrawRectangleRounded(shopRec.getRect(), 0.1, 50, (Color) {130, 130, 130, 200});
    DrawTexture(pumpTexture, 100 + shopRec.x, 150 + shopRec.y, WHITE);
    
    DrawTexture(bookmarkTexture, bookmarkRec.x, bookmarkRec.y, WHITE);
    DrawTexture(storeTexture, (windowSize.x / 8) * 6 + 6 * windowDeltaScales.x, 150 * windowDeltaScales.y, WHITE);

    DrawTexture(settingsTexture, settingsB.x, settingsB.y, WHITE);
    EndDrawing();

    if (waterFrameC == (fpsc / 6) * 2) {
      waterFrameC = 0;

      if (waterTextureC == 4) {
        waterTextureC = 0;
      } else {
        waterTextureC++;
      }
    } else {
      waterFrameC++;
    }

    if (WindowShouldClose()) {
      shouldClose = true;
      nextScreen = LEAVE;
    }
  }
  CloseWindow();
  return nextScreen;
}
