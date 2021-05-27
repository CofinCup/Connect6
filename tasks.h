#pragma once
#include <stdio.h>
#include <iostream>
#include <thread>
#include <string>
#include <io.h>
#include <Windows.h>
#include <iomanip>
#include <direct.h>
#include <fstream>
#include <set>
#include "commons.h"
#include "Define.h"
using namespace std;



void database_ini();

void BeginWritingReplay();

void WriteOneStep(int RealChessBoard[19][19], int color);

int evaluateSteps(int ChessBoard[19][19], const int MyColor, const int row1, const int col1, const int row2, const int col2, const int mode);

void ShowStepRoads(int ChessBoard[19][19], const int MyColor, const int row1, const int col1, const int row2, const int col2, const int mode);

void printAccessField(set<pair<int, int>> field);

void analysisHorizon(int row1, int col1, int row2, int col2, int MyColor, const int ChessBoard[19][19], int numberOfMyRoad[7], int numberOfEnemyRoad[7]);

void analysisVertical(int row1, int col1, int row2, int col2, int MyColor, const int ChessBoard[19][19], int numberOfMyRoad[7], int numberOfEnemyRoad[7]);

void analysisRight(int row1, int col1, int row2, int col2, int MyColor, const int ChessBoard[19][19], int numberOfMyRoad[7], int numberOfEnemyRoad[7]);

void analysisLeft(int row1, int col1, int row2, int col2, int MyColor, const int ChessBoard[19][19], int numberOfMyRoad[6], int numberOfEnemyRoad[6]);

int GetScore(int numberOfMyRoad[7], int numberOfEnemyRoad[7], int mode);

int GetMode(int numberOfMyRoad[7], int numberOfEnemyRoad[7]);

void UpdateAccessField(const int ChessBoard[19][19], set<pair<int, int>> Accessfield, int row, int col);

