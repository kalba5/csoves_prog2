//
// Created by Kalász Bálint on 2024. 11. 24..
//

#include "Sink.hpp"

bool Sink::isActive() {
    return status;
}

void Sink::setStatus(bool status) {
    status = status;
}

string Sink::getColor() {
    return color;
}

void Sink::printIt() const {
    cout << "Sink_color: " << color << endl;
    cout << "x: " << coord.first << "  y:" << coord.second << endl;
    cout << "Sink_dir:" << endl;
    for(auto it: dirs){
        cout << it << endl;
    }
}
