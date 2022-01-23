#include "controller.h"
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

using namespace std;

Controller::Controller() {
    char key = 'a';
    for(int i = 0; i < 8; ++i) {
        coor[key] = i;
        key++;
    }
   
    pt["H"] = PlayerType::H;
    pt["C1"] = PlayerType::C1;
    pt["C2"] = PlayerType::C2;
    pt["C3"] = PlayerType::C3;
    pt["C4"] = PlayerType::C4;

    colours["White"] = Colour::White;
    colours["Black"] = Colour::Black;

    pieceTypes["K"] = PieceType::K;
    pieceTypes["N"] = PieceType::N;
    pieceTypes["Q"] = PieceType::Q;
    pieceTypes["P"] = PieceType::P;
    pieceTypes["R"] = PieceType::R;
    pieceTypes["B"] = PieceType::B;
}

bool outOfRange(int coor) {
    return coor < 0 || coor > 7;
}


void Controller::move() {
	Player *player = ((count % 2) == 0) ? players.at(0) : players.at(1);
        Colour playerColour = player->getColour();
        Colour oppColour = (playerColour == Colour::White) ? Colour::Black : Colour::White;
        string oppColourString = oppColour == Colour::Black ? "Black" : "White";
        PlayerType playerType = player->getPlayerType();

	if(playerType == PlayerType::H) {	
        	string coordinates;
        	getline(cin, coordinates);

        	string coordinatesNoWS = coordinates;
      
        	coordinatesNoWS.erase(std::remove(coordinatesNoWS.begin(),coordinatesNoWS.end(),' '),coordinatesNoWS.end());

        	int oldR, oldC, newR, newC;

        	oldC = coor[coordinatesNoWS[0]];
        	oldR = 8 - (coordinatesNoWS[1] - 48);
        	newC = coor[coordinatesNoWS[2]];
        	newR = 8 - (coordinatesNoWS[3] - 48);


           	if(coordinates.length() != 6 || coordinatesNoWS.length() != 4 ||
                   outOfRange(oldC) || outOfRange(oldR) ||
                   outOfRange(newC) || outOfRange(newR)) { 
//                      cout << "hello jacob"<<endl; 
                      throw InvalidCommand();
                 }      


		string type = "";
		if (b.ifPawnReachEnd(oldR,oldC,newR,newC)) {
			cin >> type;
		}
		if (b.isMyPiece(playerColour, oldR, oldC)) {
			if (b.move(oldR, oldC, newR, newC, type)) {
                            count++;
                            cout << b;
                        }
        	}
	} else if(playerType == PlayerType::C1) {
		b.level1(playerColour);
                	count++;
			cout << b;
	} else if(playerType == PlayerType::C2) {
                b.level2(playerColour);
                count++;
                cout << b;
             
        } else if(playerType == PlayerType::C3) {
                b.level3(playerColour);
                count++;
                cout << b;
        } else if(playerType == PlayerType::C4) {
               	b.maxValuemove(playerColour);
                count++;
                cout << b;
        } 

       


           if(isGameOver(oppColour)) {
               printInstructions();
//	       playGame();
           } else if(b.willbechecked(oppColour)) {
            cout << oppColourString << " is in check.";       
           }
}

bool Controller::isGameOver(Colour oppColour) {
	   bool blackWins = false;
           bool whiteWins = false;
           bool staleMate = b.stalemate(oppColour);
           if((count % 2) == 0 && b.checkmate(Colour::White)) {
               blackWins = true;
               players.at(1)->addscore(1);
               cout << "Checkmate!" << endl;
               cout << "Black Wins This Match!" << endl;
           } else if ((count % 2) != 0 && b.checkmate(Colour::Black)) {
               whiteWins = true;
               players.at(0)->addscore(1);
               cout << "Checkmate!" << endl;
               cout << "White Wins This Match!" << endl;
           }


          if(staleMate) {
                gameInStalemate();
           }

           if(blackWins || whiteWins || staleMate) {
	       b.clearBoard();
               return true;
           }
	   return false;
}
void printInstructions(bool beginGame) {
     cout << "Enter a new command: " << endl; 
    cout << "setup: enters setup mode." << endl;
    cout << "game: starts a new game." << endl; 
    cout << "control d: ends the entire game." << endl;
}

void printInstructions() {
	cout << "The match is over." << endl;
    cout << "Enter a new command: " << endl;
    cout << "setup: enters setup mode." << endl;
    cout << "game: starts a new game." << endl;
    cout << "control d: ends the entire game." << endl;
}

void Controller::resign() {
    if((count % 2) == 0) {
        players.at(1)->addscore(1);
        cout << "White resigns. Black wins!" << endl;
    } else {
        players.at(0)->addscore(1);
        cout << "Black resigns. White wins!" << endl;
    }

    initalized = false;
    customSetup = false;

    printInstructions();
    b.clearBoard();
}

void Controller::setup() {
    cout << "Entering SetUp Mode." << endl;
    cout << b;    
    Colour currColour = Colour::White;

  while(true) {
    string cmd;
    string cmd2;
    string coordinates;

    cin >> cmd;
  
    if(cmd == "+") {

      cin >> cmd2;
      PieceType pt = pieceTypes[cmd2];
 
      cin >> coordinates;

      int col = coor[coordinates[0]];
      int row = 8 - (coordinates[1] - 48);
      if(coordinates.length() != 2 || outOfRange(col) || outOfRange(row)) { continue; }

      b.addPiece(pt, currColour,  row, col);  
      b.updateGraphics(row, col);    
      cout << b;
    } else if (cmd == "-") {
        cin >> coordinates;

      int col = coor[coordinates[0]];
      int row = 8 - (coordinates[1] - 48);

      if(coordinates.length() != 2 || outOfRange(col) || outOfRange(row)) { continue; }

      b.removePiece(row,  col);
      b.updateGraphics(row, col);
      cout << b;
    } else if (cmd == "=") { 
      cin >> cmd2;

      if(colours.count(cmd2) == 0) { 
          cout << "Enter a valid colour." << endl;
          continue;
       }

      currColour = colours[cmd2];
         
    } else if(cmd == "done") {
      if(!b.validBoard() || b.willbechecked(Colour::White) || b.willbechecked(Colour::Black)) {
          cout << "The board is not valid." << endl;
      } else {
          break;
      }  

     } 
  }

    customSetup = true;
}

void Controller::gameInStalemate() {
	cout << "Stalemate!" << endl;
        players.at(0)->addscore(0.5);
        players.at(1)->addscore(0.5);

}

int Controller::getScore(Colour colour) const {
    int playersSize = players.size();
    for(int i = 0; i < playersSize; ++i) {
        if(players.at(i)->getColour() == colour) {
            return players.at(i)->getScore();
        }
    }

    return 0;
}

void Controller::addNewPlayer(PlayerType playerType, string playerName, Colour playerColour) {
    players.emplace_back(Player::createPlayer(playerType, playerName, playerColour)); 
}

void Controller::game() {
       string p1Name;
       string p2Name;
       cin >> p1Name;
       cin >> p2Name;

       cout << "What type of player is the first player?" << endl;
       cout << "Type H for Human or Cx for Computer level x" << endl;

       string playerType;
       cin >> playerType;
       
       addNewPlayer(pt[playerType], p1Name, Colour::White);

       cout << "What type of player is the second player?" << endl;
       cout << "Type H for Human or Cx for Computer level x" << endl;

       cin >> playerType;
       addNewPlayer(pt[playerType], p2Name, Colour::Black);       
       initalized = true;
       if(!customSetup) {
	  b.clearBoard();
          b.SetDefaultBoard(); 
       }
       cout << b;

       if(b.stalemate(Colour::White)) {
                gameInStalemate();
      } 
}


void Controller::playGame() {
    printInstructions(true);
    while(true) {
        try {
        string cmd;
        cin >> cmd;
        
       if (cin.eof()) {
          cout << "Final Score:" << endl;
          cout << "White: " << getScore(Colour::White) << endl;
          cout << "Black: " << getScore(Colour::Black) << endl;
	  for (int i = 0; i < players.size(); i++) {
		delete players[i];
	  }
	  b.clearBoard();
          break;
       }  

       if(cmd == "setup") {
           setup();
       }

       else if(cmd == "game" && !initalized) {
           game();   
       }

       else if (cmd == "resign" && initalized) {
           resign();
       }

       else if (cmd == "move" && initalized) {
           move();
       } 

       } catch(InvalidCommand &e) {cout<<"caught"<<endl; continue; }
    }
}
         

    

