#include "WorldObjects/WorldObjModel.hpp"

bool WorldObjModel::loadObjectFile(const std::string &filename) {
    return m_obj.loadObjectFile(filename);
}

void WorldObjModel::internalDraw() const {

    pushMatrixAnd([&]() {
        glTranslatef(m_pos.x, m_pos.y, m_pos.z);
        m_obj.draw();
    });
}
