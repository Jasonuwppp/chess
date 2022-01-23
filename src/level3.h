#ifndef _LEVEL3_H_
#define _LEVEL3_H_

#include "computer.h"
#include <string>

class Level3: public Computer {
public:
    Level3(PlayerType type, std::string name, Colour colour);
};

#endif
