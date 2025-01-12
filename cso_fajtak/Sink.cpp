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

string Sink::getColor() const{
    return color;
}

void Sink::printIt() const {
    cout << "SINK:" << endl;
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

PipeIdom *Sink::clone() const {
    return new Sink(*this);
}
