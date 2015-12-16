// A class that merges functionality of the world object base + animated md5
// models
#pragma once

#include <string>

#include "WorldObjects/WorldObjectBase.hpp"
#include "MD5/md5mesh.h" //includes md5model.h already
#include "MD5/md5anim.h"

class Md5Object : public WorldObject {
public:
    Md5Object(const std::string &modelFile, float scale);
    Md5Object(const std::string &modelFile, const std::string &animationFile,
              float scale);
    ~Md5Object();

    void update(double t, double dt) override;

protected:
    virtual void internalDraw() const override;

private:
    bool loadModel(const std::string &filename);
    bool loadAnimation(const std::string &filename);

    struct md5_model_t m_model;
    struct md5_anim_t m_animation;
    struct anim_info_t m_anim_info;
    struct md5_joint_t *m_skeleton;

    bool m_animated;
    float m_scale;
};
