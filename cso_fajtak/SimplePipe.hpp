#ifndef CSOVESPROG2_SIMPLEPIPE_HPP
#define CSOVESPROG2_SIMPLEPIPE_HPP

#include "PipeIdom.hpp"

class SimplePipe: public PipeIdom {
     const Types type = SIMPLE_PIPE;
public:
    SimplePipe(set<Directions> dirs): PipeIdom(dirs){}

    void printIt(){
        cout << "SimplePipe_dir:" << endl;
        for(auto it: dirs){
            cout << it << endl;
        }
    }
};


#endif //CSOVESPROG2_SIMPLEPIPE_HPP
