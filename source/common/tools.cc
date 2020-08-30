#include "tools.h"

#include <algorithm>
#include <cstdlib>
#include <cmath>

namespace {
common::Point operator-(const common::Point& a, const common::Point& b) {
  common::Point result;
  result.y = a.y - b.y;
  result.x = a.x - b.x;
  return result;
}

float PointDotProduct(const common::Point& a, const common::Point& b) {
  return a.x * b.x + a.y * b.y;
}

float PointCrossProduct(const common::Point& a, const common::Point& b) {
  return a.x * b.y - a.y * b.x;
}

void NormalizeVector(common::Point* const pt) {
  float length = std::sqrt(pt->x * pt->x + pt->y * pt->y);
  pt->x /= length;
  pt->y /= length;
}

float GetRayToLineDistanceSegmentIntersection(const common::Point& ray_origin, const common::Point& ray_direction, 
                                              const common::Point& segment_point_1, const common::Point& segment_point_2) {
  common::Point v1 = ray_origin - segment_point_1;
  common::Point v2 = segment_point_2 - segment_point_1;
  common::Point v3 = common::Point(-ray_direction.y, ray_direction.x);

  float dot = PointDotProduct(v2, v3);
  if (std::fabs(dot) < 0.000001f) {
    return -1.0f;
  }

  float t1 = PointCrossProduct(v2, v1) / dot;
  float t2 = PointDotProduct(v1, v3) / dot;

  if (t1 >= 0.0 && (t2 >= 0.0 && t2 <= 1.0)) {
    return t1;
  }

  return -1.0f;
}
}  // namespace

namespace common {

bool CheckRectangleIntersect(const Rectangle& a, const Rectangle& b) {
  return !(a.y2 < b.y1 ||  a.y1 > b.y2 ||  a.x2 < b.x1 || a.x1 > b.x2);
}

float RandomFloat(float a, float b) {
  float random = (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX));
  float diff = b - a;
  float r = random * diff;
  return a + r;
}

float GetRandomNormilizedFloat() {
  return RandomFloat(-1.0f, 1.0f);
}

float EstimateDistance(float x1, float y1, float x2, float y2) {
  return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

float GetDistanceFromRayToReactangle(const Point& ray_origin, const Point& ray_direction, const Rectangle& rectangle) {
  auto normilized_ray_direction = ray_direction;
  NormalizeVector(&normilized_ray_direction);
  
  float result = gInfinity;
  float current_distance 
      = GetRayToLineDistanceSegmentIntersection(ray_origin, normilized_ray_direction, Point(rectangle.x1, rectangle.y1), Point(rectangle.x2, rectangle.y1));
  if (current_distance > 0.0f) {
    result = std::min(result, current_distance);
  }
  current_distance 
      = GetRayToLineDistanceSegmentIntersection(ray_origin, normilized_ray_direction, Point(rectangle.x1, rectangle.y1), Point(rectangle.x1, rectangle.y2));
  if (current_distance > 0.0f) {
    result = std::min(result, current_distance);
  }
  current_distance 
      = GetRayToLineDistanceSegmentIntersection(ray_origin, normilized_ray_direction, Point(rectangle.x2, rectangle.y2), Point(rectangle.x1, rectangle.y2));
  if (current_distance > 0.0f) {
    result = std::min(result, current_distance);
  }
  current_distance 
      = GetRayToLineDistanceSegmentIntersection(ray_origin, normilized_ray_direction, Point(rectangle.x2, rectangle.y2), Point(rectangle.x2, rectangle.y1));
  if (current_distance > 0.0f) {
    result = std::min(result, current_distance);
  }
  return result;
}
} // namespace common