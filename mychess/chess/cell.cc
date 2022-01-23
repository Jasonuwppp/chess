#include "cell.h"
#include <utility>
#include "piece.h"
#include "state.h"
#include <iostream>
//#include <memory>
using namespace std;

Cell::Cell(int r, int c) : r{r}, c{c} {
	p = nullptr;
}

Cell::~Cell() {}
void Cell::setEmpty() {
	//delete p;
	p.reset();
	validMove = false;
	setState(State{StateType::Display,Direction::N,9,9,9,9,false});//gaicheng ti->notify()
	notifyObservers();
}

shared_ptr<Piece> Cell::getPiece() {return p;}

Colour Cell::getColour() {return colour;}

void Cell::setPiece(PieceType pt, Colour c) {
	this->p = Piece::CreatePiece(pt,c);
	validMove = false;
	setState(State{StateType::Display,Direction::N,9,9,9,9,false});
	notifyObservers();
}
void Cell::updatePiece(shared_ptr<Piece> p) {
	swap(this->p,p);
	setState(State{StateType::Display,Direction::N,9,9,9,9,false});
	notifyObservers();
}

bool Cell::ifValid(int newR, int newC) {
	validMove = false;
	setState(State{StateType::OriPiece,Direction::N,newR, newC, r, c,false});
	notifyObservers();
	return validMove; 
}


void Cell::notify(Subject<Info, State> &whoFrom) {
    if (whoFrom.getState().type == StateType::OriPiece) {
         if (r == whoFrom.getState().newR && c == whoFrom.getState().newC) {		
		if (r + 1 == whoFrom.getInfo().row && c + 1 == whoFrom.getInfo().col) {
                    setState(State{StateType::Reply, Direction::SE,whoFrom.getState().newR,whoFrom.getState().newC, whoFrom.getState().oldR, whoFrom.getState().oldC,true});
                    notifyObservers();
                }
                else if (r + 1 == whoFrom.getInfo().row  && c == whoFrom.getInfo().col) {
                    setState(State{StateType::Reply, Direction::S,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR, whoFrom.getState().oldC,true});
                    notifyObservers();
                }
                else if (r + 1 == whoFrom.getInfo().row  && c - 1 == whoFrom.getInfo().col) {
                    setState(State{StateType::Reply, Direction::SW,whoFrom.getState().newR, whoFrom.getState().newC,whoFrom.getState().oldR, whoFrom.getState().oldC,true});
                    notifyObservers();
                }
                else if (r == whoFrom.getInfo().row  && c + 1 == whoFrom.getInfo().col) {
                    setState(State{StateType::Reply, Direction::E,whoFrom.getState().newR, whoFrom.getState().newC,whoFrom.getState().oldR, whoFrom.getState().oldC,true});
                    notifyObservers();
                }
                else if (r == whoFrom.getInfo().row  && c - 1 == whoFrom.getInfo().col) {
                    setState(State{StateType::Reply, Direction::W,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR, whoFrom.getState().oldC,true});
                    notifyObservers();
                }
                else if (r - 1 == whoFrom.getInfo().row  && c + 1 == whoFrom.getInfo().col) {
                    setState(State{StateType::Reply, Direction::NE,whoFrom.getState().newR, whoFrom.getState().newC,whoFrom.getState().oldR, whoFrom.getState().oldC,true});
                    notifyObservers();
                }
                else if (r - 1 == whoFrom.getInfo().row  && c == whoFrom.getInfo().col) {
                    setState(State{StateType::Reply, Direction::N,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR, whoFrom.getState().oldC,true});
                    notifyObservers();
                }
                else if (r - 1 == whoFrom.getInfo().row && c - 1 == whoFrom.getInfo().col) {
                    setState(State{StateType::Reply, Direction::NW,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR, whoFrom.getState().oldC,true});
                    notifyObservers();
                }
        } else {
            if (r + 1 == whoFrom.getInfo().row && c + 1 == whoFrom.getInfo().col) {
		if (p == nullptr) {
                	setState(State{StateType::Relay, Direction::NW,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,false});
                	notifyObservers();
		}
            }
            else if (r + 1 == whoFrom.getInfo().row  && c == whoFrom.getInfo().col) {
		if (p == nullptr) {
                	setState(State{StateType::Relay, Direction::N,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,false});
                	notifyObservers();
		}
            }
            else if (r + 1 == whoFrom.getInfo().row  && c - 1 == whoFrom.getInfo().col) {
                if (p == nullptr) {
			setState(State{StateType::Relay, Direction::NE,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,false});
                	notifyObservers();
		}	
            }
            else if (r == whoFrom.getInfo().row  && c + 1 == whoFrom.getInfo().col) {
                if (p == nullptr) {
			setState(State{StateType::Relay, Direction::W,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,false});
                	notifyObservers();
		}
            }
            else if (r == whoFrom.getInfo().row  && c - 1 == whoFrom.getInfo().col) {
                if (p == nullptr) {
			setState(State{StateType::Relay, Direction::E,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,false});
                	notifyObservers();
		}
            }
            else if (r - 1 == whoFrom.getInfo().row  && c + 1 == whoFrom.getInfo().col) {
                if (p == nullptr) {
			setState(State{StateType::Relay, Direction::SW,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,false});
                	notifyObservers();
		}
            }
            else if (r - 1 == whoFrom.getInfo().row  && c == whoFrom.getInfo().col) {
                if (p == nullptr) {
			setState(State{StateType::Relay, Direction::S,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,false});
                	notifyObservers();
		}
            }
            else if (r - 1 == whoFrom.getInfo().row && c - 1 == whoFrom.getInfo().col) {
                if (p == nullptr) {
			setState(State{StateType::Relay,Direction::SE,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,false});
                	notifyObservers();
		}
            }
	}
    }
    else if (whoFrom.getState().type == StateType::Relay) {
        if ((r + 1 == whoFrom.getInfo().row) && (c == whoFrom.getInfo().col) && (whoFrom.getState().direction == Direction::N)) {
            if ((r == whoFrom.getState().newR) && (c == whoFrom.getState().newC)) {
                    setState(State{StateType::Reply, Direction::S,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,true});
                    notifyObservers();
                }
             else {
                if (p == nullptr) {
                	setState(State{StateType::Relay,Direction::N,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,false});
                	notifyObservers();
            	}
	    }
	}
        else if ((r - 1 == whoFrom.getInfo().row) && (c == whoFrom.getInfo().col) && (whoFrom.getState().direction == Direction::S)) {
            if ((r == whoFrom.getState().newR) && (c == whoFrom.getState().newC)) {
                    setState(State{StateType::Reply, Direction::N,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,true});
                    notifyObservers();
            } else {
                if (p == nullptr) {
			setState(State{StateType::Relay, Direction::S,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,false});
                	notifyObservers();
		}
            }
	} 
	else if ((r == whoFrom.getInfo().row) && (c + 1 == whoFrom.getInfo().col) && whoFrom.getState().direction == Direction::W) {
            if ((r == whoFrom.getState().newR) && (c == whoFrom.getState().newC)) {
                    setState(State{StateType::Reply, Direction::E,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,true});
                    notifyObservers();
            } else {
                if (p == nullptr) {
			setState(State{StateType::Relay, Direction::W,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,false});
                	notifyObservers();
		}
            }
        }
        else if ((r == whoFrom.getInfo().row) && (c - 1 == whoFrom.getInfo().col) && (whoFrom.getState().direction == Direction::E)) {
            if ((r == whoFrom.getState().newR) && (c == whoFrom.getState().newC)) {
                    setState(State{StateType::Reply,Direction::W,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,true});
                    notifyObservers();
            } else {
                if (p == nullptr) {
			setState(State{StateType::Relay,Direction::E,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,false});
                	notifyObservers();
		}
            }
        }
        else if ((r + 1 == whoFrom.getInfo().row) && (c + 1 == whoFrom.getInfo().col) && (whoFrom.getState().direction == Direction::NW)) {
            if ((r == whoFrom.getState().newR) && (c == whoFrom.getState().newC)) {
		setState(State{StateType::Reply, Direction::SE,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,true});
		notifyObservers();
            } else {
                if (p == nullptr){
			setState(State{StateType::Relay,Direction::NW,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,false});
                	notifyObservers();
		}
            }
        }
        else if ((r + 1 == whoFrom.getInfo().row) && (c - 1 == whoFrom.getInfo().col) && (whoFrom.getState().direction == Direction::NE)) {
            if ((r == whoFrom.getState().newR) && (c == whoFrom.getState().newC)) {
                    setState(State{StateType::Reply,Direction::SW,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,true});
                    notifyObservers();
            } else {
                if (p == nullptr) {
			setState(State{StateType::Relay, Direction::NE,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,false});
                	notifyObservers();
		}
            }
        }
        else if ((r - 1 == whoFrom.getInfo().row) && (c + 1 == whoFrom.getInfo().col) && (whoFrom.getState().direction == Direction::SW)) {
            if ((r == whoFrom.getState().newR) && (c == whoFrom.getState().newC)) {
                    setState(State{StateType::Reply, Direction::NE,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,true});
                    notifyObservers();
            } else {
                if (p == nullptr){
			setState(State{StateType::Relay,Direction::SW,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,false});
                	notifyObservers();
		}
            }
        }
        else if ((r - 1 == whoFrom.getInfo().row) && (c - 1 == whoFrom.getInfo().col) && (whoFrom.getState().direction == Direction::SE)) {
            if ((r == whoFrom.getState().newR) && (c == whoFrom.getState().newC)) {
                    setState(State{StateType::Reply, Direction::NW,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,true});
                    notifyObservers();
            } else {
                if (p == nullptr) {
			setState(State{StateType::Relay,Direction::SE,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,false});
                	notifyObservers();
		}
            }
        } else return;
    }
    else if (whoFrom.getState().type == StateType::Reply) {
        if ((r + 1 == whoFrom.getInfo().row) && (c == whoFrom.getInfo().col) && (whoFrom.getState().direction == Direction::N)) {
            if ((r == whoFrom.getState().oldR) && (c == whoFrom.getState().oldC)) {
                validMove = true;
                return;
            } else {
                setState(State{StateType::Reply, Direction::N,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,true});
                notifyObservers();
            }
        }
        else if ((r - 1 == whoFrom.getInfo().row) && (c == whoFrom.getInfo().col) && (whoFrom.getState().direction == Direction::S)) {
            if ((r == whoFrom.getState().oldR) && (c == whoFrom.getState().oldC)) {
                validMove = true;
                return;
            } else {
                setState(State{StateType::Reply, Direction::S,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,true});
                notifyObservers();
            }
        }
        else if ((r == whoFrom.getInfo().row) && (c + 1 == whoFrom.getInfo().col) && (whoFrom.getState().direction == Direction::W)) {
            if ((r == whoFrom.getState().oldR) && (c == whoFrom.getState().oldC)) {
                validMove = true;
                return;
            } else {
                setState(State{StateType::Reply, Direction::W,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,true});
                notifyObservers();
            }
            
        }
        else if ((r == whoFrom.getInfo().row) && (c - 1 == whoFrom.getInfo().col) && (whoFrom.getState().direction == Direction::E)) {
            if ((r == whoFrom.getState().oldR) && (c == whoFrom.getState().oldC)) {
                validMove = true;
                return;
            } else {
                setState(State{StateType::Reply, Direction::E,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,true});
                notifyObservers();
            }
        }
        else if ((r + 1 == whoFrom.getInfo().row) && (c + 1 == whoFrom.getInfo().col) && (whoFrom.getState().direction == Direction::NW)) {
            if ((r == whoFrom.getState().oldR) && (c == whoFrom.getState().oldC)) {
                validMove = true;
                return;
            } else {
                setState(State{StateType::Reply, Direction::NW,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,true});
                notifyObservers();
            }
            
        }
        else if ((r + 1 == whoFrom.getInfo().row) && (c - 1 == whoFrom.getInfo().col) && (whoFrom.getState().direction == Direction::NE)) {
            if ((r == whoFrom.getState().oldR) && (c == whoFrom.getState().oldC)) {
                validMove = true;
                return;
            } else {
                setState(State{StateType::Reply, Direction::NE,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,true});
                notifyObservers();
            }
            
        }
        else if ((r - 1 == whoFrom.getInfo().row) && (c + 1 == whoFrom.getInfo().col) && (whoFrom.getState().direction == Direction::SW)) {
            if ((r == whoFrom.getState().oldR) && (c == whoFrom.getState().oldC)) {
                validMove = true;
                return;
            } else {
		    setState(State{StateType::Reply, Direction::SW,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,true});
		    notifyObservers();
            }
            
        }
        else if ((r - 1 == whoFrom.getInfo().row) && (c - 1 == whoFrom.getInfo().col) && (whoFrom.getState().direction == Direction::SE)) {
            if ((r == whoFrom.getState().oldR) && (c == whoFrom.getState().oldC)) {
                validMove = true;
                return;
            } else {
                setState(State{StateType::Reply, Direction::SE,whoFrom.getState().newR,whoFrom.getState().newC,whoFrom.getState().oldR,whoFrom.getState().oldC,true});
                notifyObservers();
            }
        } else return;
    }
}

Info Cell::getInfo() const { return Info{r,c,p,colour, validMove};}



