#ifndef CSOVESPROG2_DOUBLETURN_HPP
#define CSOVESPROG2_DOUBLETURN_HPP

#include "SimplePipe.hpp"

///2 kanyarodó SimplePipe-ból épített duplakanyar 1 mezőn. \n
///Ha jól értettem ilyen nem jön az inputról, hanem én építhetek ilyet.
class DoubleTurn{
    SimplePipe sp1;
    SimplePipe sp2;

public:
    DoubleTurn(SimplePipe simplePipe1, SimplePipe simplePipe2): sp1(simplePipe1), sp2(simplePipe2){}
};


#endif //CSOVESPROG2_DOUBLETURN_HPP
