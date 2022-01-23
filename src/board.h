#ifndef _Board_H_
#define _Board_H_
#include <vector>
#include "cell.h"
#include "state.h"
#include "info.h"
#include <iostream>
#include "piece.h"
#include <random>
#include <string>
#include <algorithm>

struct Pos {
    int r1, c1, r2, c2;
};

struct Loc {
    int x1, x2;
};

class TextDisplay;
class GraphicsDisplay;
class InvalidMove{};

class Board {
    std::vector<std::vector<Cell>> theBoard;
    TextDisplay *td = nullptr;
    GraphicsDisplay *gd = nullptr;
    const int boardLength = 8;
    bool EnPassant = false;
    bool moveK(int r1, int c1, int r2, int c2);
    bool moveQ(int r1, int c1, int r2, int c2);
    bool moveB(int r1, int c1, int r2, int c2);
    bool moveN(int r1, int c1, int r2, int c2);
    bool moveR(int r1, int c1, int r2, int c2);
    bool moveP(int r1, int c1, int r2, int c2, std::string type);
    bool oneKingEachSide();
    bool noPawnOnSide();
public:
    Board();
    ~Board();
    void clearBoard();
    bool move(int r1, int c1, int r2, int c2, std::string type);
    void SetDefaultBoard();
    PieceType identifyType(std::string type);
    bool validBoard();
    bool ifPawnReachEnd(int oldR, int oldC, int newR, int newC);
    void addPiece(PieceType pt, Colour co, int r, int c);
    void removePiece(int r, int c);
    bool isMyPiece(Colour co, int r, int c);
    bool canCastling(Colour, int, int, int, int);
    bool isLocValid(int r1, int c1, int r2, int c2);
    bool generatemove(Colour colour);
    bool capturingmove(Colour colour);
    bool check(Colour colour);
    std::vector<Loc> validcapturingmove(Colour colour, Loc loc);
    bool capturedmove(Colour colour);
    bool validcaptured(Colour colour, Loc loc);
    std::vector<Pos> escape(Colour colour, Loc loc);
    bool maxValuemove(Colour colour);
    bool willbechecked(Colour);
    bool checkmate(Colour);
    bool stalemate(Colour);
    bool level1(Colour);
    bool level2(Colour);//only true enemy king is taken
    bool level3(Colour);
//    bool level4(Colour);
    void updateGraphics(int,int);
    friend std::ostream &operator<<(std::ostream &out, const Board &b);
};

bool outOfRange(int a);
#endif

