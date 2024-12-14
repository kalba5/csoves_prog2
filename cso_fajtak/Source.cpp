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
    cout << "Source_color: " << color << endl;
    cout << "x: " << coord.first << "  y:" << coord.second << endl;
    cout << "Source_dir:" << endl;
    for(auto it: dirs){
        cout << it << endl;
    }
}
