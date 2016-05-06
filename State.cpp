#include "StateManager.h"
#include <cassert>

void State::popSelf(unsigned int level, std::string pass) {
    assert(isActive);
    sm->pop(level, pass);
}
