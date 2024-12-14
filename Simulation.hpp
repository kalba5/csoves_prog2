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
    class Element{
        bool isDoubleTurn = false;
        bool isOpen = true;
        Types type;
        set<Directions> dirs;
        pair<int, int> coord;
    public:
        Element(Types _type, set<Directions> _dirs): type(_type), dirs(_dirs) {}
        pair<int, int> getCoord(){
            return coord;
        }

        set<Directions> getDirs(){
            return dirs;
        }
    };


    vector<SimplePipe*> simplePipes;
    vector<Valve*> valves;
    vector<Source*> sources;
    vector<Sink*> sinks;

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
    bool isConnectedToSink(vector<Element*>, pair<int, int>, Directions);
    bool sinkConnections(const vector<Element*>&, Sink*);


public:
    Simulation(vector<SimplePipe*>, vector<Valve*>, vector<Source*>, vector<Sink*>);

    void getSolution(){};
};


#endif //CSOVESPROG2_SIMULATION_HPP
