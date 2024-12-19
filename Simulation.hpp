#ifndef CSOVESPROG2_SIMULATION_HPP
#define CSOVESPROG2_SIMULATION_HPP

#include "vector"
#include "set"
#include ".\cso_fajtak\PipeIdom.hpp"
#include ".\cso_fajtak\SimplePipe.hpp"
#include ".\cso_fajtak\Valve.hpp"
#include ".\cso_fajtak\Source.hpp"
#include ".\cso_fajtak\Sink.hpp"
#include ".\cso_fajtak\PipeIdom.hpp"
using namespace std;


class Simulation {
private:
    vector<PipeIdom*> solution;

    vector<PipeIdom*> simplePipes;
    vector<PipeIdom*> valves;
    vector<PipeIdom*> sources;
    vector<PipeIdom*> sinks;

    //Tfh forrásból és nyelőből sem lehet 2 azonos színű
    ///egyenlőre csak 1 forrásra és egy nyelőre működik
    PipeIdom* source;
    PipeIdom* sink;



    bool isIn(vector<PipeIdom*> inThat, pair<int,int> onThatCoord);
    bool haveThatDirection(PipeIdom* lmnt, Directions dir);
    bool isConnectedTo(vector<PipeIdom*> grid, pair<int, int> lmntCoord, Directions lmntDir);
    bool isAllConnected(const vector<PipeIdom*> &grid, PipeIdom *sinkItem);
    void connect(pair<int, int>, Directions, PipeIdom*& actual);
    Directions chooseDirection(PipeIdom *idom, vector<pair<int, int>> occ_coords);
    bool isBadSoulution(vector<PipeIdom*> grid, vector<vector<PipeIdom*>> badSolutions);
    PipeIdom* firstLeak(vector<PipeIdom*> grid);
    Directions oppositeSide(Directions side);
    void rotateMore(PipeIdom* &actual, PipeIdom* previous);

    bool isSinkConnected(vector<PipeIdom*> grid);   ///Kritérium_1
    bool haveOpenOutput(PipeIdom* idom, vector<pair<int,int>> occ_coords);  ///Kritérium_2


public:
    Simulation(vector<PipeIdom*>, vector<PipeIdom*>, vector<PipeIdom*>, vector<PipeIdom*>);
    vector<PipeIdom*> getSolution();

    //ideiglenesen public a tesztelés miatt
    void searchPath();
};




#endif //CSOVESPROG2_SIMULATION_HPP
