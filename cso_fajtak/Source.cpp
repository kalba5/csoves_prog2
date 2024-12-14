#include "Source.hpp"

bool Source::isActive() {
    return status;
}

void Source::setStatus(bool _status) {
    status = _status;
}

string Source::getColor() {
    return color;
}
