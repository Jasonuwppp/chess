#ifndef _PIECE_H_
#define _PIECE_H_

#include "state.h"
#include "info.h"


class Piece {
    PieceType pt;
    Colour colour;
    int value;
    bool touched = false;
public:
    Piece(PieceType pt, Colour colour, int value) : pt{pt}, colour{colour}, value{value} {}
    static std::shared_ptr<Piece> CreatePiece(PieceType type, Colour colour);
    PieceType getPieceType() const;
    Colour getColour() const;
    int getValue() const;
    bool isFirstMove() const {return !touched;}
    void touch() { touched = true;}
};



#endif

