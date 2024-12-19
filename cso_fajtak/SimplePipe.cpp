//
// Created by Kalász Bálint on 2024. 11. 24..
//

#include "SimplePipe.hpp"

void SimplePipe::printIt() const {
    cout << "SimplePipe_dir:" << endl;
    for(auto it: dirs){
        cout << it << endl;
    }
    cout << "x: " << coord.first << "  y: " << coord.second << endl;
}

string SimplePipe::getColor() const {
    return std::string();
}
