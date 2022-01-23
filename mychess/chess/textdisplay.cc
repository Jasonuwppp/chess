#include "textdisplay.h"
#include <iostream>
using namespace std;


TextDisplay::TextDisplay() : theDisplay{vector<vector<char>>(boardSize, vector<char>(boardSize, '-')) } {


    char num = '8';
    char letter = 'a';
    char blank = ' ';

    for(size_t i = 1; i <= boardSize - 2; ++i) {
        theDisplay.at(i - 1).at(0) = num;
        theDisplay.at(9).at(i + 1) = letter;
        num--;
        letter++;
    }

    for(size_t j = 0; j < boardSize; ++j) {
        theDisplay.at(j).at(1) = blank;
        theDisplay.at(8).at(j) = blank;
    }

    theDisplay.at(boardSize - 1).at(0) = blank;
}


void TextDisplay::normalSetUp() {
    // setting ponds for both players
    for(int k = 2; k < boardSize; ++k) {
        char bPawn = 'p';
        char wPawn = 'P';

        theDisplay.at(boardSize - 9).at(k) = bPawn;
        theDisplay.at(boardSize - 4).at(k) = wPawn;
    }

    // setting other pieces for both players
    for(int h = 2; h < boardSize; ++h) {
        char bPiece = '-';
        char wPiece = '-';

        if(h == 2 || h == 9) {
            bPiece = 'r';
            wPiece = 'R';
        } else if (h == 3 || h == 8) {
            bPiece = 'n';
            wPiece = 'N';
        } else if (h == 4 || h == 7) {
            bPiece = 'b';
            wPiece = 'B';
        } else if (h == 5) {
            bPiece = 'q';
            wPiece = 'Q';
        } else if (h == 6) {
            bPiece = 'k';
            wPiece = 'K';
        }

        theDisplay.at(boardSize - boardSize).at(h) = bPiece;
        theDisplay.at(boardSize - 3).at(h) = wPiece;
    }
}



void TextDisplay::notify(Subject<Info, State> &whoNotified) {  //change the char in corresponding position
    Info info = whoNotified.getInfo();
    int row = info.row;
    int col = info.col;
    if (info.pt == nullptr) {
         theDisplay[row][col+2] = '-';
         return;
    }
    PieceType pt = (info.pt)->getPieceType();
    Colour colour = (info.pt)->getColour(); //colour of Piece, not the colour of Cell
    if(pt == PieceType::R && colour == Colour::White) {
        theDisplay[row][col+2] = 'R';
    } else if (pt == PieceType::N && colour == Colour::White) {
        theDisplay[row][col+2] = 'N';
    } else if (pt == PieceType::K && colour == Colour::White) {
        theDisplay[row][col+2] = 'K';
    } else if (pt == PieceType::Q && colour == Colour::White) {
        theDisplay[row][col+2] = 'Q';
    } else if (pt == PieceType::B && colour == Colour::White) {
        theDisplay[row][col+2] = 'B';
    } else if (pt == PieceType::P && colour == Colour::White) {
        theDisplay[row][col+2] = 'P';
    } else if(pt == PieceType::R && colour == Colour::Black) {
        theDisplay[row][col+2] = 'r';
    } else if (pt == PieceType::N && colour == Colour::Black) {
        theDisplay[row][col+2] = 'n';
    } else if (pt == PieceType::K && colour == Colour::Black) {
        theDisplay[row][col+2] = 'k';
    } else if (pt == PieceType::Q && colour == Colour::Black) {
        theDisplay[row][col+2] = 'q';
    } else if (pt == PieceType::B && colour == Colour::Black) {
        theDisplay[row][col+2] = 'b';
    } else if (pt == PieceType::P && colour == Colour::Black) {
        theDisplay[row][col+2] = 'p';
    }
}

ostream &operator<<(ostream &out, const TextDisplay &td) {
    for(size_t i = 0; i < td.boardSize; i++) {
        for(size_t j = 0; j < td.boardSize; j++) {
            out << (td.theDisplay).at(i).at(j);
        } 
        out << endl;
    }
    return out;
  }
