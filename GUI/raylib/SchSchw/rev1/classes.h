#pragma once
#include <raylib.h>

// TODO:
// Constructors

class PRect {
public:
  float x;
  float y;
  float width;
  float height;

  PRect() {
    x = 0;
    y = 0;
    width = 0;
    height = 0;
  }
  PRect(Rectangle rec) {
    x = rec.x;
    y = rec.y;
    width = rec.width;
    height = rec.height;
  }
  PRect(float x, float y, float width, float height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
  }

  Rectangle getRect();
  void setRect(Rectangle rec);
  void updateSize(Vector2 windowDeltaScale);
};

class WaterObject {
  int waterAddition;

public:
  void addWater(int *water);
};

class Droplet {
  PRect rec;
  int speed;

public:
  void updatePos();
};
