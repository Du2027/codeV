#include <raylib.h>
#include <string>
#include <vector>

class plant {
  Vector2 positionVec;
  double bewaesserungsFequenz; // 1.5 2
  bool heuteBewaessert;        //??
  int textureID;

public:
  plant() {
    this->bewaesserungsFequenz = 0;
    this->heuteBewaessert = false;
    this->positionVec = (Vector2){0, 0};
    this->textureID = -1;
  }
  plant(double bewaesserungsFrequenz, bool heuteBewaessert, Vector2 relPosition, int textureID) {
    this->bewaesserungsFequenz = bewaesserungsFrequenz;
    this->heuteBewaessert = heuteBewaessert;
    this->positionVec = relPosition;
    this->textureID = textureID;
  }

  void bewaessert() { this->heuteBewaessert = true; }
  void changeFrequenz(double frequenz) { this->bewaesserungsFequenz = frequenz; }
  void changeTexture(int textureID) { this->textureID = textureID; }
  void addPosition(Vector2 position) { this->positionVec = position; }
  void changePosition(Vector2 deltaPosition) {
    this->positionVec.x += deltaPosition.x;
    this->positionVec.y += deltaPosition.y;
  }
  float getX() {
    return this->positionVec.x;
  }
  float getY() {
    return this->positionVec.y;
  }
  Rectangle getRec(float plantSize) {
    return (Rectangle){positionVec.x, positionVec.y, plantSize, plantSize};
  }
};

int getPlantIndex(std::vector<plant> plants, float plantSize, Rectangle mouseRec) {
  int selectedIndex = -1;
  for (int i = 0; i < plants.size(); i++) {
    if (CheckCollisionRecs(plants.at(i).getRec(plantSize), mouseRec)) {
      selectedIndex = i;
      break;
    }
  }
  return selectedIndex;
}

int main() {
  InitWindow(0, 0, "TEMP_WINDOW");
  ToggleFullscreen();
  Vector2 windowSize = (Vector2){static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
  CloseWindow();
  InitWindow(windowSize.x, windowSize.y, "Bewaesserung");
  SetTargetFPS(144);

  float plantSize = windowSize.x / 100;
  float menuBarMargin = 14;
  std::string filePath = "";
  bool isHolding = false;
  plant holdPlant;
  std::vector<plant> plants; // Initialisieren???
  bool hasSelected = false;
  int selectedIndex;
  bool newBPressed = false;

  Rectangle menuBarRec = (Rectangle){0, 0, windowSize.x, windowSize.y / 35};

  Image uploadImage = LoadImage("assets/file_up.png");
  Image downloadImage = LoadImage("assets/file_down.png");

  ImageResize(&uploadImage, menuBarRec.height - 17, menuBarRec.height - 20); // nur 5x2 Pixel auf 720p???
  ImageResize(&downloadImage, uploadImage.width, uploadImage.height);

  Texture uploadTexture = LoadTextureFromImage(uploadImage);
  Texture downloadTexture = LoadTextureFromImage(downloadImage);

  Rectangle xRec = (Rectangle){};
  Rectangle uploadRec = (Rectangle){menuBarMargin, (menuBarRec.height - uploadImage.height) / 2, static_cast<float>(uploadImage.width), static_cast<float>(uploadImage.height)};
  Rectangle downloadRec = (Rectangle){menuBarMargin * 2 + uploadRec.width, (menuBarRec.height - uploadImage.height) / 2, static_cast<float>(uploadImage.width), static_cast<float>(uploadImage.height)};
  Rectangle selectionRec = (Rectangle){0, menuBarRec.height, windowSize.x / 6, windowSize.y};
  Rectangle menuBarLinesRec = (Rectangle){downloadRec.x + downloadRec.width + menuBarMargin, uploadRec.y - 2, menuBarRec.width - (menuBarMargin * 4) - (uploadRec.width * 2), uploadRec.height + 4};
  Rectangle newBRec = (Rectangle){selectionRec.width / 16, selectionRec.height - selectionRec.height / 10.0f, (selectionRec.width / 8) * 7, selectionRec.height / 20};

  Color backgroudColor = GetColor(0x2d1c2eff); // 0x as prefix to show its a hexadecimal
  Color secondaryBackgroundColor = GetColor(0x472f3dff);
  Color mainColor = GetColor(0xa99089ff);
  Color mainDarkColor = GetColor(0x281628ff);

  Color newBColor;

  bool shouldClose = false;
  while (!shouldClose) {
    Vector2 mousePos = GetMousePosition();
    Rectangle mouseRec = (Rectangle){mousePos.x, mousePos.y, 1, 1};
    if (IsMouseButtonPressed(0)) {
      // buttons
      if (CheckCollisionRecs(uploadRec, mouseRec)) {
        // WIP
      } else if (CheckCollisionRecs(downloadRec, mouseRec)) {
        // WIP
      } else if (CheckCollisionRecs(newBRec, mouseRec) && isHolding == false) {
        isHolding = true;
        holdPlant = plant();
        holdPlant.addPosition(mousePos);
        holdPlant.changePosition((Vector2){-plantSize, -plantSize}); // because plant is next to cursor
      }

      // plant related
      if (hasSelected && !CheckCollisionRecs(plants.at(selectedIndex).getRec(plantSize), mouseRec)) {
        hasSelected = false;
      }
      if (!hasSelected) {
        selectedIndex = getPlantIndex(plants, plantSize, mouseRec);
        if (selectedIndex > -1) {
          hasSelected = true;
        }
      }
      if (!isHolding && hasSelected && CheckCollisionRecs(mouseRec, plants.at(selectedIndex).getRec(plantSize))) {
        isHolding = true;
        holdPlant = plants.at(selectedIndex);
        plants.erase(plants.begin() + selectedIndex);
        hasSelected = false;
      }
    }
    if (IsMouseButtonDown(0)) {
      if (isHolding == true) {
        holdPlant.changePosition(GetMouseDelta()); // untested
      }
    } else if (isHolding) {
      isHolding = false;
      if (!CheckCollisionRecs(selectionRec, mouseRec)) {
        plants.push_back(holdPlant);
        hasSelected = true;
        selectedIndex = plants.size() - 1;
      }
    }

    if (CheckCollisionRecs(newBRec, mouseRec)) {
      newBColor = mainDarkColor;
      if (IsMouseButtonDown(0)) {
        newBPressed = true;
      } else {
        newBPressed = false;
      }
    } else {
      newBColor = mainColor;
      newBPressed = false;
    }

    BeginDrawing();
    ClearBackground(backgroudColor);
    
    // menu bar
    DrawRectangleRec(menuBarRec, secondaryBackgroundColor);
    DrawRectangleRoundedLines(menuBarLinesRec, 0.2, 8, mainColor);
    DrawTexture(uploadTexture, uploadRec.x, uploadRec.y, WHITE);
    DrawTexture(downloadTexture, downloadRec.x, downloadRec.y, WHITE);

    // selection bar
    DrawRectangleRec(selectionRec, secondaryBackgroundColor);
    DrawRectangleRounded(newBRec, 0.6, 50, newBColor);
    if (newBPressed) {
      DrawRectangleRoundedLines(newBRec, 0.6, 50, mainColor);
    }
    DrawText("+", newBRec.x + newBRec.width / 2, newBRec.y + newBRec.height / 3, 20, WHITE);
    
    // info bar
    // WIP
    EndDrawing();

    Color plantColor;
    for (int i = 0; i < plants.size(); i++) {
      if (i != selectedIndex || hasSelected == false) {
        plantColor = PINK;
      } else {
        plantColor = BLACK;
      }
      DrawRectangle(plants.at(i).getX(), plants.at(i).getY(), plantSize, plantSize, plantColor);
    }

    if (isHolding) {
      DrawRectangleRec(holdPlant.getRec(plantSize), PINK);
    }

    if (WindowShouldClose()) {
      shouldClose = true;
    }
  }
  CloseWindow();
  return 0;
}
