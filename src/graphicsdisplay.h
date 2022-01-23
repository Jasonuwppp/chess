#ifndef GRAPH
#define GRAPH
#include <iostream>
#include <vector>
#include "observer.h"
#include "state.h"
#include "info.h"
#include "window.h"
#include "subject.h"
#include "cell.h"
#include "piece.h"
#include "rook.h"
#include "knight.h"
#include "king.h"
#include "queen.h"
#include "bishop.h"
#include "pawn.h"

class GraphicsDisplay: public Observer<Info, State> {
    Xwindow *xwin;
    
public:
    GraphicsDisplay();
    
    void notify(Subject<Info, State> &whoNotified) override;
    void normalSetUp();
    ~GraphicsDisplay();
    
};
#endif
