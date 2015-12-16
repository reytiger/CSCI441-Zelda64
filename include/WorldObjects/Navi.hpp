#include "WorldObjects/WorldObjModel.hpp"
#include "WorldObjects/Lighting/Light.hpp"

class Navi : public WorldObjModel {
public:
    Navi();
    virtual void update(double t, double dt) override;

protected:
    virtual void internalDraw() const override;

private:
    Light m_light;
};
