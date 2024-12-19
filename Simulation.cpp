#include "Simulation.hpp"

Simulation::Simulation(vector<PipeIdom*> sp, vector<PipeIdom*> va, vector<PipeIdom*> so, vector<PipeIdom*> si) {
    simplePipes = sp;
    valves = va;
    sources = so;
    sinks = si;

    searchPath();
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

    pair<int,int> actualCoords;
    PipeIdom* actualIdom;

    occupiedCoords.push_back(source->getCoord());
    occupiedCoords.push_back(sink->getCoord());

    bool finish = false;
    while (!finish){




        ///Ha a goodSolutions-ben van olyan grid ahol nincs leak akkor véget ér a while
        for (auto solution:goodSolutions) {
            if(firstLeak(solution) == nullptr){
                finish = true;
            }
        }
    }
    solution = goodSolutions[goodSolutions.size()-1];
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

///Megnézi hogy a kapott grid benne-van e a badSolutions-ben
bool Simulation::isBadSoulution(vector<PipeIdom *> grid, vector<vector<PipeIdom *>> badSolutions){ //todo: nincs tesztelve
    for (int i = 0; i < badSolutions.size(); i++)
    {
        for (int j = 0; j < grid.size(); j++)
        {
            if (badSolutions[i][j]->getCoord() == grid[j]->getCoord() and badSolutions[i][j]->getDirs() == grid[j]->getDirs())
            {
                return true;
            }
        }
    }
    return false;
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
    //pénisz
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

///kap direction-t és megnézi h a kapott pipeidom-nak van e ilyenje
bool Simulation::haveThatDirection(PipeIdom *lmnt, Directions dir) { //todo:lehet majd törölni kell
    for (auto d: lmnt->getDirs()) {
        if(d == dir){
            return true;
        }
    }
    return false;
}

/// Megnézi az első leaket a source, grid és sinkek között.
PipeIdom* Simulation::firstLeak(vector<PipeIdom *> grid) { ///todo: teszt, majd ha töbhb source és sink lesz, akkor a push_backeket javítani

    vector<PipeIdom*>allElements;

    allElements.push_back(source);
    allElements.insert(allElements.end(), grid.begin(), grid.end());
    allElements.push_back(sink);

    for (auto element: allElements) {
        if (isAllConnected(allElements, element)) {
            return element;
        }
    }
    return nullptr;
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

///Van e kimenete az utoljára lerakott idom-nak vagyis a kritérium_2
bool Simulation::haveOpenOutput(PipeIdom* idom, vector<pair<int, int>> occ_coords) { //todo: TESZTELNI!!!
    for (auto item: idom->getDirs()) {
        pair<int,int> coord;
        switch (item) {
            case RIGHT:
                coord.first = idom->getCoord().first;
                coord.second = idom->getCoord().second+1;

                for (int i = 0; i < occ_coords.size(); ++i) {
                    if(!(occ_coords[i] == coord)){
                        return true;
                    }
                }
                break;
            case UP:
                coord.first = idom->getCoord().first-1;
                coord.second = idom->getCoord().second;

                for (int i = 0; i < occ_coords.size(); ++i) {
                    if(!(occ_coords[i] == coord)){
                        return true;
                    }
                }
                break;
            case LEFT:
                coord.first = idom->getCoord().first;
                coord.second = idom->getCoord().second-1;

                for (int i = 0; i < occ_coords.size(); ++i) {
                    if(!(occ_coords[i] == coord)){
                        return true;
                    }
                }
                break;
            case DOWN:
                coord.first = idom->getCoord().first+1;
                coord.second = idom->getCoord().second;

                for (int i = 0; i < occ_coords.size(); ++i) {
                    if(!(occ_coords[i] == coord)){
                        return true;
                    }
                }
                break;
        }
    }
    return false;
}

///Kritérium_1
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

///Ezt csak a connect használhatja különben megbaszódhat az egész
///Visszaadja egy pipeIdom szabad output-ját
///Mindenképpen lesz visszatérési értéke mivel ha connect-et hívunk akkor tudunk is connectelni.
///idom: amihez connectelni akarunk
Directions Simulation::chooseDirection(PipeIdom *idom, vector<pair<int, int>> occ_coords) {     //todo: TESZTELNIIIIIIIII
    pair<int,int> coord;
    if(idom->getDirs().contains(RIGHT)){
        coord.first = idom->getCoord().first;
        coord.second = idom->getCoord().second+1;
        for (auto occ_item: occ_coords) {
            if(occ_item != coord){
                return RIGHT;
            }
        }
    }

    if(idom->getDirs().contains(UP)){
        coord.first = idom->getCoord().first-1;
        coord.second = idom->getCoord().second;
        for (auto occ_item: occ_coords) {
            if(occ_item != coord){
                return UP;
            }
        }
    }

    if(idom->getDirs().contains(LEFT)){
        coord.first = idom->getCoord().first;
        coord.second = idom->getCoord().second-1;
        for (auto occ_item: occ_coords) {
            if(occ_item != coord){
                return LEFT;
            }
        }
    }

    if(idom->getDirs().contains(DOWN)){
        coord.first = idom->getCoord().first+1;
        coord.second = idom->getCoord().second;
        for (auto occ_item: occ_coords) {
            if(occ_item != coord){
                return DOWN;
            }
        }
    }
}




///ez csak a baszás izéhez kell
vector<PipeIdom *> Simulation::getSolution() {
    return solution;
}

