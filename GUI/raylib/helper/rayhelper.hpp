#include "raylib.h"
#include <cstdio>
#include <string>
// via Classes?
// combo box, checkbox, labels?, table, buttonText, centeredText, errormessage, check mouse collision

Color backgroundColor = GetColor(0x281628ff);
Color linesColor = GetColor(0x472f3dff);
Color textColor = WHITE;

float lineThickness = 1;
float fontSize = 30;

int fpsc = 144;

// using namespace PGUI::getMiddle() for only func to get into scope
namespace PGUI {

float getMiddle(float beginnCord, float originWidth, float targetWidth) {
  return beginnCord + (originWidth / 2) - (targetWidth / 2);
}

Rectangle getMouseRec(Vector2 mousePos) {
  return (Rectangle){mousePos.x, mousePos.y, 1, 1};
}

int getFittingFontsize(int targetHeight) {
  for (int fontSize = 300; MeasureTextEx(GetFontDefault(), "X", fontSize, 1).y > targetHeight; fontSize--) {
  }
  return fontSize;
}

void DrawTextbox(Rectangle rec, std::string displayString) {
}

void DrawCheckBox(Rectangle rec, bool &status, Vector2 mousePos) {
  if (IsMouseButtonPressed(0)) {
    if (CheckCollisionRecs(rec, getMouseRec(mousePos))) {
      status = !status;
    }
  }
  switch (status) {
  case true:
    DrawRectangleRec(rec, backgroundColor);
    DrawRectangleLinesEx(rec, lineThickness + 3, linesColor);
    break;
  case false:
    DrawRectangleRec(rec, linesColor);
    DrawRectangleLinesEx(rec, lineThickness + 3, backgroundColor);
    break;
  }
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

void DrawCheckButtonGroup_Textures(Rectangle allRec, Vector2 boxSize, int count, Texture textures[], Vector2 mousePos, int padding, int &selectedTextureId) {
  /* validation
  if(allRec.height < boxSize.y){
    return -1;
  }
  else if((boxSize.x * count) + (padding * count - 1) > allRec.width){
    return -1;
    }*/

  Rectangle recs[count];
  for (int i = 0; i < count; i++) { // maybe in one loop
    recs[i] = (Rectangle){allRec.x + padding * i + boxSize.x * i, allRec.y, boxSize.x, boxSize.y};
    DrawTexture(textures[i], recs[i].x, recs[i].y, WHITE);
    if (i == selectedTextureId) {
      DrawRectangleLinesEx(recs[i], lineThickness + 3, backgroundColor);
    }
  }

  Rectangle mouseRec = getMouseRec(mousePos);
  if (CheckCollisionRecs(allRec, mouseRec) && IsMouseButtonPressed(0)) {
    for (int i = 0; i < count; i++) {
      if (CheckCollisionRecs(recs[i], mouseRec)) {
        selectedTextureId = i;
      }
    }
  }
}

void DrawLabel(Rectangle rec, std::string &text, int fontSize, bool &isEditing, Vector2 mousePos, int frameC, int &bufferBeginn) { // FrameCounter so that backspace slows down
  if (isEditing && !CheckCollisionRecs(rec, getMouseRec(mousePos)) && IsMouseButtonPressed(0)) {
    isEditing = false;
  } else if (CheckCollisionRecs(rec, getMouseRec(mousePos)) && IsMouseButtonPressed(0)) {
    isEditing = true;
  }

  // finding out end and start of buffer
  int fittingChars = 0;
  for (int i = 0; i < text.length(); i++) { // own func
    if (MeasureText(text.substr(bufferBeginn, bufferBeginn + i + 1).c_str(), fontSize) + 10 < rec.width) { // overflows sometimes
      fittingChars++;
    } else {
      break;
    }
  }
  int bufferEnd = bufferBeginn + fittingChars;
  if (bufferEnd > text.length()) {
    bufferEnd = text.length();
  }
  if (isEditing) {
    char key = GetCharPressed();
    if (key > 31 && key < 127) {
      text.push_back(key);
    } else if (IsKeyPressed(KEY_INSERT)) {
      text.append(GetClipboardText());
    } else if (IsKeyPressed(KEY_DELETE)) {
      text.clear();
    } else if (IsKeyPressed(KEY_ENTER)) {
      isEditing = false;
    } else if ((frameC % (fpsc / 16)) == 0) { // so that it dosnt go off as quick
      if (IsKeyDown(KEY_BACKSPACE) && text.size() > 0) {
        text.pop_back();
      } else if (IsKeyDown(KEY_RIGHT) && bufferBeginn < text.length() && bufferEnd < text.length()) {
        bufferBeginn++;
      } else if (IsKeyDown(KEY_LEFT) && bufferBeginn > 0) {
        bufferBeginn--;
      }
    }
  }

  std::string textBuffer = text.substr(bufferBeginn, bufferEnd);
  Color temp_bgColor;
  Color temp_LinesColor;
  if (isEditing) {
    temp_bgColor = backgroundColor;
    temp_LinesColor = linesColor;
  } else {
    temp_bgColor = linesColor;
    temp_LinesColor = backgroundColor;
  }
  DrawRectangleRec(rec, temp_bgColor);
  DrawRectangleLinesEx(rec, 2, temp_LinesColor);
  DrawText(textBuffer.c_str(), rec.x + 5, getMiddle(rec.y, rec.height, MeasureTextEx(GetFontDefault(), "X", fontSize, 1).y), fontSize, textColor);
}
} // namespace PGUI
