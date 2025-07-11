#include "raylib.h"
#include <string>
// via Classes?
// combo box, checkbox, labels?, table, buttonText, centeredText, errormessage, check mouse collision

Color backgroundColor = GetColor(0x281628ff);
Color linesColor = GetColor(0x472f3dff);
Color textColor = LIGHTGRAY;

float lineThickness = 1;
float fontSize = 30;

// using namespace PGUI::getMiddle() for only func to get into scope
namespace PGUI {

float getMiddle(float beginnCord, float originWidth, float targetWidth) {
  return beginnCord + (originWidth / 2) - (targetWidth / 2);
}

Rectangle getMouseRec(Vector2 mousePos) {
  return (Rectangle){mousePos.x, mousePos.y, 1, 1};
}

void DrawTextbox(Rectangle rec, std::string displayString) {
}

int DrawUpDownButtons(Rectangle rec, Vector2 mousePos) {
  int pressed = 0; // 1 top, -1 bottom
  Rectangle mouseRec = getMouseRec(mousePos);
  Rectangle topRec = (Rectangle){rec.x, rec.y, rec.width, rec.height / 2};
  Rectangle botRec = (Rectangle){rec.x, rec.y + rec.height / 2, rec.width, rec.height / 2};
  Vector2 textSizes;

  textSizes = MeasureTextEx(GetFontDefault(), "+", fontSize, 1);
  DrawRectangleRec(topRec, backgroundColor);
  DrawText("+", getMiddle(topRec.x, topRec.width, textSizes.x), getMiddle(topRec.y, topRec.height, textSizes.y) + 1, fontSize, textColor);
  DrawRectangleLinesEx(topRec, lineThickness, linesColor);

  textSizes = MeasureTextEx(GetFontDefault(), "-", fontSize, 1);
  DrawRectangleRec(botRec, backgroundColor);
  DrawText("-", getMiddle(botRec.x, botRec.width, textSizes.x), getMiddle(botRec.y, botRec.height, textSizes.y) + 1, fontSize, textColor);
  DrawRectangleLinesEx(botRec, lineThickness, linesColor);

  if (IsMouseButtonPressed(0)) {
    if (CheckCollisionRecs(topRec, mouseRec)) {
      pressed = 1;
    } else if (CheckCollisionRecs(botRec, mouseRec)) {
      pressed = -1;
    }
  }
  return pressed;
}
} // namespace PGUI
