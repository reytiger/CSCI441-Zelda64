#pragma once

#include <iostream>

struct Vec {
public:
  union {
    double x = 0.0;
    double theta;
  };
  union {
    double y = 0.0;
    double phi;
  };
  union {
    double z = 0.0;
    double r;
  };

  static double inner(const Vec &a, const Vec &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
  }

  ~Vec() = default;
  Vec() = default;
  // Allow easy emulation of 2D vectors by making z arguments default to 0.0.
  constexpr Vec(double x, double y, double z = 0.0) : x(x), y(y), z(z) {}

  Vec toCart();

  double norm() const;
  Vec normalized() {
    auto v = *this;
    return v.normalize();
  }
  Vec normalize();
};

#define by_components(OP)                                                      \
  constexpr inline Vec operator OP(const Vec &a, const Vec &b) {               \
    return Vec(a.x OP b.x, a.y OP b.y, a.z OP b.z);                            \
  }

#define by_scalar(OP)                                                          \
  constexpr inline Vec operator OP(const Vec &a, double s) {                   \
    return Vec(a.x OP s, a.y OP s, a.z OP s);                                  \
  }                                                                            \
  constexpr inline Vec operator OP(double s, const Vec &a) {                   \
    return Vec(s OP a.x, s OP a.y, s OP a.z);                                  \
  }

#define compound(OP, COMP, TYPE)                                               \
  inline Vec operator COMP(Vec &a, const TYPE &b) { return a = a OP b; }

by_components(+);
by_components(-);
by_components(*);
by_components(/ );

by_scalar(+);
by_scalar(-);
by_scalar(*);
by_scalar(/ );

// Vectors
compound(+, +=, Vec);
compound(-, -=, Vec);
compound(*, *=, Vec);
compound(/, /=, Vec);

// Scalars
compound(+, +=, double);
compound(-, -=, double);
compound(*, *=, double);
compound(/, /=, double);

#undef by_components
#undef by_scalar
#undef compound

constexpr inline Vec operator-(const Vec &a) { return -1 * a; }

// Clamp 'a' by 'lo' and 'hi' component wise.
inline Vec clamp(const Vec &a, const Vec &lo, const Vec &hi) {
  return Vec(clamp<double>(a.x, lo.x, hi.x), clamp<double>(a.y, lo.y, hi.y),
             clamp<double>(a.z, lo.z, hi.z));
}

inline Vec Vec::toCart() {
  double x = cos(this->theta) * cos(this->phi);
  double y = sin(this->theta) * cos(this->phi);
  double z = sin(this->phi);
  return Vec(x, y, z).normalize();
}

inline double Vec::norm() const { return sqrt(inner(*this, *this)); }

inline Vec Vec::normalize() {
  double norm_ = this->norm();
  assert(norm_ != 0);
  this->x /= norm_;
  this->y /= norm_;
  this->z /= norm_;
  return *this;
}

inline std::ostream &operator<<(std::ostream &os, const Vec &vec) {
  return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}
