#include "tasks.h"
#define BLACK 0
#define WHITE 1
#define ATTACK 0
#define DEFEND 1

int AttackScoreLine[2][7] = { 0,1,10,45,550,350,1000000,0,1,10,55,35000,35000,100000 };
int DefendScoreLine[2][7] = { 0,1,10,45,550,350,1000000,0,1,10,55,35000,35000,100000 };
int BaseScoreLine[2][7] = { 0,1,10,45,550,350,1000000, 0,1,10,55,35000,35000,100000 };

//=============================================================
//IO module

void database_ini()
{
	string databasePath = "./database";
	if (0 != _access(databasePath.c_str(), 0)) {
		_mkdir(databasePath.c_str());
	}
}

void BeginWritingReplay()
{
	ofstream board;
	board.open("./database/replay.txt", ios::app);
	if (!board.is_open()) {
		cout << "Undefined fileopen error." << endl;
		return;
	}
	board << endl << "*********GAME*********BEGIN************" << endl;
	board.close();
}

void WriteOneStep(int RealChessBoard[19][19], int color)
{
	ofstream board;
	board.open("./database/replay.txt", ios::app);
	if (!board.is_open()) {
		cout << "Undefined fileopen error." << endl;
		return;
	}
	board << "===========================================================" << endl;
	board << "my Color:";
	if (color == BLACK)
		board << "BLACK" << endl;
	else
		board << "WHITE" << endl;
	for (int i = 0; i < 19; ++i) {
		for (int j = 0; j < 19; ++j) {
			switch (RealChessBoard[i][j])
			{
			case 0:
				board << ".";
				break;
			case 1:
				board << "@";
				break;
			case 7:
				board << "O";
				break;
			default:
				board << "x";
				break;
			}
		}
		board << "\n";
	}
	board.close();
}

int evaluateSteps(int ChessBoard[19][19], const int MyColor, const int row1, const  int col1, const int row2, const  int col2, const int mode)
{
	int Ori_numberOfMyRoad[7] = { 0 };
	int Ori_numberOfEnemyRoad[7] = { 0 };
	int New_numberOfMyRoad[7] = { 0 };
	int New_numberOfEnemyRoad[7] = { 0 };
	analysisHorizon(row1, col1, row2, col2, MyColor, ChessBoard, Ori_numberOfMyRoad, Ori_numberOfEnemyRoad);
	//cout << "At Horizon1, RealChessBoard[9][9] is:" << (char)(ChessBoard[9][9] + '0') << endl;
	analysisVertical(row1, col1, row2, col2, MyColor, ChessBoard, Ori_numberOfMyRoad, Ori_numberOfEnemyRoad);
	//cout << "At Vertical1, RealChessBoard[9][9] is:" << (char)(ChessBoard[9][9] + '0') << endl;
	analysisLeft(row1, col1, row2, col2, MyColor, ChessBoard, Ori_numberOfMyRoad, Ori_numberOfEnemyRoad);
	//cout << "At Left1, RealChessBoard[9][9] is:" << (char)(ChessBoard[9][9] + '0') << endl;
	analysisRight(row1, col1, row2, col2, MyColor, ChessBoard, Ori_numberOfMyRoad, Ori_numberOfEnemyRoad);
	//cout << "At Right1, RealChessBoard[9][9] is:" << (char)(ChessBoard[9][9] + '0') << endl;
	ChessBoard[row1][col1] = (MyColor == BLACK) ? 1 : 7;
	ChessBoard[row2][col2] = (MyColor == BLACK) ? 1 : 7;
	int Ori_Score = GetScore(Ori_numberOfMyRoad, Ori_numberOfEnemyRoad, mode);
	analysisHorizon(row1, col1, row2, col2, MyColor, ChessBoard, New_numberOfMyRoad, New_numberOfEnemyRoad);
	//cout << "At Horizon2, RealChessBoard[9][9] is:" << (char)(ChessBoard[9][9] + '0') << endl;
	analysisVertical(row1, col1, row2, col2, MyColor, ChessBoard, New_numberOfMyRoad, New_numberOfEnemyRoad);
	//cout << "At Vertical2, RealChessBoard[9][9] is:" << (char)(ChessBoard[9][9] + '0') << endl;
	analysisLeft(row1, col1, row2, col2, MyColor, ChessBoard, New_numberOfMyRoad, New_numberOfEnemyRoad);
	//cout << "At Left2, RealChessBoard[9][9] is:" << (char)(ChessBoard[9][9] + '0') << endl;
	analysisRight(row1, col1, row2, col2, MyColor, ChessBoard, New_numberOfMyRoad, New_numberOfEnemyRoad);
	//cout << "At Right2, RealChessBoard[9][9] is:" << (char)(ChessBoard[9][9] + '0') << endl;
	int New_Score = GetScore(New_numberOfMyRoad, New_numberOfEnemyRoad, mode);
	ChessBoard[row1][col1] = 0;
	ChessBoard[row2][col2] = 0;
	return New_Score - Ori_Score;
}

void ShowStepRoads(int ChessBoard[19][19], const int MyColor, const int row1, const  int col1, const int row2, const  int col2, const int mode)
{
	int Ori_numberOfMyRoad[7] = { 0 };
	int Ori_numberOfEnemyRoad[7] = { 0 };
	int New_numberOfMyRoad[7] = { 0 };
	int New_numberOfEnemyRoad[7] = { 0 };
	analysisHorizon(row1, col1, row2, col2, MyColor, ChessBoard, Ori_numberOfMyRoad, Ori_numberOfEnemyRoad);
	//cout << "At Horizon1, RealChessBoard[9][9] is:" << (char)(ChessBoard[9][9] + '0') << endl;
	analysisVertical(row1, col1, row2, col2, MyColor, ChessBoard, Ori_numberOfMyRoad, Ori_numberOfEnemyRoad);
	//cout << "At Vertical1, RealChessBoard[9][9] is:" << (char)(ChessBoard[9][9] + '0') << endl;
	analysisLeft(row1, col1, row2, col2, MyColor, ChessBoard, Ori_numberOfMyRoad, Ori_numberOfEnemyRoad);
	//cout << "At Left1, RealChessBoard[9][9] is:" << (char)(ChessBoard[9][9] + '0') << endl;
	analysisRight(row1, col1, row2, col2, MyColor, ChessBoard, Ori_numberOfMyRoad, Ori_numberOfEnemyRoad);
	//cout << "At Right1, RealChessBoard[9][9] is:" << (char)(ChessBoard[9][9] + '0') << endl;
	ChessBoard[row1][col1] = (MyColor == BLACK) ? 1 : 7;
	ChessBoard[row2][col2] = (MyColor == BLACK) ? 1 : 7;
	int Ori_Score = GetScore(Ori_numberOfMyRoad, Ori_numberOfEnemyRoad, mode);
	analysisHorizon(row1, col1, row2, col2, MyColor, ChessBoard, New_numberOfMyRoad, New_numberOfEnemyRoad);
	//cout << "At Horizon2, RealChessBoard[9][9] is:" << (char)(ChessBoard[9][9] + '0') << endl;
	analysisVertical(row1, col1, row2, col2, MyColor, ChessBoard, New_numberOfMyRoad, New_numberOfEnemyRoad);
	//cout << "At Vertical2, RealChessBoard[9][9] is:" << (char)(ChessBoard[9][9] + '0') << endl;
	analysisLeft(row1, col1, row2, col2, MyColor, ChessBoard, New_numberOfMyRoad, New_numberOfEnemyRoad);
	//cout << "At Left2, RealChessBoard[9][9] is:" << (char)(ChessBoard[9][9] + '0') << endl;
	analysisRight(row1, col1, row2, col2, MyColor, ChessBoard, New_numberOfMyRoad, New_numberOfEnemyRoad);
	//cout << "At Right2, RealChessBoard[9][9] is:" << (char)(ChessBoard[9][9] + '0') << endl;
	int New_Score = GetScore(New_numberOfMyRoad, New_numberOfEnemyRoad, mode);
	ChessBoard[row1][col1] = 0;
	ChessBoard[row2][col2] = 0;
	cout << "\033[;31m";
	cout << "Original my roads number:" << Ori_numberOfMyRoad[1] << " " << Ori_numberOfMyRoad[2] << " " << Ori_numberOfMyRoad[3] << " " << Ori_numberOfMyRoad[4] << " " << Ori_numberOfMyRoad[5] << " " << Ori_numberOfMyRoad[6] << endl;
	cout << "Now my roads number:" << New_numberOfMyRoad[1] << " " << New_numberOfMyRoad[2] << " " << New_numberOfMyRoad[3] << " " << New_numberOfMyRoad[4] << " " << New_numberOfMyRoad[5] << " " << New_numberOfMyRoad[6] << endl;
	cout << "Original enemy roads number:" << Ori_numberOfEnemyRoad[1] << " " << Ori_numberOfEnemyRoad[2] << " " << Ori_numberOfEnemyRoad[3] << " " << Ori_numberOfEnemyRoad[4] << " " << Ori_numberOfEnemyRoad[5] << " " << Ori_numberOfEnemyRoad[6] << endl;
	cout << "enemy roads number:" << New_numberOfEnemyRoad[1] << " " << New_numberOfEnemyRoad[2] << " " << New_numberOfEnemyRoad[3] << " " << New_numberOfEnemyRoad[4] << " " << New_numberOfEnemyRoad[5] << " " << New_numberOfEnemyRoad[6] << endl;
	cout << "\033[;0m" << endl;
}

void printAccessField(set<pair<int, int>> field)
{
	cout << "AccessField:";
	for (set<pair<int, int>>::iterator it = field.begin(); it != field.end(); ++it) {
		cout << "(" << (*it).first << "," << (*it).second << ")";
	}
	cout << endl;

}

void analysisHorizon(const int row1, const int col1, const int row2, const int col2, const int MyColor, const int ChessBoard[19][19], int numberOfMyRoad[7], int numberOfEnemyRoad[7])
{
	//cout << "IN Horizon2 Begin, RealChessBoard[9][9] is:" << (char)(ChessBoard[9][9] + '0') << endl;
	for (int i = col1 - 5 > 0 ? col1 - 5 : 0; i <= col1 && i + 5 < 19; ++i) {
		int number = ChessBoard[row1][i] + ChessBoard[row1][i + 1] + ChessBoard[row1][i + 2] + ChessBoard[row1][i + 3]
			+ ChessBoard[row1][i + 4] + ChessBoard[row1][i + 5];
		if (number == 0 || (number > 6 && number % 7 != 0)) {
			continue;
		}
		if (number < 7) {
			if (MyColor == BLACK) {
				numberOfMyRoad[number]++;
			}
			else {
				numberOfEnemyRoad[number]++;
			}
		}
		else {
			if (MyColor == BLACK) {
				numberOfEnemyRoad[number / 7]++;
			}
			else {
				numberOfMyRoad[number / 7]++;
			}
		}
	}
	//cout << "IN Horizon2 Mid, RealChessBoard[9][9] is:" << (char)(ChessBoard[9][9] + '0') << endl;
	for (int i = col2 - 5 > 0 ? col2 - 5 : 0; i <= col2 && i + 5 < 19; ++i) {
		int xx = col1;
		if (i == xx || i + 1 == xx || i + 2 == xx || i + 3 == xx || i + 4 == xx || i + 5 == xx) {
			continue;
		}
		int number = ChessBoard[row2][i] + ChessBoard[row2][i + 1] + ChessBoard[row2][i + 2] + ChessBoard[row2][i + 3]
			+ ChessBoard[row2][i + 4] + ChessBoard[row2][i + 5];
		if (number == 0 || (number > 6 && number % 7 != 0)) {
			continue;
		}
		if (number < 7) {
			if (MyColor == BLACK) {
				numberOfMyRoad[number]++;
			}
			else {
				numberOfEnemyRoad[number]++;
			}
		}
		else {
			if (MyColor == BLACK) {
				numberOfEnemyRoad[number / 7]++;
			}
			else {
				numberOfMyRoad[number / 7]++;
			}
		}
	}
	//cout << "IN Horizon2 End, RealChessBoard[9][9] is:" << (char)(ChessBoard[9][9] + '0') << endl;
	return;
}

void analysisVertical(const int row1, const int col1, const int row2, const int col2, const int MyColor, const int ChessBoard[19][19], int numberOfMyRoad[7], int numberOfEnemyRoad[7])
{
	for (int i = row1 - 5 > 0 ? row1 - 5 : 0; i <= row1 && i + 5 < 19; ++i) {
		int number = ChessBoard[i][col1] + ChessBoard[i + 1][col1] + ChessBoard[i + 2][col1] + ChessBoard[i + 3][col1]
			+ ChessBoard[i + 4][col1] + ChessBoard[i + 5][col1];
		if (number == 0 || (number > 6 && number % 7 != 0)) {
			continue;
		}
		if (number < 7) {
			if (MyColor == BLACK) {
				numberOfMyRoad[number]++;
			}
			else {
				numberOfEnemyRoad[number]++;
			}
		}
		else {
			if (MyColor == BLACK) {
				numberOfEnemyRoad[number / 7]++;
			}
			else {
				numberOfMyRoad[number / 7]++;
			}
		}
	}
	for (int i = row2 - 5 > 0 ? row2 - 5 : 0; i <= row2 && i + 5 < 19; ++i) {
		int xx = row1;
		if (i == xx || i + 1 == xx || i + 2 == xx || i + 3 == xx || i + 4 == xx || i + 5 == xx) {
			continue;
		}
		int number = ChessBoard[i][col2] + ChessBoard[i + 1][col2] + ChessBoard[i + 2][col2] + ChessBoard[i + 3][col2]
			+ ChessBoard[i + 4][col2] + ChessBoard[i + 5][col2];
		if (number == 0 || (number > 6 && number % 7 != 0)) {
			continue;
		}
		if (number < 7) {
			if (MyColor == BLACK) {
				numberOfMyRoad[number]++;
			}
			else {
				numberOfEnemyRoad[number]++;
			}
		}
		else {
			if (MyColor == BLACK) {
				numberOfEnemyRoad[number / 7]++;
			}
			else {
				numberOfMyRoad[number / 7]++;
			}
		}
	}
	return;
}

void analysisRight(const int row1, const int col1, const int row2, const int col2, const int MyColor, const int ChessBoard[19][19], int numberOfMyRoad[7], int numberOfEnemyRoad[7])
// 就是“\”这种方向的路计数。
{
	int begin1 = myMax(myMax(-row1, -col1), -5);
	int begin2 = myMax(myMax(-row2, -col2), -5);
	int end1 = myMin(myMin(13 - row1, 13 - col1), 0);
	int end2 = myMin(myMin(13 - row2, 13 - col2), 0);

	for (int i = begin1; i <= end1; ++i) {
		int number = 0;
		number = ChessBoard[row1 + i][col1 + i] + ChessBoard[row1 + i + 1][col1 + i + 1] + ChessBoard[row1 + i + 2][col1 + i + 2]
			+ ChessBoard[row1 + i + 3][col1 + i + 3] + ChessBoard[row1 + i + 4][col1 + i + 4] + ChessBoard[row1 + i + 5][col1 + i + 5];
		if (number == 0 || (number > 6 && number % 7 != 0)) {
			continue;
		}
		if (number < 7) {
			if (MyColor == BLACK) {
				numberOfMyRoad[number]++;
			}
			else {
				numberOfEnemyRoad[number]++;
			}
		}
		else {
			if (MyColor == BLACK) {
				numberOfEnemyRoad[number / 7]++;
			}
			else {
				numberOfMyRoad[number / 7]++;
			}
		}
	}
	if (row2 - col2 == row1 - col1) {//若在同一对角线，进入可跳转形式。
		for (int i = begin2; i <= end2; ++i) {
			if (row1 + begin1 <= row2 + i && row2 + i <= row1 + end1)
				continue;
			int number = ChessBoard[row2 + i][col2 + i] + ChessBoard[row2 + i + 1][col2 + i + 1] + ChessBoard[row2 + i + 2][col2 + i + 2]
				+ ChessBoard[row2 + i + 3][col2 + i + 3] + ChessBoard[row2 + i + 4][col2 + i + 4] + ChessBoard[row2 + i + 5][col2 + i + 5];
			if (number == 0 || (number > 6 && number % 7 != 0)) {
				continue;
			}
			if (number < 7) {
				if (MyColor == BLACK) {
					numberOfMyRoad[number]++;
				}
				else {
					numberOfEnemyRoad[number]++;
				}
			}
			else {
				if (MyColor == BLACK) {
					numberOfEnemyRoad[number / 7]++;
				}
				else {
					numberOfMyRoad[number / 7]++;
				}
			}
		}
	}
	else {
		for (int i = begin2; i <= end2; ++i) {
			int number = ChessBoard[row2 + i][col2 + i] + ChessBoard[row2 + i + 1][col2 + i + 1] + ChessBoard[row2 + i + 2][col2 + i + 2]
				+ ChessBoard[row2 + i + 3][col2 + i + 3] + ChessBoard[row2 + i + 4][col2 + i + 4] + ChessBoard[row2 + i + 5][col2 + i + 5];
			if (number == 0 || (number > 6 && number % 7 != 0)) {
				continue;
			}
			if (number < 7) {
				if (MyColor == BLACK) {
					numberOfMyRoad[number]++;
				}
				else {
					numberOfEnemyRoad[number]++;
				}
			}
			else {
				if (MyColor == BLACK) {
					numberOfEnemyRoad[number / 7]++;
				}
				else {
					numberOfMyRoad[number / 7]++;
				}
			}
		}
	}
}


void analysisLeft(const int row1, const int col1, const int row2, const int col2, const int MyColor, const int ChessBoard[19][19], int numberOfMyRoad[6], int numberOfEnemyRoad[6])
//就是/方向的计数。
{
	//cout << "\033[31mnow point:(" << row1 << "," << col1 << ")&(" << row2 << "," << col2 << ")\033[0m" << endl;
	int begin1 = myMax(myMax(col1 - 18, -row1), -5);
	int end1 = myMin(myMin(13 - row1, col1 - 5), 0);
	int begin2 = myMax(myMax(col2 - 18, -row2), -5);
	int end2 = myMin(myMin(13 - row2, col2 - 5), 0);

	for (int i = begin1; i <= end1; ++i) {
		int number = ChessBoard[row1 + i][col1 - i] + ChessBoard[row1 + i + 1][col1 - i - 1] + ChessBoard[row1 + i + 2][col1 - i - 2]
			+ ChessBoard[row1 + i + 3][col1 - i - 3] + ChessBoard[row1 + i + 4][col1 - i - 4] + ChessBoard[row1 + i + 5][col1 - i - 5];
		if (number == 0 || (number > 6 && number % 7 != 0)) {
			continue;
		}
		if (number < 7) {
			if (MyColor == BLACK) {
				numberOfMyRoad[number]++;
			}
			else {
				numberOfEnemyRoad[number]++;
			}
		}
		else {
			if (MyColor == BLACK) {
				numberOfEnemyRoad[number / 7]++;
			}
			else {
				numberOfMyRoad[number / 7]++;
			}
		}
	}
	if (row2 - col2 == row1 - col1) {//若在同一对角线，进入可跳转形式。
		for (int i = begin2; i <= end2; ++i) {
			if (row1 + begin1 <= row2 + i && row2 + i <= row1 + end1)
				continue;
			int number = ChessBoard[row2 + i][col2 - i] + ChessBoard[row2 + i + 1][col2 - i - 1] + ChessBoard[row2 + i + 2][col2 - i - 2]
				+ ChessBoard[row2 + i + 3][col2 - i - 3] + ChessBoard[row2 + i + 4][col2 - i - 4] + ChessBoard[row2 + i + 5][col2 - i - 5];
			if (number == 0 || (number > 6 && number % 7 != 0)) {
				continue;
			}
			if (number < 7) {
				if (MyColor == BLACK) {
					numberOfMyRoad[number]++;
				}
				else {
					numberOfEnemyRoad[number]++;
				}
			}
			else {
				if (MyColor == BLACK) {
					numberOfEnemyRoad[number / 7]++;
				}
				else {
					numberOfMyRoad[number / 7]++;
				}
			}
		}
	}
	else {
		for (int i = begin2; i <= end2; ++i) {
			int number = ChessBoard[row2 + i][col2 - i] + ChessBoard[row2 + i + 1][col2 - i - 1] + ChessBoard[row2 + i + 2][col2 - i - 2]
				+ ChessBoard[row2 + i + 3][col2 - i - 3] + ChessBoard[row2 + i + 4][col2 - i - 4] + ChessBoard[row2 + i + 5][col2 - i - 5];
			if (number == 0 || (number > 6 && number % 7 != 0)) {
				continue;
			}
			if (number < 7) {
				if (MyColor == BLACK) {
					numberOfMyRoad[number]++;
				}
				else {
					numberOfEnemyRoad[number]++;
				}
			}
			else {
				if (MyColor == BLACK) {
					numberOfEnemyRoad[number / 7]++;
				}
				else {
					numberOfMyRoad[number / 7]++;
				}
			}
		}
	}
}

int GetScore(int numberOfMyRoad[7], int numberOfEnemyRoad[7], int mode)
{
	//if (numberOfEnemyRoad[4] != 0)
		//cout << "\033[31mFind 4 Road\033[0m" << endl;
	if (numberOfMyRoad[6] != 0)
		return 99999999;

	int score = 0;
	if (mode == ATTACK)
		for (int i = 1; i < 7; ++i) {
			score += numberOfMyRoad[i] * AttackScoreLine[0][i];
			score -= numberOfEnemyRoad[i] * AttackScoreLine[1][i];
		}
	else
		for (int i = 1; i < 7; ++i) {
			score += numberOfMyRoad[i] * DefendScoreLine[0][i];
			score -= numberOfEnemyRoad[i] * DefendScoreLine[1][i];
		}
	return score;
}

int GetMode(int numberOfMyRoad[7], int numberOfEnemyRoad[7])
{
	int score = 0;
	for (int i = 1; i < 7; ++i) {
		score += numberOfMyRoad[i] * BaseScoreLine[0][i];
		score -= numberOfEnemyRoad[i] * BaseScoreLine[1][i];
	}
	if (score >= 0)
		return ATTACK;
	else
		return DEFEND;
}

void UpdateAccessField(const int ChessBoard[19][19], set<pair<int, int>> Accessfield, int row, int col)
{
	for (int i = row - 2; i < row + 3; ++i) {
		for (int j = col - 2; j < col + 3; ++j) {
			if (i < 0 || j < 0 || i>18 || j>18)
				continue;
			if (ChessBoard[i][j] == 0)
				Accessfield.insert(make_pair(i, j));
			else
				continue;
		}
	}
	if (Accessfield.count(make_pair(row, col)) != 0)
		Accessfield.erase(make_pair(row, col));
	printAccessField(Accessfield);
}

void UpdateRoad_Horizon(const int ChessBoard[19][19], int AllMyRoads[7], int AllEnemyRoads[7], int MyColor)
{
	for (int row = 0; row < 19; ++row) {
		for (int col = 0; col < 14; ++col) {
			int number = ChessBoard[row][col] + ChessBoard[row][col + 1] + ChessBoard[row][col + 2] + ChessBoard[row][col + 3]
				+ ChessBoard[row][col + 4] + ChessBoard[row][col + 5];
			if (number == 0 || (number > 6 && number % 7 != 0)) {
				continue;
			}
			if (number < 7) {
				if (MyColor == BLACK) {
					AllMyRoads[number]++;
				}
				else {
					AllEnemyRoads[number]++;
				}
			}
			else {
				if (MyColor == BLACK) {
					AllEnemyRoads[number / 7]++;
				}
				else {
					AllMyRoads[number / 7]++;
				}
			}
		}
	}
}

void UpdateRoad_Vertical(const int ChessBoard[19][19], int AllMyRoads[7], int AllEnemyRoads[7], int MyColor)
{
	for (int col = 0; col < 19; ++col) {
		for (int row = 0; row < 14; ++row) {
			int number = ChessBoard[row][col] + ChessBoard[row + 1][col] + ChessBoard[row + 2][col] + ChessBoard[row + 3][col]
				+ ChessBoard[row + 4][col] + ChessBoard[row + 5][col];
			if (number == 0 || (number > 6 && number % 7 != 0)) {
				continue;
			}
			if (number < 7) {
				if (MyColor == BLACK) {
					AllMyRoads[number]++;
				}
				else {
					AllEnemyRoads[number]++;
				}
			}
			else {
				if (MyColor == BLACK) {
					AllEnemyRoads[number / 7]++;
				}
				else {
					AllMyRoads[number / 7]++;
				}
			}

		}
	}
}

void UpdateRoad_Right(const int ChessBoard[19][19], int AllMyRoads[7], int AllEnemyRoads[7], int MyColor)
{
	for (int row = 0; row < 14; ++row) {
		for (int col = 0; col < 14; ++col) {
			int number = 0;
			number = ChessBoard[row][col] + ChessBoard[row + 1][col + 1] + ChessBoard[row + 2][col + 2]
				+ ChessBoard[row + 3][col + 3] + ChessBoard[row + 4][col + 4] + ChessBoard[row + 5][col + 5];
			if (number == 0 || (number > 6 && number % 7 != 0)) {
				continue;
			}
			if (number < 7) {
				if (MyColor == BLACK) {
					AllMyRoads[number]++;
				}
				else {
					AllEnemyRoads[number]++;
				}
			}
			else {
				if (MyColor == BLACK) {
					AllEnemyRoads[number / 7]++;
				}
				else {
					AllMyRoads[number / 7]++;
				}
			}
		}
	}
}

void UpdateRoad_Left(const int ChessBoard[19][19], int AllMyRoads[7], int AllEnemyRoads[7], int MyColor)
{
	for (int row = 0; row < 14; ++row) {
		for (int col = 5; col < 19; ++col) {
			int number = 0;
			number = ChessBoard[row][col] + ChessBoard[row + 1][col - 1] + ChessBoard[row + 2][col - 2]
				+ ChessBoard[row + 3][col - 3] + ChessBoard[row + 4][col - 4] + ChessBoard[row + 5][col - 5];
			if (number == 0 || (number > 6 && number % 7 != 0)) {
				continue;
			}
			if (number < 7) {
				if (MyColor == BLACK) {
					AllMyRoads[number]++;
				}
				else {
					AllEnemyRoads[number]++;
				}
			}
			else {
				if (MyColor == BLACK) {
					AllEnemyRoads[number / 7]++;
				}
				else {
					AllMyRoads[number / 7]++;
				}
			}
		}
	}
}

void UpdateRoad(const int ChessBoard[19][19], int AllMyRoads[7], int AllEnemyRoads[7], int MyColor)
{
	for (int i = 0; i < 7; ++i) {
		AllEnemyRoads[i] = 0;
		AllMyRoads[i] = 0;
	}
	UpdateRoad_Horizon(ChessBoard, AllMyRoads, AllEnemyRoads, MyColor);
	UpdateRoad_Vertical(ChessBoard, AllMyRoads, AllEnemyRoads, MyColor);
	UpdateRoad_Left(ChessBoard, AllMyRoads, AllEnemyRoads, MyColor);
	UpdateRoad_Right(ChessBoard, AllMyRoads, AllEnemyRoads, MyColor);
}