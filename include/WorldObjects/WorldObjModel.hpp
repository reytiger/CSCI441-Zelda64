// A world object that uses a wavefront OBJ
#pragma once

#include <string>

#include "WorldObjects/WorldObjectBase.hpp"
#include "ModelLoader.hpp"

class WorldObjModel : public WorldObject {
public:
    bool loadObjectFile(const std::string &filename);

protected:
    virtual void internalDraw() const override;

private:
    paone::Object m_obj;
};
