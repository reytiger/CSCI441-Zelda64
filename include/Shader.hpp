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


class ShaderProgram {
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

    void attach(const Shader &vert, const Shader &frag);
    void link();

    void use() const { glUseProgram(handle()); }
    void usingProgram(std::function<void(const ShaderProgram &)> code);

    // Attributes
    GLint getAttribLocation(const std::string &name) const;

    // Uniforms
    GLint getUniformLocation(const std::string &name) const;

    void attachUniform(const std::string &name, float value);
    void attachUniform(const std::string &name, Vec value);

private:
    // Default to using the FFS.
    GLint m_handle = 0;

    mutable LocationsMap m_uniforms;
    mutable LocationsMap m_attribs;
};
