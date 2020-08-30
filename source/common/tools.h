#ifndef AAIRACE_SOURCE_COMMON_TOOLS_H_
#define AAIRACE_SOURCE_COMMON_TOOLS_H_

namespace common {
struct Point {
  Point() : x(0.0f), y(0.0f) {}
  Point(float _x, float _y) : x(_x), y(_y) {}
  float x, y;
};

struct Rectangle {
  Rectangle() : x1(0.0f), y1(0.0f), x2(0.0f), y2(0.0f) {}
  Rectangle(float _x1, float _y1, float _x2, float _y2) 
      : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}
  float x1, y1, x2, y2;
};

const float gPI = 3.14159265f;
const float gInfinity = 1e9;

bool CheckRectangleIntersect(const Rectangle& a, const Rectangle& b);
float RandomFloat(float a, float b);
float GetRandomNormilizedFloat();
float EstimateDistance(float x1, float y1, float x2, float y2);
float GetDistanceFromRayToReactangle(const Point& ray_origin, const Point& ray_direction, const Rectangle& rectangle);
}  // namespace common

#endif  // AAIRACE_SOURCE_COMMON_TOOLS_H_