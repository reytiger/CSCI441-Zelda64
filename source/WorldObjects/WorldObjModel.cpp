#include "WorldObjects/WorldObjModel.hpp"


WorldObjModel::WorldObjModel() { m_material = Material::WhitePlastic; }


bool WorldObjModel::loadObjectFile(const std::string &filename) {
    return m_obj.loadObjectFile(filename);
}

void WorldObjModel::internalDraw() const {

    pushMatrixAnd([&]() {
        glTranslatef(m_pos.x, m_pos.y, m_pos.z);
        glScalef(m_scale, m_scale, m_scale);
        m_obj.draw();
    });
}
