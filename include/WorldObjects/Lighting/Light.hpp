#pragma once
#include "Utils.hpp"

#include "WorldObjects/WorldObjectBase.hpp"

class Light : public WorldObject {
public:
    // A count of active lights.
    static GLint s_lights;

    Light() = default;
    virtual ~Light() override { glDisable(m_lightid); }

    virtual void update(float t, float dt) override;
    void enable();

    Color ambient() { return m_ambient; }
    Color diffuse() { return m_diffuse; }
    Color specular() { return m_specular; }

    void ambient(const float *colorv);
    void diffuse(const float *colorv);
    void specular(const float *colorv);

protected:
    // These are the defaults OpenGL uses.
    // See:
    // https://msdn.microsoft.com/en-us/library/windows/desktop/dd373578%28v=vs.85%29.aspx
    Color m_ambient  = Color(0.0, 0.0, 0.0);
    Color m_diffuse  = Color(1.0, 1.0, 1.0);
    Color m_specular = Color(1.0, 1.0, 1.0);

    // Each lighting object corresponds to a different GL_LIGHTi variable.
    // 0 is an invalid enum, but OpenGL guarentees that GL_LIGHTi = GL_LIGHT0 +
    // i,
    // and we take advantage of that.
    // This value is safe to pass to OpenGL calls like glLightfv().
    GLint m_lightid = 0;

    void sanity_check() const;
    virtual void internalDraw() const override;
};
