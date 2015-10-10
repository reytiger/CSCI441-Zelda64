#include "WorldObjects/BezierCurve.hpp"

#include <fstream>
#include <vector>

Vec BezierCurve::evalCubic(Vec p0, Vec p1, Vec p2, Vec p3, double t) const {
    auto b0 = (1 - t) * (1 - t) * (1 - t);
    auto b1 = 3 * (1 - t) * (1 - t) * t;
    auto b2 = 3 * (1 - t) * t * t;
    auto b3 = t * t * t;
    return m_pos + b0 * p0 + b1 * p1 + b2 * p2 + b3 * p3;
}

void BezierCurve::loadFile(const std::string &filename) {
    std::ifstream fi;
    fi.open(filename);

    if (!fi) {
        int err = errno;
        fprintf(stderr,
                "Unable to load \"%s\". %s\n",
                filename.c_str(),
                strerror(err));
        return;
    }

    unsigned expected_count = 0;
    fi >> expected_count;
    // Ignore the rest of the line.
    fi.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

// The more I use them, the more I wish C++ had nested functions. Even if they
// had restrictions, like always being inlined and not being allowed to return
// pointers to them.

// We check for eof first. If that fails, the point won't be emplaced at the end
// and the function should complain after the while loop.
#define check()                                                                \
    if (fi.eof()) {                                                            \
        break;                                                                 \
    } else if (!fi) {                                                          \
        int err = errno;                                                       \
        fprintf(                                                               \
            stderr, "ERROR:%s:%d: %s\n", __FILE__, __LINE__, strerror(err));   \
        exit(3);                                                               \
    }

    while (fi) {
        std::string str;

        getline(fi, str, ',');
        double x = strtod(str.c_str(), nullptr);
        check();

        getline(fi, str, ',');
        double y = strtod(str.c_str(), nullptr);
        check();

        // The third value isn't delimited by a comma, but rather a newline.
        getline(fi, str, '\n');
        double z = strtod(str.c_str(), nullptr);
        check();

        pointsV.emplace_back(x, y, z);
    }

#undef check

    if (expected_count != pointsV.size()) {
        // The standard way to print type size_t is to use %zu, but Windows
        // doesn't support it. Instead, we cast to a double. (Which is portable
        // and
        // hard to break.)
        printf("Read in %.0f control pointsV from \"%s\", but expected %d.\n",
               (double)pointsV.size(),
               filename.c_str(),
               expected_count);
        for (Vec pt : pointsV) {
            printf("%f, %f, %f\n", pt.x, pt.y, pt.z);
        }
    }

    recomputeCurve(10000);
}

void BezierCurve::draw() const {
    glLineWidth(5.0f);
    if (drawCage) {
        // Draw the pointsV
        glDisable(GL_LIGHTING);
        glColor3d(0.0, 1.0, 0.0);
        for (Vec point : pointsV) {
            pushMatrixAnd([=]() {

                glTranslated(
                    point.x + m_pos.x, point.y + m_pos.y, point.z + m_pos.z);
                glScaled(0.01, 0.01, 0.01);

                glutSolidSphere(1.0, 50, 50);
            });
        }

        // And then the connections.
        for (size_t i = 1; i < pointsV.size(); i += 1) {
            glColor3d(1.0, 1.0, 0.0);
            pushMatrixAnd([this, i]() {
                glBegin(GL_LINES);
                auto a = pointsV[i - 1] + m_pos;
                auto b = pointsV[i] + m_pos;
                glVertex3d(a.x, a.y, a.z);
                glVertex3d(b.x, b.y, b.z);
                glEnd();

            });
        }
        glEnable(GL_LIGHTING);
    }

    if (drawPath) {
        glDisable(GL_LIGHTING);
        glColor3d(0.0, 0.0, 1.0);
        double dt = 1.0 / 45;
        glBegin(GL_LINES);
        for (double t = 0; t <= 1.0; t += dt) {
            auto point = eval(t);
            glVertex3d(point.x, point.y, point.z);
        }
        glEnd();
        glEnable(GL_LIGHTING);

        dt = 1.0 / 45;
        glBegin(GL_LINES);
        for (double t = 0; t < 1.0; t += dt) {
            auto point = evalCubicPoint(t);
            glVertex3d(point.x, point.y, point.z);
        }
        glEnd();
        glEnable(GL_LIGHTING);
    }
}

void BezierCurve::drawCurve() const {
    glDisable(GL_LIGHTING);
    glColor3d(0.0, 0.0, 1.0);
    double dt = 1.0 / 45;
    glBegin(GL_LINES);
    // TODO: draw the vec as well.
    // for (double t = 0; t <= 1.0; t += dt) {
    //     auto point = eval(t);
    //     glVertex3d(point.x, point.y, point.z);
    // }
    glEnd();

    dt = 1.0 / 45;
    glBegin(GL_LINES);
    for (double t = 0; t <= 1.0; t += dt) {
        auto point = evalCubicPoint(t);
        glVertex3d(point.x, point.y, point.z);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

Vec BezierCurve::eval(double t) const {
    // info("%s", t);
    if (ts.size() == 0) {
        recomputeCurve(100);
    }
    t        = fmod(t, 1.0);
    size_t i = 1;
    while (i < ts.size() && ts[i] < t) {
        i += 1;
    }

    // Find how close we are to the two neighboring cached t values.
    auto ratio = (t - ts[i - 1]) / (ts[i] - ts[i - 1]);
    // ... and lerp it!
    return lerp(ratio, ss[i], ss[i - 1]) + m_pos;
}

Vec BezierCurve::evalCubicPoint(double tt) const {
    if (pointsP.size() < 4) {
        return Vec();
    }
    // equation used from:
    //   https://en.wikipedia.org/wiki/B%C3%A9zier_curve
    // navigate to the Cubic Bézier curves
    float t    = static_cast<float>(tt);
    auto b0    = (1 - t) * (1 - t) * (1 - t);
    auto b1    = 3 * (1 - t) * (1 - t) * t;
    auto b2    = 3 * (1 - t) * t * t;
    auto b3    = t * t * t;
    Vec target = b0 * pointsV.at(0) + b1 * pointsV.at(1) + b2 * pointsV.at(2)
                 + b3 * pointsV.at(3);
    return target;
}

void BezierCurve::recomputeCurve(int resolution) const {
    assert(!pointsV.empty());
    // assert(!pointsP.empty());
    assert(resolution > 0);

    ss.clear();
    ts.clear();

    ss.push_back(pointsV.front());
    ts.push_back(0);

    const double dt     = 1.0 / resolution;
    const size_t cubics = (pointsV.size() + 1) / 3;
    for (size_t i = 0; i + 3 < pointsV.size(); i += 3) {
        for (double t = 0; t <= 1.0 + dt; t += dt) {
            auto pt = evalCubic(
                pointsV[i], pointsV[i + 1], pointsV[i + 2], pointsV[i + 3], t);
            ss.push_back(pt);
            ts.push_back((t + i / 3) / cubics);
        }
    }
}

void BezierCurve::evalMaxMin() {
    double xMin = pointsV.at(0).x;
    double xMax = pointsV.at(0).x;
    for (int i = 0; i < pointsV.size(); ++i) {
        if (pointsV.at(i).x < xMin) {
            xMin = pointsV.at(i).x;
        }
        if (pointsV.at(i).x > xMax) {
            xMax = pointsV.at(i).x;
        }
    }
    double zMin = pointsV.at(0).z;
    double zMax = pointsV.at(0).z;
    for (int i = 0; i < pointsV.size(); ++i) {
        if (pointsV.at(i).z < zMin) {
            zMin = pointsV.at(i).z;
        }
        if (pointsV.at(i).z > zMax) {
            zMax = pointsV.at(i).z;
        }
    }
    m_xMin = xMin;
    m_xMax = xMax;
    m_zMin = zMin;
    m_zMax = zMax;
}
