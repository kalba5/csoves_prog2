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

    void printIt(){
        cout << "Source_color: " << color << endl;
        cout << "x: " << coord.first << "  y:" << coord.second << endl;
        cout << "Source_dir:" << endl;
        for(auto it: dirs){
            cout << it << endl;
        }
    }
};


#endif //CSOVESPROG2_SOURCE_HPP
