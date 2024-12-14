#include "Valve.hpp"

bool Valve::isOpen() {
    return open;
}

void Valve::setValve(bool val) {
    open = val;
}

void Valve::printIt() const {
    cout << "Valve_dir:" << endl;
    for(auto it: dirs){
        cout << it << endl;
    }
}

string Valve::getColor() const {
    return std::string();
}
