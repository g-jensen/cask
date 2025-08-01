#pragma once

#include "Renderable.hpp"
#include "Steppable.hpp"

template <typename T>
class Engine : public Renderable<T>, public Steppable<T>{
public:
  virtual bool shouldHalt(T state) = 0;
};