#pragma once
#include "Utils.hpp"

#include "WorldObjects.hpp"

class PacmanGame {
public:
    PacmanGame() = default;

    WorldObject *hero() {
        assert(m_hero != nullptr);
        return m_hero;
    }

    void initScene(WorldObject *hero);
    void draw() const;
    void update(double t, double dt);

    void endGame();

private:
    float m_score       = 0.0f;
    WorldObject *m_hero = nullptr;
};
