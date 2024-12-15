#include "Simulation.hpp"

Simulation::Simulation(vector<PipeIdom*> sp, vector<PipeIdom*> va, vector<PipeIdom*> so, vector<PipeIdom*> si) {
    simplePipes = sp;
    valves = va;
    sources = so;
    sinks = si;
    /*
    for (auto & i : so) {
        if (i->getColor() == "GREEN"){
            greenSource = dynamic_cast<Source *>(i);
        }
        else if(i->getColor() == "BLUE"){
            blueSource = dynamic_cast<Source *>(i);
        }
        else if(i->getColor() == "RED"){
            redSource = dynamic_cast<Source *>(i);
        }
        else if(i->getColor() == "YELLOW"){
            yellowSource = dynamic_cast<Source *>(i);
        }
        else if(i->getColor() == "PURPLE"){
            purpleSource = dynamic_cast<Source *>(i);
        }
    }
    for (auto & i : si) {
        if (i->getColor() == "GREEN"){
            greenSink = dynamic_cast<Sink *>(i);
        }
        else if(i->getColor() == "BLUE"){
            blueSink = dynamic_cast<Sink *>(i);
        }
        else if(i->getColor() == "RED"){
            redSink = dynamic_cast<Sink *>(i);
        }
        else if(i->getColor() == "YELLOW"){
            yellowSink = dynamic_cast<Sink *>(i);
        }
        else if(i->getColor() == "PURPLE"){
            purpleSink = dynamic_cast<Sink *>(i);
        }
    }
     */
}


void Simulation::searchPath() {
    vector<vector<PipeIdom*>> badSolutions;
    vector<vector<PipeIdom*>> goodSolutions;

    vector<PipeIdom*> elements;  //azok a simplePipe-ok és valve-ok vannak benne amik még nincsenek felhasználva
    elements.insert(elements.end(), simplePipes.begin(), simplePipes.end());
    elements.insert(elements.end(), valves.begin(), valves.end()); //ezt még nem használjuk
    vector<PipeIdom*> grid;  //használatban lévő elemek
    vector<pair<int,int>> occupiedCoords;
    vector<PipeIdom*> stack;

    int whichSide = 1;
    int elementsSize = elements.size();
    for (int i = 0; i < elementsSize; ++i) {
        PipeIdom* actualElement = elements[i]; //todo: azért majd teszteljük egyszer
        Directions sourceFirstDir = *source->getDirs().begin();


    }
    //proba push a githubra
}


///kap egy pipe idom-ot, és egy másik idomnak a coord-ját és a kiválasztott dirjét
void Simulation::connect(pair<int, int> prevCoord, Directions prevDir, PipeIdom*& actual) { //todo: tesztelni kellene majd
    switch (prevDir) {
        case RIGHT:
            actual->setCoord(prevCoord.first, prevCoord.second+1);
            while (!actual->getDirs().contains(LEFT)) {
                actual->rotate();
            }
            break;
        case UP:
            actual->setCoord(prevCoord.first-1, prevCoord.second);
            while (!actual->getDirs().contains(DOWN)) {
                actual->rotate();
            }
            break;
        case LEFT:
            actual->setCoord(prevCoord.first, prevCoord.second-1);
            while (!actual->getDirs().contains(RIGHT)) {
                actual->rotate();
            }
            break;
        case DOWN:
            actual->setCoord(prevCoord.first+1, prevCoord.second);
            while (!actual->getDirs().contains(UP)) {
                actual->rotate();
            }
            break;
    }
}


///isConnectedTo akkor tér vissza igazzal ha a kapott coordinátán létezik element\n
/// és annak a dirs-e tartalmaz olyat amit kapott paraméterként a fv.

bool Simulation::isConnectedTo(vector<PipeIdom*> grid, pair<int, int> lmntCoord, Directions lmntDir) {
    for (auto &item: grid) {
        if(item->getCoord() == lmntCoord and item->getDirs().contains(lmntDir)){
            return true;
        }
    }
    return false;
}

bool Simulation::isAllConnected(const vector<PipeIdom*>& grid, PipeIdom* sinkItem) {
    ///true  --> mindegyik dir-jére kapcsolódik element \n
    ///false --> van olyan dir-je amire nem kapcsolódik element
    pair<int, int> sinkCoord = sinkItem->getCoord();
    set<Directions> sinkDirs = sinkItem->getDirs();
    bool right = false;
    bool up = false;
    bool left = false;
    bool down = false;

    //megnézi, hogy mindegyik directionjére igaz-e
    for (auto dirs_i: sinkDirs) {
        switch (dirs_i) {
            case RIGHT:
                right = isConnectedTo(grid, pair<int, int>(sinkCoord.first, sinkCoord.second + 1), LEFT);
                if(!right){
                    return false;
                }
                break;
            case UP:
                up = isConnectedTo(grid, pair<int, int>(sinkCoord.first - 1, sinkCoord.second), DOWN);
                if(!up){
                    return false;
                }
                break;
            case LEFT:
                left = isConnectedTo(grid, pair<int, int>(sinkCoord.first, sinkCoord.second - 1), RIGHT);
                if(!left){
                    return false;
                }
                break;
            case DOWN:
                down = isConnectedTo(grid, pair<int, int>(sinkCoord.first + 1, sinkCoord.second), UP);
                if(!down){
                    return false;
                }
                break;
        }
    }
    return true;
}

bool Simulation::isIn(vector<PipeIdom *> inThat, pair<int, int> onThatCoord) {
    for (auto& item: inThat) {
        if(item->getCoord() == onThatCoord){
            return true;
        }
    }
    return false;
}

bool Simulation::haveThatDirection(PipeIdom *lmnt, Directions dir) {
    for (auto d: lmnt->getDirs()) {
        if(d == dir){
            return true;
        }
    }
    return false;
}

bool Simulation::isSinkConnected(vector<PipeIdom *> grid) { //todo: azért majd tesztelni kéne
    for (auto sinkDir: sink->getDirs()) {
        for (auto idom: grid) {
            if(idom->getDirs().contains(oppositeSide(sinkDir))){
                return true;
            }
        }
    }
    return false;
}

Directions Simulation::oppositeSide(Directions side) {
    switch (side) {
        case RIGHT:
            return LEFT;
        case LEFT:
            return RIGHT;
        case UP:
            return DOWN;
        case DOWN:
            return UP;
    }
}


