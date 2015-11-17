#include "PacmanGame.hpp"

void PacmanGame::initScene(WorldObject *hero) {
    m_hero         = hero;
    m_crowd.target = hero;

    m_crowd.material(Material::Ruby);
}

void PacmanGame::draw() const {
    assert(m_hero != nullptr);
    m_crowd.draw();
    m_hero->draw();
}

void PacmanGame::update(double t, double dt) {
    m_crowd.update(t, dt);
    m_score = t;

    // We're dead.
    if (m_hero->radius() == 0.0f) {
        endGame();
    }
}

void PacmanGame::endGame() {
    info("You lose! You lasted %s seconds", m_score);
    exit(0);
}
