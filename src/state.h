#ifndef STATE_H
#define STATE_H
#include <cstddef>
class Piece;
/* State types are:
    NewPiece -- I am a new piece.  I was just placed on the board.  I'm letting
                you know that I exist, in case you need to change colour.

    Relay -- I have a neighbour (or a neighbour's neighbour, etc.) who is a 
             new piece, not of my colour, and is a straight line away from me.
             I'm letting you know that that neighbour exists, and in which
             direction it is, in case you need to change colour.

    Reply -- I have a neighbour (or a neighbour's neighbour, etc.) who is a
             new piece, of my colour, and is a straight line away from me.  OR
             I am an in between piece, and am changing my colour now.
*/
enum class StateType { OriPiece, Relay, Reply, Display};
enum class Colour { Black, White};
enum class Direction { NW, N, NE, W, E, SW, S, SE };
enum class PieceType { N, Q, R, B, K, P,Wr };
enum class PlayerType {H, C1, C2, C3, C4 };


struct State {
  StateType type;  // See above
  Direction direction;  // In which direction from me is the new piece?
  int newR,newC,oldR,oldC;
  bool validMove;//pass false if there is a piece on the way
};

#endif
