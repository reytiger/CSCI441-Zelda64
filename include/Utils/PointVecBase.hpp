#pragma once

#include <iostream>

#include <cmath>

struct Vec;
struct Point3;
struct VecPolar;

struct Vec {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    ~Vec() = default;

    Vec() = default;
    // Make it easy to fake a 2D vector.
    Vec(double x, double y, double z = 0.0) : x(x), y(y), z(z) {}

    double dot(const Vec &other) const;
    double norm() const;
    Vec normalize() const;
};

struct Point3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    ~Point3() = default;

    Point3() = default;
    // Make it easy to fake a 2D point.
    Point3(double x, double y, double z = 0.0) : x(x), y(y), z(z) {}
};

struct VecPolar {
    double theta = 0.0;
    double phi   = 0.0;
    double r     = 1.0;

    ~VecPolar() = default;
    VecPolar() = default;
    VecPolar(double theta, double phi, double r = 1.0)
        : theta(theta), phi(phi), r(r) {}
    VecPolar(Vec vec)
        : theta(atan(vec.y / vec.x))
        , phi(atan(sqrt(vec.x * vec.x + vec.y * vec.y) / vec.z))
        , r(vec.norm()) {}

    // Convert to cartesian, aka Vec.
    Vec cart() const {
        double x = cos(this->theta) * cos(this->phi);
        double y = sin(this->theta) * cos(this->phi);
        double z = sin(this->phi);
        return Vec(x, y, z).normalize();
    }
    operator Vec() const { return cart(); }
};

#define def_op_by_components(T1, OP, T2, T3)                                   \
    inline T3 operator OP(const T1 &a, const T2 &b) {                          \
        return T3(a.x OP b.x, a.y OP b.y, a.z OP b.z);                         \
    }

#define def_op_by_scalar(T1, OP, Scalar)                                       \
    inline T1 operator OP(const T1 &a, Scalar s) {                             \
        return T1(a.x OP s, a.y OP s, a.z OP s);                               \
    }                                                                          \
    inline T1 operator OP(Scalar s, const T1 &a) {                             \
        return T1(s OP a.x, s OP a.y, s OP a.z);                               \
    }

#define def_compound_ops(T1, OP, COMP, T2)                                     \
    inline T1 operator COMP(T1 &a, const T2 &b) { return a = a OP b; }

// Vec + Vec -> Vec
def_op_by_components(Vec, +, Vec, Vec);
def_op_by_components(Vec, -, Vec, Vec);
def_op_by_components(Vec, *, Vec, Vec);
def_op_by_components(Vec, /, Vec, Vec);

// Vec + Point3 -> Point3
def_op_by_components(Point3, +, Vec, Point3);
def_op_by_components(Point3, -, Vec, Point3);
def_op_by_components(Vec, +, Point3, Point3);
def_op_by_components(Vec, -, Point3, Point3);

// Point3 - Point3 -> Vec;
def_op_by_components(Point3, -, Point3, Vec);

// Vectors can be scaled.
def_op_by_scalar(Vec, +, double);
def_op_by_scalar(Vec, -, double);
def_op_by_scalar(Vec, *, double);
def_op_by_scalar(Vec, /, double);

// Compund operators!
def_compound_ops(Vec, +, +=, double);
def_compound_ops(Vec, -, -=, double);
def_compound_ops(Vec, *, *=, double);
def_compound_ops(Vec, /, /=, double);

def_compound_ops(Vec, +, +=, Vec);
def_compound_ops(Vec, -, -=, Vec);
def_compound_ops(Vec, *, *=, Vec);
def_compound_ops(Vec, /, /=, Vec);

#undef by_components
#undef by_scalar
#undef compound

// Polar coordinates are not done component wise.
static inline VecPolar operator*(const VecPolar &polar, double scalar) {
    auto res = polar;
    res.r *= scalar;
    return res;
}
static inline VecPolar operator/(const VecPolar &polar, double scalar) {
    auto res = polar;
    res.r /= scalar;
    return res;
}

// Clamping can be done component wise.
static inline Vec clamp(const Vec &a, const Vec &lo, const Vec &hi) {
    return Vec(clamp<double>(a.x, lo.x, hi.x),
               clamp<double>(a.y, lo.y, hi.y),
               clamp<double>(a.z, lo.z, hi.z));
}
static inline Point3 clamp(const Point3 &a, const Point3 &lo,
                           const Point3 &hi) {
    return Point3(clamp<double>(a.x, lo.x, hi.x),
                  clamp<double>(a.y, lo.y, hi.y),
                  clamp<double>(a.z, lo.z, hi.z));
}
static inline VecPolar clamp(const VecPolar &a, const VecPolar &lo,
                             const VecPolar &hi) {
    return VecPolar(clamp<double>(a.theta, lo.theta, hi.theta),
                    clamp<double>(a.phi, lo.phi, hi.phi),
                    clamp<double>(a.r, lo.r, hi.r));
}

// + and - are usually pretty ubiquitous.
template <typename T>
static inline T operator+(const T &a) {
    return 1 * a;
}

template <typename T>
static inline T operator-(const T &a) {
    return -1 * a;
}

// Seeing what we have is important.
static inline std::ostream &operator<<(std::ostream &os, const Vec &vec) {
    return os << "Vec(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}
static inline std::ostream &operator<<(std::ostream &os, const Point3 &vec) {
    return os << "Point3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}
static inline std::ostream &operator<<(std::ostream &os, const VecPolar &vec) {
    return os << "VecPolar(" << vec.theta << ", " << vec.phi << ", " << vec.r
              << ")";
}

inline double Vec::dot(const Vec &other) const {
    return x * other.x + y * other.y + z * other.z;
}

inline double Vec::norm() const { return sqrt(this->dot(*this)); }

inline Vec Vec::normalize() const {
    assert(norm() != 0);
    return *this / norm();
}
