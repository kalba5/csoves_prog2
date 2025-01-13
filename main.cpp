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
#include "Simulation.hpp"

#include <bits/stdc++.h>
#include "vector"
#include "set"

using namespace std;

///Az egész programban a koordináták úgy vannak értelmezve, hogy\n
///az ELSŐ koordináta adja meg, hogy hányadik SOR,\n
///a MÁSODIK koordináta adja meg, hogy hányadik OSZLOP
const string fileName = "../testData1.csv";  //ne felejtsd el belerakni a build mappába a file-t 😊

void printAllPipeElements(vector<PipeIdom*> simplePipes, vector<PipeIdom*> valves, vector<PipeIdom*> sources, vector<PipeIdom*> sinks){
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









bool isInBadSoulutions(vector<PipeIdom*> grid, vector<vector<PipeIdom*>> badSolutions){
    for (int i = 0; i < badSolutions.size(); i++)
    {
        bool allGood = true;
        for (int j = 0; j < grid.size(); j++)
        {
            if (badSolutions[i][j]->getCoord() != grid[j]->getCoord() or badSolutions[i][j]->getDirs() != grid[j]->getDirs())
            {
                allGood = false;
            }
            if(allGood){
                return true;
            }
        }
    }
    return false;
}









int main() {
///Az adatokat a mellékelt (data.csv) struktúrájú fájlból tudja a program beolvasni.\n
///Fejléc kötelező és az adatok sorrendje nem változhat a (data.csv)-hez képest

//Ezekben a vectorokban vannak eltárolva a rendelkezésre álló idomok.
    vector<PipeIdom*> simplePipes;
    vector<PipeIdom*> valves;
    vector<PipeIdom*> sources;
    vector<PipeIdom*> sinks;

//BEOLVASAS ELEJE
    {
        ifstream file(fileName);
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

            if (type == "simplePipe") {
                //azért kell ide a for mert 'count' darabot kell ebbol a fajtabol belerakni a vectorba
                for (int i = 0; i < count; i++) {
                    SimplePipe *simplePipe = new SimplePipe(dir);
                    simplePipes.push_back(simplePipe);
                }
            } else if (type == "valve") {
                for (int i = 0; i < count; i++) {
                    Valve *valve = new Valve(dir);
                    valves.push_back(valve);
                }
            } else if (type == "source") {
                //forrásból nem lehet több ugyanolyan színű
                Source *source = new Source(dir, x, y, color);
                sources.push_back(source);
            } else if (type == "sink") {
                for (int i = 0; i < count; i++) {
                    Sink *sink = new Sink(dir, x, y, color);
                    sinks.push_back(sink);
                }
            }
        }
    }
//BEOLVASAS VEGE
//beolvasas_tesztelés:
    cout<<"Print All Pipe Elements ELEJE" << endl;
    printAllPipeElements(simplePipes, valves, sources, sinks);
    cout<<"Print All Pipe Elements VEGE" << endl;

//todo: teszt, törölni

    //TESZT: testData1 adatokkal itt leírva
    //nem használom inkább file-ból olvasok be. (testData1.csv)
    {
        vector<PipeIdom *> sP;
        vector<PipeIdom *> va;
        vector<PipeIdom *> so;
        vector<PipeIdom *> si;

        set<Directions> sP1_dirs = {UP, DOWN};
        set<Directions> sP2_dirs = {DOWN};
        set<Directions> sP3_dirs = {LEFT, UP};
        set<Directions> sP4_dirs = {LEFT, UP, RIGHT};
        set<Directions> source1_dirs = {RIGHT};
        set<Directions> sink1_dirs = {LEFT};

        SimplePipe *sP1 = new SimplePipe(sP1_dirs);
        SimplePipe *sP2 = new SimplePipe(sP2_dirs);
        SimplePipe *sP3 = new SimplePipe(sP3_dirs);
        SimplePipe *sP4 = new SimplePipe(sP4_dirs);
        Source *source1 = new Source(source1_dirs, 0, 0, "GREEN");
        Sink *sink1 = new Sink(sink1_dirs, 1, 2, "GREEN");


        sP.push_back(sP1);
        sP.push_back(sP2);
        sP.push_back(sP3);
        sP.push_back(sP4);
        so.push_back(source1);
        si.push_back(sink1);
    }




    Simulation simulation(simplePipes, valves, sources, sinks);
    try {
        simulation.searchPath();
        for (auto idom: simulation.getSolution()) {
            idom->printIt();
        }
    }
    catch (runtime_error c){
        cout << "searchPath()-ben exception dobodott: ";
        cout << c.what() << endl;
    }



    return 0;
}
