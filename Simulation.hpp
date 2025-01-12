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

/**
 * @details A Simulation osztályban van megírva a program logikája, az útvonaltervezés illetve az ehez kellő segédfüggvények\n
 * Az útvonalat a searchPath() fv számítja ki, majd elmenti a solution változóba a legjobb megoldást.\n Ezt a getSolution() fv-vel lehet megkapni az osztályon kívülről.
 */
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
    bool isConnectedTo(vector<PipeIdom*> grid, pair<int, int> idomCoord, Directions idomDir);
    bool isAllConnected(const vector<PipeIdom*> &grid, PipeIdom *idom);
    void connect(pair<int, int> prevCoord, Directions prevDir, PipeIdom *&actual, vector<pair<int, int >> &occ_coords);
    Directions chooseDirection(PipeIdom *idom, vector<pair<int, int>> occ_coords);
    bool canConnect(PipeIdom* actual, PipeIdom* prev, Directions prevsSelectedDir, vector<PipeIdom*> grid);
    bool isInBadSoulutions(vector<PipeIdom*> grid, vector<vector<PipeIdom*>> badSolutions);
    PipeIdom* firstLeak(vector<PipeIdom*> grid);
    Directions oppositeSide(Directions side);
    void rotateMore(PipeIdom* &actual, PipeIdom* previous);

    bool isSinkConnected(vector<PipeIdom*> grid);   ///Kritérium_1
    bool haveOpenOutput(PipeIdom* idom, vector<pair<int,int>> occ_coords);  ///Kritérium_2

    //Teszt függvények
    void testPrintContainers(int count, vector<PipeIdom*> elements, vector<PipeIdom*> grid, vector<PipeIdom*> stack);


public:
    Simulation(vector<PipeIdom*>, vector<PipeIdom*>, vector<PipeIdom*>, vector<PipeIdom*>);
    vector<PipeIdom*> getSolution();

    //todo: ideiglenesen public a tesztelés miatt, majd vissza kell rakni private-ba
    void searchPath();
};




#endif //CSOVESPROG2_SIMULATION_HPP
