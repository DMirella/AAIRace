#ifndef AAIRACE_SOURCE_TOOLS_H_
#define AAIRACE_SOURCE_TOOLS_H_

#include <cstdlib>

namespace tools {
struct Point {
  Point() : x(0.0f), y(0.0f) {}
  Point(float _x, float _y) : x(_x), y(_y) {}
  float x, y;
};

struct Rectangle {
  float x1, y1, x2, y2;
};

const float gPI = 3.14159265f;
const float gInfinity = 1e9;

bool CheckRectangleIntersect(const Rectangle& a, const Rectangle& b);
float RandomFloat(float a, float b);
float GetRandomNormilizedFloat();
float EstimateDistance(float x1, float y1, float x2, float y2);
float GetDistanceFromRayToReactangle(const Point& ray_origin, const Point& ray_direction, const Rectangle& rectangle);
}  // namespace tools

#endif  // AAIRACE_SOURCE_TOOLS_H_