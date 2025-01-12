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
    vector<PipeIdom*> stack;
    vector<pair<int,int>> occupiedCoords;   //todo: át kéne írni olyan adatszerkezetre amibe 1-nél több ugyanolyan dolgot nem lehet belerakni, mert egyszerre csak 1 idom lehet 1 coorsinátán

    //pair<int,int> actualCoords;
    PipeIdom* actualIdom = source;
    PipeIdom* prevIdom = source;

    occupiedCoords.push_back(source->getCoord());
    occupiedCoords.push_back(sink->getCoord());

    int count = 1;
    while (solution.empty()){//!finish){
        //1. éés 2. feltételek ellenőrzése:
        if(isSinkConnected(grid)){  //todo: feltételezzük hogy a source nem úgy van megadva hogy at egyből kapcsolódik a sink egyetlen kimenetéhez
            if(haveOpenOutput(actualIdom, occupiedCoords)){
                //KAPCS sink-hez és VAN kimenete az utolsónak

            }
            else{
                //KAPCS sink-hez és NICNS kimenete az utolsónak

            }
        }
        else{
            if(haveOpenOutput(actualIdom, occupiedCoords)){
                //NEM kapcs sink-hez és VAN kimenete az utolsónak\n
                //--> választunk egy új actualIdom-ot
                if(!elements.empty()){
                    //Ha nem üres az elements akkor onnan választunk
                    prevIdom = actualIdom;
                    actualIdom = elements[0];
                    elements.pop_back();
                    connect(prevIdom->getCoord(), chooseDirection(prevIdom, occupiedCoords), actualIdom, occupiedCoords);
                    //todo: indul előlről a while
                }
                else{
                    if(!stack.empty()){
                        //Ha üres az elements akkor a stack-ből választunk (ha tudunk)
                        prevIdom = actualIdom;
                        actualIdom = stack[stack.size()-1];
                        stack.pop_back();
                        vector<Directions> tmp_prevDirs(prevIdom->getDirs().begin(), prevIdom->getDirs().end());
                        bool connecteltunk = true;
                        for (int i = 0; i < 3; ++i) {
                            //todo: if(canConnect)
                        }
                    }
                    else{
                        //todo: ilyenkor mit kell csinálni???
                        throw std::runtime_error("There is no idom in either the elements or the stack");
                    }
                }

            }
            else{
                //NEM kapcs sink-hez és NINCS kimenete az utolsónak

            }
        }



        ///Ha a goodSolutions-ben van olyan grid ahol nincs leak akkor véget ér a while.\n
        ///Vagyis jelenleg addig megy a while amíg nem talál perfect solution-t.
        for (auto sol:goodSolutions) {
            if(firstLeak(sol) == nullptr){
                solution = sol;
            }
        }

        count++;
    }
    //WHILE VÉGE



    //solution = goodSolutions[goodSolutions.size()-1];
}


/**
 * @brief "Hozzáileszt egy idom-ot egy másikhoz"
 * @details A kapott koordinátán lévő idom kapott dir-jéhez illeszti a kapott idomot.\n
 * Valójában azt csinálja, hogy a kapott (param: actual) idomot addíg forgatja amíg annak az egyik kimenete és a prev idom egyik kimenete pont
 * ellenkező irányba néznek, a megadott koordinátáknak megfelelően.\n
 * És beállítja a kapott idom coordinátáját.\n
 * Továbbá ezt a koordinátát belerakja az occupiedCoords-ba
 * @param prevCoord A prevIdom coordinátája.
 * @param prevDir Azt adja meg, hogy a prevIdom melyik dir-jéhez akarunk connectelni.\n Ehez MAJDNEM mindig a chooseDirection() fv-t kell meghívni
 * @param actual Ez az az idom amit connectelni szeretnénk.
 * @warning "Ha connectelni szeretnénk, akkor biztosan tudunk is." Vagyis ezt a függvényt csak akkor szabad meghívni ha biztosan el tudja végezni a feladatát.
 */
void Simulation::connect(pair<int, int> prevCoord, Directions prevDir, PipeIdom *&actual, vector<pair<int, int >> &occ_coords) { //todo: tesztelni kellene majd
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
    occ_coords.push_back(actual->getCoord());
}

/**
 * @details Ezt csak a connect használhatja különben megbaszódhat az egész és exceptiont dob.\n
 * Visszaadja egy pipeIdom szabad output-ját.\n
 * Mindenképpen lesz visszatérési értéke mivel ha connect-et hívunk akkor tudunk is connectelni!?
 * @param idom Amelyik idomhoz szeretnénk connectelni. Ez általában a prevIdom
 * @param occ_coords occupiedCoords vector
 * @warning Azt nem vizsgálja hogy ha lerakunk egy idom-ot akkor annak a többi kimenete nem sérti-e a szomszédos koordinátán álló idomokat.
 */
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
 * @details Megnézi, hogy lehet-e connectelni az actual-t a prev-nek a választott dir-jéhez.\n
 * Azt is vizsgálja hogy üres-e a szükséges koordináta,\n illetve azt is hogy ha arra a koordinátára lerakjuk az idomot
 * akkor a többi grid-en lévő idommal nem lesz e érvénytelen csatlakozás, tehát pl nem lesz-e olyan hogy az actual egyik
 * kimenete egy szomszédos idomnak a falához és nem kimenetéhez csatlakozik
 * @param actual Amit csatlakoztatni akarunk
 * @param prev Amihez csatlakoztatni akarunk
 * @param prevsSelectedDir A prev-nek az a dir-je amihez csatlakoztatni akarunk
 * @param grid grid
 * @return true --> ha tudunk \n false --> ha nem tudunk
 * @warning Csak 1 source és 1 sink esetén működik
 * @todo tesztelni kéne
 */
bool Simulation::canConnect(PipeIdom *actual, PipeIdom *prev, Directions prevsSelectedDir, vector<PipeIdom *> grid) {//todo: tesztelni kéne
    vector<PipeIdom*> allIdoms;
    allIdoms.push_back(source);
    allIdoms.insert(allIdoms.end(), grid.begin(), grid.end());
    allIdoms.push_back(sink);

    pair<int, int> actualCoord = prev->getCoord();

        switch (prevsSelectedDir) {
            case RIGHT:
                //coord = {idom->getCoord().first, idom->getCoord().second + 1};
                actualCoord.second++;
                break;
            case UP:
                //coord = {idom->getCoord().first - 1, idom->getCoord().second};
                actualCoord.first--;
                break;
            case LEFT:
                //coord = {idom->getCoord().first, idom->getCoord().second - 1};
                actualCoord.second--;
                break;
            case DOWN:
                //coord = {idom->getCoord().first + 1, idom->getCoord().second};
                actualCoord.first++;
                break;
        }

        //megnézzük hogy szabad-e az óhajtott koordináta
        bool isInOcc = false;
        for (const auto& oc : allIdoms) {
            if (actualCoord == oc->getCoord()) {
                isInOcc = true;
                break;
            }
        }


        //ha szabad az óhajtott koordináta, megnézzük, hogy a szomszédokkal nem lesz-e bunyó
        if(!isInOcc){
            for (auto actDir: actual->getDirs()) {
                pair<int, int> neighbourCoord;
                switch (actDir) {
                    case RIGHT:
                        neighbourCoord = {actualCoord.first, actualCoord.second+1};
                        for (auto idom: allIdoms) {
                            if(neighbourCoord == idom->getCoord()){
                                if(!idom->getDirs().contains(LEFT)){
                                    return false;
                                }
                            }
                        }
                        break;
                    case UP:
                        neighbourCoord = {actualCoord.first-1, actualCoord.second};
                        for (auto idom: allIdoms) {
                            if(neighbourCoord == idom->getCoord()){
                                if(!idom->getDirs().contains(DOWN)){
                                    return false;
                                }
                            }
                        }
                        break;
                    case LEFT:
                        neighbourCoord = {actualCoord.first, actualCoord.second-1};
                        for (auto idom: allIdoms) {
                            if(neighbourCoord == idom->getCoord()){
                                if(!idom->getDirs().contains(RIGHT)){
                                    return false;
                                }
                            }
                        }
                        break;
                    case DOWN:
                        neighbourCoord = {actualCoord.first+1, actualCoord.second};
                        for (auto idom: allIdoms) {
                            if(neighbourCoord == idom->getCoord()){
                                if(!idom->getDirs().contains(UP)){
                                    return false;
                                }
                            }
                        }
                        break;
                }
            }
        }
        else{
            return false;
        }

    return true;
}

/**
 * @brief Megnézi, hogy a kapott @b grid benne van e a badSolutions-ban
 * @param grid
 * @param badSolutions
 * @return Ha igaz akkor benne van, ha nem akkor nincs benne
 */
bool Simulation::isInBadSoulutions(vector<PipeIdom*> grid, vector<vector<PipeIdom*>> badSolutions){ //todo: nincs tesztelve
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
bool Simulation::isConnectedTo(vector<PipeIdom*> grid, pair<int, int> idomCoord, Directions idomDir) {
    for (auto &item: grid) {
        if(item->getCoord() == idomCoord and item->getDirs().contains(idomDir)){
            return true;
        }
    }
    return false;
}

/**
 * true  --> mindegyik dir-jére kapcsolódik element \n
 * false --> van olyan dir-je amire nem kapcsolódik element
*/
bool Simulation::isAllConnected(const vector<PipeIdom*>& grid, PipeIdom* idom) {
    pair<int, int> sinkCoord = idom->getCoord();
    set<Directions> sinkDirs = idom->getDirs();
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

/**
 * @details Végigmegy a kapott tárolón és megnézi hogy van e benne elem a kapott koordinátán.
 * @param inThat Az adott tároló. Fontos hogy ez vector<PipeIdom*> tipusú legyen, pl.: grid (, stack, elements)
 * @param onThatCoord Ezen a koordinátán keresünk idomot
 * @return Ha talált a koordinátán idomot akkor igazzal tér vissza, különben hamissal.
 * @todo lehet majd törölni kéne mert sztem nem fog kelleni
 */
bool Simulation::isIn(vector<PipeIdom*> inThat, pair<int, int> onThatCoord) {
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

/// Megkeresi az első leaket a source, grid és sinkek között majd ezzel a pipeIdom*-mal visszatér.
PipeIdom* Simulation::firstLeak(vector<PipeIdom *> grid) { ///todo: teszt. Majd ha több source és sink lesz, akkor a push_backeket javítani
    vector<PipeIdom*>allElements;

    allElements.push_back(source);
    allElements.insert(allElements.end(), grid.begin(), grid.end());
    allElements.push_back(sink);

    for (auto element: allElements) {
        if (!isAllConnected(allElements, element)) {
            return element;
        }
    }
    return nullptr;
}

/**
 * @details Visszatér egy dir ellentétjével. Pl.: UP-ból DOWN lesz.
 * @param side választott dir
 * @return A side ellentétje.
 */
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

///Kritérium_1 \n Azt nézi meg hogy a sink összes dir-jéhez van e valami connectelve a gridben
bool Simulation::isSinkConnected(vector<PipeIdom *> grid) { //todo: ez csak akkor működik ha egy sink idom van a rendszerben
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

///Nem biztos hogy értem hogy hogy működik - Bálint
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



/**
 * @brief Csak a debugoláshoz kellő kiírató fv
 * @details Kiírja a consolra, hogy hányadik ciklus iteráció van illetve a grid, az elements és a stack összes elemét.
 * @param count searchPath-ben a while ciklus iteráció számolására van kitalálva
 * @param elements
 * @param grid
 * @param stack
 */
void Simulation::testPrintContainers(int count, vector<PipeIdom *> elements, vector<PipeIdom *> grid, vector<PipeIdom *> stack) {
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
}