#ifndef __INFO_H__
#define __INFO_H__
#include <cstddef>
#include "state.h"
#include <memory>
#include <iostream>
class Piece;
struct Info {
    int row, col;
    std::shared_ptr<Piece> pt;
    Colour colour;
    bool validMove;
};

#endif
