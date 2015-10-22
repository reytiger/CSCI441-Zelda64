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
    void draw() const;

    // Called every frame to update logical components of the object.
    void update(double t, double dt);

    // ==== Motion related methods ============================================

    // Relative motion
    void moveBy(Vec ds) { m_pos += ds; }
    void moveByX(double dx) { m_pos.x += dx; }
    void moveByY(double dy) { m_pos.y += dy; }
    void moveByZ(double dz) { m_pos.z += dz; }

    // Absolute motion
    void moveTo(Vec pos) { m_pos = pos; }
    void moveToX(double x) { m_pos.x = x; }
    void moveToY(double y) { m_pos.y = y; }
    void moveToZ(double z) { m_pos.z = z; }

    void setVelocity(Vec ds) { m_vel = ds; }
    void setVelocityX(double dx) { m_vel.x = dx; }
    void setVelocityY(double dy) { m_vel.y = dy; }
    void setVelocityZ(double dz) { m_vel.z = dz; }

    // ==== Getters and Setters ===============================================

    // Look at specified position.
    void lookAtThing(Vec pos) { lookInDir(pos - this->pos()); }

    // Look in a specified direction.
    void lookInDir(Vec dir) {
        assert(dir.norm() != 0);
        m_arc = dir.normalize().polar();
    }

    // The direction in which the object looks.
    VecPolar lookDir() const { return m_arc; }

    Vec pos() const { return m_pos; }
    Vec up() const { return m_up; }

    void follow(WorldObject *wo);
    void setUpdateFunc(UpdateFunc func) { m_update = func; }

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

    double m_radius = 1.0;
    double m_height = 0.0;

    bool m_visible = true;

private:
    // ==== Private Virtual Methods ===========================================

    // The method which describes how to render the image.
    virtual void internalDraw() const = 0;
};

// Inlined functions are good. Clobbering the definition... no so much.
void WorldObject::draw() const {
    if (this->visible()) {
        this->internalDraw();
    }
}