#ifndef _COMPUTER_H_
#define _COMPUTER_H_

#include "player.h"
#include <string>


class Computer: public Player {
public:
    Computer(PlayerType type, std::string name, Colour colour);
};

#endif
