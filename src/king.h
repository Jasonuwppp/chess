#ifndef _KING_H_
#define _KING_H_

#include "piece.h"

class King : public Piece {
public:
    King(PieceType pt, Colour colour, int value);
};

#endif 
