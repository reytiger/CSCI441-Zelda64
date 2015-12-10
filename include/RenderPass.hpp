#pragma once
#include "Utils.hpp"

#include "Shader.hpp"

#include <functional>

class RenderPass {
public:
    using SetupFunc = std::function<void()>;

    // Setup and render a quad to use for this render pass.
    static void renderQuad();

    RenderPass() = default;

    void setup(SetupFunc func);
    void render();

    void program(const ShaderProgram &program) { m_program = program; }
    ShaderProgram &program() { return m_program; }

private:
    SetupFunc m_setup;
    ShaderProgram m_program;
};
