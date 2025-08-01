#pragma once

#include <functional>
#include <chrono>
#include <iostream>

#include "Engine.hpp"

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
using Milliseconds = std::chrono::milliseconds;
using lli = long long;

struct GameLoopOptions {
  std::function<TimePoint(void)> getTimeFn;
  lli msPerUpdate;
};

lli millisBetween(TimePoint start, TimePoint end);

template <typename T>
T fastForward(Engine<T> *engine, T state, lli lag, lli msPerUpdate);
  
template <typename T>
inline 
T fastForward(Engine<T> *engine, T state, lli lag, lli msPerUpdate) {
  lli delta = 0;
  while (delta < lag && !engine->shouldHalt(state)) {
    state = engine->nextState(state);
    delta+=msPerUpdate;
  }
  return state;
}

template <typename T>
T gameLoop(Engine<T> *engine, T initialState, GameLoopOptions options);

template <typename T>
inline 
T gameLoop(Engine<T> *engine, T initialState, GameLoopOptions options) {
  T state = initialState;
  auto timeFn = options.getTimeFn;
  TimePoint start = timeFn();
  while (!engine->shouldHalt(state)) {
    engine->render(state);
    TimePoint end = timeFn();
    lli lag = millisBetween(start,end);
    state = fastForward(engine,state,lag,options.msPerUpdate);
    start = timeFn();
  }
  return initialState;
}