#pragma once
#include "Utils/Logging.hpp"

#include <iostream>

#include <cmath>

struct Vec;
struct VecPolar;

struct Vec {
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;

    ~Vec() = default;

    Vec() = default;

    template <typename X, typename Y>
    Vec(X x, Y y) : x(as<float>(x)), y(as<float>(y)), z(0.0f) {}

    template <typename X, typename Y, typename Z>
    Vec(X x, Y y, Z z) : x(as<float>(x)), y(as<float>(y)), z(as<float>(z)) {}

    VecPolar polar() const;

    float dot(const Vec &other) const;
    float norm() const;
    Vec normalize() const;
    Vec cross(const Vec &other) const;
};

struct VecPolar {
    float theta = 0.0;
    float phi   = 0.0;
    float r     = 1.0;

    ~VecPolar() = default;
    VecPolar() = default;

    template <typename Theta, typename Phi>
    VecPolar(Theta theta, Phi phi)
        : theta(as<float>(theta)), phi(as<float>(phi)), r(1.0f) {}

    template <typename Theta, typename Phi, typename R>
    VecPolar(Theta theta, Phi phi, R r)
        : theta(as<float>(theta)), phi(as<float>(phi)), r(as<float>(r)) {}

    explicit VecPolar(Vec vec) {
        r = vec.norm();
        if (r == 0.0) {
            return;
        }
        vec = vec.normalize();

        theta = atan2(vec.x, vec.z);
        phi   = asin(vec.y);
        glChk(); // asin sets errno on bad input.
    }

    // Convert to cartesian, aka Vec.
    Vec cart() const {
        float x = r * sin(theta) * cos(phi);
        float y = r * sin(phi);
        float z = r * cos(theta) * cos(phi);
        return Vec(x, y, z);
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

static inline bool operator==(const Vec &a, const Vec &b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

// Vec + Vec -> Vec
def_op_by_components(Vec, +, Vec, Vec);
def_op_by_components(Vec, -, Vec, Vec);
def_op_by_components(Vec, *, Vec, Vec);
def_op_by_components(Vec, /, Vec, Vec);

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
static inline VecPolar operator*(const VecPolar &polar, float scalar) {
    auto res = polar;
    res.r *= scalar;
    return res;
}
static inline VecPolar operator/(const VecPolar &polar, float scalar) {
    auto res = polar;
    res.r /= scalar;
    return res;
}

// Clamping can be done component wise.
static inline Vec clamp(const Vec &a, const Vec &lo, const Vec &hi) {
    return Vec(clamp<float>(a.x, lo.x, hi.x),
               clamp<float>(a.y, lo.y, hi.y),
               clamp<float>(a.z, lo.z, hi.z));
}

static inline VecPolar clamp(const VecPolar &a, const VecPolar &lo,
                             const VecPolar &hi) {
    return VecPolar(clamp<float>(a.theta, lo.theta, hi.theta),
                    clamp<float>(a.phi, lo.phi, hi.phi),
                    clamp<float>(a.r, lo.r, hi.r));
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

static inline std::ostream &operator<<(std::ostream &os, const VecPolar &vec) {
    return os << "VecPolar(" << vec.theta << ", " << vec.phi << ", " << vec.r
              << ")";
}

inline VecPolar Vec::polar() const { return VecPolar(*this); }

inline float Vec::dot(const Vec &other) const {
    return x * other.x + y * other.y + z * other.z;
}

inline float Vec::norm() const { return sqrt(this->dot(*this)); }

inline Vec Vec::normalize() const {
    assert(norm() != 0);
    return *this / norm();
}

inline Vec Vec::cross(const Vec &v) const {
    const Vec &u = *this;
    return Vec(
        u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}
