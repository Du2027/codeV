#pragma once
#include <raylib.h>

// TODO:
// Constructors

class PRect {
public:
  int x;
  int y;
  int width;
  int height;

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
