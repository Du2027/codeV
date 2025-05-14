#pragma once

#include <raylib.h>
#include <vector>

class Field {
public:
  Rectangle rec;
  std::vector<char> textVec;

  void updatePos(Vector2 pos);
  virtual void draw();
};

class SeqField : Field {
  SeqField(Rectangle rec);

  void UpdateText(char Char);
  void draw();
};

class ManagerBlock {
public:
  Vector2 pos;
  std::vector<Field> fiels;

  ManagerBlock(Vector2 pos);

  void appendField(Field field);
  void drawAllFields();

  float getWidth();
  float getHeight();
};
