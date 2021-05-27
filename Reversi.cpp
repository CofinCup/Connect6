#define _CRT_SECURE_NO_WARNINGS
#include "Reversi.h"

#define random(x) (rand()%x)
#define ROWS 19
#define COLS 19
#define ROUNDS 2
#define BLACK 0
#define WHITE 1

int RealChessBoard[19][19];
set<pair<int, int>> accessfield;
int AllMyRoads[7] = { 0 };
int AllEnemyRoads[7] = { 0 };

Reversi::Reversi() {
	client_socket = ClientSocket();
	oppositeColor = ownColor = -1;
}

Reversi::~Reversi() {}
void Reversi::setOwnColor(int color)
{
	if (color != BLACK || color != WHITE)
		return;
	ownColor = color;
	oppositeColor = 1 - color;
}

/*
 send id and password to server by socket
 rtn != 0 represents socket transfer error
 */
void Reversi::authorize(const char* id, const char* pass)
{
	client_socket.connectServer();
	std::cout << "Authorize " << id << std::endl;
	char msgBuf[BUFSIZE];
	memset(msgBuf, 0, BUFSIZE);
	msgBuf[0] = 'A';
	memcpy(&msgBuf[1], id, 9);
	memcpy(&msgBuf[10], pass, 6);
	int rtn = client_socket.sendMsg(msgBuf);
	if (rtn != 0) printf("Authorized Failed!\n");
}

// 用户id输入，服务器上需要有对应的账号密码：对应文件 players-0.txt
void Reversi::gameStart()
{
	char id[12] = { 0 }, passwd[10] = { 0 };
	strcpy(id, ID);
	strcpy(passwd, PASSWORD);
	authorize(id, passwd);

	printf("Game Start!\n");

	for (int round = 0; round < ROUNDS; round++) {
		roundStart(round);
		oneRound();
		roundOver(round);
	}
	gameOver();
	client_socket.close();
}

void Reversi::gameOver()
{
	printf("Game Over!\n");
}

//发一次消息，走哪一步，等两个消息，1.自己的步数行不行 2.对面走了哪一步 
void Reversi::roundStart(int round)
{
	//////////////////////////////////
	accessfield.clear();
	for (int i = 0; i < 19; ++i) {
		for (int j = 0; j < 19; ++j) {
			RealChessBoard[i][j] = 0;
		}
	}
	RealChessBoard[9][9] = 7;
	for (int i = 8; i < 11; ++i) {
		for (int j = 8; j < 11; ++j) {
			accessfield.insert(make_pair(i, j));
		}
	}
	accessfield.erase(make_pair(9, 9));
	for (int i = 0; i < 7; ++i) {
		AllMyRoads[i] = 0;
		AllEnemyRoads[i] = 0;
	}
	//////////////////////////////////
	printf("Round %d Ready Start!\n", round);
	BeginWritingReplay();
	// first time receive msg from server
	int rtn = client_socket.recvMsg();
	if (rtn != 0) return;
	if (strlen(client_socket.getRecvMsg()) < 2)
		printf("Authorize Failed!\n");
	else
		printf("Round start received msg %s\n", client_socket.getRecvMsg());
	switch (client_socket.getRecvMsg()[1]) {
		// this client : black chessman
	case 'B':
		ownColor = BLACK;
		oppositeColor = WHITE;
		rtn = client_socket.sendMsg("BB");
		printf("Send BB -> rtn: %d\n", rtn);
		if (rtn != 0) return;
		break;
	case 'W':
		ownColor = WHITE;
		oppositeColor = BLACK;
		rtn = client_socket.sendMsg("BW");
		printf("Send BW -> rtn: %d\n", rtn);
		if (rtn != 0) return;
		break;
	default:
		printf("Authorized Failed!\n");
		break;
	}
}

void Reversi::oneRound()
{
	//cout << "Round Begin, RealChessBoard[9][9] is:" << (char)(RealChessBoard[9][9] + '0') << endl;
	RealChessBoard[9][9] = 7;
	int STEP = 1;
	switch (ownColor) {
	case 0:
		while (STEP < 10000) {
			//	cout << "Before Step, RealChessBoard[9][9] is:" << (char)(RealChessBoard[9][9] + '0') << endl;

			pair<pair<int, int>, pair<int, int>> chess = step();                        // take action, send message

		//	cout << "before Observe, RealChessBoard[9][9] is:" << (char)(RealChessBoard[9][9] + '0') << endl;

			// lazi only excute after server's message confirm  in observe function
			generateOneStepMessage(chess.first.first, chess.first.second, chess.second.first, chess.second.second);

			if (observe() >= 1) break;     // receive RET Code

			if (observe() >= 1) break;    // see white move
			STEP++;
			// saveChessBoard();
		}
		printf("One Round End\n");
		break;
	case 1:
		while (STEP < 10000) {

			if (observe() >= 1) break;    // see black move

			pair<pair<int, int>, pair<int, int>> chess = step();                        // take action, send message
			// lazi only excute after server's message confirm  in observe function
			generateOneStepMessage(chess.first.first, chess.first.second, chess.second.first, chess.second.second);


			if (observe() >= 1) break;     // receive RET Code
			// saveChessBoard();
			STEP++;
		}
		printf("One Round End\n");
		break;

	default:
		break;
	}
}

void Reversi::roundOver(int round)
{
	printf("Round %d Over!\n", round);
	// reset initializer

	ownColor = oppositeColor = -1;
}

int Reversi::observe()
{
	int rtn = 0;
	int recvrtn = client_socket.recvMsg();
	if (recvrtn != 0) return 1;
	printf("receive msg %s\n", client_socket.getRecvMsg());
	switch (client_socket.getRecvMsg()[0]) {
	case 'R':
	{
		switch (client_socket.getRecvMsg()[1]) {
		case 'Y':   // valid step
			switch (client_socket.getRecvMsg()[2]) {
			case 'P':   // update chessboard
			{
				//	cout << "Before HandleMessage1, RealChessBoard[9][9] is:" << (char)(RealChessBoard[9][9] + '0') << endl;
				int desRow1 = (client_socket.getRecvMsg()[3] - '0') * 10 + client_socket.getRecvMsg()[4] - '0';
				int desCol1 = (client_socket.getRecvMsg()[5] - '0') * 10 + client_socket.getRecvMsg()[6] - '0';
				int desRow2 = (client_socket.getRecvMsg()[7] - '0') * 10 + client_socket.getRecvMsg()[8] - '0';
				int desCol2 = (client_socket.getRecvMsg()[9] - '0') * 10 + client_socket.getRecvMsg()[10] - '0';
				int color = (client_socket.getRecvMsg()[11] - '0');
				//你应该在这里处理desRow和desCol，推荐使用函数
				handleMessage(desRow1, desCol1, desRow2, desCol2, color);

				printf("a valid step of : (%d %d) (%d %d)\n", desRow1, desCol1, desRow2, desCol2);
				break;
			}
			case 'N':   // R0N: enemy wrong step
			{
				//
				printf("a true judgement of no step\n");
				break;
			}
			}

			break;
		case 'W':
			// invalid step
			switch (client_socket.getRecvMsg()[2]) {
			case 'P': {
				int desRow1 = (client_socket.getRecvMsg()[3] - '0') * 10 + client_socket.getRecvMsg()[4] - '0';
				int desCol1 = (client_socket.getRecvMsg()[5] - '0') * 10 + client_socket.getRecvMsg()[6] - '0';
				int desRow2 = (client_socket.getRecvMsg()[7] - '0') * 10 + client_socket.getRecvMsg()[8] - '0';
				int desCol2 = (client_socket.getRecvMsg()[9] - '0') * 10 + client_socket.getRecvMsg()[10] - '0';
				int color = (client_socket.getRecvMsg()[11] - '0');
				printf("Invalid step , server random a true step of : (%d %d) (%d %d)\n", desRow1, desCol1, desRow2, desCol2);
				//你应该在这里处理desRow和desCol，推荐使用函数
				handleMessage(desRow1, desCol1, desRow2, desCol2, color);
				break;
			}
			case 'N': {
				printf("a wrong judgement of no step\n");
				break;
			}
			default:
				break;
			}
			break;
		case '1':

			printf("Error -1: Msg format error!\n");
			rtn = -1;
			break;
		case '2':

			printf("Error -2: Corrdinate error!\n");
			rtn = -2;
			break;
		case '4':

			printf("Error -4: Invalid step!\n");
			rtn = -4;
			break;
		default:

			printf("Error -5: Other error!\n");
			rtn = -5;
			break;
		}
		break;
	}
	case 'E':
	{
		switch (client_socket.getRecvMsg()[1]) {
		case '0':
			// game over
			rtn = 2;
			break;
		case '1':
			// round over
			rtn = 1;
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
	return rtn;
}

void Reversi::generateOneStepMessage(int row1, int col1, int row2, int col2)
{
	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));

	//put row and col in the message
	msg[0] = 'S';
	msg[1] = 'P';
	msg[2] = '0' + row1 / 10;
	msg[3] = '0' + row1 % 10;
	msg[4] = '0' + col1 / 10;
	msg[5] = '0' + col1 % 10;
	msg[6] = '0' + row2 / 10;
	msg[7] = '0' + row2 % 10;
	msg[8] = '0' + col2 / 10;
	msg[9] = '0' + col2 % 10;
	msg[10] = '\0';

	//print
	printf("generate one step at possition (%2d,%2d,%2d,%2d) : %s\n", row1, col1, row2, col2, msg);

	client_socket.sendMsg(msg);
}

/*-------------------------last three function--------------------------------
* step : find a good position to lazi your chess.
* saveChessBoard : save the chess board now.
* handleMessage: handle the message from server.
*/

pair<pair<int, int>, pair<int, int>> Reversi::step()
{
	//TODO:Generate a valid step here
	int mode = GetMode(AllMyRoads, AllEnemyRoads);
	pair<pair<int, int>, pair<int, int>> bestPair;
	pair<int, int> step1, step2;
	int bestScore = 0;
	for (set<pair<int, int>>::iterator it1 = accessfield.begin(); it1 != accessfield.end(); ++it1) {
		step1 = (*it1);
		//复制建立一个新的accessboard。
		set<pair<int, int>>newAccessfield;
		for (set<pair<int, int>>::iterator it = accessfield.begin(); it != accessfield.end(); ++it) {
			newAccessfield.insert((*it));//copy the original field
		}
		//if ((*it1) == make_pair(14, 9)) {
		//	cout << endl;
		//}
		for (int i = (*it1).first - 1; i < (*it1).first + 2; ++i) {
			for (int j = (*it1).second - 1; j < (*it1).second + 2; ++j) {
				//if (i == 15 && j == 10) {
					//cout << RealChessBoard[i][j] << endl;
				//}
				if (i < 0 || j < 0 || i>18 || j>18)
					continue;
				if (RealChessBoard[i][j] == 0)
					newAccessfield.insert(make_pair(i, j));
				else
					continue;

			}
		}
		if (newAccessfield.count(make_pair((*it1).first, (*it1).second)) != 0)
			newAccessfield.erase(make_pair((*it1).first, (*it1).second));
		for (set<pair<int, int>>::iterator it2 = newAccessfield.begin(); it2 != newAccessfield.end(); ++it2) {
			int score = 0;
			//cout << "evaluatesteps has variables:" << (int)(*it1).first <<" "<< (int)(*it1).second <<" "<< (int)(*it2).first <<" "<< (int)(*it2).second << endl;
			score = evaluateSteps(RealChessBoard, ownColor, (*it1).first, (*it1).second, (*it2).first, (*it2).second, mode);

			if (score > bestScore) {
				bestScore = score;
				bestPair.first = *it1;
				bestPair.second = *it2;
			}
		}
	}
	for (int i = (bestPair).first.first - 1; i < (bestPair).first.first + 2; ++i) {
		for (int j = (bestPair).first.second - 1; j < (bestPair).first.second + 2; ++j) {
			if (i < 0 || j < 0 || i>18 || j>18)
				continue;
			if (RealChessBoard[i][j] == 0)
				accessfield.insert(make_pair(i, j));
			else
				continue;
		}
	}
	if (accessfield.count(make_pair((bestPair).first.first, (bestPair).first.second)) != 0)
		accessfield.erase(make_pair((bestPair).first.first, (bestPair).first.second));

	for (int i = (bestPair).second.first - 1; i < (bestPair).second.first + 2; ++i) {
		for (int j = (bestPair).second.second - 1; j < (bestPair).second.second + 2; ++j) {
			if (i < 0 || j < 0 || i>18 || j>18)
				continue;
			if (RealChessBoard[i][j] == 0)
				accessfield.insert(make_pair(i, j));
			else
				continue;
		}
	}
	if (accessfield.count(make_pair((bestPair).second.first, (bestPair).second.second)) != 0)
		accessfield.erase(make_pair((bestPair).second.first, (bestPair).second.second));
	int tempAllMyRoads[7] = { 0 };
	int tempAllEnemyRoads[7] = { 0 };
	//ShowStepRoads(RealChessBoard, ownColor, 9, 7, 9, 7, mode);
	//cout << "\033[33mBestStepIs:(" << bestPair.first.first <<","<< bestPair.first.second <<")("<< bestPair.second.first <<","<< bestPair.second.second << ")\033[0m" << endl;
	//cout << "\033[31mBest Score Is: " << bestScore << endl<<"\033[0m";
	return bestPair;
}



void Reversi::saveChessBoard()
{
	//saveChessBoard is used in the HandleMessageModule.
}

void Reversi::handleMessage(int row1, int col1, int row2, int col2, int color)
{
	cout << "handling message" << endl;
	int Ori_numberOfMyRoad[7] = { 0 };
	int Ori_numberOfEnemyRoad[7] = { 0 };
	int New_numberOfMyRoad[7] = { 0 };
	int New_numberOfEnemyRoad[7] = { 0 };
	//cout << "horizon" << endl;
	analysisHorizon(row1, col1, row2, col2, ownColor, RealChessBoard, Ori_numberOfMyRoad, Ori_numberOfEnemyRoad);
	//cout << "RealChessBoard[9][9] is:" << (char)(RealChessBoard[9][9] + '0') << endl;
	//cout << "vertical" << endl;
	analysisVertical(row1, col1, row2, col2, ownColor, RealChessBoard, Ori_numberOfMyRoad, Ori_numberOfEnemyRoad);
	//cout << "RealChessBoard[9][9] is:" << (char)(RealChessBoard[9][9] + '0') << endl;
	//cout << "left" << endl;
	analysisLeft(row1, col1, row2, col2, ownColor, RealChessBoard, Ori_numberOfMyRoad, Ori_numberOfEnemyRoad);
	//cout << "RealChessBoard[9][9] is:" << (char)(RealChessBoard[9][9] + '0') << endl;
	//cout << "right" << endl;
	analysisRight(row1, col1, row2, col2, ownColor, RealChessBoard, Ori_numberOfMyRoad, Ori_numberOfEnemyRoad);
	//cout << "RealChessBoard[9][9] is:" << (char)(RealChessBoard[9][9] + '0') << endl;
	if (color == BLACK) {
		RealChessBoard[row1][col1] = 1;
		RealChessBoard[row2][col2] = 1;
	}
	else {
		RealChessBoard[row1][col1] = 7;
		RealChessBoard[row2][col2] = 7;
	}
	//cout << "horizon" << endl;
	analysisHorizon(row1, col1, row2, col2, ownColor, RealChessBoard, New_numberOfMyRoad, New_numberOfEnemyRoad);
	//cout << "RealChessBoard[9][9] is:" << (char)(RealChessBoard[9][9] + '0') << endl;
	//cout << "vertical" << endl;
	analysisVertical(row1, col1, row2, col2, ownColor, RealChessBoard, New_numberOfMyRoad, New_numberOfEnemyRoad);
	//cout << "RealChessBoard[9][9] is:" << (char)(RealChessBoard[9][9] + '0') << endl;
	//cout << "left" << endl;
	analysisLeft(row1, col1, row2, col2, ownColor, RealChessBoard, New_numberOfMyRoad, New_numberOfEnemyRoad);
	//cout << "RealChessBoard[9][9] is:" << (char)(RealChessBoard[9][9] + '0') << endl;
	//cout << "right" << endl;
	analysisRight(row1, col1, row2, col2, ownColor, RealChessBoard, New_numberOfMyRoad, New_numberOfEnemyRoad);
	//cout << "RealChessBoard[9][9] is:" << (char)(RealChessBoard[9][9] + '0') << endl;

	//WriteOneStep(RealChessBoard,ownColor);
	for (int i = row1 - 1; i < row1 + 2; ++i) {
		for (int j = col1 - 1; j < col1 + 2; ++j) {
			if (i < 0 || j < 0 || i>18 || j>18)
				continue;
			if (RealChessBoard[i][j] == 0)
				accessfield.insert(make_pair(i, j));
			else
				continue;
		}
	}
	if (accessfield.count(make_pair(row1, col1)) != 0)
		accessfield.erase(make_pair(row1, col1));
	for (int i = row2 - 1; i < row2 + 2; ++i) {
		for (int j = col2 - 1; j < col2 + 2; ++j) {
			if (i < 0 || j < 0 || i>18 || j>18)
				continue;
			if (RealChessBoard[i][j] == 0)
				accessfield.insert(make_pair(i, j));
			else
				continue;
		}
	}
	if (accessfield.count(make_pair(row2, col2)) != 0)
		accessfield.erase(make_pair(row2, col2));
}
