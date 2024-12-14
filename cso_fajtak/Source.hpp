#ifndef CSOVESPROG2_SOURCE_HPP
#define CSOVESPROG2_SOURCE_HPP

#include "PipeIdom.hpp"
#include "string"

class Source: public PipeIdom{
    string color;
    const Types type = SOURCE;
    bool status;
public:
    Source(set<Directions> dirs, int _x, int _y, string _c):
        PipeIdom(dirs, _x, _y), color(_c), status(false){}

    bool isActive();

    void setStatus(bool);

    string getColor();

    void printIt() const override;
};


#endif //CSOVESPROG2_SOURCE_HPP
