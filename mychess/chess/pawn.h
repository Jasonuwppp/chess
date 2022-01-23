#ifndef _PAWN_H_
#define _PAWN_H_

#include "piece.h"

class Pawn : public Piece {
public:
    Pawn(PieceType pt, Colour colour, int value);
};

#endif

