#include "StateManager.h"
#include <cassert>

void State::popSelf(unsigned int level, const std::string& pass) {
    assert(isActive);
    sm->pop(level, pass);
}
