#include "../rayhelper/rayhelper.hpp"
#include <cmath>
#include <cstdio>
#include <format>
#include <fstream>
#include <iostream>
#include <ostream>
#include <raylib.h>
#include <sstream> // Required for std::istringstream
#include <string>
#include <vector>

using namespace PGUI;

#define FPSC 144

class plant {
  std::string title;
  Vector2 positionVec;
  int bewaesserungsFequenz; // 1.5 2
  int nennerFrequenz;       // for 1/2 or 1/8
  int textureID;
  bool bewaessert; //??

public:
  plant() {
    this->title = "";
    this->bewaesserungsFequenz = 1;
    this->nennerFrequenz = 0;
    this->bewaessert = false;
    this->positionVec = (Vector2){0, 0};
    this->textureID = -1;
  }
  plant(std::string title, int bewaesserungsFrequenz, int nennerFrequenz, bool bewaessert, Vector2 relPosition, int textureID) {
    this->title = title;
    this->bewaesserungsFequenz = bewaesserungsFrequenz;
    this->nennerFrequenz = nennerFrequenz;
    this->bewaessert = bewaessert;
    this->positionVec = relPosition;
    this->textureID = textureID;
  }

  void changeFrequenz(double frequenz) { this->bewaesserungsFequenz = frequenz; }
  void changeTexture(int textureID) { this->textureID = textureID; }
  void addPosition(Vector2 position) { this->positionVec = position; }
  void changePosition(Vector2 deltaPosition) {
    this->positionVec.x += deltaPosition.x;
    this->positionVec.y += deltaPosition.y;
  }
  void addToFreq() {
    if (bewaesserungsFequenz > 0) {
      bewaesserungsFequenz++;
    } else if (nennerFrequenz == 2) {
      nennerFrequenz = 1;
      bewaesserungsFequenz = 1;
    } else if (nennerFrequenz > 2) {
      nennerFrequenz = nennerFrequenz / 2;
    }
  }

  void subToFreq() { // sub for substract
    if (bewaesserungsFequenz > 1) {
      bewaesserungsFequenz--;
    } else if (bewaesserungsFequenz == 1) {
      bewaesserungsFequenz = 0;
      nennerFrequenz = 2;
    } else if (nennerFrequenz >= 2) {
      nennerFrequenz = nennerFrequenz * 2;
    }
  }

  bool &getBewaessertRef() {
    return bewaessert;
  }
  int &getTextureIdRef() {
    return textureID;
  }
  std::string &getTitleRef() {
    return title;
  }

  int getBewaesserungsFreq() { return this->bewaesserungsFequenz; }
  int getNennerFreq() { return this->nennerFrequenz; }
  float getX() { return this->positionVec.x; }
  float getY() { return this->positionVec.y; }
  int getTextureId() { return this->textureID; }
  Rectangle getRec(float plantSize) { return (Rectangle){positionVec.x, positionVec.y, plantSize, plantSize}; }
  std::string getTitle() { return this->title; }
  std::string getBewaessertString() { return std::to_string((bewaessert == true) ? 1 : 0); }

  std::string getFreqStr() {
    std::string freqStr = "";
    (nennerFrequenz >= 2) ? freqStr = std::format("1/{}", nennerFrequenz) : freqStr = std::format("{}", bewaesserungsFequenz);
    return freqStr;
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
  SetTargetFPS(FPSC);

  float plantSize = windowSize.x / 50;
  float menuBarMargin = 14;
  float infoBarMargin = 10;
  float fontHeight = MeasureTextEx(GetFontDefault(), "X", 30, 1).y;
  bool isHolding = false;
  plant holdPlant;
  std::vector<plant> plants; // Initialisieren???
  bool hasSelected = false;
  int selectedIndex;
  bool newBPressed = false;
  bool isEditing = false;
  int frequenzyNum = 0;
  int frameCounter = 0;
  int bufferBeginn = 0;

  Rectangle menuBarRec = (Rectangle){0, 0, windowSize.x, windowSize.y / 35};

  Image uploadImage = LoadImage("assets/file_up.png");
  Image downloadImage = LoadImage("assets/file_down.png");
  Image temp_plantImages;

  ImageResize(&uploadImage, menuBarRec.height - 17, menuBarRec.height - 20); // nur 5x2 Pixel auf 720p???
  ImageResize(&downloadImage, uploadImage.width, uploadImage.height);

  Texture uploadTexture = LoadTextureFromImage(uploadImage);
  Texture downloadTexture = LoadTextureFromImage(downloadImage);
  Texture plantTextures[5];
  for (int i = 0; i < 5; i++) {
    temp_plantImages = LoadImage(std::format("assets/plant{}.png", i + 1).c_str());
    ImageResize(&temp_plantImages, static_cast<int>(plantSize), static_cast<int>(plantSize));
    plantTextures[i] = LoadTextureFromImage(temp_plantImages);
  }

  Rectangle uploadRec = (Rectangle){menuBarMargin, (menuBarRec.height - uploadImage.height) / 2, static_cast<float>(uploadImage.width), static_cast<float>(uploadImage.height)};
  Rectangle downloadRec = (Rectangle){menuBarMargin * 2 + uploadRec.width, (menuBarRec.height - uploadImage.height) / 2, static_cast<float>(uploadImage.width), static_cast<float>(uploadImage.height)};

  Rectangle menuBarLinesRec = (Rectangle){downloadRec.x + downloadRec.width + menuBarMargin, uploadRec.y - 2, menuBarRec.width - (menuBarMargin * 4) - (uploadRec.width * 2), uploadRec.height + 4};
  Rectangle newBRec = (Rectangle){PGUI::getMiddle(0, windowSize.x, windowSize.x / 10), windowSize.y - (windowSize.y / 20) - 50, windowSize.x / 20, windowSize.y / 20};

  Rectangle infoBarRec = (Rectangle){(windowSize.x / 6) * 5, menuBarRec.height, windowSize.x / 6, windowSize.y - menuBarRec.height};
  Rectangle metaSectionRec = (Rectangle){infoBarRec.x + infoBarMargin, infoBarRec.y + infoBarRec.height / 2, infoBarRec.width - 2 * infoBarMargin, (infoBarRec.height / 2) - infoBarMargin};
  Rectangle frequenzyChangeRec = (Rectangle){infoBarRec.x + infoBarMargin + 100, infoBarRec.y + infoBarMargin * 8.5f + MeasureTextEx(GetFontDefault(), "X", 30, 1).y * 3 + plantSize, 50, 50};
  Rectangle plantTitleLabelRec = (Rectangle){infoBarRec.x + (infoBarMargin * 2) + MeasureText("title: ", 30), infoBarRec.y + infoBarMargin * 3 + fontHeight, infoBarRec.width - infoBarMargin * 3 - static_cast<float>(MeasureText("title: ", 30)), fontHeight};
  Rectangle wetCheckBoxRec = (Rectangle){infoBarRec.x + infoBarMargin * 2 + MeasureText("Wet: ", 30), infoBarRec.y + infoBarMargin * 10 + fontHeight * 4 + plantSize, 50, 50};
  Rectangle iconsRec = (Rectangle){infoBarRec.x + infoBarMargin, infoBarRec.y + infoBarMargin * 7 + fontHeight * 3, plantSize * 5 + infoBarMargin * 4, plantSize};

  Color backgroudColor = GetColor(0x2d1c2eff); // 0x as prefix to show its a hexadecimal
  Color secondaryBackgroundColor = GetColor(0x472f3dff);
  Color mainColor = GetColor(0xa99089ff);
  Color mainDarkColor = GetColor(0x281628ff);

  Color newBColor;
  Color plantColor;
  Color textureTint;

  bool shouldClose = false;
  while (!shouldClose) {
    Vector2 mousePos = GetMousePosition();
    Rectangle mouseRec = (Rectangle){mousePos.x, mousePos.y, 1, 1};
    if (IsMouseButtonPressed(0) && !isEditing) {
      // buttons
      if (CheckCollisionRecs(uploadRec, mouseRec) && FileExists("save.txt")) {
        plants.clear();
        std::ifstream inputFile("save.txt");
        std::string line;
        int plantc = 0;
        while (getline(inputFile, line)) {
          std::istringstream iss(line); // create an input string stream
          std::string attribute;
          std::vector<std::string> attributes;

          while (std::getline(iss, attribute, ';')) { // read until delimiter
            attributes.push_back(attribute);
          }
          if (attributes.size() != 7) {
            printf("ERR: No 7 Attributes on Line %d\n", plantc);
            continue;
          }
          plants.push_back(plant(attributes[0], std::stoi(attributes[1]), std::stoi(attributes[2]), std::stoi(attributes[3]), (Vector2){std::stof(attributes[4]), std::stof(attributes[5])}, std::stoi(attributes[6])));
          // string title, int bewaesserungsFrequenz, int nennerFrequenz, bool bewaessert, Vector2 relPosition, int textureID
          plantc++;
        }
        inputFile.close();
      } else if (CheckCollisionRecs(downloadRec, mouseRec)) {
        std::ofstream outputFile("save.txt");
        plant savePlant;
        if (outputFile.is_open()) {
          for (int i = 0; i < plants.size(); i++) {
            savePlant = plants.at(i);
            outputFile << savePlant.getTitle() << ";";
            outputFile << std::to_string(savePlant.getBewaesserungsFreq()) << ";";
            outputFile << std::to_string(savePlant.getNennerFreq()) << ";";
            outputFile << savePlant.getBewaessertString() << ";";
            outputFile << std::to_string(savePlant.getX()) << ";";
            outputFile << std::to_string(savePlant.getY()) << ";";
            outputFile << std::to_string(savePlant.getTextureId());
            outputFile << std::endl;
          }
          outputFile.close();
        } else {
          printf("ERR: FILEWRITE\n");
        }
      } else if (CheckCollisionRecs(newBRec, mouseRec) && isHolding == false) {
        isHolding = true;
        holdPlant = plant();
        holdPlant.addPosition(mousePos);
        holdPlant.changePosition((Vector2){-plantSize, -plantSize}); // because plant is next to cursor
      }

      // plant related
      if (hasSelected && !CheckCollisionRecs(plants.at(selectedIndex).getRec(plantSize), mouseRec) && !CheckCollisionRecs(mouseRec, infoBarRec)) {
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
    if (IsMouseButtonDown(0) && !isEditing) {
      if (isHolding == true) {
        holdPlant.changePosition(GetMouseDelta()); // untested
      }
    } else if (isHolding) {
      isHolding = false;
      if (!CheckCollisionRecs(newBRec, mouseRec)) {
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

    if (hasSelected) {
      frequenzyChangeRec.x = metaSectionRec.x + MeasureText(std::format("Freq: every {} Day", plants.at(selectedIndex).getFreqStr()).c_str(), 30) + infoBarMargin * 2;
    }

    switch (frequenzyNum) {
    case -1:
      plants.at(selectedIndex).subToFreq();
      break;
    case 1:
      plants.at(selectedIndex).addToFreq();
      break;
    }

    BeginDrawing();
    ClearBackground(backgroudColor);

    // menu bar
    DrawRectangleRec(menuBarRec, secondaryBackgroundColor);
    DrawRectangleRoundedLines(menuBarLinesRec, 0.2, 8, mainColor);
    DrawTexture(uploadTexture, uploadRec.x, uploadRec.y, WHITE);
    DrawTexture(downloadTexture, downloadRec.x, downloadRec.y, WHITE);

    // selection bar
    DrawRectangleRounded(newBRec, 0.6, 50, newBColor);
    if (newBPressed) {
      DrawRectangleRoundedLines(newBRec, 0.6, 50, mainColor);
    }
    DrawText("+", newBRec.x + (newBRec.width / 2) - (MeasureText("+", 30) / 2), newBRec.y + (newBRec.height / 2) - (MeasureTextEx(GetFontDefault(), "+", 30, 1).y / 2), 30, WHITE);

    for (int i = 0; i < plants.size(); i++) {
      if (i != selectedIndex || hasSelected == false) {
        plantColor = PINK;
        textureTint = WHITE;
      } else {
        plantColor = BLACK;
        textureTint = GRAY;
      }
      if (plants.at(i).getTextureId() <= -1 || plants.at(i).getTextureId() >= 5) {
        DrawRectangle(plants.at(i).getX(), plants.at(i).getY(), plantSize, plantSize, plantColor);
      } else {
        DrawTexture(plantTextures[plants.at(i).getTextureId()], plants.at(i).getX(), plants.at(i).getY(), textureTint);
      }
    }

    if (hasSelected) {
      DrawRectangleRec(infoBarRec, secondaryBackgroundColor);
      DrawRectangleRounded(metaSectionRec, 0.06, 20, mainDarkColor);
      DrawText("Infos", infoBarRec.x + infoBarMargin, infoBarRec.y + infoBarMargin, 38, GRAY);
      DrawText("title:", infoBarRec.x + infoBarMargin, infoBarRec.y + infoBarMargin * 3 + fontHeight, 30, WHITE);
      PGUI::DrawLabel(plantTitleLabelRec, plants.at(selectedIndex).getTitleRef(), 30, isEditing, mousePos, frameCounter, bufferBeginn);

      DrawText("Icon:", infoBarRec.x + infoBarMargin, infoBarRec.y + infoBarMargin * 5 + fontHeight * 2, 30, WHITE);
      PGUI::DrawCheckButtonGroup_Textures(iconsRec, (Vector2){plantSize, plantSize}, 5, plantTextures, mousePos, infoBarMargin, plants.at(selectedIndex).getTextureIdRef());

      DrawText(std::format("Freq: every {} Day", plants.at(selectedIndex).getFreqStr()).c_str(), infoBarRec.x + infoBarMargin, infoBarRec.y + infoBarMargin * 9 + fontHeight * 3 + plantSize, 30, WHITE);
      frequenzyNum = PGUI::DrawUpDownButtons(frequenzyChangeRec, mousePos);
      DrawText("Wet:", infoBarRec.x + infoBarMargin, infoBarRec.y + infoBarMargin * 11 + fontHeight * 4 + plantSize, 30, WHITE);
      PGUI::DrawCheckBox(wetCheckBoxRec, plants.at(selectedIndex).getBewaessertRef(), mousePos);

      DrawText("Meta", metaSectionRec.x + infoBarMargin, metaSectionRec.y + infoBarMargin, 38, GRAY);
      int xPos = std::round(plants.at(selectedIndex).getX());
      int yPos = std::round(plants.at(selectedIndex).getY() - menuBarRec.height);
      DrawText(std::format("Pos    {}:{}", xPos, yPos).c_str(), metaSectionRec.x + infoBarMargin, metaSectionRec.y + infoBarMargin * 3 + fontHeight, 30, WHITE);

      DrawText(std::format("Size   {}:{}", plantSize, plantSize).c_str(), metaSectionRec.x + infoBarMargin, metaSectionRec.y + infoBarMargin * 5 + fontHeight * 2, 30, WHITE);

      DrawText(std::format("Index  {}", selectedIndex).c_str(), metaSectionRec.x + infoBarMargin, metaSectionRec.y + infoBarMargin * 7 + fontHeight * 3, 30, WHITE);
    }
    if (isHolding) {
      if (holdPlant.getTextureId() != -1) {
        DrawTexture(plantTextures[holdPlant.getTextureId()], holdPlant.getX(), holdPlant.getY(), WHITE);
      } else {
        DrawRectangleRec(holdPlant.getRec(plantSize), PINK);
      }
    }

    if (frameCounter == 100 * FPSC) {
      frameCounter = 0;
    } else {
      frameCounter++;
    }

    if (WindowShouldClose()) {
      shouldClose = true;
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
