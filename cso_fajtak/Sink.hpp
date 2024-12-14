#ifndef CSOVESPROG2_SINK_HPP
#define CSOVESPROG2_SINK_HPP

#include "PipeIdom.hpp"

class Sink: public PipeIdom{
    string color;
    bool status;
    const Types type = SINK;
public:
    Sink(set<Directions> dirs, int _x, int _y, string _c):
        PipeIdom(dirs, _x, _y), color(_c), status(false){}

    bool isActive();
    void setStatus(bool);
    string getColor();


    void printIt(){
        cout << "Sink_color: " << color << endl;
        cout << "x: " << coord.first << "  y:" << coord.second << endl;
        cout << "Sink_dir:" << endl;
        for(auto it: dirs){
            cout << it << endl;
        }
    }
};


#endif //CSOVESPROG2_SINK_HPP
