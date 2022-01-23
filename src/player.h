#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>
#include "observer.h"
#include "state.h"
#include <string>
#include "cell.h"

class Player {
    PlayerType type;
    std::string name;
    Colour colour;
    double score = 0;
public:
    Player(PlayerType type, std::string name, Colour colour);
    void addscore(double score);
    double getScore() const;
    Colour getColour() const;
    PlayerType getPlayerType() const { return type; }
    static Player *createPlayer(PlayerType playerType, std::string playerName, Colour playerColour);
};

#endif
