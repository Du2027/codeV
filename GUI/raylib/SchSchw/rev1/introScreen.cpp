#include "classes.h"
#include "constants.h"
#include <cmath>
#include <raylib.h>

Screen IntroScreen(Vector2 windowDeltaScales) {
  InitWindow(windowSize.x * windowDeltaScales.x, windowSize.y * windowDeltaScales.y, "SchSchwGame");
  SetTargetFPS(fpsc);
  bool shouldClose = false;
  Screen nextScreen = LEAVE;
  
  int waterFrameC = 0;
  int waterTextureC = 0;
  double waterTexturePx = 80.0;
  const int waterTextureOffset = 30;
  int waterTextureOffsetC = 0;
  Image waterImages[5] = {LoadImage("assets/water_ani/80/1.png"),
                          LoadImage("assets/water_ani/80/2.png"),
                          LoadImage("assets/water_ani/80/3.png"),
                          LoadImage("assets/water_ani/80/4.png"),
                          LoadImage("assets/water_ani/80/5.png")};

  Texture waterTextures[5];
  for (int i = 0; i < 5; i++) {
    waterTextures[i] = LoadTextureFromImage(waterImages[i]);
  }
  
  Image logo = LoadImage("assets/logo_verz.png");
  ImageResize(&logo, (windowSize.x /3) *2, windowSize.y/4);
  Texture logoTexture = LoadTextureFromImage(logo);

  Image start = LoadImage("assets/start_wood.png");
  ImageResize(&start, (windowSize.x /5) * 2, windowSize.y/6);
  Texture startTexture = LoadTextureFromImage(start);
 
  Image settings = LoadImage("assets/settings_icon.png");
  ImageResize(&settings, (windowSize.x /10), windowSize.y/12);
  Texture settingsTexture = LoadTextureFromImage(settings);
  
  PRect settingsB(500 *windowDeltaScales.x, 700 *windowDeltaScales.y, settings.width, settings.height);
  PRect startB(windowSize.x/2 - start.width/2, 350*windowDeltaScales.y, start.width, start.height); 
  
  while (!shouldClose) {
    windowDeltaScales.x = GetMonitorWidth(0) / 2560.0f;
    windowDeltaScales.y = GetMonitorHeight(0) / 1440.0f;

    if (IsMouseButtonDown(0)) {
      Vector2 mousePos = GetMousePosition();
      Rectangle mouseRec = (Rectangle){mousePos.x, mousePos.y, 1, 1};
      if (CheckCollisionRecs(settingsB.getRect(), mouseRec)) {
        nextScreen = SETTINGSSCREEN; // TODO: change
        shouldClose = true;
      } else if (CheckCollisionRecs(startB.getRect(), mouseRec)) {
        nextScreen = GAME;
        shouldClose = true;
      }
    }

    BeginDrawing();
    ClearBackground(backgroundColor);
   
    // water texture & animation
    for(int i = 0; i < ceil(windowSize.y/waterTexturePx); i++){
      int spritesX = ceil(windowSize.x/waterTexturePx);
      for(int n = 0; n < spritesX + ceil((waterTextureOffset * i) / waterTexturePx *1.0); n++){
        DrawTexture(waterTextures[waterTextureC], waterTexturePx*n - waterTextureOffset*i, waterTexturePx*i, WHITE);
      }
    }
    DrawTexture(logoTexture, windowSize.x/2 - logo.width/2, 50*windowDeltaScales.y, WHITE);
    DrawTexture(startTexture, startB.x, startB.y, WHITE);
   
    DrawTexture(settingsTexture, settingsB.x, settingsB.y, WHITE); 
    EndDrawing();

    if(waterFrameC == (fpsc/6) * 2){
      waterFrameC = 0;
      
      if(waterTextureC == 4){
        waterTextureC = 0;
      }else {waterTextureC++;}
    }
    else {waterFrameC++;}
    
    if (WindowShouldClose()) {
      shouldClose = true;
      nextScreen = LEAVE;
    }
  }
  CloseWindow();
  return nextScreen;
}
