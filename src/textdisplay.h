#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H
#include <iostream>
#include <vector>
#include "subject.h"
#include "state.h"
#include "info.h"
#include "cell.h"
#include "observer.h"
#include "piece.h"

class Cell;

class TextDisplay: public Observer<Info, State> {
    const size_t boardSize = 10;
    std::vector<std::vector<char>> theDisplay;
public:
    TextDisplay();
    void normalSetUp(); 
    void notify(Subject<Info, State> &whoNotified) override;
    
    friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};
#endif
