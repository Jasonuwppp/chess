#ifndef CELL_H
#define CELL_H
#include <cstddef>
#include "state.h"
#include "subject.h"
#include "observer.h"
#include "info.h"
#include <iostream>
#include <memory>
class Piece;

class Cell : public Subject<Info, State>, public Observer<Info, State> {
    int r, c;
    std::shared_ptr<Piece> p;
    Colour colour;
    bool validMove = false;
public:
    Cell(int r, int c);
    ~Cell();
    void updatePiece(std::shared_ptr<Piece> p);
    void setEmpty();
    void setPiece(PieceType,Colour);
    std::shared_ptr<Piece> getPiece();
    Colour getColour();
    bool getValidMove() {return validMove;}
    void setValidMove(bool b) {validMove = b;}
    bool ifValid(int newR, int newC);
    void move(int newR, int newC);
    void notify(Subject<Info, State> &whoFrom) override;
    Info getInfo() const override;
    friend std::ostream& operator<< (std::ostream& out, const Cell&);
};
#endif


