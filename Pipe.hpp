#ifndef CSOVESPROG2_PIPE_HPP
#define CSOVESPROG2_PIPE_HPP

#include "set"
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


class Pipe {
    set<Directions> dirs;
    Types type;
    pair<int, int> coord;   //csak a forrásra és a nyelőre van előre definiálva
    Colors color;   //csak a forrásra és a nyelőre vonatkozik
    bool status;    //csak a forrásra és a nyelőre vonatkozik
    bool open;  //csak a csapokra vonatkozik

public:
    Pipe(set<Directions> _dirs, int _x, int _y);

    Types getType() const;
    void rotate();
};


#endif //CSOVESPROG2_PIPE_HPP
