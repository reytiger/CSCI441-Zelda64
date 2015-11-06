#include "Shader.hpp"

#include <fstream>
#include <sstream>
#include <vector>

void Shader::loadFromFile(const std::string &filename, GLenum kind) {
    std::ifstream fi;
    std::stringstream ss;

    fi.open(filename);
    if (!fi) {
        glChk();
        fatal("Couldn't open '%s'.", filename);
    }

    ss << fi.rdbuf();

    std::string contents = ss.str();
    loadFromString(contents, kind);
}
void Shader::loadFromString(const std::string &str, GLenum kind) {
    glChk();
    m_handle = glCreateShader(kind);
    glChk();

    const char *cstr = str.c_str();
    glShaderSource(handle(), 1, &cstr, nullptr);
    glChk();

    glCompileShader(handle());
    glChk();

    GLint compiled = 0;
    glGetShaderiv(handle(), GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
        GLint len = 0;
        glGetShaderiv(handle(), GL_INFO_LOG_LENGTH, &len);

        std::vector<GLchar> log(len);
        glGetShaderInfoLog(handle(), len, &len, &log[0]);

        auto error = std::string(log.begin(), log.end());
        fatal("%s\n", error);

        glDeleteShader(handle());
        m_handle = -1;
    }
}

void ShaderProgram::link(const Shader &vert, const Shader &frag) {
    glAttachShader(handle(), vert.handle());
    glChk();

    glAttachShader(handle(), frag.handle());
    glChk();

    glLinkProgram(handle());
    glChk();

    GLint linked = 0;
    glGetProgramiv(handle(), GL_LINK_STATUS, &linked);
    glChk();
    if (linked == GL_FALSE) {
        GLint len = 0;
        glGetProgramiv(handle(), GL_INFO_LOG_LENGTH, &len);

        std::vector<GLchar> log(len);
        glGetProgramInfoLog(handle(), len, &len, &log[0]);

        auto error = std::string(log.begin(), log.end());
        fatal("%s\n", error);

        glDeleteShader(handle());
        m_handle = -1;

        glDeleteProgram(handle());
        glDeleteShader(vert.handle());
        glDeleteShader(frag.handle());
    }
    glChk();

    // Always detach shaders after a successful link.
    glDetachShader(handle(), vert.handle());
    glChk();

    glDetachShader(handle(), frag.handle());
    glChk();
}
