#ifndef _LEVEL1_H_
#define _LEVEL1_H_

#include "computer.h"
#include <string>


class Level1: public Computer {
public:
    Level1(PlayerType type, std::string name, Colour colour);
};

#endif
