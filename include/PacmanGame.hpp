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

private:
    WorldObject *m_hero = nullptr;
    EnemyCrowd m_crowd;
};
