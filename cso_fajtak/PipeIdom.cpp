#include "PipeIdom.hpp"

void PipeIdom::rotate() {
    set<Directions> newSet;
    for (auto it: dirs) {
        switch (it) {
            case UP:
                newSet.insert(LEFT);
                break;
            case LEFT:
                newSet.insert(DOWN);
                break;
            case DOWN:
                newSet.insert(RIGHT);
                break;
            case RIGHT:
                newSet.insert(UP);
                break;
        }
    }
    dirs = newSet;
}

set<Directions> PipeIdom::getDirs() {
    return dirs;
}

pair<int, int> PipeIdom::getCoord() {
    return coord;
}

Types PipeIdom::getType() const {
    return type;
}


//todo: Törölni
void PipeIdom::printDirs() {
    for(auto it: dirs){
        cout << it << endl;
    }
}

void PipeIdom::setCoord(int a, int b) {
    coord.first = a;
    coord.second = b;
}

bool PipeIdom::operator==(const PipeIdom &other) const {
    return dirs == other.dirs &&
           coord.first == other.coord.first &&
           coord.second == other.coord.second;
}


