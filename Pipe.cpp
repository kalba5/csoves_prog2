//
// Created by Kalász Bálint on 2024. 11. 24..
//

#include "Pipe.hpp"

Pipe::Pipe(set<Directions> _dirs, int _x, int _y): dirs(_dirs){
    coord = make_pair(_x, _y);
}

Types Pipe::getType() const {
    return type;
}


