#ifndef AAIRACE_SOURCE_TOOLS_H_
#define AAIRACE_SOURCE_TOOLS_H_

#include <cstdlib>

struct Rectangle {
  float x1, y1, x2, y2;
};

bool CheckRectangleIntersect(const Rectangle& a, const Rectangle& b);
float RandomFloat(float a, float b);

#endif  // AAIRACE_SOURCE_TOOLS_H_