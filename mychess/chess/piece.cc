#include "piece.h"
#include "king.h"
#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "pawn.h"
using namespace std;


shared_ptr<Piece> Piece::CreatePiece(PieceType pt, Colour colour) {
    if(colour == Colour::White) {
        if(pt == PieceType::K) {
            shared_ptr<Piece> res1 (new King(pt,colour,900));
            return res1;
        } else if (pt == PieceType::Q) {
	    shared_ptr<Piece> res2 (new Queen(pt, colour, 90));
            return res2;
        } else if (pt == PieceType::R) {
	    shared_ptr<Piece> res3 (new Rook(pt, colour, 50)); 
            return res3;
        } else if (pt == PieceType::B) {
            shared_ptr<Piece> res4 (new Bishop(pt, colour, 30));
            return res4;
        } else if (pt == PieceType::N) {
	    shared_ptr<Piece> res5 (new Knight(pt, colour, 30));
            return res5;
        } else if (pt == PieceType::P) {
	    shared_ptr<Piece> res6 (new Pawn(pt, colour, 10));
            return res6;
        }
    } else if (colour == Colour::Black){
        if(pt == PieceType::K) {
	    shared_ptr<Piece> res7(new King(pt, colour, -900));
            return res7;
        } else if (pt == PieceType::Q) {
	    shared_ptr<Piece> res8(new Queen(pt, colour, -90));
            return res8;
        } else if (pt == PieceType::R) {
	    shared_ptr<Piece> res9(new Rook(pt, colour, -50));
            return res9;
        } else if (pt == PieceType::B) {
            shared_ptr<Piece> res10(new Bishop(pt, colour, -30));
            return res10;
        } else if (pt == PieceType::N) {
	    shared_ptr<Piece> res11(new Knight(pt, colour, -30));
            return res11;
        } else if (pt == PieceType::P) {
	    shared_ptr<Piece> res12(new Pawn(pt, colour, -10));
            return res12;
        }
    }
    return nullptr;
}

PieceType Piece::getPieceType() const {
    return this->pt;
}

Colour Piece::getColour() const {
    return this->colour;
}

int Piece::getValue() const {
    return this->value;
}



