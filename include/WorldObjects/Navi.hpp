#include "WorldObjects/WorldObjModel.hpp"
#include "WorldObjects/Lighting/Light.hpp"

class Navi : public WorldObjModel
{
  public:
    Navi();

  protected:
    virtual void internalDraw() const override;

  private:
    Light m_light;
};
