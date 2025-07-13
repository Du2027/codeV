#include <raylib.h>
#include <vector>

#include "classes.h"
#include "constants.cpp"

class Field {
public:
  Rectangle rec;
  std::vector<char> textVec;

  void updatePos(Vector2 pos) {
    this->rec.x = pos.x;
    this->rec.y = pos.y;
  }

  virtual void draw() {}
};

class SeqField : Field {
  SeqField(Rectangle rec) {
    this->rec = rec;
  }

  void UpdateText(char Char) {
    textVec.push_back(Char);

    if (MeasureText(textVec.data(), 20) + SeqTextPadding * 2 > rec.width) {
      rec.width = MeasureText(textVec.data(), 20) + 8 * 2;
    }
  }

  void draw() {
    // char Vector to char[]
    char TexttoDraw[textVec.size() + 1];
    for (int i = 0; i < textVec.size(); i++) {
      TexttoDraw[i] = textVec.at(i);
    }
    TexttoDraw[textVec.size()] = 0;

    DrawRectangle(rec.x, rec.y, rec.width, rec.height, SeqBackgroundColor);
    DrawText(TexttoDraw, rec.x + SeqTextPadding, rec.y + SeqTextPadding, FieldTextFont, FieldTextColor);
  }
};

class ManagerBlock {
public:
  Vector2 pos;
  std::vector<Field> fields = std::vector<Field>();

  ManagerBlock(Vector2 pos) {
    this->pos = pos;
  }

  void appendField(Field field) {
    fields.push_back(field); // Need to implement!
  }

  void drawAllFields() {
    for (int i = 0; i < fields.size(); i++) {
      fields.at(i).draw();
    }
  }

  float getWidth() {
    float biggestWidth = 0;
    for (int i = 0; i < fields.size(); i++) {
      if (fields.at(i).rec.width > biggestWidth) {
        biggestWidth = fields.at(i).rec.width;
      }
    }
    return biggestWidth;
  }

  float getHeight() {
    float height = 0;
    for (int i = 0; i < fields.size(); i++) {
      height = height + fields.at(i).rec.height;
    }
    return height;
  }
};
