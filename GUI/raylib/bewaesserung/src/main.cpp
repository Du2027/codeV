#include <raylib.h>
#include <string>

class plant{
  Vector2 relPosition;
  double bewaesserungsFequenz; // 1.5 2
  bool heuteBewaessert; //??
  int textureID;

  public:
  plant(){
      this->bewaesserungsFequenz = 0;
      this->heuteBewaessert = false;
      this->relPosition = (Vector2){0,0};
      this->textureID = -1;
  }
  plant(double bewaesserungsFrequenz, bool heuteBewaessert, Vector2 relPosition, int textureID){
      this->bewaesserungsFequenz = bewaesserungsFrequenz;
      this->heuteBewaessert = heuteBewaessert;
      this->relPosition = relPosition;
      this->textureID = textureID;
  }
  void bewaessert(){this->heuteBewaessert = true;}
  void changeFrequenz(double frequenz){this->bewaesserungsFequenz = frequenz;}
  void changeTexture(int textureID){this->textureID = textureID;}
  void addPosition(Vector2 position){this->relPosition = position;}
  void changePosition(Vector2 deltaPosition){
      this->relPosition.x += deltaPosition.x;
    this->relPosition.y += deltaPosition.y;
  }
};

int main() {
  InitWindow(0, 0, "TEMP_WINDOW");
  ToggleFullscreen();
  Vector2 windowSize = (Vector2){static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
  CloseWindow();
  InitWindow(windowSize.x, windowSize.y, "Bewaesserung");
  SetTargetFPS(144);

  float menuBarMargin = 14;
  std::string filePath = "";

  Rectangle menuBarRec = (Rectangle){0,0,windowSize.x, windowSize.y/35};

  Image uploadImage = LoadImage("assets/file_up.png");
  Image downloadImage = LoadImage("assets/file_down.png");

  ImageResize(&uploadImage, menuBarRec.height - 17, menuBarRec.height - 20); // nur 5x2 Pixel???
  ImageResize(&downloadImage, uploadImage.width, uploadImage.height);

  Texture uploadTexture = LoadTextureFromImage(uploadImage);
  Texture downloadTexture = LoadTextureFromImage(downloadImage);

  Rectangle xRec = (Rectangle){};
  Rectangle uploadRec = (Rectangle){menuBarMargin, (menuBarRec.height - uploadImage.height) /2, static_cast<float>(uploadImage.width), static_cast<float>(uploadImage.height)};
  Rectangle downloadRec = (Rectangle){menuBarMargin *2 + uploadRec.width, (menuBarRec.height - uploadImage.height) /2, static_cast<float>(uploadImage.width), static_cast<float>(uploadImage.height)};
  Rectangle selectionRec = (Rectangle){0,menuBarRec.height, windowSize.x/6, windowSize.y};
  Rectangle menuBarLinesRec = (Rectangle){downloadRec.x + downloadRec.width + menuBarMargin, uploadRec.y - 2, menuBarRec.width - (menuBarMargin*4) - (uploadRec.width*2), uploadRec.height + 4};

  Color backgroudColor = GetColor(0x2d1c2eff); // 0x as prefix to show its a hexadecimal
  Color secondaryBackgroundColor = GetColor(0x472f3dff);
  Color mainColor = GetColor(0xa99089ff);
  Color mainDarkColor = GetColor(0x281628ff);

  bool shouldClose = false;
  while (!shouldClose) {
    if(IsMouseButtonDown(0)){
      Vector2 mousePos = GetMousePosition();
      Rectangle mouseRec = (Rectangle){mousePos.x, mousePos.y, 1, 1};

      if(CheckCollisionRecs(uploadRec, mouseRec)){
        // WIP
      }
      else if (CheckCollisionRecs(downloadRec, mouseRec)) {
        // WIP
      }
    }

    BeginDrawing();
    ClearBackground(backgroudColor);

    // menu bar
    DrawRectangleRec(menuBarRec, secondaryBackgroundColor);
    DrawRectangleRoundedLines(menuBarLinesRec, 0.2, 8, mainColor);
    DrawTexture(uploadTexture, uploadRec.x, uploadRec.y, WHITE);
    DrawTexture(downloadTexture, downloadRec.x, downloadRec.y, WHITE);
    DrawRectangleRec(selectionRec, secondaryBackgroundColor);
    EndDrawing();

    if (WindowShouldClose()) {
      shouldClose = true;
    }
  }
  CloseWindow();
  return 0;
}
