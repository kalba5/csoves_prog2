#ifndef CSOVESPROG2_VALVE_HPP
#define CSOVESPROG2_VALVE_HPP


#include "PipeIdom.hpp"

class Valve: public PipeIdom {
private:
    bool open;
    const Types type = VALVE;

public:
    Valve(set<Directions> dirs):
        PipeIdom(dirs), open(false) {}

    bool isOpen();
    void setValve(bool);

    void printIt(){
        cout << "Valve_dir:" << endl;
        for(auto it: dirs){
            cout << it << endl;
        }
    }
};


#endif //CSOVESPROG2_VALVE_HPP
