#ifndef Reversi_h
#define Reversi_h
#include <stdio.h>
#include <set>
#include <thread>
#include "ClientSocket.h"
#include "tasks.h"

using namespace std;

class Reversi{
private:
    ClientSocket client_socket;
    int ownColor;
    int oppositeColor;

	//function 

	 // according to chessman position (row , col) , generate one step message in order to send to server
    void generateOneStepMessage(int row1 , int col1, int row2, int col2);

public:
	pair<pair<int, int>, pair<int, int>> step();
    
    void saveChessBoard();

	void handleMessage(int row1, int col1, int row2, int col2, int color);
public:
    Reversi();
    ~Reversi();
	void setOwnColor(int color);

    void authorize(const char *id , const char *pass);
    
    void gameStart();
    
    void gameOver();
    
    void roundStart(int round);
    
    void oneRound();
    
    void roundOver(int round);
    
    int observe();
    
};

#endif /* Reversi_h */
