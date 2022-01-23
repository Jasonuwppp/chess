#include "board.h"
#include "textdisplay.h"
#include "graphicsdisplay.h"
#include <vector>
#include "piece.h"
#include <iostream>
#include <string>
using namespace std;

int absVal(int a) {
	if (a > 0) return a;
	return -a;
}


Board::Board(): td{new TextDisplay()}, gd{new GraphicsDisplay()}
{
    // inserting cells into theGrid    
    for(int r = 0; r < boardLength; ++r) {
        vector<Cell> row;
        for(int c = 0; c < boardLength; ++c) {
            Cell cell(r, c);
            row.emplace_back(cell);
        }
        theBoard.emplace_back(row);
    }

    for(int r = 0; r < boardLength; ++r) {
        for(int c = 0; c < boardLength; ++c) {
            int minr = ((r - 1) < 0) ? 0 : r - 1;
            int minc = ((c - 1) < 0) ? 0 : c - 1;

            int maxr = ((r + 1) > boardLength - 1) ? boardLength - 1 : r + 1;
            int maxc = ((c + 1) > boardLength - 1) ? boardLength - 1: c + 1;

            // attaching the cell's neighbours
            for(int mr = minr; mr <= maxr; ++mr) {
                for(int mc = minc; mc <= maxc; ++mc) {
                    if(mr != r ||  mc != c) {
                        theBoard.at(r).at(c).attach(&theBoard.at(mr).at(mc));
                    }
                }
            }

            // attaching the TextDisplay and GraphicsDisplay
           theBoard.at(r).at(c).attach(td);
            //theBoard.at(r).at(c).attach(gd);
         }
    }
}

Board::~Board() {
    delete td;
    delete gd;
    
}

void Board::clearBoard() {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			theBoard.at(i).at(j).setEmpty();
			updateGraphics(i,j);
		}
	}
}
bool Board::moveK(int r1, int c1, int r2, int c2) {
	EnPassant = false;
        bool successfulMove = false;
	shared_ptr<Piece> piece = theBoard.at(r1).at(c1).getPiece();
	if (canCastling(Colour::Black,r1,c1,r2,c2)) {
			if (c1 - c2 == 2) {//left
				theBoard.at(r1).at(c1).setEmpty();
				theBoard.at(0).at(0).setEmpty();
				theBoard.at(r2).at(c2).setPiece(PieceType::K,Colour::Black);
				theBoard.at(0).at(3).setPiece(PieceType::R,Colour::Black);
				theBoard.at(r2).at(c2).getPiece()->touch();
				theBoard.at(0).at(3).getPiece()->touch();
				updateGraphics(0,0);
				updateGraphics(0,3);
				successfulMove = true;
			}
			else if (c1 - c2 == -2) {//right
				theBoard.at(r1).at(c1).setEmpty();
                theBoard.at(0).at(7).setEmpty();
                theBoard.at(r2).at(c2).setPiece(PieceType::K,Colour::Black);
                theBoard.at(0).at(5).setPiece(PieceType::R,Colour::Black);
				theBoard.at(r2).at(c2).getPiece()->touch();
				theBoard.at(0).at(5).getPiece()->touch();
				updateGraphics(0,5);
				updateGraphics(0,7);
				successfulMove =  true;
			}
	}
	else if (canCastling(Colour::White,r1,c1,r2,c2)) {
			if (c1 - c2 == 2) {//left
                theBoard.at(r1).at(c1).setEmpty();
                theBoard.at(7).at(0).setEmpty();
                theBoard.at(r2).at(c2).setPiece(PieceType::K,Colour::White);
                theBoard.at(7).at(3).setPiece(PieceType::R,Colour::White);
				theBoard.at(r2).at(c2).getPiece()->touch();
				theBoard.at(7).at(3).getPiece()->touch();
				updateGraphics(7,3);
				updateGraphics(7,0);
				successfulMove = true;
            }
            else if (c1 - c2 == -2) {//right
                theBoard.at(r1).at(c1).setEmpty();
                theBoard.at(7).at(7).setEmpty();
                theBoard.at(r2).at(c2).setPiece(PieceType::K,Colour::White);
				theBoard.at(r2).at(c2).getPiece()->touch();
                theBoard.at(7).at(5).setPiece(PieceType::R,Colour::White);
				theBoard.at(7).at(5).getPiece()->touch();
				updateGraphics(7,7);
				updateGraphics(7,5);
				successfulMove = true;
                        }
	}
	else if (absVal(r1 - r2) <= 1 && absVal(c1 - c2) <= 1){
			theBoard.at(r2).at(c2).setPiece(PieceType::K, piece->getColour());
			theBoard.at(r1).at(c1).setEmpty();
			theBoard.at(r2).at(c2).getPiece()->touch();
			successfulMove = true;
	}

        if(successfulMove) {
            updateGraphics(r1, c1);
            updateGraphics(r2, c2);
        }
	return successfulMove;
}

bool Board::moveQ(int r1, int c1, int r2, int c2) {
	EnPassant = false;
	bool successfulMove = false;
	if(theBoard.at(r1).at(c1).ifValid(r2, c2)) {
        	theBoard.at(r2).at(c2).setPiece(PieceType::Q,theBoard.at(r1).at(c1).getPiece()->getColour());
		theBoard.at(r1).at(c1).setEmpty();
		successfulMove = true;
	} 
	if (successfulMove) {
		updateGraphics(r1,c1);
		updateGraphics(r2,c2);
	}
	return successfulMove;
}

bool Board::moveN(int r1, int c1, int r2, int c2) {
	EnPassant = false;
	bool successfulMove = false;
	if (((absVal(r1 - r2) == 2) && (absVal(c1 - c2) == 1)) ||
	    ((absVal(r1 - r2) == 1) && (absVal(c1 - c2) == 2))) {
		theBoard.at(r2).at(c2).setPiece(PieceType::N, theBoard.at(r1).at(c1).getPiece()->getColour());
                theBoard.at(r1).at(c1).setEmpty();
		successfulMove = true;
	}
	if (successfulMove) {
                updateGraphics(r1,c1);
                updateGraphics(r2,c2);
        }
        return successfulMove;
}

bool Board::moveR(int r1, int c1, int r2, int c2) {
	EnPassant = false;
	bool successfulMove = false;
	if ((r2 == r1) || (c2 == c1))  {
		if (theBoard.at(r1).at(c1).ifValid(r2,c2)) {
			theBoard.at(r2).at(c2).setPiece(PieceType::R, theBoard.at(r1).at(c1).getPiece()->getColour());
                        theBoard.at(r1).at(c1).setEmpty();
			theBoard.at(r2).at(c2).getPiece()->touch();
			successfulMove = true;
		}
	}
	if (successfulMove) {
                updateGraphics(r1,c1);
                updateGraphics(r2,c2);
        }
	return successfulMove;
}

bool Board::moveB(int r1, int c1, int r2, int c2) {
	EnPassant = false;
        bool successfulMove = false;
	if ((r2 - r1 == c2 - c1) || (r2 - r1 == c1 - c2)) {
		if (theBoard.at(r1).at(c1).ifValid(r2, c2)) {
			theBoard.at(r2).at(c2).setPiece(PieceType::B, theBoard.at(r1).at(c1).getPiece()->getColour());
			theBoard.at(r1).at(c1).setEmpty();
			successfulMove = true;
		}
	}

        if (successfulMove) {
                updateGraphics(r1,c1);
                updateGraphics(r2,c2);
        }

	return successfulMove;
}

bool Board::moveP(int r1, int c1, int r2, int c2,string type) {
        bool successfulMove = false;
	if (theBoard.at(r1).at(c1).getPiece()->getColour() == Colour::Black) {//add en passet
		if ((c2 == c1) && (r2 == r1 + 1)) {
			if (theBoard.at(r2).at(c2).getPiece() == nullptr) {
				if (r2 == 7) {
					theBoard.at(r2).at(c2).setPiece(identifyType(type),theBoard.at(r1).at(c1).getPiece()->getColour());
                        		theBoard.at(r1).at(c1).setEmpty();
				} else {
					theBoard.at(r2).at(c2).setPiece(PieceType::P, theBoard.at(r1).at(c1).getPiece()->getColour());
					theBoard.at(r1).at(c1).setEmpty();
					theBoard.at(r2).at(c2).getPiece()->touch();
				}
				EnPassant = false;
				successfulMove = true;
			}
		}
		else if ((c2 == c1)&& (r2 == r1 + 2)) {
			if (theBoard.at(r1).at(c1).getPiece()->isFirstMove() && theBoard.at(r1).at(c1).ifValid(r2, c2) && theBoard.at(r2).at(c2).getPiece() == nullptr) {
                                theBoard.at(r2).at(c2).setPiece(PieceType::P, theBoard.at(r1).at(c1).getPiece()->getColour());
                                theBoard.at(r1).at(c1).setEmpty();
                                theBoard.at(r2).at(c2).getPiece()->touch();
				EnPassant = false;
				if (c2 + 1 <= 7 && (theBoard.at(r2).at(c2 + 1).getPiece() != nullptr) && (theBoard.at(r2).at(c2 + 1).getPiece()->getPieceType() == PieceType::P) && (theBoard.at(r2).at(c2 + 1).getPiece()->getColour() == Colour::White)) {
					EnPassant = true;
				}
				else if (c2 - 1 >= 0 && (theBoard.at(r2).at(c2 - 1).getPiece() != nullptr) && (theBoard.at(r2).at(c2- 1).getPiece()->getPieceType() == PieceType::P) && (theBoard.at(r2).at(c2 - 1).getPiece()->getColour() == Colour::White)){	
					EnPassant = true;
				}
				successfulMove = true;
			} 
		}
		else if (((c2 - c1 == 1) || (c2 - c1 == -1)) && (r2 - r1 == 1)) {
			if (theBoard.at(r2).at(c2).getPiece() == nullptr) {
				if (EnPassant && r1 == 4 && r2 == 5 && theBoard.at(r1).at(c1).getPiece()->getColour() == Colour::Black) {
					theBoard.at(r2).at(c2).setPiece(PieceType::P,Colour::Black);
					theBoard.at(r2).at(c2).getPiece()->touch();
					theBoard.at(r1).at(c1).setEmpty();
					theBoard.at(r1).at(c2).setEmpty();
				}
				EnPassant = false;
			}
			else if (theBoard.at(r2).at(c2).getPiece()->getColour() == Colour::White) {
				theBoard.at(r2).at(c2).setEmpty();
                                if (r2 == 7) {
                                        theBoard.at(r2).at(c2).setPiece(identifyType(type),theBoard.at(r1).at(c1).getPiece()->getColour());
                                        theBoard.at(r1).at(c1).setEmpty();
                                } else {
                                        theBoard.at(r2).at(c2).setPiece(PieceType::P, theBoard.at(r1).at(c1).getPiece()->getColour());
                                        theBoard.at(r1).at(c1).setEmpty();
                                        theBoard.at(r2).at(c2).getPiece()->touch();
                                }
				EnPassant = false;
				successfulMove = true;
			}
		}
	}
	else if (theBoard.at(r1).at(c1).getPiece()->getColour() == Colour::White) {
		if (c2 == c1 && (r2 == r1 - 1)) {
                        if (theBoard.at(r2).at(c2).getPiece() == nullptr) {
                                if (r2 == 0) {
					if (identifyType(type) == PieceType::Wr) return false;
					theBoard.at(r2).at(c2).setPiece(identifyType(type),theBoard.at(r1).at(c1).getPiece()->getColour());
                                        theBoard.at(r1).at(c1).setEmpty();
                                } else {
					theBoard.at(r2).at(c2).setPiece(PieceType::P, theBoard.at(r1).at(c1).getPiece()->getColour());
                                        theBoard.at(r1).at(c1).setEmpty();
                                        theBoard.at(r2).at(c2).getPiece()->touch();
                                }
				EnPassant = false;
				successfulMove = true;
                        }
                }
                else if ((c2 == c1) && (r2 == r1 - 2)) {
                        if (theBoard.at(r1).at(c1).getPiece()->isFirstMove() && theBoard.at(r1).at(c1).ifValid(r2, c2) && theBoard.at(r2).at(c2).getPiece() == nullptr) {
				theBoard.at(r2).at(c2).setPiece(PieceType::P, theBoard.at(r1).at(c1).getPiece()->getColour());
                                theBoard.at(r1).at(c1).setEmpty();
                                theBoard.at(r2).at(c2).getPiece()->touch();
				EnPassant = false;
				if (c2 + 1 <= 7 && (theBoard.at(r2).at(c2 + 1).getPiece() != nullptr) && (theBoard.at(r2).at(c2 + 1).getPiece()->getPieceType() == PieceType::P) && (theBoard.at(r2).at(c2 + 1).getPiece()->getColour() == Colour::Black)) {
                                        EnPassant = true;
                                }
                                else if (c2 - 1 >= 0 && (theBoard.at(r2).at(c2 - 1).getPiece() != nullptr) && (theBoard.at(r2).at(c2- 1).getPiece()->getPieceType() == PieceType::P) && (theBoard.at(r2).at(c2 - 1).getPiece()->getColour() == Colour::Black)){
                                        EnPassant = true;
                                }
				successfulMove = true;
                        }
                }
                else if (((c2 - c1 == 1) || (c2 - c1 == -1)) && (r2 - r1 == -1)) {
			if (theBoard.at(r2).at(c2).getPiece() == nullptr) {
				if (EnPassant && r1 == 3 && r2 == 2 && theBoard.at(r1).at(c1).getPiece()->getColour() == Colour::White) {
					theBoard.at(r2).at(c2).setPiece(PieceType::P,Colour::White);
					theBoard.at(r2).at(c2).getPiece()->touch();
                                	theBoard.at(r1).at(c1).setEmpty();
                                	theBoard.at(r1).at(c2).setEmpty();
				}
				EnPassant = false;
                        }
                        else if (theBoard.at(r2).at(c2).getPiece()->getColour() == Colour::Black) {	
				theBoard.at(r2).at(c2).setEmpty();
                                if (r2 == 0) {
					theBoard.at(r2).at(c2).setPiece(identifyType(type),theBoard.at(r1).at(c1).getPiece()->getColour());
                                        theBoard.at(r1).at(c1).setEmpty();
                                } else {
					theBoard.at(r2).at(c2).setPiece(PieceType::P, theBoard.at(r1).at(c1).getPiece()->getColour());
                                        theBoard.at(r1).at(c1).setEmpty();
                                        theBoard.at(r2).at(c2).getPiece()->touch();
                                }
				EnPassant = false;
				successfulMove = true;
                        }
                }
        }

        if (successfulMove) {
                updateGraphics(r1,c1);
                updateGraphics(r2,c2);
        }
	return successfulMove;
}

bool Board::move(int r1, int c1, int r2, int c2,string type) {
	//stop if invalid pos is given
	if (r1 < 0 || r1 > 7 || c1 < 0 || c1 > 7 || r2 < 0 || r2 > 7 || c2 < 0 || c2 > 7) {
		return false;
	}
	//stop if ori has nothing
        if (theBoard.at(r1).at(c1).getPiece() == nullptr) return false;
	// stop if new loc and old loc are the same
        if ((r1 == r2) && (c1 == c2)) return false;
        //stop if dest has my piece
        if (theBoard.at(r2).at(c2).getPiece() != nullptr && theBoard.at(r1).at(c1).getPiece()->getColour() == theBoard.at(r2).at(c2).getPiece()->getColour()) {
                return false;
        }
        // stop if king will be checked after this piece has moved.
	shared_ptr<Piece> oritemp = theBoard.at(r1).at(c1).getPiece();
	if(canCastling(oritemp->getColour(), r1, c1, r2, c2)) {
             int rookC = c2 < c1 ? 0 : 7;
             int newRookC = rookC == 0 ? 3 : 5;
             shared_ptr<Piece> rookTemp = theBoard.at(r1).at(rookC).getPiece();

             theBoard.at(r1).at(rookC).updatePiece(nullptr);
             theBoard.at(r1).at(newRookC).updatePiece(rookTemp);
             theBoard.at(r1).at(c1).updatePiece(nullptr);
             theBoard.at(r2).at(c2).updatePiece(oritemp);
             if(willbechecked(oritemp->getColour())) {
                 theBoard.at(r1).at(rookC).updatePiece(rookTemp);
                 theBoard.at(r1).at(newRookC).updatePiece(nullptr);
                 theBoard.at(r1).at(c1).updatePiece(oritemp);
                 theBoard.at(r2).at(c2).updatePiece(nullptr);
                 return false;
             }

             theBoard.at(r1).at(rookC).updatePiece(rookTemp);
             theBoard.at(r1).at(newRookC).updatePiece(nullptr);
             theBoard.at(r1).at(c1).updatePiece(oritemp);
             theBoard.at(r2).at(c2).updatePiece(nullptr);
        }
	else { 
		shared_ptr<Piece> destemp = theBoard.at(r2).at(c2).getPiece();
        	theBoard.at(r1).at(c1).updatePiece(nullptr);
        	theBoard.at(r2).at(c2).updatePiece(oritemp);
		if (willbechecked(oritemp->getColour())) {
			theBoard.at(r1).at(c1).updatePiece(oritemp);
			theBoard.at(r2).at(c2).updatePiece(destemp);
			return false;
		}
		theBoard.at(r1).at(c1).updatePiece(oritemp);//put it back        
		theBoard.at(r2).at(c2).updatePiece(destemp);//put it back
	}
	switch(theBoard.at(r1).at(c1).getPiece()->getPieceType()) {
		case PieceType::K :
			return moveK(r1,c1,r2,c2); break;
		case PieceType::Q :
			return moveQ(r1,c1,r2,c2); break;
		case PieceType::B :
			return moveB(r1,c1,r2,c2); break;
		case PieceType::N :
			return moveN(r1,c1,r2,c2); break;
		case PieceType::R :
			return moveR(r1,c1,r2,c2); break;
		case PieceType::P :
			return moveP(r1,c1,r2,c2,type); break;
		default:
			break;
	}
	return false;
}


ostream &operator<<(ostream &out, const Board &b) {
    out << *(b.td);
    return out;
}


void Board::SetDefaultBoard() {
    // Black at top, White at bottom
    Colour black = Colour::Black;
    Colour white = Colour::White;
    
    // setting up Ponds for Black and White
    for(int i = 0; i < boardLength; ++i) {
        theBoard.at(1).at(i).setPiece(PieceType::P, black);
        theBoard.at(6).at(i).setPiece(PieceType::P, white);
	updateGraphics(1,i);
	updateGraphics(6,i);
    }


    // setting up other pieces for Black and White
    for(int j = 0; j < boardLength; ++j) {
        PieceType type;

        if(j == 0 || j == 7) {
		type = PieceType::R;
        } else if(j == 1 || j == 6) {
		type = PieceType::N;
        } else if (j == 2 || j == 5) {
		type = PieceType::B;
        } else if (j == 3) {
		type = PieceType::Q;
        } else if (j == 4) {
		type = PieceType::K;
        }
        theBoard.at(0).at(j).setPiece(type,black);
	updateGraphics(0,j);
        theBoard.at(7).at(j).setPiece(type,white);
	updateGraphics(7,j);
    }
    td->normalSetUp();
}

bool Board::validBoard() {
	return oneKingEachSide() && noPawnOnSide() && !willbechecked(Colour::Black) && !willbechecked(Colour::White);
}

bool Board::oneKingEachSide() {
    int whiteKingCount = 0;
    int blackKingCount = 0;
   

    for(int i = 0; i < boardLength; ++i) {
	for(int j = 0; j < boardLength; ++j) {
            	shared_ptr<Piece> currPiece = theBoard.at(i).at(j).getInfo().pt;
          	if(currPiece) {
            		bool currPieceKing = currPiece->getPieceType() == PieceType::K;
            		Colour currColour = currPiece->getColour();
            		bool currPieceBlack = (currColour == Colour::Black);
            		bool currPieceWhite = (currColour == Colour::White);
            		if(currPieceKing && currPieceWhite) {
                		++whiteKingCount;
            		} else if(currPieceKing && currPieceBlack) {
                		++blackKingCount;
            		}
	
         	}
     	}
    }
    return whiteKingCount == 1 && blackKingCount == 1;
}

bool Board::noPawnOnSide() {
	for (int i = 0; i < 8; i++ ) {
            shared_ptr<Piece> topPiece = theBoard.at(0).at(i).getPiece();
            shared_ptr<Piece> bottomPiece = theBoard.at(7).at(i).getPiece();
            
            if((topPiece && topPiece->getPieceType() == PieceType::P) ||
               (bottomPiece && bottomPiece->getPieceType() == PieceType::P)) {
	        return false;
	}
    }
    return true;
}

PieceType Board::identifyType(string type) {
	if (type == "Q") return PieceType::Q;
	else if (type == "B") return PieceType::B;
	else if (type == "R") return PieceType::R;
	else if (type == "P") return PieceType::P;
	else if (type == "N") return PieceType::N;
	return PieceType::Wr;
} 

bool Board::ifPawnReachEnd(int oldR, int oldC, int newR, int newC) {
	if (theBoard.at(oldR).at(oldC).getPiece() == nullptr) return false;
	if (theBoard.at(oldR).at(oldC).getPiece()->getPieceType() == PieceType::P) {
		if (newR == 0 || newR == 7) { return true;}
	}
	return false;
}

void Board::addPiece(PieceType pt, Colour co, int r, int c) {
        theBoard.at(r).at(c).setPiece(pt,co);
}

void Board::removePiece(int r, int c) {
	theBoard.at(r).at(c).setEmpty();
}

bool Board::isMyPiece(Colour co, int r, int c) {
	if (theBoard.at(r).at(c).getPiece() == nullptr) return false;
	Colour colour = theBoard.at(r).at(c).getPiece()->getColour();
	return co == colour;
} 


		
bool Board::canCastling(Colour c, int r1, int c1, int r2, int c2) {
	if (theBoard.at(r1).at(c1).getPiece()->isFirstMove() == false) return false;
	if (c == Colour::Black && r1 == 0 && r2 == 0 && c1 == 4) {
		if (c2 == 6) {
			if (theBoard.at(0).at(7).getPiece() != nullptr &&
			    theBoard.at(0).at(7).getPiece()->getPieceType() == PieceType::R &&
			    theBoard.at(0).at(7).getPiece()->isFirstMove() == true &&
			    theBoard.at(r1).at(c1).ifValid(0,7)) {
				return true;
			}
		}
		else if (c2 == 2) {
			if (theBoard.at(0).at(0).getPiece() != nullptr && 
			    theBoard.at(0).at(0).getPiece()->getPieceType() == PieceType::R &&
                            theBoard.at(0).at(0).getPiece()->isFirstMove() == true &&
                            theBoard.at(r1).at(c1).ifValid(0,0)) {
                                return true;
                        }
		}
	}
	else if (c == Colour::White && r1 == 7 && r2 == 7 && c1 == 4) {
		if (c2 == 6) {
			if (theBoard.at(7).at(7).getPiece() != nullptr &&
			    theBoard.at(7).at(7).getPiece()->getPieceType() == PieceType::R &&
                            theBoard.at(7).at(7).getPiece()->isFirstMove() == true &&
                            theBoard.at(r1).at(c1).ifValid(7,7)) {
                                return true;
                        }
                }
                else if (c2 == 2) {
                        if (theBoard.at(7).at(0).getPiece() != nullptr &&
			    theBoard.at(7).at(0).getPiece()->getPieceType() == PieceType::R &&
                            theBoard.at(7).at(0).getPiece()->isFirstMove() == true &&
                            theBoard.at(r1).at(c1).ifValid(7,0)) {
                                return true;
                        }
                }
        }
	return false;
}

bool Board::isLocValid(int r1, int c1, int r2, int c2) {
	if (r1 > 7 || r1 < 0 || r2 > 7 || r2 < 0 || c1 > 7 || c1 < 0 || c2 > 7 || c2 < 0) return false;
	shared_ptr<Piece> piece = theBoard.at(r1).at(c1).getPiece();
	if (piece->getPieceType() == PieceType::K) {
		if (piece->getColour() == Colour::Black && r1 == 0) {
			if (canCastling(Colour::Black,r1,c1,r2,c2)) return absVal(c1 - c2) == 2;
			return (absVal(r2 - r1) <= 1 && absVal(c2 - c1) <= 1);
		}
		else if (piece->getColour() == Colour::White && r1 == 7){
			if (canCastling(Colour::White,r1,c1,r2,c2)) return absVal(c1 - c2) == 2;
			return (absVal(r2 - r1) <= 1 && absVal(c2 - c1) <= 1);
		}
		return (absVal(r2 - r1) <= 1 && absVal(c2 - c1) <= 1);
	} 
	else if (piece->getPieceType() == PieceType::Q) {
		return (r1 == r2) || (c1 == c2) || (absVal(r2 - r1) == absVal(c2 - c1));
	}
	else if (piece->getPieceType() == PieceType::B) {
		return absVal(r2 - r1) == absVal(c2 - c1);
	}
	else if (piece->getPieceType() == PieceType::N) {
		return (absVal(r2 - r1) == 2 && absVal(c1 - c2) == 1) || (absVal(r2 - r1) == 1 && absVal(c1 - c2) == 2);
	}
	else if (piece->getPieceType() == PieceType::R) {
		return (c1 == c2) || (r1 == r2);
	}
	else if (piece->getPieceType() == PieceType::P) {
        	if (piece->getColour() == Colour::Black) {
                	if (c2 == c1 && (r2 == r1 + 1) && theBoard.at(r2).at(c2).getPiece() == nullptr)  return true;
                	else if ((c2 == c1)&& (r2 == r1 + 2) && theBoard.at(r2).at(c2).getPiece() == nullptr) {
                       		if (piece->isFirstMove() && theBoard.at(r1).at(c1).ifValid(r2, c2)) return true;
                        }
                	else if ( absVal(c1 - c2) == 1 && (r2 - r1 == 1)) {
		//		if ((theBoard.at(r2).at(c2) == nullptr) && EnPassat) return true;
                        	if (isMyPiece(Colour::White, r2,c2)) return true;
			}
		}
		else if(piece->getColour() == Colour::White) {
			if (c2 == c1 && (r2 == r1 - 1) && theBoard.at(r2).at(c2).getPiece() == nullptr) {
                                return true;
                        }
                        else if ((c2 == c1)&& (r2 == r1 - 2) && theBoard.at(r2).at(c2).getPiece() == nullptr) {
                                if (piece->isFirstMove() && theBoard.at(r1).at(c1).ifValid(r2, c2)) return true;
                        }
                        else if (absVal(c1 - c2) == 1 && (r2 - r1 == -1)) {
		//		if ((theBoard.at(r2).at(c2) == nullptr) && EnPasset) return true;
                            if (isMyPiece(Colour::Black, r2,c2)) return true;
                        }
		}
		return false;
	}
}
void Board::updateGraphics(int r, int c) {
	gd->notify(theBoard.at(r).at(c));
}

//********************************************************************
bool Board::generatemove(Colour colour) {
    //cout << "enter generatemove" << endl;
    vector<Cell *> mine;
    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            if(theBoard[i][j].getPiece() != nullptr && theBoard[i][j].getPiece()->getColour() == colour) {
                mine.push_back(&theBoard[i][j]);
            }
        }
    }
    while(!mine.empty()) {
        int index = rand() % mine.size();
        Info info = mine[index]->getInfo();
        int r = static_cast<int>(info.row);
        int c = static_cast<int>(info.col);
        vector<Loc> steps;
        if(theBoard[r][c].getPiece()->getPieceType() == PieceType::K) {
            steps.push_back({-1, 0});
            steps.push_back({-1, 1});
            steps.push_back({0, 1});
            steps.push_back({1, 1});
            steps.push_back({1, 0});
            steps.push_back({1, -1});
            steps.push_back({0, -1});
            steps.push_back({-1, -1});
        } else if (theBoard[r][c].getPiece()->getPieceType() == PieceType::N) {
            steps.push_back({1, 2});
            steps.push_back({1, -2});
            steps.push_back({-1, 2});
            steps.push_back({-1, -2});
            steps.push_back({2, 1});
            steps.push_back({2, -1});
            steps.push_back({-2, 1});
            steps.push_back({-2, -1});
        } else if (theBoard[r][c].getPiece()->getPieceType() == PieceType::B) {
            for(int i = 1; i < 8; i++) {
                steps.push_back({i, i});
                steps.push_back({-i, i});
                steps.push_back({-i, -i});
                steps.push_back({i, -i});
            }
        } else if (theBoard[r][c].getPiece()->getPieceType() == PieceType::P) {
            steps = {{1, 0}, {-1, 0}, {2, 0}, {-2, 0}, {-1, -1}, {-1, 1}, {1, 1}, {1, -1}};
        } else if (theBoard[r][c].getPiece()->getPieceType() == PieceType::Q) {
            for(int i = 1; i < 8; i++) {
                steps.push_back({i, i});
                steps.push_back({-i, i});
                steps.push_back({-i, -i});
                steps.push_back({i, -i});
            }
            for(int i = -c; i <= (7-c); i++) {
                steps.push_back({0, i});
            }
            for(int i = -r; i <= (7-r); i++) {
                steps.push_back({i, 0});
            }
        } else if (theBoard[r][c].getPiece()->getPieceType() == PieceType::R) {
            for(int i = -c; i <= (7-c); i++) {
                steps.push_back({0, i});
            }
            for(int i = -r; i <= (7-r); i++) {
                steps.push_back({i, 0});
            }
        }
        while(!steps.empty()) {
            int d = rand() % steps.size();
            if(isLocValid(r, c, r + steps[d].x1, c + steps[d].x2) && !isMyPiece(colour, r + steps[d].x1, c + steps[d].x2)) {
                if(theBoard[r][c].getPiece()->getPieceType() == PieceType::N) {
                    move(r, c, r + steps[d].x1, c + steps[d].x2, "");
                    return true;
                } else if(theBoard[r][c].getPiece()->getPieceType() != PieceType::N && mine[index]->ifValid(r + steps[d].x1, c + steps[d].x2)) {
                    move(r, c, r + steps[d].x1, c + steps[d].x2, "");
                    return true;
                }
            }
            steps.erase(steps.begin() + d);
        }
        mine.erase(mine.begin() + index);
    }
    return false;
}





bool Board::capturingmove(Colour colour) {
    //cout << "enter capturing function" << endl;
    vector<Loc> mine;
    vector<Loc> enemy;
    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            if(theBoard[i][j].getPiece() != nullptr && theBoard[i][j].getPiece()->getColour() == colour) {
                mine.push_back({i, j});
            } else if(theBoard[i][j].getPiece() != nullptr && theBoard[i][j].getPiece()->getColour() != colour) {
                enemy.push_back({i, j});
            }
        }
    }
    vector<Pos> pos;
    for(int i = 0; i < static_cast<int>(mine.size()); i++) {
        for(int j = 0; j < static_cast<int>(enemy.size()); j++) {
            if(isLocValid(mine[i].x1, mine[i].x2, enemy[j].x1, enemy[j].x2) && !isMyPiece(colour, enemy[j].x1, enemy[j].x2)) {
                if(theBoard[mine[i].x1][mine[i].x2].getPiece()->getPieceType() == PieceType::N) {
                    //cout << "r1: " << mine[i].x1<< " c1: " << mine[i].x2 << " r2: " << enemy[j].x1 << " c2: " << enemy[j].x2 << endl;
                    pos.emplace_back(Pos{mine[i].x1, mine[i].x2, enemy[j].x1, enemy[j].x2});
                } else if (theBoard[mine[i].x1][mine[i].x2].getPiece()->getPieceType() != PieceType::N && theBoard[mine[i].x1][mine[i].x2].ifValid(enemy[j].x1, enemy[j].x2)) {
                    //cout << "r1: " << mine[i].x1<< " c1: " << mine[i].x2 << " r2: " << enemy[j].x1 << " c2: " << enemy[j].x2 << endl;
                    pos.emplace_back(Pos{mine[i].x1, mine[i].x2, enemy[j].x1, enemy[j].x2});
                }
            }
        }
    }
    if(pos.empty()) {
        //cout << "capturing return false" << endl;
        return false;
    }
    int d = rand() % pos.size();
    move(pos[d].r1, pos[d].c1, pos[d].r2, pos[d].c2, "");
    //cout << "capturing return true" << endl;
    return true;
}





bool Board::check(Colour colour) {
    Loc k;
    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            if(theBoard[i][j].getPiece() != nullptr && theBoard[i][j].getPiece()->getColour() != colour && theBoard[i][j].getPiece()->getPieceType() == PieceType::K) {
                k.x1 = i; k.x2 = j;
            }
        }
    }
    vector<Loc> locs;
    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            locs.push_back({i, j});
        }
    }
    while(!locs.empty()) {
        int num = rand() % locs.size();
        Loc loc = locs[num];
        if(theBoard[loc.x1][loc.x2].getPiece() != nullptr && theBoard[loc.x1][loc.x2].getPiece()->getColour() == colour) {
            if(theBoard[loc.x1][loc.x2].getPiece()->getPieceType() == PieceType::N && isLocValid(loc.x1, loc.x2, k.x1, k.x2)){
                move(loc.x1, loc.x2, k.x1, k.x2, "");
                return true;
            } else if(theBoard[loc.x1][loc.x2].getPiece()->getPieceType() != PieceType::N && theBoard[loc.x1][loc.x2].ifValid(k.x1, k.x2) && isLocValid(loc.x1, loc.x2, k.x1, k.x2)) {
                move(loc.x1, loc.x2, k.x1, k.x2, "");
                return true;
            }
        } locs.erase(locs.begin() + num);
    } return false;
}





vector<Loc> Board::validcapturingmove(Colour colour, Loc loc) {
    vector<Loc> steps;
    vector<Loc> valid;
    if(theBoard[loc.x1][loc.x2].getPiece()->getPieceType() == PieceType::K) {
        steps.push_back({-1, 0});
        steps.push_back({-1, 1});
        steps.push_back({0, 1});
        steps.push_back({1, 1});
        steps.push_back({1, 0});
        steps.push_back({1, -1});
        steps.push_back({0, -1});
        steps.push_back({-1, -1});
    } else if (theBoard[loc.x1][loc.x2].getPiece()->getPieceType() == PieceType::N) {
        steps.push_back({1, 2});
        steps.push_back({1, -2});
        steps.push_back({-1, 2});
        steps.push_back({-1, -2});
        steps.push_back({2, 1});
        steps.push_back({2, -1});
        steps.push_back({-2, 1});
        steps.push_back({-2, -1});
    } else if (theBoard[loc.x1][loc.x2].getPiece()->getPieceType() == PieceType::B) {
        for(int i = 1; i < 8; i++) {
            steps.push_back({i, i});
            steps.push_back({-i, i});
            steps.push_back({-i, -i});
            steps.push_back({i, -i});
        }
    } else if (theBoard[loc.x1][loc.x2].getPiece()->getPieceType() == PieceType::P) {
        steps = {{1, 0}, {-1, 0}, {2, 0}, {-2, 0}, {-1, -1}, {-1, 1}, {1, 1}, {1, -1}};
    } else if (theBoard[loc.x1][loc.x2].getPiece()->getPieceType() == PieceType::Q) {
        for(int i = 1; i < 8; i++) {
            steps.push_back({i, i});
            steps.push_back({-i, i});
            steps.push_back({-i, -i});
            steps.push_back({i, -i});
        }
        for(int i = -loc.x2; i <= (7-loc.x2); i++) {
            steps.push_back({0, i});
        }
        for(int i = -loc.x1; i <= (7-loc.x1); i++) {
            steps.push_back({i, 0});
        }
    } else if (theBoard[loc.x1][loc.x2].getPiece()->getPieceType() == PieceType::R) {
        for(int i = -loc.x2; i <= (7-loc.x2); i++) {
            steps.push_back({0, i});
        }
        for(int i = -loc.x1; i <= (7-loc.x1); i++) {
            steps.push_back({i, 0});
        }
    }
    for(int i = 0; i < static_cast<int>(steps.size()); i++) {
        
        if(isLocValid(loc.x1, loc.x2, loc.x1 + steps[i].x1, loc.x2 + steps[i].x2) && !isMyPiece(colour, loc.x1 + steps[i].x1, loc.x2 + steps[i].x2)) {
            if(theBoard[loc.x1][loc.x2].getPiece()->getPieceType() == PieceType::N){
                valid.push_back({loc.x1 + steps[i].x1, loc.x2 + steps[i].x2});
            } else if(theBoard[loc.x1][loc.x2].getPiece()->getPieceType() != PieceType::N && theBoard[loc.x1][loc.x2].ifValid(loc.x1 + steps[i].x1, loc.x2 + steps[i].x2)) {
                valid.push_back({loc.x1 + steps[i].x1, loc.x2 + steps[i].x2});
            }
        }
    }
    return valid;
}




bool Board::capturedmove(Colour colour) {
    // move the piece that can completely escape, if not, return false
    //cout << "enter capturedmove" << endl;
    vector<Loc> mine;
    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            if(theBoard[i][j].getPiece() != nullptr && theBoard[i][j].getPiece()->getColour() == colour) {
                mine.push_back({i, j});
            }
        }
    }
    //cout << "before canbecaptured" << endl;
    vector<Loc> canbecaptured;
    for(int i = 0; i < static_cast<int>(mine.size()); i++) {
        if(validcaptured(colour, mine[i])) {
            canbecaptured.emplace_back(mine[i]);
        }
    }
    //cout << "after creating canbecaptured" << endl;
    vector<Pos> candidate;
    for(int i = 0; i < static_cast<int>(canbecaptured.size()); i++) {
        vector<Loc> validmove = validcapturingmove(colour, canbecaptured[i]);
        PieceType pt = theBoard[canbecaptured[i].x1][canbecaptured[i].x2].getPiece()->getPieceType();
        //cout << "Before getColour" << endl;
        Colour c = theBoard[canbecaptured[i].x1][canbecaptured[i].x2].getPiece()->getColour();
        theBoard[canbecaptured[i].x1][canbecaptured[i].x2].setEmpty();
        for(int j = 0; j < static_cast<int>(validmove.size()); j++) {
            if(!validcaptured(colour, validmove[j])) {
                //cout << canbecaptured[i].x1 << canbecaptured[i].x2 << validmove[j].x1 << validmove[j].x2 << " <= is valid" << endl;
                candidate.emplace_back(Pos{canbecaptured[i].x1, canbecaptured[i].x2, validmove[j].x1, validmove[j].x2});
            }
        }
        theBoard[canbecaptured[i].x1][canbecaptured[i].x2].setPiece(pt, c);
    }
    if(candidate.empty()) {
        //cout << "capturedmove return false" << endl;
        return false;
    } else {
        int index = rand() % candidate.size();
        //cout << candidate[index].r1 << candidate[index].c1 << candidate[index].r2 << candidate[index].c2 << " <= is choosen" << endl;
        move(candidate[index].r1, candidate[index].c1, candidate[index].r2, candidate[index].c2, "");
        //cout << "capturedmove return true" << endl;
        return true;
    }
}




// ***************************** Level4 *****************************

bool Board::validcaptured(Colour colour, Loc loc) {
    // return whether or not loc will be captured by enemy, me = colour, enemy != colour
    // acturally, loc either has ecolour piece, or does no piece
    //return true if loc been captured, false otherwise
    int check = 0;
    PieceType pt = PieceType::N;
    Colour c = Colour::White;
    if(theBoard[loc.x1][loc.x2].getPiece() == nullptr) {
        check = 1;
    } else {
        pt = theBoard[loc.x1][loc.x2].getPiece()->getPieceType();
        c = theBoard[loc.x1][loc.x2].getPiece()->getColour();
        theBoard[loc.x1][loc.x2].setEmpty();
    }
    theBoard[loc.x1][loc.x2].setPiece(PieceType::N, colour);
    vector<Loc> enemy;
    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            if(theBoard[i][j].getPiece() != nullptr && theBoard[i][j].getPiece()->getColour() != colour) {
                if(isLocValid(i, j, loc.x1, loc.x2) && theBoard[i][j].getPiece()->getPieceType() == PieceType::N) {
                    if(check == 1) {
                        theBoard[loc.x1][loc.x2].setEmpty();
                    } else {
                        theBoard[loc.x1][loc.x2].setEmpty();
                        theBoard[loc.x1][loc.x2].setPiece(pt, c);
                    }
                    return true;
                } else if(theBoard[i][j].getPiece()->getPieceType() != PieceType::N && isLocValid(i, j, loc.x1, loc.x2) && theBoard[i][j].ifValid(loc.x1, loc.x2)) {
                    if(check == 1) {
                        theBoard[loc.x1][loc.x2].setEmpty();
                    } else {
                        theBoard[loc.x1][loc.x2].setEmpty();
                        theBoard[loc.x1][loc.x2].setPiece(pt, c);
                    }
                    return true;
                }
            }
        }
    }
    if(check == 1) {
        theBoard[loc.x1][loc.x2].setEmpty();
    } else {
        theBoard[loc.x1][loc.x2].setEmpty();
        theBoard[loc.x1][loc.x2].setPiece(pt, c);
    }
    return false;
}




vector<Pos> Board::escape(Colour colour, Loc loc) {
    // suppose we know loc will be captured by enemy, move the piece at loc, return all possible escape location
    // loc has colour = colour
    vector<Pos> candidate;
    vector<Loc> validmove = validcapturingmove(colour, loc);
    shared_ptr<Piece> p = theBoard[loc.x1][loc.x2].getPiece();
    theBoard[loc.x1][loc.x2].updatePiece(nullptr);
    for(int j = 0; j < static_cast<int>(validmove.size()); j++) {
        if(!validcaptured(colour, validmove[j])) {
            candidate.emplace_back(Pos{loc.x1, loc.x2, validmove[j].x1, validmove[j].x2});
        }
    }
    theBoard[loc.x1][loc.x2].updatePiece(p);
    return candidate;
}





bool Board::maxValuemove(Colour colour) {
    if(check(colour)) {
        return true;
    }
    vector<Pos> validmove;
    vector<Pos> captured;
    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            if(theBoard[i][j].getPiece() != nullptr && theBoard[i][j].getPiece()->getColour() == colour) {
                vector<Loc> tmp1 = validcapturingmove(colour, {i, j});
                shared_ptr<Piece> p = theBoard[i][j].getPiece();
                theBoard[i][j].updatePiece(nullptr);
                for(int k = 0; k < static_cast<int>(tmp1.size()); k++) {
                    validmove.emplace_back(Pos{i, j, tmp1[k].x1, tmp1[k].x2});
                    if(validcaptured(colour, {tmp1[k].x1, tmp1[k].x2})) {
                        captured.emplace_back(Pos{i, j, tmp1[k].x1, tmp1[k].x2});
                    }
                }
                theBoard[i][j].updatePiece(p);
            }
        }
    }
    
    vector<int> marks;
    for(int i = 0; i < static_cast<int>(validmove.size()); i++) {
        int value1 = 0;
        if(theBoard[validmove[i].r2][validmove[i].c2].getPiece() != nullptr) {
            value1 = abs(theBoard[validmove[i].r2][validmove[i].c2].getPiece()->getValue());
        } marks.emplace_back(value1);
        for(int j = 0; j < static_cast<int>(captured.size()); j++) {
            if(captured[j].r1 == validmove[i].r1 && captured[j].c1 == validmove[i].c1 && captured[j].r2 == validmove[i].r2 && captured[j].c2 == validmove[i].c2) {
                int value2 = 0;
                if(theBoard[validmove[i].r1][validmove[i].c1].getPiece() != nullptr) {
                    value2 = abs(theBoard[validmove[i].r1][validmove[i].c1].getPiece()->getValue());
                }
                marks.pop_back();
                marks.emplace_back(value1 - value2);
                break;
            }
        }
    }
    
    int max1 = -1000;
    for(int i = 0; i < static_cast<int>(marks.size()); i++) {
        if(marks[i] > max1) {
            max1 = marks[i];
        }
    }
    
    vector<Pos> escapemoves;
    vector<int> reservevalue;
    int max2 = -1000;
    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            if(theBoard[i][j].getPiece() != nullptr && theBoard[i][j].getPiece()->getColour() == colour) {
                vector<Pos> escapepos = escape(colour, {i, j});
                if(validcaptured(colour, {i, j}) && !escapepos.empty()) {
                    for(int k = 0; k < static_cast<int>(escapepos.size()); k++) {
                        reservevalue.emplace_back(abs(theBoard[i][j].getPiece()->getValue()));
                        if(abs(theBoard[i][j].getPiece()->getValue()) > max2) {
                            max2 = abs(theBoard[i][j].getPiece()->getValue());
                        }
                        escapemoves.emplace_back(escapepos[k]);
                    }
                }
            }
        }
    }
    vector<int> maxvaluepos;
    if(max1 > 0 || escapemoves.empty()) {
        for(int i = 0; i < static_cast<int>(marks.size()); i++) {
            if(max1 == marks[i]) {
                maxvaluepos.emplace_back(i);
            }
        }
        int index = rand() % maxvaluepos.size();
        move(validmove[maxvaluepos[index]].r1, validmove[maxvaluepos[index]].c1, validmove[maxvaluepos[index]].r2, validmove[maxvaluepos[index]].c2, "");
        
    } else {
        for(int i = 0; i < static_cast<int>(reservevalue.size()); i++) {
            if(max2 == reservevalue[i]) {
                maxvaluepos.emplace_back(i);
            }
        }
        int index = rand() % maxvaluepos.size();
        move(escapemoves[maxvaluepos[index]].r1, escapemoves[maxvaluepos[index]].c1, escapemoves[maxvaluepos[index]].r2, escapemoves[maxvaluepos[index]].c2, "");
        
    }
    return false;
}



//bool outOfRange(int a) {
 //   return (a < 0) || (a > 7);
//}

// **********************************************************************************************

bool Board::willbechecked(Colour colour) {
    // return true if king in colour can be captured by ecolour, false otherwise
    Loc myking;
    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            if(theBoard[i][j].getPiece() != nullptr && theBoard[i][j].getPiece()->getColour() == colour && theBoard[i][j].getPiece()->getPieceType() == PieceType::K) {
                myking.x1 = i; myking.x2 = j;
            }
        }
    }

	int kingR = myking.x1;
     	int kingC = myking.x2; 
	Colour enemyColour = (colour == Colour::White) ? Colour::Black : Colour::White;
	for(int r = kingR - 2; r <= kingR + 2; r += 4) {
           for(int c = kingC - 1; c <= kingC + 1; c += 2) {
               if(!outOfRange(r) && !outOfRange(c)) {
                   shared_ptr<Piece> enemyPiece = theBoard.at(r).at(c).getPiece();
                   if(enemyPiece && enemyPiece->getColour() == enemyColour && enemyPiece->getPieceType() == PieceType::N) return true;
            }
         }
       }
       for(int r = kingR - 1; r <= kingR + 1; r += 2) {
          for(int c = kingC - 2; c <= kingC + 2; c += 4) {
              if(!outOfRange(r) && !outOfRange(c)) {
                  shared_ptr<Piece> enemyPiece = theBoard.at(r).at(c).getPiece();
                  if(enemyPiece && enemyPiece->getColour() == enemyColour && enemyPiece->getPieceType() == PieceType::N) return true;
           }
         }
      }

    if(validcaptured(colour, myking)) {
        return true;
    } else {
        return false;
    }
}





bool Board::checkmate(Colour colour) {
    // return true if king in colour can be captured by ecolour, and move any valid move of colour cannot safe king in colour
    
    if(!willbechecked(colour)) {
        return false;
    }
    Loc myking;
    vector<Loc> minerest;
    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            if(theBoard[i][j].getPiece() != nullptr && theBoard[i][j].getPiece()->getColour() == colour && theBoard[i][j].getPiece()->getPieceType() == PieceType::K) {
                myking.x1 = i; myking.x2 = j;
            } else if (theBoard[i][j].getPiece() != nullptr && theBoard[i][j].getPiece()->getColour() == colour) {
                minerest.push_back({i, j});
            }
        }
    }
    if(canCastling(colour, myking.x1, myking.x2, myking.x1, 2)) {
                shared_ptr<Piece> rookTemp = theBoard.at(myking.x1).at(0).getPiece();
		shared_ptr<Piece> kingTemp = theBoard.at(myking.x1).at(myking.x2).getPiece();
                if(rookTemp) {
                    theBoard.at(myking.x1).at(0).updatePiece(nullptr);
                    theBoard.at(myking.x1).at(3).updatePiece(rookTemp);
                    theBoard.at(myking.x1).at(4).updatePiece(nullptr);
                    theBoard.at(myking.x1).at(2).updatePiece(kingTemp);
//cout << "KingCheck1: " << willbechecked(colour) << endl;
                    if(!willbechecked(colour)) {
                        theBoard.at(myking.x1).at(0).updatePiece(rookTemp);
                        theBoard.at(myking.x1).at(3).updatePiece(nullptr);
                        theBoard.at(myking.x1).at(4).updatePiece(kingTemp);
                        theBoard.at(myking.x1).at(2).updatePiece(nullptr);
                        return false;
                    }
                    theBoard.at(myking.x1).at(0).updatePiece(rookTemp);
                    theBoard.at(myking.x1).at(3).updatePiece(nullptr);
                    theBoard.at(myking.x1).at(4).updatePiece(kingTemp);
                    theBoard.at(myking.x1).at(2).updatePiece(nullptr);
                }
             }

    if(canCastling(colour, myking.x1, myking.x2, myking.x1, 6)) {
                shared_ptr<Piece> rookTemp = theBoard.at(myking.x1).at(7).getPiece();
		shared_ptr<Piece> kingTemp = theBoard.at(myking.x1).at(myking.x2).getPiece();
                if(rookTemp) {
                    theBoard.at(myking.x1).at(7).updatePiece(nullptr);
                    theBoard.at(myking.x1).at(5).updatePiece(rookTemp);
                    theBoard.at(myking.x1).at(4).updatePiece(nullptr);
                    theBoard.at(myking.x1).at(6).updatePiece(kingTemp);
//cout << "KingCheck2: " << willbechecked(colour) << endl;

                    if(!willbechecked(colour)) {
                        theBoard.at(myking.x1).at(7).updatePiece(rookTemp);
                        theBoard.at(myking.x1).at(5).updatePiece(nullptr);
                        theBoard.at(myking.x1).at(4).updatePiece(kingTemp);
                        theBoard.at(myking.x1).at(6).updatePiece(nullptr);
                        return false;
                    }

                     theBoard.at(myking.x1).at(7).updatePiece(rookTemp);
                     theBoard.at(myking.x1).at(5).updatePiece(nullptr);
                     theBoard.at(myking.x1).at(4).updatePiece(kingTemp);
                     theBoard.at(myking.x1).at(6).updatePiece(nullptr);
                }
             }
    if(!escape(colour, myking).empty()) {
        return false;
    }
    for(int i = 0; i < static_cast<int>(minerest.size()); i++) {
        vector<Loc> validmove = validcapturingmove(colour, minerest[i]);
        shared_ptr<Piece> oldp = theBoard[minerest[i].x1][minerest[i].x2].getPiece();
        shared_ptr<Piece> newp = nullptr;
        theBoard[minerest[i].x1][minerest[i].x2].updatePiece(nullptr);
        for(int j = 0; j < static_cast<int>(validmove.size()); j++) {
            newp = theBoard[validmove[j].x1][validmove[j].x2].getPiece();
            theBoard[validmove[j].x1][validmove[j].x2].updatePiece(oldp);
            if(!willbechecked(colour)) {
                theBoard[minerest[i].x1][minerest[i].x2].updatePiece(oldp);
                theBoard[validmove[j].x1][validmove[j].x2].updatePiece(newp);
                return false;
            }
            theBoard[validmove[j].x1][validmove[j].x2].updatePiece(newp);
        }
        theBoard[minerest[i].x1][minerest[i].x2].updatePiece(oldp);
    }
    return true;
}




bool Board::stalemate(Colour colour) {
    // return true if colour is currently not in check, but make any valid move will put king into check
    
    if(willbechecked(colour)) {
        return false;
    }
    vector<Loc> mine;
    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            if (theBoard[i][j].getPiece() != nullptr && theBoard[i][j].getPiece()->getColour() == colour) {
                mine.push_back({i, j});
            }
        }
    }
    for(int i = 0; i < static_cast<int>(mine.size()); i++) {
        vector<Loc> validmove = validcapturingmove(colour, mine[i]);
        shared_ptr<Piece> oldp = theBoard[mine[i].x1][mine[i].x2].getPiece();
        shared_ptr<Piece> newp = nullptr;
        theBoard[mine[i].x1][mine[i].x2].updatePiece(nullptr);
        for(int j = 0; j < static_cast<int>(validmove.size()); j++) {
            newp = theBoard[validmove[j].x1][validmove[j].x2].getPiece();
            theBoard[validmove[j].x1][validmove[j].x2].updatePiece(oldp);
            if(!willbechecked(colour)) {
                theBoard[mine[i].x1][mine[i].x2].updatePiece(oldp);
                theBoard[validmove[j].x1][validmove[j].x2].updatePiece(newp);
                return false;
            }
            theBoard[validmove[j].x1][validmove[j].x2].updatePiece(newp);
        }
        theBoard[mine[i].x1][mine[i].x2].updatePiece(oldp);
    }
    return true;
}






bool Board::level1(Colour colour) {
    generatemove(colour);
    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            if(theBoard[i][j].getPiece() != nullptr && theBoard[i][j].getPiece()->getColour() != colour && theBoard[i][j].getPiece()->getPieceType() == PieceType::K) {
                return false;
            }
        }
    }
    return true;
}


bool Board::level2(Colour colour) {
    Loc myking;
    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            if(theBoard[i][j].getPiece() != nullptr && theBoard[i][j].getPiece()->getColour() == colour && theBoard[i][j].getPiece()->getPieceType() == PieceType::K) {
                myking.x1 = i; myking.x2 = j;
            }
        }
    }
    
    if(!check(colour)) {
        if(willbechecked(colour)) {
            vector<Pos> escapeking = escape(colour, myking);
            if(!escapeking.empty()) {
                int i = rand() % escapeking.size();
                move(escapeking[i].r1, escapeking[i].c1, escapeking[i].r2, escapeking[i].c2, "");
                return false;
            }
        }
        if(!capturingmove(colour)) {
            generatemove(colour);
        }
        return false;
    } else {
        //cout << "successfullt check enemy" << endl;
        return true;
    }
}



bool Board::level3(Colour colour) {
    Loc myking;
    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            if(theBoard[i][j].getPiece() != nullptr && theBoard[i][j].getPiece()->getColour() == colour && theBoard[i][j].getPiece()->getPieceType() == PieceType::K) {
                myking.x1 = i; myking.x2 = j;
            }
        }
    }
    
    if(!check(colour)) {
        if(willbechecked(colour)) {
            vector<Pos> escapeking = escape(colour, myking);
            if(!escapeking.empty()) {
                int i = rand() % escapeking.size();
                move(escapeking[i].r1, escapeking[i].c1, escapeking[i].r2, escapeking[i].c2, "");
                return false;
            }
        }
        if(!capturedmove(colour)) {
            if(!capturingmove(colour)) {
                generatemove(colour);
            }
        }
        return false;
    }
    else {
        //cout << "successfully check enemy" << endl;
        return true;
    }
}


