#include "graphicsdisplay.h"
#include "window.h"
using namespace std;

GraphicsDisplay::GraphicsDisplay() {
    xwin = new Xwindow();
    int width = 500 / 8;
    for(int h = 0; h < 8; h++) {
        for(int w = 0; w < 8; w++) {
            if((h+w) % 2 == 0){
                xwin->fillRectangle(w * width, h * width, width, width,0);
                //xwin->fillRectangle(w * width, h * width, width, width, 0);
            } else {
                xwin->fillRectangle(w * width, h * width, width, width,5);
            }
        }
    }
}

void GraphicsDisplay::normalSetUp() {}

void GraphicsDisplay::notify(Subject<Info, State> &whoNotified) {
    int width = 500 / 8;
   /* for(int h = 0; h < 8; h++) {
        for(int w = 0; w < 8; w++) {
            if((h+w) % 2 == 0){
                xwin->fillRectangle(w * width, h * width, width, width, 0);
            } else {
                xwin->fillRectangle(w * width, h * width, width, width, 4);
            }
        }
    }*/
    Info info = whoNotified.getInfo();
    int row = info.row;
    int col = info.col;
    if ((row+col) % 2 == 0) {
	xwin->fillRectangle(col * width, row * width, width, width, 0);
    } else {
	xwin->fillRectangle(col * width, row * width, width, width, 5);
    }
    if (info.pt == nullptr) return;
    PieceType s = (info.pt)->getPieceType();
    Colour colour = (info.pt)->getColour(); //colour of Piece, not the colour of Cell
    if(s == PieceType::R && colour == Colour::White) {
        xwin->drawBigString((col * 2 + 1) * 31, (row * 2 + 1) * 31, "R");
    } else if (s == PieceType::N && colour == Colour::White) {
        xwin->drawBigString((col * 2 + 1) * 31, (row * 2 + 1) * 31, "N");
    } else if (s == PieceType::K && colour == Colour::White) {
        xwin->drawBigString((col * 2 + 1) * 31, (row * 2 + 1) * 31, "K");
    } else if (s == PieceType::Q && colour == Colour::White) {
        xwin->drawBigString((col * 2 + 1) * 31, (row * 2 + 1) * 31, "Q");
    } else if (s == PieceType::B && colour == Colour::White) {
        xwin->drawBigString((col * 2 + 1) * 31, (row * 2 + 1) * 31, "B");
    } else if (s == PieceType::P && colour == Colour::White) {
        xwin->drawBigString((col * 2 + 1) * 31, (row * 2 + 1) * 31, "P");
    } else if(s == PieceType::R && colour == Colour::Black) {
        xwin->drawBigString((col * 2 + 1) * 31, (row * 2 + 1) * 31, "r");
    } else if (s == PieceType::N && colour == Colour::Black) {
        xwin->drawBigString((col * 2 + 1) * 31, (row * 2 + 1) * 31, "n");
    } else if (s == PieceType::K && colour == Colour::Black) {
        xwin->drawBigString((col * 2 + 1) * 31, (row * 2 + 1) * 31, "k");
    } else if (s == PieceType::Q && colour == Colour::Black) {
        xwin->drawBigString((col * 2 + 1) * 31, (row * 2 + 1) * 31, "q");
    } else if (s == PieceType::B && colour == Colour::Black) {
        xwin->drawBigString((col * 2 + 1) * 31, (row * 2 + 1) * 31, "b");
    } else if (s == PieceType::P && colour == Colour::Black) {
        xwin->drawBigString((col * 2 + 1) * 31, (row * 2 + 1) * 31, "p");
    }
}

GraphicsDisplay::~GraphicsDisplay() {
    delete xwin;
}
