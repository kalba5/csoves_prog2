#include "Simulation.hpp"
#include "stdexcept"
/**
 * @brief Simulation konstruktora
 * @param sp
 * @param va
 * @param so
 * @param si
 */
Simulation::Simulation(vector<PipeIdom*> sp, vector<PipeIdom*> va, vector<PipeIdom*> so, vector<PipeIdom*> si) {
    simplePipes = sp;
    valves = va;
    sources = so;
    sinks = si;

    source = sources[0];
    sink = sinks[0];

    //searchPath();     todo:ezt a kommentet majd vissza kell venni ha végeztünk a gány teszteléssel
}

/**
 * @brief Ez a függvény keresi meg a megoldást.
 * @details Jó kérdés, hogy hogyan :(
 */
void Simulation::searchPath() {
    vector<vector<PipeIdom*>> badSolutions;
    vector<vector<PipeIdom*>> goodSolutions;

    vector<PipeIdom*> elements;  //azok a simplePipe-ok és valve-ok vannak benne amik még nincsenek felhasználva
    elements.insert(elements.end(), simplePipes.begin(), simplePipes.end());
    elements.insert(elements.end(), valves.begin(), valves.end()); //ezt még nem használjuk
    vector<PipeIdom*> grid;  //használatban lévő elemek
    vector<pair<int,int>> occupiedCoords;
    vector<PipeIdom*> stack;

    //pair<int,int> actualCoords;
    PipeIdom* actualIdom;
    PipeIdom* prevIdom = source;

    occupiedCoords.push_back(source->getCoord());
    occupiedCoords.push_back(sink->getCoord());

    bool finish = false;
    int count = 1;
    while (count != 15){//!finish){
        if(!elements.empty()){
            if (!grid.empty()){
                prevIdom = grid[grid.size()-1];
            }
            actualIdom = elements[0];

            elements.erase(elements.begin());
            connect(prevIdom->getCoord(), chooseDirection(prevIdom, occupiedCoords), actualIdom);
            grid.push_back(actualIdom);
            occupiedCoords.push_back(actualIdom->getCoord());
        }
        else if(!stack.empty()){
            actualIdom = stack[stack.size()-1];
            stack.pop_back();
            grid.push_back(actualIdom);
            occupiedCoords.push_back(actualIdom->getCoord());
            if (grid.size() >= 2) {
                prevIdom = grid[grid.size() - 2];
            } else {
                prevIdom = source;
            }
            connect(prevIdom->getCoord(), chooseDirection(prevIdom, occupiedCoords) ,actualIdom);
        }
        else{
            cout << "Nem volt item se az elements-ben se a stack-ben" << endl;
        }

        if(!isSinkConnected(grid) and !haveOpenOutput(actualIdom, occupiedCoords)){
            grid.pop_back();
            occupiedCoords.pop_back();
            stack.push_back(actualIdom);
            if(elements.empty() and !grid.empty()){
                actualIdom = grid[grid.size() - 1];
                if (grid.size() >= 2) {
                    prevIdom = grid[grid.size() - 2];
                }
                else {
                    prevIdom = source;
                }

                badSolutions.push_back(grid);

                for (int i = 0; i < actualIdom->getDirs().size(); ++i) {    //todo: forgatásokat tesztelni kéne geci
                    rotateMore(actualIdom, prevIdom);
                    if(isBadSoulution(grid, badSolutions)){
                        continue;
                    }
                    else{
                        break;
                    }
                }

            }
        }






        ///Ha a goodSolutions-ben van olyan grid ahol nincs leak akkor véget ér a while
        for (auto solution:goodSolutions) {
            if(firstLeak(solution) == nullptr){
                finish = true;
            }
        }

        //teszt
        cout << count << "-ik while iteracio!" << endl <<"\n";
        cout << "GRID:" << endl;
        for (auto item: grid) {
            item->printIt();
        }

        cout << "ELEMENTS:" << endl;
        for (auto item: elements) {
            item->printIt();
        }

        cout << "STACK:" << endl;
        for (auto item: stack) {
            item->printIt();
        }
        cout << "\n----------------------------------------" << endl;


        count++;
    }




    /* //ez csak teszteléshez kellett
    if(elements.size() != 0) {
        actualIdom = elements[0];
        elements.erase(elements.begin());
        connect(prevIdom->getCoord(), chooseDirection(prevIdom, occupiedCoords), actualIdom);
        grid.push_back(actualIdom);
        occupiedCoords.push_back(actualIdom->getCoord());

        source->printIt();
        sink->printIt();

        //teszt
        for (auto idom: grid) {
            idom->printIt();
        }
    }
    */


    solution = goodSolutions[goodSolutions.size()-1];
}


/**
 * @brief "Hozzáileszt egy idom-ot egy másikhoz"
 * @details "Ha connectelni szeretnénk, akkor biztosan tudunk is."\n
 * A kapott koordinátán lévő
 * @param prevCoord
 * @param prevDir ehez igazábol mindig a @ref chooseDirection() fv-t kell meghívni
 * @param actual
 */
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

///@details Ezt csak a connect használhatja különben megbaszódhat az egész és exceptiont dob.\n
///Visszaadja egy pipeIdom szabad output-ját.\n
///Mindenképpen lesz visszatérési értéke mivel ha connect-et hívunk akkor tudunk is connectelni.\n
///idom: amihez connectelni akarunk\n
///@param idom Amelyik idomhoz szeretnénk connectelni.
///@param occ_coords occupiedCoords vector
Directions Simulation::chooseDirection(PipeIdom *idom, vector<pair<int, int>> occ_coords) {
    pair<int, int> coord;
    for (auto dir : idom->getDirs()) { // Vizsgáld végig az összes irányt
        switch (dir) {
            case RIGHT:
                coord = {idom->getCoord().first, idom->getCoord().second + 1};
                break;
            case UP:
                coord = {idom->getCoord().first - 1, idom->getCoord().second};
                break;
            case LEFT:
                coord = {idom->getCoord().first, idom->getCoord().second - 1};
                break;
            case DOWN:
                coord = {idom->getCoord().first + 1, idom->getCoord().second};
                break;
        }

        bool isInOcc = false;
        for (const auto& oc : occ_coords) {
            if (oc == coord) {
                isInOcc = true;
                break;
            }
        }
        if (!isInOcc) {
            return dir;
        }
    }
    //Ha nem talál szabad irányt, kivételt dob
    throw std::runtime_error("No available direction to connect!");
}

/**
 * @def Megnézi, hogy a kapott grid benne van e a badSolutions-ban
 * @param grid
 * @param badSolutions
 * @return Ha igaz akkor benne van, ha nem akkor nincs benne
 */
bool Simulation::isBadSoulution(vector<PipeIdom*> grid, vector<vector<PipeIdom*>> badSolutions){ //todo: nincs tesztelve
    //todo: ha lassú lenne a program, először ezt kell átalakítani hash függvényes, unordered_set-es témára. A chatGpt már megírta csak implementálni kell.
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

///ez csak a baszás izéhez kell (eredmény lekéréséhez)
vector<PipeIdom *> Simulation::getSolution() {
    return solution;
}

///Nem biztos hogy értem hogy hogy működik
void Simulation::rotateMore(PipeIdom *&actual, PipeIdom *previous) {
    Directions prevConnectDir;

    if(actual->getCoord().first == previous->getCoord().first){
        if(actual->getCoord().second < previous->getCoord().second){
            prevConnectDir = LEFT;
        }
        else{
            prevConnectDir = RIGHT;
        }
    }
    else{
        if(actual->getCoord().first < previous->getCoord().first){
            prevConnectDir = UP;
        }
        else{
            prevConnectDir = DOWN;
        }
    }

    for (int i = 0; i < 3; ++i) {
        actual->rotate();
        if(actual->getDirs().contains(oppositeSide(prevConnectDir))){
            break;
        }
    }
}

