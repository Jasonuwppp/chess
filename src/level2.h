#ifndef _LEVEL2_H_
#define _LEVEL2_H_

#include "computer.h"
#include <string>

class Level2: public Computer {
public:
    Level2(PlayerType type, std::string name, Colour colour);
};

#endif
