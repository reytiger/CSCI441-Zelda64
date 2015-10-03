#include "PrettyGLUT.hpp"

// This function is expected by PrettyGLUT, because I designed it to get done
// fast, not smart. We can change this later, but this makes sure it builds.
// It takes in t and dt, the time and time since the last updateScene was
// called.
void updateScene(double, double) {}

void initScene() {}

int main(int argc, char **argv) {
  srand(time(nullptr));

  PrettyGLUT::initGLUT(&argc, argv);
  PrettyGLUT::printOpenGLInformation();
  initScene();
  PrettyGLUT::start();
  return 0;
}
