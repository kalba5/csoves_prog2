#include "Simulation.hpp"

Simulation::Simulation(vector<SimplePipe *> sp, vector<Valve *> va, vector<Source *> so, vector<Sink*> si) {
    simplePipes = sp;
    valves = va;
    sources = so;
    sinks = si;
    for (auto & i : so) {
        if (i->getColor() == "GREEN"){
            greenSource = i;
        }
        else if(i->getColor() == "BLUE"){
            blueSource = i;
        }
        else if(i->getColor() == "RED"){
            redSource = i;
        }
        else if(i->getColor() == "YELLOW"){
            yellowSource = i;
        }
        else if(i->getColor() == "PURPLE"){
            purpleSource = i;
        }
    }
    for (auto & i : si) {
        if (i->getColor() == "GREEN"){
            greenSink = i;
        }
        else if(i->getColor() == "BLUE"){
            blueSink = i;
        }
        else if(i->getColor() == "RED"){
            redSink = i;
        }
        else if(i->getColor() == "YELLOW"){
            yellowSink = i;
        }
        else if(i->getColor() == "PURPLE"){
            purpleSink = i;
        }
    }
}


void Simulation::searchPath() {
    vector<vector<Element>> votma;
    bool foundIt = false;



    while(!foundIt) {
        //Inicializalja az alap cuccokat. todo: Ezt nem így kéne mert így nagyon lassú de most jobb nem jutott eszembe
        vector<Element*> elements;  //azok a simplePipe-ok és valve-ok vannak benne amik még nincsenek felhasználva
        vector<Element*> grid;  //használatban lévő elemek todo: szerintem felesleges bonyolítás lenne mátrixban tárolni
        for (auto &i: simplePipes) {
            Element *element = new Element(i->getType(), i->getDirs());
            elements.push_back(element);
        }
        for (auto &i: valves) {
            Element *element = new Element(i->getType(), i->getDirs());
            elements.push_back(element);
        }




        //ha nincs elvarratlan szál
        //ÉS ha csatlakozik valami mindegyik sink mindegyik végére
        bool isSinksOk = true;
        //Megnézi mindegyik sink-et
        for (auto &sinkItem: sinks) {
            if(!sinkConnections(grid, sinkItem)){
                isSinksOk = false;
                break;
            }
        }

        //ha nem teljesülnek a feltételek és elfogytak az elemek akkor kezdje újból
        //ha nem teljesülnek a feltételek akkor mentse el és kezdje újból
        //todo: mikor álljon le a while <-- célom: olyan elrendezést találni, hogy a lehető legtöbb element legyen lent
        //todo: akkor álljon le --> ha nem tud új elrendezést találni. --> végtelen pálya esetén ez kurva sok, kéne valami megszorítás
    }



}

///isConnectedToSink akkor tér vissza igazzal ha a kapott coordinátán létezik element\n
/// és annak a dirs-e tartalmaz olyat amit kapott paraméterként a fv.

bool Simulation::isConnectedToSink(vector<Element *> grid, pair<int, int> lmntCoord, Directions lmntDir) {
    for (auto &item: grid) {
        if(item->getCoord() == lmntCoord and item->getDirs().contains(lmntDir)){
            return true;
        }
    }
    return false;
}

//todo: tesztelni kéne mert nem fix hogy jó
bool Simulation::sinkConnections(const vector<Element *>& grid, Sink* sinkItem) {
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
                right = isConnectedToSink(grid, pair<int, int>(sinkCoord.first, sinkCoord.second+1), LEFT);
                if(!right){
                    return false;
                }
                break;
            case UP:
                up = isConnectedToSink(grid, pair<int, int>(sinkCoord.first-1, sinkCoord.second), DOWN);
                if(!up){
                    return false;
                }
                break;
            case LEFT:
                left = isConnectedToSink(grid, pair<int, int>(sinkCoord.first, sinkCoord.second-1), RIGHT);
                if(!left){
                    return false;
                }
                break;
            case DOWN:
                down = isConnectedToSink(grid, pair<int, int>(sinkCoord.first+1, sinkCoord.second), UP);
                if(!down){
                    return false;
                }
                break;
        }
    }


    return true;
}

