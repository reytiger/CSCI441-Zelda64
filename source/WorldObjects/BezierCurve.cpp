#include "WorldObjects/BezierCurve.hpp"

#include <fstream>
#include <vector>

Vec BezierCurve::evalCubic(Vec p0, Vec p1, Vec p2, Vec p3, float t) const {
    auto b0 = (1 - t) * (1 - t) * (1 - t);
    auto b1 = 3 * (1 - t) * (1 - t) * t;
    auto b2 = 3 * (1 - t) * t * t;
    auto b3 = t * t * t;
    return m_pos + (b0 * p0) + (b1 * p1) + (b2 * p2) + (b3 * p3);
}

Vec BezierCurve::evalCubicDeriv(Vec p0, Vec p1, Vec p2, Vec p3, float t) const {
    auto b0 = 3 * (1 - t) * (1 - t);
    auto b1 = 6 * (1 - t) * t;
    auto b2 = 3 * t * t;
    return b0 * (p1 - p0) + b1 * (p2 - p1) + b2 * (p3 - p2);
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
        float x = strtof(str.c_str(), nullptr);
        check();

        getline(fi, str, ',');
        float y = strtof(str.c_str(), nullptr);
        check();

        // The third value isn't delimited by a comma, but rather a newline.
        getline(fi, str, '\n');
        float z = strtof(str.c_str(), nullptr);
        check();

        m_points.emplace_back(x, y, z);
    }

#undef check

    if (expected_count != m_points.size()) {
        // The standard way to print type size_t is to use %zu, but Windows
        // doesn't support it. Instead, we cast to a float. (Which is portable
        // and
        // hard to break.)
        printf("Read in %.0f control m_points from \"%s\", but expected %d.\n",
               (float)m_points.size(),
               filename.c_str(),
               expected_count);
        for (Vec pt : m_points) {
            printf("%f, %f, %f\n", pt.x, pt.y, pt.z);
        }
    }

    recomputeCurve(10000);
}

void BezierCurve::internalDraw() const {
    glLineWidth(5.0f);
    if (drawCage) {
        // Draw the m_points
        glDisable(GL_LIGHTING);
        glColor3d(0.0, 1.0, 0.0);
        for (Vec point : m_points) {
            pushMatrixAnd([=]() {

                glTranslated(
                    point.x + m_pos.x, point.y + m_pos.y, point.z + m_pos.z);
                glScaled(0.01, 0.01, 0.01);

                glutSolidSphere(1.0, 50, 50);
            });
        }

        // And then the connections.
        for (size_t i = 1; i < m_points.size(); i += 1) {
            glColor3d(1.0, 1.0, 0.0);
            pushMatrixAnd([this, i]() {
                glBegin(GL_LINES);
                auto a = m_points[i - 1] + m_pos;
                auto b = m_points[i] + m_pos;
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
        float dt = 1.0f / 45;
        glBegin(GL_LINES);
        for (float t = 0; t <= 1.0; t += dt) {
            auto point = eval_t(t);
            glVertex3d(point.x, point.y, point.z);
        }
        glEnd();
        glEnable(GL_LIGHTING);
    }
}

void BezierCurve::drawCurve() const {
    glDisable(GL_LIGHTING);
    glColor3d(0.0, 0.0, 1.0);
    float dt = 1.0f / 45;
    glBegin(GL_LINES);
    // TODO: draw the vec as well.
    for (float t = 0; t <= 1.0; t += dt) {
        auto point = eval_t(t);
        glVertex3d(point.x, point.y, point.z);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

Vec BezierCurve::eval_arc(float arc) const {
    if (m_cache_arc.size() == 0) {
        recomputeCurve(100);
    }

    arc = fmod(arc, m_cache_arc.back());

    size_t idx = 1;
    while (idx < m_cache_arc.size() && m_cache_arc[idx] < arc) {
        idx += 1;
    }
    assert(idx < m_cache_arc.size());

    // Find how close we are to the two neighboring cached t values.
    auto ratio = (arc - m_cache_arc[idx - 1]) //
                 / (m_cache_arc[idx] - m_cache_arc[idx - 1]);
    return lerp(ratio, m_cache_pos[idx], m_cache_pos[idx - 1]) + m_pos;
}

Vec BezierCurve::eval_t(float t) const {
    if (m_cache_t.size() == 0) {
        recomputeCurve(100);
    }

    t = fmod(t, 1.0f);

    size_t idx = 1;
    while (idx < m_cache_t.size() && m_cache_t[idx] < t) {
        idx += 1;
    }

    // Find how close we are to the two neighboring cached t values.
    auto ratio = (t - m_cache_t[idx - 1]) //
                 / (m_cache_t[idx] - m_cache_t[idx - 1]);
    return lerp(ratio, m_cache_pos[idx], m_cache_pos[idx - 1]) + m_pos;
}

Vec BezierCurve::eval_deriv_arc(float arc) const {
    if (m_cache_arc.size() == 0) {
        recomputeCurve(100);
    }

    arc = fmod(arc, m_cache_arc.back());

    size_t idx = 1;
    while (idx < m_cache_arc.size() && m_cache_arc[idx] < arc) {
        idx += 1;
    }
    assert(idx < m_cache_arc.size());

    // Find how close we are to the two neighboring cached t values.
    auto ratio = (arc - m_cache_arc[idx - 1]) //
                 / (m_cache_arc[idx] - m_cache_arc[idx - 1]);
    return lerp(ratio, m_cache_deriv[idx], m_cache_deriv[idx - 1]);
}

Vec BezierCurve::eval_deriv_t(float t) const {
    if (m_cache_t.size() == 0) {
        recomputeCurve(100);
    }

    t = fmod(t, 1.0f);

    size_t idx = 1;
    while (idx < m_cache_t.size() && m_cache_t[idx] < t) {
        idx += 1;
    }

    // Find how close we are to the two neighboring cached t values.
    auto ratio = (t - m_cache_t[idx - 1]) //
                 / (m_cache_t[idx] - m_cache_t[idx - 1]);
    return lerp(ratio, m_cache_deriv[idx], m_cache_deriv[idx - 1]);
}

void BezierCurve::recomputeCurve(int resolution) const {
    assert(!m_points.empty());
    assert(resolution > 0);

    m_cache_t.clear();
    m_cache_pos.clear();
    m_cache_arc.clear();

    const float dt      = 1.0f / resolution;
    const size_t cubics = (m_points.size() + 1) / 3;

    for (size_t i = 0; i + 3 < m_points.size(); i += 3) {
        Vec last;
        float arc;
        for (float t = 0; t <= 1.0 + dt; t += dt) {
            auto pt = evalCubic(m_points[i],
                                m_points[i + 1],
                                m_points[i + 2],
                                m_points[i + 3],
                                t);
            if (m_cache_t.empty()) {
                last = pt;
                arc  = 0.0;
            } else {
                last = m_cache_pos.back();
                arc  = (pt - last).norm() + m_cache_arc.back();
            }
            auto deriv = evalCubicDeriv(m_points[i],
                                        m_points[i + 1],
                                        m_points[i + 2],
                                        m_points[i + 3],
                                        t);

            m_cache_t.emplace_back((t + i / 3) / cubics);
            m_cache_arc.emplace_back(arc);
            m_cache_pos.emplace_back(pt);
            m_cache_deriv.emplace_back(deriv);
        }
    }

    assert(!m_cache_t.empty());

    assert(m_cache_t.size() == m_cache_pos.size());
    assert(m_cache_t.size() == m_cache_arc.size());
}

void BezierCurve::evalMaxMin() {
    float xMin = m_points.at(0).x;
    float xMax = m_points.at(0).x;
    for (size_t i = 0; i < m_points.size(); ++i) {
        if (m_points.at(i).x < xMin) {
            xMin = m_points.at(i).x;
        }
        if (m_points.at(i).x > xMax) {
            xMax = m_points.at(i).x;
        }
    }
    float zMin = m_points.at(0).z;
    float zMax = m_points.at(0).z;
    for (size_t i = 0; i < m_points.size(); ++i) {
        if (m_points.at(i).z < zMin) {
            zMin = m_points.at(i).z;
        }
        if (m_points.at(i).z > zMax) {
            zMax = m_points.at(i).z;
        }
    }
    m_xMin = xMin;
    m_xMax = xMax;
    m_zMin = zMin;
    m_zMax = zMax;
}
