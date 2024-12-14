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
    vector<PipeIdom*> simplePipes;
    vector<PipeIdom*> valves;
    vector<PipeIdom*> sources;
    vector<PipeIdom*> sinks;

    //Tfh forrásból és nyelőből sem lehet 2 azonos színű
    Source* greenSource;
    Source* blueSource;
    Source* redSource;
    Source* yellowSource;
    Source* purpleSource;

    Sink* greenSink;
    Sink* blueSink;
    Sink* redSink;
    Sink* yellowSink;
    Sink* purpleSink;


    void searchPath();
    bool isConnectedToSink(vector<PipeIdom*>, pair<int, int>, Directions);
    bool sinkConnections(const vector<PipeIdom*>&, PipeIdom*);


public:
    Simulation(vector<PipeIdom*>, vector<PipeIdom*>, vector<PipeIdom*>, vector<PipeIdom*>);

    void getSolution(){};
};


#endif //CSOVESPROG2_SIMULATION_HPP
