#include "classes.h"
#include <raylib.h>

void PRect::updateSize(Vector2 windowDeltaSizes) {
  x = x * windowDeltaSizes.x;
  width = width * windowDeltaSizes.x;
  y = y * windowDeltaSizes.y;
  height = height * windowDeltaSizes.y;
}

Rectangle PRect::getRect() {
  return (Rectangle){x, y, width, height};
}

void PRect::setRect(Rectangle rec) {
  x = rec.x;
  y = rec.y;
  width = rec.width;
  height = rec.height;
}

void WaterObject::addWater(int *water) {
  water = water + waterAddition;
}

void Droplet::updatePos() {
  rec.y = rec.y + speed;
}
