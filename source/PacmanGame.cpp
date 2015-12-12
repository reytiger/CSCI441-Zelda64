#include "PacmanGame.hpp"

void PacmanGame::initScene(WorldObject *hero) { m_hero = hero; }

void PacmanGame::draw() const {
    assert(m_hero != nullptr);
    m_hero->draw();
}

void PacmanGame::update(double t, double dt) {
    m_score = as<float>(t);

    // We're dead.
    if (m_hero->radius() == 0.0f) {
        endGame();
    }
}

void PacmanGame::endGame() {
    info("You lose! You lasted %s seconds", m_score);
    exit(0);
}
