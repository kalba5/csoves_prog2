#include "Source.hpp"

bool Source::isActive() {
    return status;
}

void Source::setStatus(bool _status) {
    status = _status;
}

string Source::getColor() const{
    return color;
}

void Source::printIt() const {
    cout << "SOURCE:" << endl;
    cout << "color: " << color << endl;
    cout << "dirs: ";
    string dir;
    for(auto it: dirs){
        switch (it) {
            case UP:
                dir = "up";
            break;
            case DOWN:
                dir = "down";
            break;
            case LEFT:
                dir = "left";
            break;
            case RIGHT:
                dir = "right";
        }
        cout << dir << ", ";
    }
    cout << endl << "coords: x: " << coord.first << "  y: " << coord.second << endl;
    cout << "--.--.--.--.--.--.--.--.--.--.--" << endl;
}
