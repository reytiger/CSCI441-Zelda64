#pragma once

#include "WorldObjects/WorldObjectBase.hpp"

using namespace std;

class Point {
public:
  bool selected;

  // consturctor
  Point();
  Point(float x, float y, float z);

  void setX(float x) { this->x = x; };
  void setY(float y) { this->y = y; };
  void setZ(float z) { this->z = z; };
  void setXYZ(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
  };

  // accessor
  float getX() const { return this->x; }
  float getY() const { return this->y; }
  float getZ() const { return this->z; }

private:
  float x, y, z;
};

Point operator*(Point p, float f);
Point operator*(float f, Point p);
Point operator+(Point a, Point b);