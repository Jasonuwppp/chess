#include "player.h"
#include "human.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
#include "level4.h"
#include <string>
using namespace std;

Player::Player(PlayerType type, string name, Colour colour) : type{type}, name{name}, colour{colour}  {}

void Player::addscore(double score) {
    this->score += score;
}

double Player::getScore() const {
    return score;
}

Colour Player::getColour() const {
    return colour;
}


Player *Player::createPlayer(PlayerType playerType, string playerName, Colour playerColour) {
    if(playerType == PlayerType::H) {
        return new Human(playerType, playerName, playerColour);
    } else if (playerType == PlayerType::C1) {
        return new Level1(playerType, playerName, playerColour);
    } else if (playerType == PlayerType::C2) {
        return new Level2(playerType, playerName, playerColour);
    } else if (playerType == PlayerType::C3) {
        return new Level3(playerType, playerName, playerColour);
    } else if (playerType == PlayerType::C4) {
        return new Level4(playerType, playerName, playerColour);
    } else {
        return nullptr;
    }
}


