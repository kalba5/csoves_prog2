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
    string getColor() const override;

    void printIt() const override;
};


#endif //CSOVESPROG2_SINK_HPP
