#ifndef CSOVESPROG2_SIMPLEPIPE_HPP
#define CSOVESPROG2_SIMPLEPIPE_HPP

#include <utility>

#include "PipeIdom.hpp"

class SimplePipe: public PipeIdom {
     const Types type = SIMPLE_PIPE;
public:
    SimplePipe(set<Directions> dirs): PipeIdom(std::move(dirs)){}

    void printIt() const override;

    string getColor() const override;


};


#endif //CSOVESPROG2_SIMPLEPIPE_HPP
