#include "WorldObjects/Md5Object.hpp"

#include "Utils/Logging.hpp"

Md5Object::Md5Object(const std::string &modelFile, float scale) {
    m_skeleton = NULL;
    m_animated = false;
    m_scale    = scale;

    material(Material::WhitePlastic);
    registerUpdateFunc();

    loadModel(modelFile);
    glChk();
};


Md5Object::Md5Object(const std::string &modelFile,
                     const std::string &animationFile, float scale) {
    m_skeleton = NULL;
    m_animated = false;
    m_scale    = scale;

    registerUpdateFunc();

    loadModel(modelFile);
    glChk();
    loadAnimation(animationFile);
    glChk();
}


Md5Object::~Md5Object() {
    FreeModel(&m_model);
    FreeAnim(&m_animation);
    if (m_animated && m_skeleton)
        free(m_skeleton);

    FreeVertexArrays();
}


void Md5Object::internalDraw() const {
    md5_mesh_t mesh;

    glPushMatrix();
    glDisable(GL_CULL_FACE);
    glRotatef(-90.f, 1.0, 0.0, 0.0); // orient models along Y instead of Z
    glScalef(m_scale, m_scale, m_scale);
    for (unsigned int i = 0; i < m_model.num_meshes; ++i) {
        mesh = m_model.meshes[i];
        PrepareMesh(&mesh, m_skeleton);
        DrawMesh(&mesh);
    }
    glPopMatrix();

    glEnable(GL_CULL_FACE);
    ShaderProgram::useFFS();
}


bool Md5Object::loadModel(const std::string &filename) {

    setupMD5Shaders(m_shader,
                    "glsl/md5shader.v.glsl",
                    "glsl/md5shader.f.glsl"); // setup our shaders

    if (!ReadMD5Model(filename.c_str(), &m_model)) {
        error("Could not load md5 model from %s\n", filename.c_str());
        return false;
    }
    errno = 0; // will always have not found .tga textures

    m_skeleton = m_model.baseSkel;

    AllocVertexArrays();

    return true;
}

bool Md5Object::loadAnimation(const std::string &filename) {
    if (!ReadMD5Anim(filename.c_str(), &m_animation)) {
        error("Could not load md5 animation from %s\n", filename.c_str());
        FreeAnim(&m_animation);
        return false;
    }

    m_anim_info.curr_frame = 0;
    m_anim_info.next_frame = 1;

    m_anim_info.last_time = 0;
    m_anim_info.max_time  = 1.0 / m_animation.frameRate;

    m_skeleton = (struct md5_joint_t *)malloc(sizeof(struct md5_joint_t)
                                              * m_animation.num_joints);
    m_animated = true;

    return true;
}

void Md5Object::registerUpdateFunc() {
    auto f = [&](double t, double dt) {
        // handle updating the skeleton for animation
        if (m_animated) {
            // get current and next frames
            Animate(&m_animation, &m_anim_info, dt);

            // interpolate the two frames' skeletons
            InterpolateSkeletons(m_animation.skelFrames[m_anim_info.curr_frame],
                                 m_animation.skelFrames[m_anim_info.next_frame],
                                 m_animation.num_joints,
                                 as<float>(m_anim_info.last_time * m_animation.frameRate),
                                 m_skeleton);
        }
    };

    setUpdateFunc(f);
}
