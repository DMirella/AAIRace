#include "tools.h"

bool CheckRectangleIntersect(const Rectangle& a, const Rectangle& b) {
  return !(a.y2 < b.y1 ||  a.y1 > b.y2 ||  a.x2 < b.x1 || a.x1 > b.x2);
}

float RandomFloat(float a, float b) {
  float random = (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX));
  float diff = b - a;
  float r = random * diff;
  return a + r;
}