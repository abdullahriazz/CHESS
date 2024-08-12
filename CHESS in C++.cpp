#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <Windows.h>
using namespace std;

int turnCount = 0;

const int rows = 8;
const int columns = 8;

void printBoard(char board[rows][columns]);
void movePiece(char board[rows][columns]);
int capture(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end);

int boundCheck_initial(char board[rows][columns], string move1);
int boundCheck_end(char board[rows][columns], string move2);
int presenceCheck(char board[rows][columns], int rowMove_initial, int colMove_initial);
int turnCheck(char board[rows][columns], int rowMove_initial, int colMove_initial, int rowMove_end, int colMove_end, int turnCount);

int bishop(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end, int turnCount);
int king(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end, int turnCount);
int rook(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end, int turnCount);
int queen(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end, int turnCount);
int pawn(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end, int turnCount);
int knight(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end, int turnCount);

char pawnPromotion(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end);
int castling(char board[rows][columns], int rowMove_initial, int colMove_initial, int rowMove_end, int colMove_end, int moveCount, int turnCount);
int enpassant(char board[rows][columns], int rowMove_initial, int colMove_initial, int rowMove_end, int colMove_end);
bool whitecheck(char board[rows][columns], int rowMove_initial, int colMove_initial, int rowMove_end, int colMove_end);
bool blackcheck(char board[rows][columns], int rowMove_initial, int colMove_initial, int rowMove_end, int colMove_end);
bool checkCoords(char board[rows][columns], int x, int y);

bool validBishop(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end);
bool validKing(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end);
bool validKnight(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end);
bool validRook(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end);
bool validQueen(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end);
bool validPawn(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end);

int main()
{
	ofstream movesout;
	ifstream movesin;
	ofstream modeout;
	ifstream modein;
	ofstream turnout;
	ifstream turnin;

	int count = 0;
	bool checkMate = 0, again = false;
	char handShake = 'n', w_forfeit = 'n', b_forfeit = 'n', save_exit = 'n';
	time_t w_start, w_end, b_start, b_end, w_duration = 0, b_duration = 0, wR_timeLeft = 600, bR_timeLeft = 600, wB_timeLeft = 300, bB_timeLeft = 300;
	int gameMode, start, saveSlot, loadSlot;
	char escButton;

	char board[rows][columns] =
	{
		{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
		{'p', 'p', 'p', 'p', 'p', 'p', 'p', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{'P', 'P', 'P', 'P', 'P', 'P', ' ', 'p'},
		{'R', 'N', 'B', 'Q', 'K', ' ', ' ', ' '},
	};

	system("Color F0");
	cout << endl << "                                             1. NEW GAME" << endl;
	cout << endl << "                                             2. LOAD GAME" << endl;
	cin >> start;
	system("CLS");

	if (start == 1)
	{
		cout << endl << " SELECT GAME MODE" << endl;
		cout << "\n 1. Classic \n 2. Rapid \n 3. Blitz \n 4. Lightening \n \n ";
		cin >> gameMode;
		cout << endl;
		system("CLS");
	}
	else if (start == 2)
	{
		cout << "LOAD FROM SLOT 1 | 2 | 3: " << endl;
		cin >> loadSlot;

		if (loadSlot == 1)
		{
			movesin.open("slot 1.txt");
			modein.open("slot1_mode.txt");
			turnin.open("turn1.txt");
		}
		else if (loadSlot == 2)
		{
			movesin.open("slot 2.txt");
			modein.open("slot2_mode.txt");
			turnin.open("turn2.txt");
		}
		else
		{
			movesin.open("slot 3.txt");
			modein.open("slot3_mode.txt");
			turnin.open("turn3.txt");
		}

		turnin >> turnCount;
		turnin.close();

		modein >> gameMode;
		modein.close();

		for (int r = 0; r < 8; r++)
		{
			for (int c = 0; c < 8; c++)
			{
				movesin.get(board[r][c]);
			}
		}

		movesin.close();
		cout << endl << "GAME LOADED" << endl;

		turnCount++;
	}
	system("CLS");
	printBoard(board);

	if (gameMode == 1 || gameMode == 2 || gameMode == 3)
	{
		while (checkMate == 0 && (handShake == 'n' || handShake == 'N') && (w_forfeit == 'n' || w_forfeit == 'N') && (b_forfeit == 'n' || b_forfeit == 'N'))
		{
			if (gameMode == 1)
			{
				cout << endl << "CLASSIC";
			}
			else if (gameMode == 2)
			{
				cout << endl << "RAPID";
			}
			else if (gameMode == 3)
			{
				cout << endl << "BLITZ";
			}

			if (turnCount % 2 == 0)
			{
				cout << endl;
				cout << endl << "WHITE's turn: " << endl;

				if (gameMode == 1)
				{
					cout << endl << "Total time taken by WHITE: " << w_duration << " seconds " << endl;
				}
				else if (gameMode == 2)
				{
					cout << endl << "Time Left for WHITE: " << wR_timeLeft << " seconds " << endl;
				}
				else if (gameMode == 3)
				{
					cout << endl << "Time Left for WHITE: " << wB_timeLeft << " seconds " << endl;
				}

				cout << endl << "SAVE & EXIT (Y/N): ";
				cin >> save_exit;

				if (save_exit == 'y' || save_exit == 'Y')
				{
					cout << "SAVE IN SLOT 1 | 2 | 3: " << endl;
					cin >> saveSlot;

					if (saveSlot == 1)
					{
						movesout.open("slot 1.txt");
						modeout.open("slot1_mode.txt");
						turnout.open("turn1.txt");
					}
					else if (saveSlot == 2)
					{
						movesout.open("slot 2.txt");
						modeout.open("slot2_mode.txt");
						turnout.open("turn2.txt");
					}
					else
					{
						movesout.open("slot 3.txt");
						modeout.open("slot3_mode.txt");
						turnout.open("turn3.txt");
					}

					if (movesout.is_open() && modeout.is_open())
					{
						turnout << turnCount;
						turnout.close();

						modeout << gameMode;
						modeout.close();

						for (int r = 0; r < 8; r++)
						{
							for (int c = 0; c < 8; c++)
							{
								movesout << board[r][c];
							}
						}

						movesout.close();
						cout << endl << "EXITED" << endl;
						break;
					}
					else
					{
						cout << "File not open";
					}
				}

				cout << endl << "HANDSHAKE (Y/N): ";
				cin >> handShake;
				while (handShake != 'n' && handShake != 'N' && handShake != 'Y' && handShake != 'y')
				{

					cout << "Invalid Input! Enter again: ";
					cin >> handShake;

				}
				if (handShake == 'Y' || handShake == 'y')
				{
					bool ch;
					cout << "does black want to draw? (1/0)";
					cin >> ch;
					if (ch == 1)
					{
						cout << endl << "DRAW!" << endl;
						break;
					}
				}

				cout << endl << "FORFEIT (Y/N): ";
				cin >> w_forfeit;
				while (w_forfeit != 'n' && w_forfeit != 'N' && w_forfeit != 'Y' && w_forfeit != 'y')
				{

					cout << "Invalid Input! Enter again: ";
					cin >> w_forfeit;

				}
				if (w_forfeit == 'Y' || w_forfeit == 'y')
				{
					cout << endl << "BLACK WINS!" << endl;
					break;
				}

				if (gameMode == 1)
				{
					time(&w_start);
					movePiece(board);
					time(&w_end);
					w_duration = w_duration + (w_end - w_start);
				}
				else if (gameMode == 2)
				{
					time(&w_start);
					movePiece(board);
					time(&w_end);
					wR_timeLeft = (wR_timeLeft - (w_end - w_start)) + 10;

					if (wR_timeLeft <= 0)
					{
						cout << endl << "Time finished for move. BLACK WINS!" << endl;
						break;
					}
				}
				else if (gameMode == 3)
				{
					time(&w_start);
					movePiece(board);
					time(&w_end);
					wB_timeLeft = wB_timeLeft - (w_end - w_start);

					if (wB_timeLeft <= 0)
					{
						cout << endl << "Time finished for move. BLACK WINS!" << endl;
						break;
					}
				}

				system("CLS");
				printBoard(board);
			}
			/////////////////////////////////////
			else
			{
				cout << endl;
				cout << endl << "BLACK's turn: " << endl;

				if (gameMode == 1)
				{
					cout << endl << "Total time taken by BLACK: " << b_duration << " seconds " << endl;
				}
				else if (gameMode == 2)
				{
					cout << endl << "Time Left for BLACK: " << bR_timeLeft << " seconds " << endl;
				}
				else if (gameMode == 3)
				{
					cout << endl << "Time Left for BLACK: " << bB_timeLeft << " seconds " << endl;
				}

				cout << endl << "SAVE & EXIT (Y/N): ";
				cin >> save_exit;

				if (save_exit == 'y' || save_exit == 'Y')
				{
					cout << "SAVE IN SLOT 1 | 2 | 3: " << endl;
					cin >> saveSlot;

					if (saveSlot == 1)
					{
						movesout.open("slot 1.txt");
						modeout.open("slot1_mode.txt");
					}
					else if (saveSlot == 2)
					{
						movesout.open("slot 2.txt");
						modeout.open("slot2_mode.txt");
					}
					else
					{
						movesout.open("slot 3.txt");
						modeout.open("slot3_mode.txt");
					}

					if (movesout.is_open() && modeout.is_open())
					{
						modeout << gameMode;
						modeout.close();

						for (int r = 0; r < 8; r++)
						{
							for (int c = 0; c < 8; c++)
							{
								movesout << board[r][c];
							}
						}

						movesout.close();
						cout << endl << "EXITED" << endl;
						break;
					}
					else
					{
						cout << "File not open";
					}
				}

				cout << endl << "HANDSHAKE (Y/N): ";
				cin >> handShake;
				while (handShake != 'n' && handShake != 'N' && handShake != 'Y' && handShake != 'y')
				{

					cout << "Invalid Input! Enter again: ";
					cin >> handShake;

				}
				if (handShake == 'Y' || handShake == 'y')
				{
					bool ch;
					cout << "does white want to draw? (1/0)";
					cin >> ch;
					if (ch == 1)
					{
						cout << endl << "DRAW!" << endl;
						break;
					}
				}

				cout << endl << "FORFEIT (Y/N): ";
				cin >> b_forfeit;
				while (b_forfeit != 'n' && b_forfeit != 'N' && b_forfeit != 'Y' && b_forfeit != 'y')
				{

					cout << "Invalid Input! Enter again: ";
					cin >> b_forfeit;

				}
				if (b_forfeit == 'Y' || b_forfeit == 'y')
				{
					cout << endl << "WHITE WINS!" << endl;
					break;
				}

				if (gameMode == 1)
				{
					time(&b_start);
					movePiece(board);
					time(&b_end);
					b_duration = b_duration + (b_end - b_start);
				}
				else if (gameMode == 2)
				{
					time(&b_start);
					movePiece(board);
					time(&b_end);
					bR_timeLeft = (bR_timeLeft - (b_end - b_start)) + 10;

					if (bR_timeLeft <= 0)
					{
						cout << endl << "Time finished for move. BLACK WINS!" << endl;
						break;
					}
				}
				else if (gameMode == 3)
				{
					time(&b_start);
					movePiece(board);
					time(&b_end);
					bB_timeLeft = bB_timeLeft - (b_end - b_start);

					if (bB_timeLeft <= 0)
					{
						cout << endl << "Time finished for move. BLACK WINS!" << endl;
						break;
					}
				}
				system("CLS");
				printBoard(board);
			}
		}

		if (gameMode == 1)
		{
			if (handShake == 'Y' || handShake == 'y')
			{
				if (w_duration < b_duration)
				{
					cout << endl << "WHITE HAS THE ADVANTAGE" << endl;
				}
				else
				{
					cout << endl << "BLACK HAS THE ADVANTAGE" << endl;
				}
			}
		}
	}
	else if (gameMode == 4)
	{
		while (checkMate == 0)
		{
			cout << endl << "LIGHTNING";

			if (turnCount == 0)
			{

				time_t wL_timeLeft = 15, bL_timeLeft = 15;
				cout << endl;
				cout << endl << "WHITE's turn: " << endl;

				cout << endl << "Time Left for this Move: " << wL_timeLeft << " seconds " << endl;

				cout << endl << "SAVE & EXIT (Y/N): ";
				cin >> save_exit;

				if (save_exit == 'y' || save_exit == 'Y')
				{
					cout << "SAVE IN SLOT 1 | 2 | 3: " << endl;
					cin >> saveSlot;

					if (saveSlot == 1)
					{
						movesout.open("slot 1.txt");
						modeout.open("slot1_mode.txt");
					}
					else if (saveSlot == 2)
					{
						movesout.open("slot 2.txt");
						modeout.open("slot2_mode.txt");
					}
					else
					{
						movesout.open("slot 3.txt");
						modeout.open("slot3_mode.txt");
					}

					if (movesout.is_open() && modeout.is_open())
					{
						modeout << gameMode;
						modeout.close();

						for (int r = 0; r < 8; r++)
						{
							for (int c = 0; c < 8; c++)
							{
								movesout << board[r][c];
							}
						}

						movesout.close();
						cout << endl << "EXITED" << endl;
						break;
					}
					else
					{
						cout << "File not open";
					}
				}

				time(&w_start);
				movePiece(board);
				time(&w_end);
				wL_timeLeft = wL_timeLeft - (w_end - w_start);

				if (wL_timeLeft <= 0)
				{
					cout << endl << "Time finished for move. BLACK WINS!" << endl;
					break;
				}


				system("CLS");
				printBoard(board);
			}
			/////////////////////////////////////
			else
			{
				time_t wL_timeLeft = 15, bL_timeLeft = 15;
				cout << endl;
				cout << endl << "BLACK's turn: " << endl;

				cout << endl << "Time Left for this Move: " << bL_timeLeft << " seconds " << endl;

				cout << endl << "SAVE & EXIT (Y/N): ";
				cin >> save_exit;

				if (save_exit == 'y' || save_exit == 'Y')
				{
					cout << "SAVE IN SLOT 1 | 2 | 3: " << endl;
					cin >> saveSlot;

					if (saveSlot == 1)
					{
						movesout.open("slot 1.txt");
						modeout.open("slot1_mode.txt");
					}
					else if (saveSlot == 2)
					{
						movesout.open("slot 2.txt");
						modeout.open("slot2_mode.txt");
					}
					else
					{
						movesout.open("slot 3.txt");
						modeout.open("slot3_mode.txt");
					}

					if (movesout.is_open() && modeout.is_open())
					{
						modeout << gameMode;
						modeout.close();

						for (int r = 0; r < 8; r++)
						{
							for (int c = 0; c < 8; c++)
							{
								movesout << board[r][c];
							}
						}

						movesout.close();
						cout << endl << "EXITED" << endl;
						break;
					}
					else
					{
						cout << "File not open";
					}
				}

				time(&b_start);
				movePiece(board);
				time(&b_end);
				bL_timeLeft = bL_timeLeft - (b_end - b_start);

				if (bL_timeLeft <= 0)
				{
					cout << endl << "Time finished for move. WHITE WINS!" << endl;
					break;
				}
				system("CLS");
				printBoard(board);
			}
		}
	}
}		
	

void printBoard(char board[rows][columns])
{
	int sideNum = 8;

	cout << endl;
	cout << "         a       b       c       d       e       f       g       h " << endl;
	cout << "     +---------------------------------------------------------------+" << endl;

	for (int row = 0; row < 8; row++)
	{
		cout << "     |       |       |       |       |       |       |       |       |" << endl;
		cout << " " << sideNum << "   |  ";

		for (int col = 0; col < 8; col++)
		{
			cout << " " << board[row][col] << "  " << " |  ";
		}

		cout << sideNum;
		cout << endl;

		cout << "     |       |       |       |       |       |       |       |       |" << endl;
		cout << "     +---------------------------------------------------------------+" << endl;

		sideNum--;
	}

	cout << "         a       b       c       d       e       f       g       h " << endl;
}

void movePiece(char board[rows][columns])
{
	bool move_valid = 0;

	string initialMove, endMove;
	int rowMove_initial, rowMove_end;
	int colMove_initial, colMove_end;
	bool invalid_boundCheck = 0;
	bool invalid_presenceCheck = 0;
	bool validTURN = false;
	int moveCount = 0;
	bool checkmate = false;

	
	if (turnCount % 2 == 0)
	{
		while (invalid_boundCheck == 0 || invalid_presenceCheck == 0 || validTURN == false)
		{
			cout << endl;
			cout << "Enter the piece coordinates to select: ";
			cin >> initialMove;

			invalid_boundCheck = boundCheck_initial(board, initialMove);

			colMove_initial = abs(initialMove[0] - 97);
			rowMove_initial = abs(initialMove[1] - '8');

			invalid_presenceCheck = presenceCheck(board, rowMove_initial, colMove_initial);

			if ((board[rowMove_initial][colMove_initial] >= 65 && board[rowMove_initial][colMove_initial] <= 90))
			{
				validTURN = true;
			}
			else if (invalid_boundCheck != 0)
			{
				cout << endl << "Wrong Piece! It is WHITE's turn" << endl;
				validTURN = false;
			}

		}
	}
	else
	{
		while (invalid_boundCheck == 0 || invalid_presenceCheck == 0 || validTURN == false)
		{
			cout << endl;
			cout << "Enter the piece coordinates to select: ";
			cin >> initialMove;

			invalid_boundCheck = boundCheck_initial(board, initialMove);

			colMove_initial = abs(initialMove[0] - 97);
			rowMove_initial = abs(initialMove[1] - '8');

			invalid_presenceCheck = presenceCheck(board, rowMove_initial, colMove_initial);

			if ((board[rowMove_initial][colMove_initial] >= 97 && board[rowMove_initial][colMove_initial] <= 122))
			{
				validTURN = true;
			}
			else if (invalid_boundCheck != 0)
			{
				cout << endl << "Wrong Piece! It is BLACK's turn" << endl;
				validTURN = false;
			}
		}
	}

	do
	{
		invalid_boundCheck = 0;
		while (invalid_boundCheck == 0)
		{
			cout << endl;
			cout << "Enter the coordinates of where to move the piece: ";
			cin >> endMove;

			invalid_boundCheck = boundCheck_end(board, endMove);

			colMove_end = abs(endMove[0] - 97);
			rowMove_end = abs(endMove[1] - '8');
		}

		if (whitecheck(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end))
		{
			cout << "White in danger";
			system("pause");
			break;
		}
		else if (blackcheck(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end))
		{
			cout << "Black in danger";
			system("pause");
			break;
		}

		if (board[rowMove_initial][colMove_initial] == 'b' || board[rowMove_initial][colMove_initial] == 'B')
		{
			move_valid = bishop(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end, turnCount);
		}
		else if (board[rowMove_initial][colMove_initial] == 'k' || board[rowMove_initial][colMove_initial] == 'K')
		{
			if (board[rowMove_end][colMove_end] == 'r' || board[rowMove_end][colMove_end] == 'R')
			{
				move_valid = castling(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end, moveCount, turnCount);
			}
			else
			{
				move_valid = king(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end, turnCount);
			}

			if (move_valid == 1)
			{
				moveCount++;
			}
		}
		else if (board[rowMove_initial][colMove_initial] == 'r' || board[rowMove_initial][colMove_initial] == 'R')
		{
			move_valid = rook(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end, turnCount);

			if (move_valid == 1)
			{
				moveCount++;
			}
		}
		else if (board[rowMove_initial][colMove_initial] == 'q' || board[rowMove_initial][colMove_initial] == 'Q')
		{
			move_valid = queen(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end, turnCount);
		}
		else if (board[rowMove_initial][colMove_initial] == 'p' || board[rowMove_initial][colMove_initial] == 'P')
		{
			move_valid = pawn(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end, turnCount);
		}
		else if (board[rowMove_initial][colMove_initial] == 'n' || board[rowMove_initial][colMove_initial] == 'N')
		{
			move_valid = knight(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end, turnCount);
		}

	} while (move_valid == 0);

	turnCount++;
}

int boundCheck_initial(char board[rows][columns], string initialMove)
{
	int len1 = initialMove.size();
	int row = initialMove[1];
	int col = initialMove[0];

	if (row < 49 || row > 56 || col < 'a' || col > 'h')
	{
		cout << endl << "Out of Bound's!" << endl;
		return 0;
	}
	else if (len1 != 2)
	{
		cout << endl << "Out of Bound's!" << endl;
		return 0;
	}

	return 1;
}

int boundCheck_end(char board[rows][columns], string endMove)
{
	int len2 = endMove.size();
	int row = endMove[1];
	int col = endMove[0];

	if (row < 49 || row > 56 || col < 'a' || col > 'h')
	{
		cout << endl << "Out of Bound's!" << endl;
		return 0;
	}
	else if (len2 != 2)
	{
		cout << endl << "Out of Bound's!" << endl;
		return 0;
	}

	return 1;
}

int presenceCheck(char board[rows][columns], int rowMove_initial, int colMove_initial)
{
	if (board[rowMove_initial][colMove_initial] == ' ')
	{
		cout << endl << "No piece selected!" << endl;
		return 0;
	}

	return 1;
}

int turnCheck(char board[rows][columns], int rowMove_initial, int colMove_initial, int rowMove_end, int colMove_end, int turnCount)
{
	if (turnCount % 2 == 0)
	{
		if ((board[rowMove_end][colMove_end] >= 97 && board[rowMove_end][colMove_end] <= 122) || board[rowMove_end][colMove_end] == ' ' || board[rowMove_end][colMove_end] == 'r' || board[rowMove_end][colMove_end] == 'R')
		{
			return 1;
		}
		else
		{
			cout << endl << "WHITE's piece already there. Cannot MOVE!" << endl;
			return 0;
		}
	}
	else
	{
		if ((board[rowMove_end][colMove_end] >= 65 && board[rowMove_end][colMove_end] <= 90) || board[rowMove_end][colMove_end] == ' ')
		{
			return 1;
		}
		else
		{
			cout << endl << "BLACK's piece already there. Cannot MOVE!" << endl;
			return 0;
		}
	}
}

int bishop(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end, int turnCount)
{
	int r = rowMove_initial;
	int c = colMove_initial;
	bool obstruction = false;
	bool validMove = true;

	if (abs(rowMove_initial - rowMove_end) == abs(colMove_initial - colMove_end))
	{
		while (r != rowMove_end && c != colMove_end)
		{
			if (rowMove_initial < rowMove_end)
			{
				r++;
			}
			else
			{
				r--;
			}

			if (colMove_initial < colMove_end)
			{
				c++;
			}
			else
			{
				c--;
			}

			if (board[r][c] != ' ' && r != rowMove_end && c != colMove_end)
			{
				obstruction = true;
			}
		}
	}
	else
	{
		validMove = false;
	}

	if (obstruction == false && validMove == true)
	{
		if (turnCheck(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end, turnCount) == 1)
		{
			if (capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 1 || capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 2)
			{
				board[rowMove_end][colMove_end] = board[rowMove_initial][colMove_initial];
				board[rowMove_initial][colMove_initial] = ' ';
				return 1;
			}
			else
			{
				swap(board[rowMove_initial][colMove_initial], board[rowMove_end][colMove_end]);
				return 1;
			}
		}
	}
	else
	{
		cout << endl << "Invalid Move!" << endl;
	}

	return 0;
}

int king(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end, int turnCount)
{
	bool validMove_row = false;
	bool validMove_col = false;

	if (abs(rowMove_initial - rowMove_end) == 1 || abs(rowMove_initial - rowMove_end) == 0)
	{
		validMove_row = true;
	}
	else
	{
		validMove_row = false;
	}

	if (abs(colMove_initial - colMove_end) == 1 || abs(colMove_initial - colMove_end) == 0)
	{
		validMove_col = true;
	}
	else
	{
		validMove_col = false;
	}

	if (validMove_row == true && validMove_col == true)
	{
		if (turnCheck(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end, turnCount) == 1)
		{
			if (capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 1 || capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 2)
			{
				board[rowMove_end][colMove_end] = board[rowMove_initial][colMove_initial];
				board[rowMove_initial][colMove_initial] = ' ';
				return 1;
			}
			else
			{
				swap(board[rowMove_initial][colMove_initial], board[rowMove_end][colMove_end]);
				return 1;
			}
		}
	}
	else
	{
		cout << endl << "Invalid Move!" << endl;
	}

	return 0;
}

int rook(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end, int turnCount)
{
	bool space = 0;

	if (rowMove_initial < rowMove_end)
	{
		for (int i = rowMove_initial + 1; i <= rowMove_end; i++)
		{
			if (board[i][colMove_initial] != ' ')
			{
				space = 0;
				break;
			}
			else
				space = 1;
		}
	}
	else if (rowMove_initial > rowMove_end)
	{
		for (int i = rowMove_end; i <= rowMove_initial - 1; i++)
		{
			if (board[i][colMove_initial] != ' ')
			{
				space = 0;
				break;
			}
			else
				space = 1;
		}
	}
	else if (colMove_initial < colMove_end)
	{
		for (int i = colMove_initial + 1; i <= colMove_end; i++)
		{
			if (board[rowMove_initial][i] != ' ')
			{
				space = 0;
				break;
			}
			else
				space = 1;
		}
	}
	else if (colMove_initial > colMove_end)
	{
		for (int i = colMove_end; i <= colMove_initial - 1; i++)
		{
			if (board[rowMove_initial][i] != ' ')
			{
				space = 0;
				break;
			}
			else
				space = 1;
		}
	}

	if (board[rowMove_end][colMove_end] == ' ')
	{
		if (space == 1)
		{
			if (abs(rowMove_initial - rowMove_end) <= 7 && abs(colMove_initial - colMove_end) == 0 || abs(rowMove_initial - rowMove_end) == 0 && abs(colMove_initial - colMove_end) <= 7)
			{
				if (turnCheck(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end, turnCount) == 1)
				{
					swap(board[rowMove_initial][colMove_initial], board[rowMove_end][colMove_end]);
					return 1;
				}

			}
		}
	}
	else
	{
		if (abs(rowMove_initial - rowMove_end) <= 7 && abs(colMove_initial - colMove_end) == 0 || abs(rowMove_initial - rowMove_end) == 0 && abs(colMove_initial - colMove_end) <= 7)
		{
			if (turnCheck(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end, turnCount) == 1)
			{
				if (capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 1 || capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 2)
				{
					board[rowMove_end][colMove_end] = board[rowMove_initial][colMove_initial];
					board[rowMove_initial][colMove_initial] = ' ';
					return 1;
				}
			}
		}

	}

	cout << endl << "Invalid Move!" << endl;
	return 0;
}


int queen(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end, int turnCount)
{
	bool valid = 0;

	if (abs(rowMove_initial - rowMove_end) == abs(colMove_initial - colMove_end))
	{
		valid = bishop(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end, turnCount);

	}
	else
	{
		valid = rook(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end, turnCount);
	}

	return valid;
}

int pawn(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end, int turnCount)
{
	bool space = 1;
	static bool enPassantValid = 0;
	static int count = 0;

	if (enPassantValid == 1 && count == 2)
	{
		enPassantValid = 0;
		count = 0;
	}


	if (rowMove_initial > rowMove_end)
	{
		for (int i = rowMove_initial - 1; i > rowMove_end - 1; i--)
		{
			if (board[i][colMove_initial] != ' ')
			{
				space = 0;
				break;
			}
		}
	}
	else
	{
		for (int i = rowMove_initial + 1; i < rowMove_end - 1; i++)
		{
			if (board[i][colMove_initial] != ' ')
			{
				space = 0;
				break;
			}
		}
	}

	if (board[rowMove_end][colMove_end] == ' ')
	{
		if (enPassantValid == 1 && count == 1)
		{
			board[rowMove_end][colMove_end] = board[rowMove_initial][colMove_initial];
			board[rowMove_initial][colMove_end] = ' ';
			board[rowMove_initial][colMove_initial] = ' ';
			return 1;
		}
		else
		{
			if (space == 1)
			{
				if (rowMove_initial > rowMove_end)
				{
					if (rowMove_initial == 6)
					{
						if ((abs(rowMove_initial - rowMove_end) == 2) || abs(rowMove_initial - rowMove_end == 1) && abs(colMove_initial - colMove_end) == 0)
						{
							if (turnCheck(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end, turnCount) == 1)
							{
								swap(board[rowMove_initial][colMove_initial], board[rowMove_end][colMove_end]);

								if ((enpassant(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end) == 1 || enpassant(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end) == 2))
								{
									enPassantValid = 1;
									count++;
								}

								if ((rowMove_end == 0 && board[rowMove_end][colMove_end] == 'P') || (rowMove_end == 7 && board[rowMove_end][colMove_end] == 'p'))
									pawnPromotion(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end);
								return 1;
							}
						}
						else if (abs(rowMove_initial - rowMove_end) == 1 && abs(colMove_initial - colMove_end) == 0)
						{
							if (turnCheck(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end, turnCount) == 1)
							{
								swap(board[rowMove_initial][colMove_initial], board[rowMove_end][colMove_end]);

								if ((enpassant(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end) == 1 || enpassant(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end) == 2))
								{
									enPassantValid = 1;
									count++;
								}

								if ((rowMove_end == 0 && board[rowMove_end][colMove_end] == 'P') || (rowMove_end == 7 && board[rowMove_end][colMove_end] == 'p'))
									pawnPromotion(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end);
								return 1;
							}
						}
					}
					else
					{
						if (abs(rowMove_initial - rowMove_end) == 1 && abs(colMove_initial - colMove_end) == 0)
						{
							if (turnCheck(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end, turnCount) == 1)
							{
								swap(board[rowMove_initial][colMove_initial], board[rowMove_end][colMove_end]);

								if ((rowMove_end == 0 && board[rowMove_end][colMove_end] == 'P') || (rowMove_end == 7 && board[rowMove_end][colMove_end] == 'p'))
									pawnPromotion(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end);
								return 1;
							}
						}
					}
				}
				else if (rowMove_initial < rowMove_end)
				{
					if (rowMove_initial == 1)
					{
						if (abs(rowMove_initial - rowMove_end) == 2 && abs(colMove_initial - colMove_end) == 0)
						{
							if (turnCheck(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end, turnCount) == 1)
							{
								swap(board[rowMove_initial][colMove_initial], board[rowMove_end][colMove_end]);

								if ((enpassant(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end) == 1 || enpassant(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end) == 2))
								{
									enPassantValid = 1;
									count++;
								}

								if ((rowMove_end == 0 && board[rowMove_end][colMove_end] == 'P') || (rowMove_end == 7 && board[rowMove_end][colMove_end] == 'p'))
									pawnPromotion(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end);
								return 1;
							}
						}
						else if (abs(rowMove_initial - rowMove_end) == 1 && abs(colMove_initial - colMove_end) == 0)
						{
							if (turnCheck(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end, turnCount) == 1)
							{
								swap(board[rowMove_initial][colMove_initial], board[rowMove_end][colMove_end]);

								if ((enpassant(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end) == 1 || enpassant(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end) == 2))
								{
									enPassantValid = 1;
									count++;
								}

								if ((rowMove_end == 0 && board[rowMove_end][colMove_end] == 'P') || (rowMove_end == 7 && board[rowMove_end][colMove_end] == 'p'))
									pawnPromotion(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end);
								return 1;
							}
						}
					}
					else
					{
						if (abs(rowMove_initial - rowMove_end) == 1 && abs(colMove_initial - colMove_end) == 0)
						{
							if (turnCheck(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end, turnCount) == 1)
							{
								swap(board[rowMove_initial][colMove_initial], board[rowMove_end][colMove_end]);

								if ((rowMove_end == 0 && board[rowMove_end][colMove_end] == 'P') || (rowMove_end == 7 && board[rowMove_end][colMove_end] == 'p'))
									pawnPromotion(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end);
								return 1;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		if ((abs(rowMove_initial - rowMove_end) == 1) && abs(colMove_initial - colMove_end) == 1)
		{
			if (turnCheck(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end, turnCount) == 1)
			{
				if (capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 1 || capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 2)
				{
					board[rowMove_end][colMove_end] = board[rowMove_initial][colMove_initial];
					board[rowMove_initial][colMove_initial] = ' ';

					if ((rowMove_end == 0 && board[rowMove_end][colMove_end] == 'P') || (rowMove_end == 7 && board[rowMove_end][colMove_end] == 'p'))
						pawnPromotion(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end);
					return 1;
				}
			}
		}
	}

	cout << endl << "Invalid Move!" << endl;
	return 0;

	if (enPassantValid == 1)
		count++;
}

int knight(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end, int turnCount)
{
	if (board[rowMove_end][colMove_end] == ' ')
	{
		if ((abs(rowMove_initial - rowMove_end) == 2 && abs(colMove_initial - colMove_end) == 1) || (abs(rowMove_initial - rowMove_end) == 1 && abs(colMove_initial - colMove_end) == 2))
		{
			if (turnCheck(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end, turnCount) == 1)
			{
				swap(board[rowMove_initial][colMove_initial], board[rowMove_end][colMove_end]);
				return 1;
			}
		}
	}
	else
	{
		if ((abs(rowMove_initial - rowMove_end) == 2 && abs(colMove_initial - colMove_end) == 1) || (abs(rowMove_initial - rowMove_end) == 1 && abs(colMove_initial - colMove_end) == 2))
		{
			if (turnCheck(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end, turnCount) == 1)
			{
				if (capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 1 || capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 2)
				{
					board[rowMove_end][colMove_end] = board[rowMove_initial][colMove_initial];
					board[rowMove_initial][colMove_initial] = ' ';
					return 1;
				}
			}
		}
	}

	cout << endl << "Invalid Move!" << endl;
	return 0;
}

int capture(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end)
{
	bool capture = 0;

	if ((board[rowMove_initial][colMove_initial] >= 97 && board[rowMove_initial][colMove_initial] <= 122) && (board[rowMove_end][colMove_end] >= 65 && board[rowMove_end][colMove_end] <= 90))
		capture = 1;
	else if ((board[rowMove_initial][colMove_initial] >= 65 && board[rowMove_initial][colMove_initial] <= 90) && (board[rowMove_end][colMove_end] >= 97 && board[rowMove_end][colMove_end] <= 122))
		capture = 2;

	return capture;
}

char pawnPromotion(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end)
{
	bool repeat = 1;

	if (rowMove_end == 0 && board[rowMove_end][colMove_end] == 'P')
	{
		do
		{
			cout << "Enter the initial letter in uppercase of the piece, except K, you want to promote your pawn to: ";
			cin >> board[rowMove_end][colMove_end];

			if (board[rowMove_end][colMove_end] != 'P' && board[rowMove_end][colMove_end] != 'R' && board[rowMove_end][colMove_end] != 'B' && board[rowMove_end][colMove_end] != 'Q' && board[rowMove_end][colMove_end] != 'N')
				repeat = 1;
			else
				repeat = 0;

		} while (repeat == 1);
	}
	else if (rowMove_end == 7 && board[rowMove_end][colMove_end] == 'p')
	{
		do
		{
			cout << "Enter the initial letter in lowercase of the piece, except k, you want to promote your pawn to: ";
			cin >> board[rowMove_end][colMove_end];

			if (board[rowMove_end][colMove_end] != 'p' && board[rowMove_end][colMove_end] != 'r' && board[rowMove_end][colMove_end] != 'b' && board[rowMove_end][colMove_end] != 'q' && board[rowMove_end][colMove_end] != 'n')
				repeat = 1;
			else
				repeat = 0;

		} while (repeat == 1);
	}

	return board[rowMove_end][colMove_end];
}

int castling(char board[rows][columns], int rowMove_initial, int colMove_initial, int rowMove_end, int colMove_end, int moveCount, int turnCount)
{
	int space = 0;
	int check = colMove_initial;
	bool kingmate = false;

	if (moveCount == 0)
	{
		if (colMove_initial < colMove_end)
		{
			while (check != colMove_end && checkCoords(board, rowMove_initial, colMove_initial) == false)
			{
				if (board[rowMove_initial][check] == ' ' && checkCoords(board, rowMove_initial, check) == false)
				{
					space++;
				}
				check++;
			}
		}
		else
		{
			while (check != colMove_end && checkCoords(board, rowMove_initial, colMove_initial) == false)
			{
				if (board[rowMove_initial][check] == ' ' && checkCoords(board, rowMove_initial, check) == false)
				{
					space++;
				}
				check--;
			}
			if (space < 3)
				space = 0;
		}

		if (space == 2)
		{
			if (colMove_initial < colMove_end)
			{
				if (turnCheck(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end, turnCount) == 1)
				{
					swap(board[rowMove_initial][colMove_initial], board[rowMove_end][colMove_end - 1]);
					swap(board[rowMove_end][colMove_end], board[rowMove_initial][colMove_initial + 1]);
					return 1;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				if (turnCheck(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end, turnCount) == 1)
				{
					swap(board[rowMove_initial][colMove_initial], board[rowMove_end][colMove_end + 1]);
					swap(board[rowMove_initial][colMove_initial], board[rowMove_end][colMove_end - 1]);
					return 1;
				}
				else
				{
					return 0;
				}
			}
		}
		else if (space == 3)
		{
			if (colMove_initial < colMove_end)
			{
				if (turnCheck(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end, turnCount) == 1)
				{
					swap(board[rowMove_initial][colMove_initial], board[rowMove_end][colMove_end - 2]);
					swap(board[rowMove_end][colMove_end], board[rowMove_initial][colMove_initial + 1]);
					return 1;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				if (turnCheck(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end, turnCount) == 1)
				{
					swap(board[rowMove_initial][colMove_initial], board[rowMove_end][colMove_end + 2]);
					swap(board[rowMove_end][colMove_end], board[rowMove_initial][colMove_initial - 1]);
					return 1;
				}
				else
				{
					return 0;
				}
			}
		}
	}
}

int enpassant(char board[rows][columns], int rowMove_initial, int colMove_initial, int rowMove_end, int colMove_end)
{
	int enpassant1 = 0;

	if (board[rowMove_end][colMove_end] == 'p' && (board[rowMove_end][colMove_end + 1] == 'P' || board[rowMove_end][colMove_end - 1] == 'P'))
		enpassant1 = 1;
	else if (board[rowMove_end][colMove_end] == 'P' && (board[rowMove_end][colMove_end + 1] == 'p' || board[rowMove_end][colMove_end - 1] == 'p'))
		enpassant1 = 2;

	return enpassant1;
}

bool validBishop(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end)
{
	int r = rowMove_initial;
	int c = colMove_initial;
	bool obstruction = false;
	bool validMove = true;

	if (abs(rowMove_initial - rowMove_end) == abs(colMove_initial - colMove_end))
	{
		while (r != rowMove_end && c != colMove_end)
		{
			if (rowMove_initial < rowMove_end)
			{
				r++;
			}
			else
			{
				r--;
			}

			if (colMove_initial < colMove_end)
			{
				c++;
			}
			else
			{
				c--;
			}

			if (board[r][c] != ' ' && r != rowMove_end && c != colMove_end)
			{
				obstruction = true;
			}
		}
	}
	else
	{
		validMove = false;
	}

	if (obstruction == false && validMove == true)
	{
		if (capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 1 || capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 2)
		{
			validMove = true;
		}
		else if (board[rowMove_end][colMove_end] == ' ')
		{
			validMove = true;
		}
	}

	return validMove;
}

bool validKing (char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end)
{
	bool validMove_row = false;
	bool validMove_col = false;
	bool validMove = false;

	if (abs(rowMove_initial - rowMove_end) == 1 || abs(rowMove_initial - rowMove_end) == 0)
	{
		validMove_row = true;
	}
	else
	{
		validMove_row = false;
	}

	if (abs(colMove_initial - colMove_end) == 1 || abs(colMove_initial - colMove_end) == 0)
	{
		validMove_col = true;
	}
	else
	{
		validMove_col = false;
	}

	if (validMove_row == true && validMove_col == true)
	{

		if (capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 1 || capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 2)
		{
			validMove = true;
		}
		else if (board[rowMove_end][colMove_end] == ' ')
		{
			validMove = true;;
		}
		
	}

	return validMove;
}

bool validKnight(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end)
{
	bool validMove = 0;

	if (board[rowMove_end][colMove_end] == ' ')
	{
		if ((abs(rowMove_initial - rowMove_end) == 2 && abs(colMove_initial - colMove_end) == 1) || (abs(rowMove_initial - rowMove_end) == 1 && abs(colMove_initial - colMove_end) == 2))
		{
			validMove = 1;
		}
	}
	else
	{
		if ((abs(rowMove_initial - rowMove_end) == 2 && abs(colMove_initial - colMove_end) == 1) || (abs(rowMove_initial - rowMove_end) == 1 && abs(colMove_initial - colMove_end) == 2))
		{
			if (capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 1 || capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 2)
			{
				validMove = 1;
			}
		}
	}

	return validMove;
}

bool validRook(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end)
{
	bool space = 0;
	bool validMove = 0;

	if (rowMove_initial < rowMove_end)
	{
		for (int i = rowMove_initial + 1; i <= rowMove_end; i++)
		{
			if (board[i][colMove_initial] != ' ')
			{
				space = 0;
				break;
			}
			else
				space = 1;
		}
	}
	else if (rowMove_initial > rowMove_end)
	{
		for (int i = rowMove_end; i <= rowMove_initial - 1; i++)
		{
			if (board[i][colMove_initial] != ' ')
			{
				space = 0;
				break;
			}
			else
				space = 1;
		}
	}
	else if (colMove_initial < colMove_end)
	{
		for (int i = colMove_initial + 1; i <= colMove_end; i++)
		{
			if (board[rowMove_initial][i] != ' ')
			{
				space = 0;
				break;
			}
			else
				space = 1;
		}
	}
	else if (colMove_initial > colMove_end)
	{
		for (int i = colMove_end; i <= colMove_initial - 1; i++)
		{
			if (board[rowMove_initial][i] != ' ')
			{
				space = 0;
				break;
			}
			else
				space = 1;
		}
	}

	if (board[rowMove_end][colMove_end] == ' ')
	{
		if (space == 1)
		{
			if (abs(rowMove_initial - rowMove_end) <= 7 && abs(colMove_initial - colMove_end) == 0 || abs(rowMove_initial - rowMove_end) == 0 && abs(colMove_initial - colMove_end) <= 7)
			{
				validMove = 1;

			}
		}
	}
	else
	{
		if (abs(rowMove_initial - rowMove_end) <= 7 && abs(colMove_initial - colMove_end) == 0 || abs(rowMove_initial - rowMove_end) == 0 && abs(colMove_initial - colMove_end) <= 7)
		{

			if (capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 1 || capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 2)
			{
				validMove = 1;
			}

		}
	}

	return validMove;
}

bool validQueen(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end)
{
	bool validMove = 0;

	if (abs(rowMove_initial - rowMove_end) == abs(colMove_initial - colMove_end))
	{
		validMove = validBishop(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end);

	}
	else
	{
		validMove = validRook(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end);
	}

	return validMove;
}

bool validPawn(char board[rows][columns], int rowMove_initial, int rowMove_end, int colMove_initial, int colMove_end)
{
	bool space = 1;
	static bool enPassantValid = 0;
	static int count = 0;
	bool validMove = 0;

	if (enPassantValid == 1 && count == 2)
	{
		enPassantValid = 0;
		count = 0;
	}

	if (rowMove_initial > rowMove_end)
	{
		for (int i = rowMove_initial - 1; i > rowMove_end - 1; i--)
		{
			if (board[i][colMove_initial] != ' ')
			{
				space = 0;
				break;
			}
		}
	}
	else
	{
		for (int i = rowMove_initial + 1; i < rowMove_end - 1; i++)
		{
			if (board[i][colMove_initial] != ' ')
			{
				space = 0;
				break;
			}
		}
	}

	if (board[rowMove_end][colMove_end] == ' ')
	{
		if (enPassantValid == 1 && count == 1)
		{
			validMove = 1;
		}
		else
		{
			if (space == 1)
			{
				if (rowMove_initial > rowMove_end)
				{
					if (rowMove_initial == 6)
					{
						if ((abs(rowMove_initial - rowMove_end) == 2) || abs(rowMove_initial - rowMove_end == 1) && abs(colMove_initial - colMove_end) == 0)
						{
							validMove = 1;

							if ((enpassant(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end) == 1 || enpassant(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end) == 2))
							{
								enPassantValid = 1;
								count++;
							}
						}
						else if (abs(rowMove_initial - rowMove_end) == 1 && abs(colMove_initial - colMove_end) == 0)
						{
							validMove = 1;

							if ((enpassant(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end) == 1 || enpassant(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end) == 2))
							{
								enPassantValid = 1;
								count++;
							}
						}
					}
					else
					{
						if (abs(rowMove_initial - rowMove_end) == 1 && abs(colMove_initial - colMove_end) == 0)
						{
							validMove = 1;
						}
					}
				}
				else if (rowMove_initial < rowMove_end)
				{
					if (rowMove_initial == 1)
					{
						if (abs(rowMove_initial - rowMove_end) == 2 && abs(colMove_initial - colMove_end) == 0)
						{
							validMove = 1;
							if ((enpassant(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end) == 1 || enpassant(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end) == 2))
							{
								enPassantValid = 1;
								count++;
							}
						}
						else if (abs(rowMove_initial - rowMove_end) == 1 && abs(colMove_initial - colMove_end) == 0)
						{
							validMove = 1;

							if ((enpassant(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end) == 1 || enpassant(board, rowMove_initial, colMove_initial, rowMove_end, colMove_end) == 2))
							{
								enPassantValid = 1;
								count++;
							}
						}
					}
					else
					{
						if (abs(rowMove_initial - rowMove_end) == 1 && abs(colMove_initial - colMove_end) == 0)
						{
							validMove = 1;
						}
					}
				}
			}
		}
	}
	else
	{
		if ((abs(rowMove_initial - rowMove_end) == 1) && abs(colMove_initial - colMove_end) == 1)
		{
			if (capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 1 || capture(board, rowMove_initial, rowMove_end, colMove_initial, colMove_end) == 2)
			{
				validMove = 1;
			}
		}
	}

	if (enPassantValid == 1)
		count++;

	return validMove;
}

bool whitecheck(char board[rows][columns], int rowMove_initial, int colMove_initial, int rowMove_end, int colMove_end)
{
	int wx, wy, wup, wdown, wright, wleft;
	bool cm = false;

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			if (board[r][c] == 'K')
			{
				wx = r;
				wy = c;
				break;
			}
		}
	}

	wup = wx - 1;
	wdown = wx + 1;
	wright = wy + 1;
	wleft = wy - 1;




	if (checkCoords(board, wx, wy))
	{
		if (board[wup][wy] == ' ' || board[wdown][wy] == ' ' || board[wx][wright] == ' ' || board[wx][wleft] == ' ' || board[wup][wright] == ' ' ||board[wup][wleft] == ' ' || board[wdown][wright] == ' ' || board[wdown][wleft]  == ' ')
		{
			if (checkCoords(board, wup, wy) || checkCoords(board, wdown, wy) || checkCoords(board, wx, wright) || checkCoords(board, wx, wleft) || checkCoords(board, wup, wright) || checkCoords(board, wup, wleft) || checkCoords(board, wdown, wright) || checkCoords(board, wdown, wleft))
			{
				cm = true;
			}
		}
		else
		{
			cm = false;
		}
	}

	return cm;
}

bool blackcheck(char board[rows][columns], int rowMove_initial, int colMove_initial, int rowMove_end, int colMove_end)
{
	int bx, by, bup, bdown, bright, bleft;
	bool cm = false;

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			if (board[r][c] == 'k')
			{
				bx = r;
				by = c;
				break;
			}
		}
	}

	bup = bx - 1;
	bdown = bx + 1;
	bright = by + 1;
	bleft = by - 1;




	if (checkCoords(board, bx, by))
	{
		if (board[bup][by] == ' ' || board[bdown][by] == ' ' || board[bx][bright] == ' ' || board[bx][bleft] == ' ' || board[bup][bright] == ' ' || board[bup][bleft] == ' ' || board[bdown][bright] == ' ' || board[bdown][bleft] == ' ')
		{
			if (checkCoords(board, bup, by) || checkCoords(board, bdown, by) || checkCoords(board, bx, bright) || checkCoords(board, bx, bleft) || checkCoords(board, bup, bright) || checkCoords(board, bup, bleft) || checkCoords(board, bdown, bright) || checkCoords(board, bdown, bleft))
			{
				cm = true;
			}
		}
		else
		{
			cm = false;
		}
	}

	return cm;
}

bool checkCoords(char board[rows][columns], int x, int y)
{
	bool danger = false;

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			if (board[r][c] == 'p' || board[r][c] == 'P')
			{
				if (validPawn(board, r, x, c, y) == true)
				{
					danger = true;
				}
			}
			if (board[r][c] == 'r' || board[r][c] == 'R')
			{

				if (validRook(board, r, x, c, y) == true)
				{
					danger = true;
				}

			}
			else if (board[r][c] == 'n' || board[r][c] == 'N')
			{

				if (validKnight(board, r, x, c, y) == true)
				{
					danger = true;
				}

			}
			else if (board[r][c] == 'b' || board[r][c] == 'B')
			{
				if (validBishop(board, r, x, c, y) == true)
				{
					danger = true;
				}
			}
			else if (board[r][c] == 'q' || board[r][c] == 'Q')
			{

				if (validQueen(board, r, x, c, y) == true)
				{
					danger = true;
				}

			}
			else if (board[r][c] == 'k' || board[r][c] == 'K')
			{

				if (validKing(board, r, x, c, y) == true)
				{
					danger = true;
				}

			}
		}

	}

	return danger;
}