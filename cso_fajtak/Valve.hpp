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

    void printIt() const override;

    string getColor() const override;

    PipeIdom *clone() const override;
};


#endif //CSOVESPROG2_VALVE_HPP
