#pragma once
#include "Utils.hpp"

#include "Material.hpp"

#include <functional>

class WorldObject {
public:
    // Function type to pass to the internal update function.
    // This lets arbitrary code run on the object update.
    // Passes in a reference to the updating object, total time,
    // and the change in time since the last update.
    // TODO: Do pass in *this - we don't right now so it'll still build.
    // TODO: Don't pass in time - nothing should need it.
    typedef std::function<void(/*WorldObject &,*/ double, double)> UpdateFunc;

    // ==== Constructors and Desctructors =====================================
    // Nothing much to see here. They're all defaulted.

    virtual ~WorldObject() = default;
    WorldObject() = default;

    // ==== Public Interface Methods ==========================================

    // Draw the object by calling the virtual method internalDraw().
    // This acts as a wrapper around it, to ensure things are always done.
    //     e.g. a no-op if m_visible is false.
    void draw() const;

    // Called every frame to update logical components of the object.
    virtual void update(double t, double dt);

    // Helper function to give objects easy WASD control.
    // Q and E move up and down.
    void doWASDControls(float speed, bool *pressed, bool use_QE = false);

    // ==== Motion related methods ============================================

    // Relative motion
    void moveBy(Vec ds) { m_pos += ds; }
    void moveByX(float dx) { m_pos.x += dx; }
    void moveByY(float dy) { m_pos.y += dy; }
    void moveByZ(float dz) { m_pos.z += dz; }

    // Absolute motion
    void moveTo(Vec pos) { m_pos = pos; }
    void moveTo(float x, float y, float z) { m_pos = Vec(x, y, z); }
    void moveToX(float x) { m_pos.x = x; }
    void moveToY(float y) { m_pos.y = y; }
    void moveToZ(float z) { m_pos.z = z; }

    void setVelocity(Vec ds) { m_vel = ds; }
    void setVelocityX(float dx) { m_vel.x = dx; }
    void setVelocityY(float dy) { m_vel.y = dy; }
    void setVelocityZ(float dz) { m_vel.z = dz; }

    // Cameras need to clamp their rotation. There is probably a better way to
    // to do this.
    virtual void rotate(float dtheta, float dphi);

    // ==== Getters and Setters ===============================================

    // Look at specified position.
    void lookAtThing(Vec pos) { lookInDir(pos - this->pos()); }

    // Look in a specified direction.
    void lookInDir(Vec dir) {
        assert(dir.norm() != 0);
        m_arc = dir.normalize().polar();
    }

    // The direction in which the object is looking.
    VecPolar lookDir() const { return m_arc; }

    Vec pos() const { return m_pos; }
    Vec vel() const { return m_vel; }
    Vec up() const { return m_up; }

    void follow(WorldObject *wo);
    void setUpdateFunc(UpdateFunc func) { m_update = func; }

    float radius() const { return m_radius; }
    void radius(float rad) { m_radius = rad; }

    Material material() const { return m_material; }
    void material(const Material &mat) { m_material = mat; }

    bool visible() const { return m_visible; }
    void hide() { m_visible = false; }
    void show() { m_visible = true; }

protected:
    UpdateFunc m_update;

    // TODO: Revamp the follow-system.
    WorldObject *m_follow = nullptr;

    // Keep track of the last place the follow was,
    // so we can undo it before updating.
    Vec m_old_follow_pos;

    Vec m_pos;
    Vec m_up = Vec(0.0, 1.0, 0.0);
    Vec m_vel;

    // The direction the object is "looking" in. Like a rotation.
    // m_arc.r isn't used and should be assumed 1.0.
    VecPolar m_arc;

    Material m_material = Material::WhiteRubber;

    float m_radius = 1.0;
    float m_height = 0.0;

    bool m_visible = true;

    // ==== Protected Virtual Methods
    // ===========================================

    // How the object is rendered. TODO: Take in a Renderer of some sort?
    virtual void internalDraw() const = 0;
};
