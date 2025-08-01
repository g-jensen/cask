#pragma once

#include "../../src/headers/Engine.hpp"

template <int iterations>
class InterationEngine : public Engine<int> {
  int setup(int state);
  int nextState(int state);
  bool shouldHalt(int state);
  void render(int state);
};