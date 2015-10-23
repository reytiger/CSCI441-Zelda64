#include "WorldObjects/CallListObject.hpp"

CallListObject::~CallListObject() { deleteLists(); }

CallListObject::CallListObject(std::function<void(GLuint)> makeit)
    : WorldObject() {
    m_handle = glGenLists(1);
    glChk();
    makeit(m_handle);
    glChk();
}

CallListObject &CallListObject::operator=(CallListObject &&other) {
    deleteLists();
    this->m_handle = other.m_handle;
    other.m_handle = 0;
    return *this;
}

void CallListObject::internalDraw() const {
    glChk();
    pushMatrixAnd([&]() {
        glTranslated(m_pos.x, m_pos.y, m_pos.z);

        glCallList(m_handle);
    });
}

void CallListObject::deleteLists() {
    if (m_handle) {
        glDeleteLists(m_handle, 1);
        glChk();
        m_handle = 0;
    }
}
