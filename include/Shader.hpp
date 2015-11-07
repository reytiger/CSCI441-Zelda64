#pragma once
#include "Utils.hpp"

#include <map>

class Shader {
public:
    GLint handle() const {
        assert(m_handle != -1);
        return m_handle;
    }

    void loadFromFile(const std::string &filename, GLenum kind);
    void loadFromString(const std::string &str, GLenum kind);

private:
    GLint m_handle = -1;
};


struct ShaderProgram {
public:
    using LocationsMap = std::map<std::string, GLint>;

    static void useFFS() { glUseProgram(0); }

    void create() {
        glChk();
        m_handle = glCreateProgram();
        glChk();
    }

    GLint handle() const {
        assert(m_handle != -1);
        return m_handle;
    }
    void link(const Shader &vert, const Shader &frag);

    void use() const { glUseProgram(handle()); }

    // TODO: template / overload this.
    void attachUniform(const std::string &name, float value);


private:
    GLint m_handle = -1;

    LocationsMap m_uniforms;

    void loadUniformLocation(const std::string &name);
};
