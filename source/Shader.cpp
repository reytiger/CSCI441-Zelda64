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

GLint ShaderProgram::getAttribLocation(const std::string &name) const {
    auto search = m_attribs.find(name);
    if (search != m_attribs.end()) {
        return search->second;
    }

    auto loc = glGetAttribLocation(handle(), name.c_str());
    glChk();
    assert(loc != 0);

    m_attribs[name] = loc;
    return loc;
}

// TODO: It'd be nice to template these.
void ShaderProgram::attachUniform(const std::string &name, float value) {
    usingProgram([&name, &value](const ShaderProgram &self) {
        auto loc = self.getUniformLocation(name);
        glUniform1f(loc, value);
        glChk();
    });
}

void ShaderProgram::attachUniform(const std::string &name, GLint value) {
    usingProgram([&name, &value](const ShaderProgram &self) {
        auto loc = self.getUniformLocation(name);
        glUniform1i(loc, value);
        glChk();
    });
}

void ShaderProgram::attachUniform(const std::string &name, Vec value) {
    usingProgram([&name, &value](const ShaderProgram &self) {
        auto loc = self.getUniformLocation(name);
        glUniform3fv(loc, 1, value.v);
        glChk();
    });
}


void ShaderProgram::usingProgram(
    std::function<void(const ShaderProgram &)> code) {

    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    glChk();

    glUseProgram(handle());
    glChk();

    code(*this);
    glChk();

    glUseProgram(current);
    glChk();
}

GLint ShaderProgram::getUniformLocation(const std::string &name) const {
    auto search = m_uniforms.find(name);
    if (search != m_uniforms.end()) {
        return search->second;
    }

    auto loc = glGetUniformLocation(handle(), name.c_str());

    m_uniforms[name] = loc;
    return loc;
}

void ShaderProgram::attach(const Shader &vert, const Shader &frag) {
    glAttachShader(handle(), vert.handle());
    glChk();

    glAttachShader(handle(), frag.handle());
    glChk();
}

void ShaderProgram::link() {
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
    }
    glChk();
}
