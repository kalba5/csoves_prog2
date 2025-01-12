#include "Valve.hpp"

bool Valve::isOpen() {
    return open;
}

void Valve::setValve(bool val) {
    open = val;
}

void Valve::printIt() const {
    cout << "VALVE:" << endl;
    cout << "open: " << open << endl;
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

string Valve::getColor() const {
    return "";
}

PipeIdom *Valve::clone() const {
    return new Valve(*this);
}
