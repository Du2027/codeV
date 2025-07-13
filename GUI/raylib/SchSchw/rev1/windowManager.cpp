#include <future>
#include <iostream>
#include <stdexcept>

#include "raylib.h"

#include "constants.h"
#include "errorScreen.h"
#include "gameScreen.h"
#include "introScreen.h"
#include "settingsScreen.h"

Screen currentScreen = INTROSCREEN;
Screen lastScreen = INTROSCREEN;

int main() {
  InitWindow(0, 0, "TEMP_WINDOW");
  Vector2 monitorSizes = (Vector2){static_cast<float>(GetMonitorWidth(0)), static_cast<float>(GetMonitorHeight(0))}; // seems to be safer than (float)
  Vector2 windowDeltaScale = (Vector2){monitorSizes.x / 2560, monitorSizes.y / 1440};
  CloseWindow();
  std::future<Screen> threadStatus;

  while (currentScreen != LEAVE) {
    switch (currentScreen) {
    case ERRORSCREEN:
      threadStatus = std::async(std::launch::async, errorScreen, windowDeltaScale, "An Error with the Thread occured!");
      break;
    case INTROSCREEN:
      threadStatus = std::async(std::launch::async, IntroScreen, windowDeltaScale); // asyncronosly to show error instead of just crashing
      lastScreen = INTROSCREEN;
      break;
    case SETTINGSSCREEN:
      threadStatus = std::async(std::launch::async, SettingsScreen, windowDeltaScale, lastScreen);
      lastScreen = SETTINGSSCREEN;
      break;
    case GAME:
      threadStatus = std::async(std::launch::async, GameScreen, windowDeltaScale);
      lastScreen = GAME;
      break;
    }

    try {
      currentScreen = threadStatus.get();
    } catch (const std::runtime_error &e) {
      std::cerr << "2nd THREAD FAILED: " << e.what() << std::endl;
      currentScreen = ERRORSCREEN;
    }
  }
}
