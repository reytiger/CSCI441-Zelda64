#include "WorldObjects/Point.hpp"

Point::Point() {
    x = 0;
    y = 0;
    z = 0;
}

Point::Point(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Point operator*(Point p, float f) {
    return Point(p.getX() * f, p.getY() * f, p.getZ() * f);
}

Point operator*(float f, Point p) {
    return Point(f * p.getX(), f * p.getY(), f * p.getZ());
}

Point operator+(Point a, Point b) {
    return Point(a.getX() + b.getX(), a.getY() + b.getY(), a.getZ() + b.getZ());
}