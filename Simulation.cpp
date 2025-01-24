#include "Simulation.hpp"
#include "stdexcept"
#include <algorithm>
#include <iterator>
#include <cstdlib>
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
    vector<pair<int,int>> occupiedCoords;

    //pair<int,int> actualCoords;
    PipeIdom* actualIdom = source;
    PipeIdom* prevIdom = source;

    for (auto s: sources) {
        occupiedCoords.push_back(s->getCoord());
    }

    for (auto s: sinks) {
        occupiedCoords.push_back(s->getCoord());
    }

    //Ha az elements üres akkor az a feladat hogy a source-okat kell lepakolni
    struct EmptySpace{
        pair<int,int> coord;
        set<Directions> neighboursDirs; //Amelyik irányba mutatnia kell annak ami ezen a koordinátán lesz.
        string color;
    };
    vector<EmptySpace> emptySpaces;
    if(elements.empty()){
        for (auto _sink: sinks) {
            for (auto sinkDir: _sink->getDirs()) {
                pair<int, int> neighbourCoord = _sink->getCoord();

                switch (sinkDir) {
                    case RIGHT:
                        //coord = {idom->getCoord().first, idom->getCoord().second + 1};
                        neighbourCoord.second++;
                        break;
                    case UP:
                        //coord = {idom->getCoord().first - 1, idom->getCoord().second};
                        neighbourCoord.first--;
                        break;
                    case LEFT:
                        //coord = {idom->getCoord().first, idom->getCoord().second - 1};
                        neighbourCoord.second--;
                        break;
                    case DOWN:
                        //coord = {idom->getCoord().first + 1, idom->getCoord().second};
                        neighbourCoord.first++;
                        break;
                }

                bool isIn = false;
                for (auto& emptySpace: emptySpaces) {
                    if(neighbourCoord == emptySpace.coord){
                        emptySpace.neighboursDirs.insert(oppositeSide(sinkDir));
                        isIn = true;
                        break;
                    }
                }

                if(!isIn){
                    EmptySpace eS;
                    eS.coord = neighbourCoord;
                    eS.neighboursDirs.insert(oppositeSide(sinkDir));
                    eS.color = _sink->getColor();
                    emptySpaces.push_back(eS);
                }
            }
        }

        for (const auto& emptySpace: emptySpaces) {
            bool doBreak = false;
            for (auto& _source: sources) {

                if(_source->getColor() == emptySpace.color){
                    for (int i = 0; i < 4; ++i) {
                        if(!(_source->getDirs() == emptySpace.neighboursDirs)){
                            _source->rotate();
                        }
                        else{
                            _source->setCoord(emptySpace.coord.first, emptySpace.coord.second);
                            solution.push_back(_source);
                            doBreak = true;
                            break;
                        }
                    }
                }
                if(doBreak){
                    break;
                }

            }
            if(doBreak){
                sources.erase(sources.begin());
            }
        }

        if(solution.size()==0){
            cout << "Nincs source amit le lehetne rakni!" << endl;
        }
        for (auto idom: solution) {
            idom->printIt();
        }

        return;
    }


    int count = 1;
    while (solution.empty()){

        bool sinkConnectedAndHaveOpen = false;
        //1. éés 2. feltételek ellenőrzése:
        if(isSinkConnected(grid)){  //todo: feltételezzük hogy a source nem úgy van megadva hogy egy kimenete van és az egyből kapcsolódik a sink egyetlen kimenetéhez
            if(firstLeak(grid) == nullptr){
                solution = grid;
            }
            if(haveOpenOutput(actualIdom, occupiedCoords)){
                //KAPCS sink-hez és VAN kimenete az utolsónak
                //todo: végiggondolni, most jobb ötlet híján kerüljön a goodSolutions-ba a grid, itt jön képbe a firstLeak
                addGridToSolutions(grid, goodSolutions);

                //todo: itt kezdodik a mokolas kedd éjjel
                sinkConnectedAndHaveOpen = true;
                actualIdom = firstLeak(grid);
                auto it = find(grid.begin(), grid.end(), actualIdom);
                int actualIndex = distance(grid.begin(), it);
                if(actualIndex == 0){
                    prevIdom = source;
                }
                else{
                    prevIdom = grid[actualIndex - 1];
                }
            }
            else{
                //KAPCS sink-hez és NICNS kimenete az utolsónak
                ///Ha a goodSolutions-ben van olyan grid ahol nincs leak akkor véget ér a while.\n
                ///Vagyis jelenleg addig megy a while amíg nem talál perfect solution-t.
                for (auto sol:goodSolutions) {
                    if(firstLeak(sol) == nullptr){
                        solution = sol;
                    }
                }
                if(!solution.empty()){
                    //kilépünk
                    break;
                }

            }
        }

        if(!isSinkConnected(grid) or sinkConnectedAndHaveOpen){
            if(haveOpenOutput(actualIdom, occupiedCoords)){

                if(elements.empty() and stack.empty()){
                    addGridToSolutions(grid, badSolutions);
                    stack.push_back(grid[grid.size()-1]);
                    inverseConnect(grid[grid.size()-1], grid, occupiedCoords);

                    if(grid.size() == 0){
                        //ilyen eset nem lehet
                        actualIdom = source;
                        prevIdom = source;
                    }
                    else if(grid.size() == 1) {
                        actualIdom = grid[0];
                        prevIdom = source;
                    }
                    else{
                        actualIdom = grid[grid.size()-1];
                        prevIdom = grid[grid.size()-2];
                    }
                }


                //NEM kapcs sink-hez és VAN kimenete az utolsónak\n
                //--> választunk egy új actualIdom-ot
                if(!elements.empty()){
                    //Ha nem üres az elements akkor onnan választunk
                    prevIdom = actualIdom;
                    actualIdom = elements[0];
                    elements.erase(elements.begin());

                    vector<Directions> possibleSolution = chooseDirection(prevIdom, occupiedCoords);
                    if(possibleSolution.empty()){
                        throw runtime_error("chooseDirection ures vectorral tert vissza");
                    }
                    int prevDirIndex = -1;
                    for (int i = 0; i < possibleSolution.size(); ++i) {
                        if(canConnect(actualIdom,prevIdom, possibleSolution[i], grid)){
                            prevDirIndex = i;
                            break;
                        }
                    }

                    if(prevDirIndex == -1){
                        throw runtime_error("A canConnect nem talalt jot");
                    }

                    connect(prevIdom->getCoord(), possibleSolution[prevDirIndex],
                            actualIdom,occupiedCoords,grid);
                    //todo: indul előlről a while
                }
                else{
                    if(!stack.empty()){
                        //Ha üres az elements akkor a stack-ből választunk (ha tudunk, ha nem akkor forgatunk vagy vége)
                        prevIdom = actualIdom;
                        int stackIdomIndex;

                        bool connecteltunk = true;

                        for (int i = 0; i < stack.size(); ++i) {
                            stackIdomIndex = stack.size()-1-i;
                            actualIdom = stack[stackIdomIndex];

                            set<Directions> prevDirsSet = prevIdom->getDirs();
                            vector<Directions> prevDirsVec(prevDirsSet.begin(), prevDirsSet.end());


                            for (int j = 0; j < 4; ++j) {
                                int prevDirsIndex;



                                if (!prevDirsVec.empty()) {
                                    prevDirsIndex = prevDirsVec.size() - 1;

                                    if (canConnect(actualIdom, prevIdom, prevDirsVec[prevDirsIndex], grid)) {
                                        connect(prevIdom->getCoord(), prevDirsVec[prevDirsIndex], actualIdom,
                                                occupiedCoords, grid);
                                        prevDirsVec.pop_back();
                                        if (isInBadSoulutions(grid, badSolutions)) {
                                            inverseConnect(actualIdom, grid, occupiedCoords);
                                            connecteltunk = false;
                                        }
                                        else{
                                            connecteltunk = true;
                                            break;
                                        }
                                    }
                                    else{
                                        //ha nem tudjuk connectelni, canConnect-->false
                                        prevDirsVec.pop_back();
                                        connecteltunk = false;
                                    }
                                }
                                else{
                                    //a prevDirsVec üres
                                    connecteltunk = false;
                                    break;
                                }
                            }
                            if(connecteltunk){
                                break;
                            }
                        }

                        if(connecteltunk){
                            stack.erase(stack.begin() + stackIdomIndex);
                        }
                        else{
                            //ha nem tudtunk connectelni --> forgatunk
                            addGridToSolutions(grid, badSolutions);

                            if(!grid.empty()){
                                actualIdom = grid[grid.size()-1];
                                if(grid.size() == 1){
                                    prevIdom = source;
                                }
                                else{
                                    prevIdom = grid[grid.size()-2];
                                }

                                bool wasBreak = false;

                                if(actualIdom == source){
                                    throw runtime_error("No possible solution!");
                                }

                                for (int i = 0; i < 4; ++i) {
                                    rotateMore(actualIdom, prevIdom);
                                    if(!isInBadSoulutions(grid, badSolutions)){
                                        wasBreak = true;
                                        break;
                                        //todo: indul előlről a ciklus
                                    }
                                }

                                if(!wasBreak){
                                    stack.push_back(actualIdom);
                                    inverseConnect(actualIdom, grid, occupiedCoords);

                                    if(grid.empty()){
                                        actualIdom = source;
                                        prevIdom = source;
                                    }
                                    else if(grid.size() == 1){
                                        actualIdom = grid[grid.size()-1];
                                        prevIdom = source;
                                    }
                                    else{
                                        actualIdom = grid[grid.size()-1];
                                        prevIdom = grid[grid.size()-2];
                                    }
                                }

                            }
                            else{
                                actualIdom = source;
                                prevIdom = source;

                                //todo: nincs megoldás?????
                                throw runtime_error("No possible solution!");
                            }
                        }

                    }
                    else{
                        //todo: ilyenkor mit kell csinálni???
                        throw std::runtime_error("There is no idom in either the elements or the stack");
                    }
                }

            }
            else{
                //NEM kapcs sink-hez és
                //NINCS kimenete az utolsónak
                if(elements.empty()){
                    //badSolutions.push_back(grid);
                    addGridToSolutions(grid, badSolutions);
                }
                stack.push_back(actualIdom);
                inverseConnect(actualIdom, grid, occupiedCoords);

                if(grid.size() == 0){
                    actualIdom = source;    //todo: ez nem biztos hogy jó
                    prevIdom = source;
                }
                else if(grid.size() == 1){
                    actualIdom = grid[0];
                    prevIdom = source;
                }
                else{
                    actualIdom = grid[grid.size()-1];
                    prevIdom = grid[grid.size()-2];
                }
            }
        }


        //testPrintContainers(count, elements, grid, stack, occupiedCoords);
        cout << "count" << count << endl;
       /* if(badSolutions.size()==13){
            break;
        }*/
        count++;
    }
    //WHILE VÉGE


    //sloution kiiratása
    cout<<"-~-~-~SOLUTION~-~-~-"<<endl;
    for (auto s: sources) {
        s->printIt();
    }
    for (auto idom: solution) {
        idom->printIt();
    }
    for (auto s: sinks) {
        s->printIt();
    }
    cout<<"-~-SOLUTION VEGE-~-"<<endl;
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
 * @param whichCase Ha false akkor a chooseDir-es ha true akkor az van amikor biztosan tudjuk melyik dir-t akarjuk.
 * @warning "Ha connectelni szeretnénk, akkor biztosan tudunk is." Vagyis ezt a függvényt csak akkor szabad meghívni ha biztosan el tudja végezni a feladatát.
 */
void Simulation::connect(pair<int, int> prevCoord, Directions prevDir, PipeIdom *&actual,
                         vector<pair<int, int >> &occ_coords, vector<PipeIdom *> &grid) { //todo: tesztelni kellene majd
        switch (prevDir) {
            case RIGHT:
                actual->setCoord(prevCoord.first, prevCoord.second + 1);
                while (!actual->getDirs().contains(LEFT)) {
                    actual->rotate();
                }
                break;
            case UP:
                actual->setCoord(prevCoord.first - 1, prevCoord.second);
                while (!actual->getDirs().contains(DOWN)) {
                    actual->rotate();
                }
                break;
            case LEFT:
                actual->setCoord(prevCoord.first, prevCoord.second - 1);
                while (!actual->getDirs().contains(RIGHT)) {
                    actual->rotate();
                }
                break;
            case DOWN:
                actual->setCoord(prevCoord.first + 1, prevCoord.second);
                while (!actual->getDirs().contains(UP)) {
                    actual->rotate();
                }
                break;
        }
        occ_coords.push_back(actual->getCoord());
        grid.push_back(actual);
}

/**
 * @details Ezt csak a connect használhatja különben megbaszódhat az egész és exceptiont dob.\n
 * Visszaadja egy pipeIdom szabad output-ját.\n
 * Mindenképpen lesz visszatérési értéke mivel ha connect-et hívunk akkor tudunk is connectelni!?
 * @param idom Amelyik idomhoz szeretnénk connectelni. Ez általában a prevIdom
 * @param occ_coords occupiedCoords vector
 * @warning Azt nem vizsgálja hogy ha lerakunk egy idom-ot akkor annak a többi kimenete nem sérti-e a szomszédos koordinátán álló idomokat.
 */
vector<Directions> Simulation::chooseDirection(PipeIdom *idom, vector<pair<int, int>> occ_coords) {
    vector<Directions> possibleDirs;

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
            possibleDirs.push_back(dir);
        }
    }

    return possibleDirs;
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
 * @return true --> ha tudunk, vagyis legalább 1 olyan forgatottja van az actualnak amiben connectelhető \n false --> ha nem tudunk
 * @warning Csak 1 source és 1 sink esetén működik (de viszonylag könnyen át lehet írni)
 * @todo tesztelni kéne
 */
bool Simulation::canConnect(PipeIdom *actual, PipeIdom *prev, Directions prevsSelectedDir, vector<PipeIdom *> grid) {//todo: tesztelni kéne
    vector<PipeIdom*> allIdoms;
    allIdoms = sources;
    allIdoms.insert(allIdoms.end(), grid.begin(), grid.end());
    allIdoms.insert(allIdoms.end(), sinks.begin(), sinks.end());

    pair<int, int> actualCoord = prev->getCoord();  //itt azert a prevCoord-al lesz egyenlo mivel kesobb ehez képest változtatjuk
    //beállítja az actualCoordot attól függően hogy a prev melyik oldalán lesz az actual
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
            return false;
        }
    }
    //ha szabad az óhajtott koordináta, megnézzük hogy az actual-t lehet-e úgy forgatni hogy a szomszédokkal ne legyen bunyó
    if(!isInOcc){
        for (int i = 0; i < 4; ++i) {
            actual->rotate();
            if(actual->getDirs().contains(oppositeSide(prevsSelectedDir))){
                bool rightOk = true;
                bool upOk = true;
                bool leftOk = true;
                bool downOk=true;

                for (auto actDir: actual->getDirs()) {
                    pair<int, int> neighbourCoord;
                    switch (actDir) {
                        case RIGHT:
                            neighbourCoord = {actualCoord.first, actualCoord.second+1};
                            for (auto idom: allIdoms) {
                                if(neighbourCoord == idom->getCoord()){
                                    if(!idom->getDirs().contains(LEFT)){
                                        rightOk = false;
                                    }
                                }
                            }
                            break;
                        case UP:
                            neighbourCoord = {actualCoord.first-1, actualCoord.second};
                            for (auto idom: allIdoms) {
                                if(neighbourCoord == idom->getCoord()){
                                    if(!idom->getDirs().contains(DOWN)){
                                        upOk = false;
                                    }
                                }
                            }
                            break;
                        case LEFT:
                            neighbourCoord = {actualCoord.first, actualCoord.second-1};
                            for (auto idom: allIdoms) {
                                if(neighbourCoord == idom->getCoord()){
                                    if(!idom->getDirs().contains(RIGHT)){
                                        leftOk = false;
                                    }
                                }
                            }
                            break;
                        case DOWN:
                            neighbourCoord = {actualCoord.first+1, actualCoord.second};
                            for (auto idom: allIdoms) {
                                if(neighbourCoord == idom->getCoord()){
                                    if(!idom->getDirs().contains(UP)){
                                        downOk = false;
                                    }
                                }
                            }
                            break;
                    }
                }

                if(rightOk and upOk and leftOk and downOk){
                    return true;
                }
            }
            //else: forgatunk mégegyet
        }
    }

    return false;
}

/**
 * @details Kitörli a kapott elemet a gridből, és a koordinátáját az occupiedCoords-ból
 * @param idomToDelete Amit inverz konnektelni akarok
 * @param grid grid
 * @param occ_coords occupiedCoords
 */
void Simulation::inverseConnect(PipeIdom *idomToDelete, vector<PipeIdom *> &grid, vector<pair<int, int>> &occ_coords) {
    int indexToDelete = -1;
    for (int i = 0; i < grid.size(); ++i) {
        if(grid[i] == idomToDelete){
            indexToDelete = i;
        }
    }
    if(indexToDelete >= 0 and indexToDelete < grid.size()){
        grid.erase(grid.begin() + indexToDelete);
    }

    indexToDelete = -1;
    for (int i = 0; i < occ_coords.size(); ++i) {
        if(occ_coords[i] == idomToDelete->getCoord()){
            indexToDelete = i;
        }
    }
    if(indexToDelete >= 0 and indexToDelete < occ_coords.size()){
        occ_coords.erase(occ_coords.begin() + indexToDelete);
    }
}

/**
 * @brief Megnézi, hogy a kapott @b grid benne van e a badSolutions-ban
 * @param grid
 * @param badSolutions
 * @return Ha igaz akkor benne van, ha nem akkor nincs benne
 */
bool Simulation::isInBadSoulutions(vector<PipeIdom*>& grid, vector<vector<PipeIdom*>>& badSolutions){
    for (int i = 0; i < badSolutions.size(); i++)
    {
        if (badSolutions[i].size() != grid.size()) {
            continue; //Ha nem egyeznek a méretek ugorjon a következőre
        }

        bool egyezes = true;
        for (int j = 0; j < grid.size(); j++)
        {
            /*
            if (badSolutions[i][j]->getCoord() != grid[j]->getCoord() or badSolutions[i][j]->getDirs() != grid[j]->getDirs())
            {
                egyezes = false;
            }
             */
            if(!(*badSolutions[i][j] == *grid[j])){
                egyezes = false;
            }

        }

        if(egyezes){
            return true;
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

    allElements = sources;
    allElements.insert(allElements.end(), grid.begin(), grid.end());
    allElements.insert(allElements.end(), sinks.begin(), sinks.end());

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
bool Simulation::isSinkConnected(vector<PipeIdom *> grid) { //todo: működik több sinkre is??? elvileg igen de test
    bool rightOk = false;
    bool upOk = false;
    bool leftOk = false;
    bool downOk = false;


    for (auto s: sinks) {
        for (auto sinkDir: s->getDirs()) {
            pair<int, int> neighbourCoord = s->getCoord();
            switch (sinkDir) {
                case RIGHT:
                    neighbourCoord.second++;
                    for (auto idom: grid) {
                        if ((idom->getCoord() == neighbourCoord) and (idom->getDirs().contains(oppositeSide(RIGHT)))) {
                            rightOk = true;
                        }
                    }
                    if (!rightOk) {
                        return false;
                    }
                    break;
                case UP:
                    neighbourCoord.first--;
                    for (auto idom: grid) {
                        if ((idom->getCoord() == neighbourCoord) and (idom->getDirs().contains(oppositeSide(UP)))) {
                            upOk = true;
                        }
                    }
                    if (!upOk) {
                        return false;
                    }
                    break;
                case LEFT:
                    neighbourCoord.second--;
                    for (auto idom: grid) {
                        if ((idom->getCoord() == neighbourCoord) and (idom->getDirs().contains(oppositeSide(LEFT)))) {
                            leftOk = true;
                        }
                    }
                    if (!leftOk) {
                        return false;
                    }
                    break;
                case DOWN:
                    neighbourCoord.first++;
                    for (auto idom: grid) {
                        if ((idom->getCoord() == neighbourCoord) and (idom->getDirs().contains(oppositeSide(DOWN)))) {
                            downOk = true;
                        }
                    }
                    if (!downOk) {
                        return false;
                    }
                    break;
            }
        }
    }
    return true;
}

///Van e kimenete az utoljára lerakott idom-nak vagyis a kritérium_2
bool Simulation::haveOpenOutput(PipeIdom* idom, vector<pair<int, int>> occ_coords) { //todo: TESZTELNI!!!
    for (auto dir: idom->getDirs()) {
        pair<int,int> neighbourCoord = idom->getCoord();
        bool neighbourOk = true;
        switch (dir) {
            case RIGHT:
                neighbourCoord.second++;
                for (const auto & occ_coord : occ_coords) {
                    if(occ_coord == neighbourCoord){
                        neighbourOk = false;
                    }
                }
                if(neighbourOk){
                    return true;
                }
                break;
            case UP:
                neighbourCoord.first--;
                for (const auto & occ_coord : occ_coords) {
                    if(occ_coord == neighbourCoord){
                        neighbourOk = false;
                    }
                }
                if(neighbourOk){
                    return true;
                }
                break;
            case LEFT:
                neighbourCoord.second--;
                for (const auto & occ_coord : occ_coords) {
                    if(occ_coord == neighbourCoord){
                        neighbourOk = false;
                    }
                }
                if(neighbourOk){
                    return true;
                }
                break;
            case DOWN:
                neighbourCoord.first++;
                for (const auto & occ_coord : occ_coords) {
                    if(occ_coord == neighbourCoord){
                        neighbourOk = false;
                    }
                }
                if(neighbourOk){
                    return true;
                }
                break;
        }
    }
    return false;
}

/**
 * @details Létrehoz egy másolatot a gridről és azt menti el a goodSolutions-ban vagy a badSolutions-ban.\n
 * Ez azért jó mert így ha a grid egyik elemén változtatok valamit akkor az a good/bad -solutions-ban nem fog változni.
 * @param grid
 * @param badOrGoodSolutions
 */
void Simulation::addGridToSolutions(vector<PipeIdom *> grid, vector<vector<PipeIdom *>> &badOrGoodSolutions) {
    vector<PipeIdom*> tmpGrid;
    for (auto idom: grid) {
        tmpGrid.push_back(idom->clone());
    }
    badOrGoodSolutions.push_back(tmpGrid);
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

///ez csak a baszás izéhez kell (eredmény lekéréséhez)
vector<PipeIdom *> Simulation::getSolution() {
    return solution;
}

/**
 * @brief Csak a debugoláshoz kellő kiírató fv
 * @details Kiírja a consolra, hogy hányadik ciklus iteráció van illetve a grid, az elements és a stack összes elemét.
 * @param count searchPath-ben a while ciklus iteráció számolására van kitalálva
 * @param elements
 * @param grid
 * @param stack
 */
void Simulation::testPrintContainers(int count, vector<PipeIdom *> elements, vector<PipeIdom *> grid,
                                     vector<PipeIdom *> stack, vector<pair<int, int>> occ_coords) {
    //teszt
    cout << count << "-ik while iteracio!" << endl <<"\n";

    cout << "ELEMENTS:" << endl;
    for (auto item: elements) {
        item->printIt();
    }

    cout << "GRID:" << endl;
    for (auto item: grid) {
        item->printIt();
    }

    cout << "STACK:" << endl;
    for (auto item: stack) {
        item->printIt();
    }
    cout << "\n----------------------------------------" << endl;
}