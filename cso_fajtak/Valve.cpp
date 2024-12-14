#include "Valve.hpp"

bool Valve::isOpen() {
    return open;
}

void Valve::setValve(bool val) {
    open = val;
}
