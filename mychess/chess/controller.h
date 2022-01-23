#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "player.h"
#include "board.h"
#include <vector>
#include <map>

class InvalidCommand{};

class Controller {
    std::vector<Player *> players;
    Board b;
    int count = 0;
    bool initalized = false;
    bool customSetup = false;
    int numOfPlayers = 2;
    std::map<char,int> coor;
    std::map<std::string,PlayerType> pt;
    std::map<std::string,Colour> colours;
    std::map<std::string,PieceType> pieceTypes;
    void addNewPlayer(PlayerType playerType, std::string playerName, Colour playerColour);
    bool gameOver();
    void gameInStalemate();
    bool isGameOver(Colour oppColour);
public:
    Controller();
    void move();
    void resign();
    void setup();
    void startGame();
    int getScore(Colour colour) const;
    void playGame();
    void game();
};

void printInstructions(bool);
void printInstructions();
#endif

