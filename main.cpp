#include <iostream>
#include <optional>
#include <fstream>
#include <sstream>
#include "vector"
#include ".\cso_fajtak\PipeIdom.hpp"
#include ".\cso_fajtak\SimplePipe.hpp"
#include ".\cso_fajtak\Valve.hpp"
#include ".\cso_fajtak\Source.hpp"
#include ".\cso_fajtak\Sink.hpp"

using namespace std;


void printAllPipeElements(vector<SimplePipe*> simplePipes, vector<Valve*> valves, vector<Source*> sources, vector<Sink*> sinks){
    for (int i = 0; i < simplePipes.size(); ++i) {
        simplePipes[i]->printIt();
    }

    for (int i = 0; i < valves.size(); ++i) {
        valves[i]->printIt();
    }

    for (int i = 0; i < sources.size(); ++i) {
        sources[i]->printIt();
    }

    for (int i = 0; i < sinks.size(); ++i) {
        sinks[i]->printIt();
    }
}


int main() {
///Az adatokat a mellékelt (data.csv) struktúrájú fájlból tudja a program beolvasni.\n
///Fejléc kötelező és az adatok sorrendje nem változhat a (data.csv)-hez képest

//Ezekben a vectorokban vannak eltárolva a rendelkezésre álló idomok.
    vector<SimplePipe*> simplePipes;
    vector<Valve*> valves;
    vector<Source*> sources;
    vector<Sink*> sinks;

//BEOLVASAS ELEJE
    ifstream file("data.csv");
    string line;

    // Skip the header line
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string type, directions, color, x_str, y_str;
        int count, x, y;

        getline(ss, type, ',');
        ss >> count;
        ss.ignore(1); // Skip comma after count
        getline(ss, directions, ',');
        getline(ss, color, ',');
        getline(ss, x_str, ',');
        getline(ss, y_str);

        // Convert x and y to integers
        x = x_str.empty() ? -1 : stoi(x_str);
        y = y_str.empty() ? -1 : stoi(y_str);

        set<Directions> dir;
        for (auto i: directions) {
            switch (i) {
                case 'U':
                    dir.insert(UP);
                    break;
                case 'L':
                    dir.insert(LEFT);
                    break;
                case 'D':
                    dir.insert(DOWN);
                    break;
                case 'R':
                    dir.insert(RIGHT);
                    break;
            }
        }

        if(type == "simplePipe"){
            //azért kell ide a for mert 'count' darabot kell ebbol a fajtabol belerakni a vectorba
            for (int i = 0; i < count; i++) {
                SimplePipe* simplePipe = new SimplePipe(dir);
                simplePipes.push_back(simplePipe);
            }
        }
        else if(type == "valve"){
            for (int i = 0; i < count; i++) {
                Valve *valve = new Valve(dir);
                valves.push_back(valve);
            }
        }
        else if(type == "source"){
            //forrásból nem lehet több ugyanolyan színű
            Source* source = new Source(dir, x, y, color);
            sources.push_back(source);
        }
        else if(type == "sink"){
            for (int i = 0; i < count; i++) {
                Sink* sink = new Sink(dir, x, y, color);
                sinks.push_back(sink);
            }
        }
    }

//BEOLVASAS VEGE
//beolvasas_tesztelés:
    printAllPipeElements(simplePipes, valves, sources, sinks);

//todo: teszt, törölni
    vector<PipeIdom*> idoms;


    set<Directions> simaCso1_dirs = {UP, DOWN};
    set<Directions> csap1_dirs = {UP, LEFT, DOWN, RIGHT};
    set<Directions> forras1_dirs = {DOWN};
    set<Directions> nyelo1_dirs = {UP};

    SimplePipe* simaCso1 = new SimplePipe(simaCso1_dirs);
    Valve* csap1 = new Valve(csap1_dirs);
    Source* forras1 = new Source(forras1_dirs, 0, 0, "GREEN");
    Sink* nyelo1 = new Sink(nyelo1_dirs, 2, 3, "GREEN");

    idoms.push_back(simaCso1);
    idoms.push_back(csap1);
    idoms.push_back(forras1);
    idoms.push_back(nyelo1);

    cout << "Idoms 2. elem dirs.size:" << idoms[1]->getDirs().size() << endl;

    return 0;
}
