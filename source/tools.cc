#include "tools.h"

#include <cmath>

bool CheckRectangleIntersect(const Rectangle& a, const Rectangle& b) {
  return !(a.y2 < b.y1 ||  a.y1 > b.y2 ||  a.x2 < b.x1 || a.x1 > b.x2);
}

float RandomFloat(float a, float b) {
  float random = (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX));
  float diff = b - a;
  float r = random * diff;
  return a + r;
}

float EstimateDistance(float x1, float y1, float x2, float y2) {
  return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}