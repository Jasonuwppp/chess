#ifndef _LEVEL4_H_
#define _LEVEL4_H_

#include "computer.h"
#include <string>

class Level4: public Computer {
public:
    Level4(PlayerType type, std::string name, Colour colour);
};

#endif
