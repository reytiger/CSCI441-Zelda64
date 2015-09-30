#pragma once
#include "Utils.hpp"

#include "WorldObjects/WorldObjectBase.hpp"

#include <functional>

class CallListObject : public WorldObject {
public:
  ~CallListObject();
  CallListObject() = default;
  CallListObject(const CallListObject &) = default;
  CallListObject &operator=(CallListObject &&other);

  virtual void draw() const;

  // 'count' is how many CallLists are create
  // 'makeit' is a function taking a single GLuint (the return value of
  // glGenLists), which does the initialization.
  CallListObject(std::function<void(GLuint)> makeit);

  GLuint handle() { return m_handle; }

private:
  GLuint m_handle = 0;

  void deleteLists();
};
