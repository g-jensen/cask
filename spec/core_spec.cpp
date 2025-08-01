#include "catch.hpp"
#include "bdd.hpp"

#include "headers/core_spec.hpp"
#include "../src/headers/core.hpp"

int millisecondsStub = 0;
int nextStateInvocations = 0;
int renderInvocations = 0;

void resetStubs() {
  millisecondsStub = 0;
  nextStateInvocations = 0;
  renderInvocations = 0;
}

template <int iterations>
int InterationEngine<iterations>::setup(int state) {
  return state;
}

template <int iterations>
int InterationEngine<iterations>::nextState(int state) {
  nextStateInvocations++;
  return state+1;
}

template <int iterations>
bool InterationEngine<iterations>::shouldHalt(int state) {
  return state == iterations;
}

template <int iterations>
void InterationEngine<iterations>::render(int state) {
  (void)state; // for compiler warnings
  renderInvocations++;
  return;
}

std::function<TimePoint(void)> timeStubFn(uint timeStep) {
  return [timeStep](){
    auto time = (TimePoint)Milliseconds{millisecondsStub};
    millisecondsStub += timeStep;
    return time;
  };
}

GameLoopOptions options(lli msPerUpdate, uint timeStep) {
  return {.getTimeFn=timeStubFn(timeStep),.msPerUpdate=msPerUpdate};
}

describe("Core", [] {

  context("gameLoop", [] {
    before_each([] {
      resetStubs();
    });

    it("steps once", [] {
      auto engine = InterationEngine<1>();
      gameLoop(&engine,0,options(1,1));
      should_eq(1,nextStateInvocations);
    });

    it("steps twice", [] {
      auto engine = InterationEngine<2>();
      gameLoop(&engine,0,options(1,1));
      should_eq(2,nextStateInvocations);
    });

    it("starts at initial state", [] {
      auto engine = InterationEngine<2>();
      gameLoop(&engine,1,options(1,1));
      should_eq(1,nextStateInvocations);
    });

    it("renders once", [] {
      auto engine = InterationEngine<1>();
      gameLoop(&engine,0,options(1,1));
      should_eq(1,renderInvocations);
    });

    it("game logic and rendering are perfectly in sync", [] {
      auto engine = InterationEngine<6>();
      gameLoop(&engine,0,options(1,1));
      should_eq(6,nextStateInvocations);
      should_eq(6,renderInvocations);
    });

    it("each render takes 2 ms", [] {
      auto engine = InterationEngine<6>();
      gameLoop(&engine,0,options(1,2));
      should_eq(6,nextStateInvocations);
      should_eq(3,renderInvocations);
    });

    it("each render takes 4 ms", [] {
      auto engine = InterationEngine<6>();
      gameLoop(&engine,0,options(1,4));
      should_eq(6,nextStateInvocations);
      should_eq(2,renderInvocations);
    });

    it("returns final state", [] {
      auto engine = InterationEngine<6>();
      should_eq(6,gameLoop(&engine,0,options(1,1)));
    });
  });
});