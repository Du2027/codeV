#include "classes.h"
#include <raylib.h>

void PRect::updateSize(Vector2 windowDeltaSizes) {
  x = static_cast<int>(x * windowDeltaSizes.x);
  width = static_cast<int>(width * windowDeltaSizes.x);
  y = static_cast<int>(y * windowDeltaSizes.y);
  height = static_cast<int>(height * windowDeltaSizes.y);
}

void WaterObject::addWater(int *water) {
  water = water + waterAddition;
}

void Droplet::updatePos() {
  rec.y = rec.y + speed;
}
