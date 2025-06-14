#include <future>
#include <iostream>
#include <stdexcept>

#include "constants.h"
#include "introScreen.h"
#include "raylib.h"

Screen currentScreen = INTROSCREEN;

int main() {
  InitWindow(0, 0, "TEMP_WINDOW");
  Vector2 monitorSizes = (Vector2){static_cast<float>(GetMonitorWidth(0)), static_cast<float>(GetMonitorHeight(0))}; // seems to be safer than (float)
  Vector2 windowDeltaScale = (Vector2){monitorSizes.x / 2560, monitorSizes.y / 1440};
  CloseWindow();
  std::future<Screen> threadStatus;

  while (currentScreen != LEAVE) {
    switch (currentScreen) {
    case ERRORSCREEN:
      // TODO:
      break;
    case INTROSCREEN:
      threadStatus = std::async(std::launch::async, IntroScreen, windowDeltaScale); // asyncronosly to show error instead of just crashing
      break;
    case SETTINGSSCREEN:
      // TODO:
      break;
    case GAME:
      // TODO:
      break;
    }

    try {
      currentScreen = threadStatus.get();
    } catch (const std::runtime_error &e) {
      printf("THREAD FAILED\n");
      std::cerr << "2nd THREAD FAILED: " << e.what() << std::endl;
    }
  }
}
