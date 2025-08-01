#pragma once

template<typename T>
class Steppable {
public:
  virtual T setup(T state) = 0;
  virtual T nextState(T state) = 0;
};