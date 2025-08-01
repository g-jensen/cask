#pragma once

template<typename T>
class Renderable {
public:
  virtual void render(T state) = 0;
};