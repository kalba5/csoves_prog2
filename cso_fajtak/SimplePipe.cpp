//
// Created by Kalász Bálint on 2024. 11. 24..
//

#include "SimplePipe.hpp"

void SimplePipe::printIt() const {
    cout << "SIMPLE_PIPE:" << endl;
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

string SimplePipe::getColor() const {
    return std::string();
}