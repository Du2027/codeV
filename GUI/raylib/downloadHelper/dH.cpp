/*
 * TODO
 * Table with Thread state (If thread finished, then get return, if not 0, ERR)
 * Path ??
 * more FPS
 */

#include <cmath>
#include <cstdlib>
#include <future>
#include <raylib.h>
#include <string>
#include <vector>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define FPS 30

enum Mode { ENTER = 1,
            NORMAL = 0 };

enum DownloadSite { SPOTIFY = 1,
                    YOUTUBE = 0 };

char *vector_to_str(std::vector<char> charVec, int charVecSize, char *tmpCharArr) {
  for (int i = 0; i < charVec.size(); i++) {
    tmpCharArr[i] = charVec.at(i);
  }
  tmpCharArr[charVec.size()] = 0;
  return tmpCharArr;
}

int download(char *link, int linkSize, DownloadSite site) {
  char tmp[linkSize + 10];
  switch (site) {
  case SPOTIFY:
    strcpy(tmp, "spotdl '");
    break;
  case YOUTUBE:
    strcpy(tmp, "yt-dlp '");
    break;
  }

  strcat(tmp, link);
  strcat(tmp, "'");
  tmp[linkSize + 10] = 0;

  return system(tmp);
}

int main() {
  InitWindow(0, 0, "TMPWINDOW");
  int monitorSizes[] = {GetMonitorWidth(0), GetMonitorHeight(0)};
  CloseWindow();

  InitWindow(monitorSizes[0] / 3, monitorSizes[1] / 2.8, "Download helper");
  SetTargetFPS(FPS);

  float monitorDeltaScaleX = GetMonitorWidth(0) / 2560.0f;
  float monitorDeltaScaleY = GetMonitorHeight(0) / 1440.0f;

  Rectangle inputField = {50 * monitorDeltaScaleX, 225 * monitorDeltaScaleY, 700 * monitorDeltaScaleX, 50 * monitorDeltaScaleY};
  Rectangle quitButton = {810 * monitorDeltaScaleX, 0, 40 * monitorDeltaScaleX, 40 * monitorDeltaScaleY};
  Rectangle slider = {75 * monitorDeltaScaleX, 325 * monitorDeltaScaleY, 650 * monitorDeltaScaleX, 30 * monitorDeltaScaleY};
  Color inputColor = {163, 163, 194, 255};
  Color quitColor;

  bool quitHover = false;
  bool shouldClose = false;
  std::vector<char> link = {};
  char key;
  Mode state = ENTER;
  int textPosition = 0;
  int textWidth = 65;

  short threadCount = 2;
  float sliderValue = threadCount;
  short currentThread = 0;
  std::future<int> results[std::thread::hardware_concurrency()];

  // i dont like it but
  GuiSetStyle(SLIDER, BASE_COLOR_NORMAL, 0xC8C8DEFF);
  GuiSetStyle(SLIDER, BORDER_COLOR_NORMAL, 0x5A5A8CFF);
  GuiSetStyle(SLIDER, BORDER_COLOR_FOCUSED, 0x5A5A8CFF);
  GuiSetStyle(SLIDER, BORDER_COLOR_PRESSED, 0x5A5A8CFF);
  GuiSetStyle(SLIDER, BASE_COLOR_PRESSED, 0x5A5A8CFF);
  GuiSetStyle(SLIDER, TEXT_COLOR_PRESSED, 0x5A5A8C99);

  while (!WindowShouldClose() && !shouldClose) {
    key = GetCharPressed();
    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionRecs(quitButton, (Rectangle){mousePos.x, mousePos.y, 1, 1})) {
      quitHover = true;
    } else {
      quitHover = false;
    }

    if (state == ENTER) {
      if (key > 31 && key < 127) {
        link.push_back(key);
      } else if (IsKeyDown(KEY_BACKSPACE) && link.size() > 0) {
        link.pop_back();
      } else if (IsKeyPressed(KEY_INSERT)) {
        char clipTxt[2000] = {0}; // REALLY unsafe, but no other opiton
        strcpy(clipTxt, GetClipboardText());
        for (int i = 0; i < 1000; i++) {
          if (clipTxt[i] != 0) {
            link.push_back(clipTxt[i]);
          }
        }
      } else if (IsKeyPressed(KEY_DELETE)) {
        link.clear();
      } else if (IsKeyPressed(KEY_ENTER)) {
        char *toDownloadLink = (char *)malloc(sizeof(char) * link.size() + 1);
        for (int i = 0; i < link.size() + 1; i++) {
          toDownloadLink[i] = 0;
        }
        vector_to_str(link, link.size(), toDownloadLink);

        DownloadSite site;
        if (toDownloadLink[13] == 's') {
          site = SPOTIFY;
        } else {
          site = YOUTUBE;
        }

        results[currentThread] = std::async(std::launch::async, download, toDownloadLink, link.size(), site);
        link.clear();

        currentThread++;
        if (currentThread == threadCount) {
          results[0].wait();
          currentThread = 0;
        }
        // free(toDownloadLink);
      } else if (IsKeyPressed(KEY_RIGHT)) {
        textPosition++;
      } else if (IsKeyPressed(KEY_LEFT) && textPosition > 0) {
        textPosition--;
      }
    }

    if (IsMouseButtonPressed(0)) {
      if (state) {
        if (!CheckCollisionRecs((Rectangle){mousePos.x, mousePos.y, 1, 1}, inputField)) {
          state = NORMAL;
        }
      } else if (CheckCollisionRecs((Rectangle){mousePos.x, mousePos.y, 1, 1}, inputField)) {
        state = ENTER;
      }
      if (CheckCollisionRecs(quitButton, (Rectangle){mousePos.x, mousePos.y, 1, 1})) {
        shouldClose = true;
      }
    }

    if (quitHover) {
      quitColor = (Color){255, 0, 0, 200};
    } else {
      quitColor = (Color){255, 0, 0, 100};
    }

    const char *tmp = std::to_string(threadCount).c_str(); // int to char*
    GuiSliderBar(slider, tmp, "", &sliderValue, 2.0f, (float)std::thread::hardware_concurrency());
    threadCount = std::round(sliderValue);

    BeginDrawing();
    ClearBackground((Color){193, 193, 215, 255});

    DrawText("Thread count", 655 * monitorDeltaScaleX, 359 * monitorDeltaScaleY, 10, (Color){55, 17, 31, 200});
    DrawText("DownloadHelper", 50 * monitorDeltaScaleX, 80 * monitorDeltaScaleY, 60, (Color){55, 17, 31, 200});

    DrawRectangleRounded(inputField, 0.9, 20, inputColor);

    DrawRectangleRounded(quitButton, 0.25, 20, quitColor);
    DrawText("X", 821 * monitorDeltaScaleX, 6 * monitorDeltaScaleY, 30, DARKGRAY);

    if (state) {
      DrawRectangleRoundedLinesEx(inputField, 0.9, 50, 3, (Color){90, 90, 140, 255});
    }

    // Buffering Text so the whole thing isnt written
    if (link.size() > 0) {
      char linkTxt[link.size()];
      vector_to_str(link, link.size(), linkTxt);
      char displayText[textWidth + 1] = {0};
      for (int i = 0; i < textWidth; i++) {
        if (i + textPosition == link.size()) {
          break;
        }
        displayText[i] = link.at(textPosition + i);
      }
      DrawText(displayText, 60 * monitorDeltaScaleX, 242 * monitorDeltaScaleY, 20, BLACK);
    } else {
      DrawText("paste(EINFG) or type link", 60 * monitorDeltaScaleX, 242 * monitorDeltaScaleY, 20, GRAY);
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
