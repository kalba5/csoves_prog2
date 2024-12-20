#ifndef CSOVESPROG2_PIPEIDOM_HPP
#define CSOVESPROG2_PIPEIDOM_HPP

#include "iostream"
#include <string>
#include <set>

using namespace std;

enum Colors{
    PURPLE = 0,
    BLUE = 1,
    GREEN = 2,
    RED = 3,
    YELLOW = 4
};

enum Directions{
    RIGHT = 0,
    UP = 90,
    LEFT = 180,
    DOWN = 270
};

enum Types{
    SIMPLE_PIPE,
    VALVE,
    SOURCE,
    SINK,
    DOUBLETURN
};


class PipeIdom {
protected:
    set<Directions> dirs;
    Types type;
    pair<int, int> coord;

public:
    PipeIdom(set<Directions> _dirs, int _x, int _y): dirs(_dirs){
        coord = make_pair(_x, _y);
    }

    PipeIdom(set<Directions> _dirs): dirs(_dirs){}


    void rotate();
    set<Directions> getDirs();
    pair<int, int> getCoord();
    void setCoord(int a, int b);
    Types getType() const;
    void printDirs();


    virtual void printIt() const = 0;
    virtual string getColor() const = 0;
};


#endif //CSOVESPROG2_PIPEIDOM_HPP
