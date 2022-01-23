#ifndef _HUMAN_H_
#define _HUMAN_H_

#include "player.h"
#include <string>

class Human: public Player {
public:
    Human(PlayerType type, std::string name, Colour colour);
};

#endif
