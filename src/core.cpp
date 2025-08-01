#include "headers/core.hpp"

lli millisBetween(TimePoint start, TimePoint end) {
    return (std::chrono::duration_cast<Milliseconds>(end - start)).count();
}