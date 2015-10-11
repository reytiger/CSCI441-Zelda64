#pragma once

#include "WorldObjects/WorldObjectBase.hpp"

using namespace std;

class Point {
public:
    bool selected;

    // consturctor
    Point();
    Point(double x, double y, double z);

    void setX(double x) { this->x = x; };
    void setY(double y) { this->y = y; };
    void setZ(double z) { this->z = z; };
    void setXYZ(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    };

    // accessor
    double getX() const { return this->x; }
    double getY() const { return this->y; }
    double getZ() const { return this->z; }

private:
    double x, y, z;
};

Point operator*(Point p, double f);
Point operator*(double f, Point p);
Point operator+(Point a, Point b);