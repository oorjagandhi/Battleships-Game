/* ENGGEN131 C Project 2023 - Battleships */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAP_SIZE 6
#define HIT 1000

// Prototype declarations of required functions
void PrintArray(int values[MAP_SIZE][MAP_SIZE]);
void InitialiseMap(int map[MAP_SIZE][MAP_SIZE]);
void AddRandomShip(int size, int map[MAP_SIZE][MAP_SIZE]);
int CountValues(int value, int map[MAP_SIZE][MAP_SIZE]);
int TopLeftPosition(int size, int *row, int *col, int map[MAP_SIZE][MAP_SIZE]);
int IsShipValid(int size, int map[MAP_SIZE][MAP_SIZE]);
void InitialiseRandomMap(int map[MAP_SIZE][MAP_SIZE]);
void FireShot(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE], int row, int col);
int CheckGameOver(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE]);


/******************************************************************************
*******************************************************************************
*******************************************************************************
// Implementations of the required functions should go below
*******************************************************************************
*******************************************************************************
******************************************************************************/

void PrintArray(int values[MAP_SIZE][MAP_SIZE]){
	// Loop through each row and column and print the current element
	for (int i = 0; i < MAP_SIZE; i++){
		for (int j = 0; j < MAP_SIZE; j++){
			printf("%d ", values[i][j]);
		}
		// After printing all columns for the current row, print a newline
		printf("\n");
	}
}

void InitialiseMap(int map[MAP_SIZE][MAP_SIZE]){
	// Clear the map by setting all values to 0
	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			map[i][j] = 0;
		}
	}

	// Size 2 ship
	map[0][0] = 2;
	map[1][0] = 2;

	// Size 3 ship
	map[3][0] = 3;
	map[3][1] = 3;
	map[3][2] = 3;

	// Size 4 ship
	map[0][4] = 4;
	map[1][4] = 4;
	map[2][4] = 4;
	map[3][4] = 4;

	// Size 5 ship
	map[5][0] = 5;
	map[5][1] = 5;
	map[5][2] = 5;
	map[5][3] = 5;
	map[5][4] = 5;
}

void AddRandomShip(int size, int map[MAP_SIZE][MAP_SIZE]){
	// Choose the orientation of the ship: 0 for horizontal, 1 for vertical
	int direction = rand() % 2;

	int startRow, startCol;

	// Horizontal placement
	if (direction == 0){											   
		startRow = rand() % MAP_SIZE;
		startCol = rand() % (MAP_SIZE - size + 1); 
		for (int i = 0; i < size; i++)
		{
			map[startRow][startCol + i] = size;
		}
	}
	// Vertical placement
	else{											   
		startRow = rand() % (MAP_SIZE - size + 1); 
		startCol = rand() % MAP_SIZE;			   
		for (int i = 0; i < size; i++){
			map[startRow + i][startCol] = size; 
		}
	}
}

int CountValues(int value, int map[MAP_SIZE][MAP_SIZE]){
	int count = 0; 

	// Loop through each row and column and increment count if the current cell matches value 
	for (int i = 0; i < MAP_SIZE; i++){
		for (int j = 0; j < MAP_SIZE; j++){
			if (map[i][j] == value){
				count++; 
			}
		}
	}
	return count; 
}

int TopLeftPosition(int size, int *row, int *col, int map[MAP_SIZE][MAP_SIZE]){
	for (int i = 0; i < MAP_SIZE; i++){
		for (int j = 0; j < MAP_SIZE; j++){
			// Find the first cell that matches the ship size
			if (map[i][j] == size){ 
				// Check if the ship is horizontal
				if ((j + size <= MAP_SIZE) && (j == 0 || map[i][j - 1] != size)) { // Ensure no previous part of the ship to the left
					int count = 0;
					for (int k = j; k < MAP_SIZE && map[i][k] == size; k++){
						count++;
					}
					if (count == size){
						*row = i;
						*col = j;
						return 1; // 1 for horizontal
					}
				}
				// Check if the ship is vertical
				if ((i + size <= MAP_SIZE) && (i == 0 || map[i - 1][j] != size)){ // Ensure no previous part of the ship above
					int count = 0;
					for (int k = i; k < MAP_SIZE && map[k][j] == size; k++){
						count++;
					}
					if (count == size){
						*row = i;
						*col = j;
						return 2; // 2 for vertical
					}
				}
			}
		}
	}
	*row = -1; // No ship found
	*col = -1;
	return 0; // No direction found
}

int IsShipValid(int size, int map[MAP_SIZE][MAP_SIZE]) {
    int i, j;

    for (i = 0; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE; j++) {
            if (map[i][j] == size) {
                // Check cells surrounding the ship part at map[i][j]
                for (int x = -1; x <= 1; x++) {
                    for (int y = -1; y <= 1; y++) {
                        // Selecting the cells that are DIRECTLY adjacent to the cell
                        if ((x == 0 || y == 0) && !(x == 0 && y == 0)) {
                            int adjRow = i + x;
                            int adjCol = j + y;

                            // Check to see if they are within bounds
                            if (adjRow >= 0 && adjRow < MAP_SIZE && adjCol >= 0 && adjCol < MAP_SIZE) {
                                // Check to see if neighboring cells have a different ship
                                if (map[adjRow][adjCol] != 0 && map[adjRow][adjCol] != size) {
                                    return 0;  // Invalid placement if adjacent cell has a different ship
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;  // Return 1 if the placement of all parts of the ship is valid
}


void InitialiseRandomMap(int map[MAP_SIZE][MAP_SIZE]){
	int shipSizes[] = {5, 4, 3, 2}; // Sizes of ships to be placed
	int numberOfShips = sizeof(shipSizes) / sizeof(shipSizes[0]);

	int validSetup = 0; // Flag to check the validity of the entire setup

	// Try to initialize the map until a valid setup is achieved
	while (!validSetup){
		// Clear the map at the start of each attempt
		for (int i = 0; i < MAP_SIZE; i++){
			for (int j = 0; j < MAP_SIZE; j++){
				map[i][j] = 0;
			}
		}

		int allShipsValid = 1; // Assume all ships can be placed validly
		for (int index = 0; index < numberOfShips; index++){
			AddRandomShip(shipSizes[index], map);
			// If any ship is invalid, break and restart
			if (!IsShipValid(shipSizes[index], map)){
				allShipsValid = 0; 
				break;
			}
		}

		// Confirm the setup is valid if all ships pass validation
		if (allShipsValid){
			validSetup = 1; 
		}
	}
}

void FireShot(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE], int row, int col) {
    if (shots[row][col] == 0) {  // Only process if no shot has been made at this location
        int sequence = MAP_SIZE * MAP_SIZE - CountValues(0, shots) + 1;  // Calculate the sequence number
        int shipSize = map[row][col];
        int start_row, start_col, orientation, sunken = 1;

        if (map[row][col] > 0) {  // If there is a ship
            shots[row][col] = sequence + 1000;  // Mark as a hit
            orientation = TopLeftPosition(shipSize, &start_row, &start_col, map);  // Find the orientation and start position

            // Check if all parts of the ship have been hit
            if (orientation == 1) {  // Horizontal
                for (int i = 0; i < shipSize; i++) {
                    if (shots[start_row][start_col + i] == 0) {
                        sunken = 0;  // Not all parts have been hit
                        break;
                    }
                }
            } else {  // Vertical
                for (int i = 0; i < shipSize; i++) {
                    if (shots[start_row + i][start_col] == 0) {
                        sunken = 0;  // Not all parts have been hit
                        break;
                    }
                }
            }

            // If all parts have been hit, mark them as sunk
            if (sunken) {
                for (int i = 0; i < shipSize; i++) {
                    if (orientation == 1) {  // Horizontal
                        shots[start_row][start_col + i] += (shipSize - 1) * 1000;
                    } else {  // Vertical
                        shots[start_row + i][start_col] += (shipSize - 1) * 1000;
                    }
                }
            }
        } else {  // No ship at this location
            shots[row][col] = sequence;  // Just record the shot number
        }
    }
}

int CheckGameOver(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE]) {
	// Loop through all rows and columns
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
			// If there's a ship part in the map and it hasn't been destroyed in the shots, game is not over
            if (map[i][j] > 0 && shots[i][j] < 1000) { 
                return 0; 
            }
        }
    }
    return 1; // All ships have been destroyed
}


/******************************************************************************
*******************************************************************************
*******************************************************************************
// Implementations of the required functions should go above
*******************************************************************************
*******************************************************************************
******************************************************************************/

// Provided functions needed for playing the Battleships game:

// First, prototype declarations for the bots:
void GetMoveBot1(int shots[MAP_SIZE][MAP_SIZE], int *row, int *col);
void GetMoveBot2(int shots[MAP_SIZE][MAP_SIZE], int *row, int *col);

// Gets the input for one move from the human player (an alphabetic row and a numeric column)
// This function converts both inputs to numeric values
void GetMoveHuman(int *row, int *col, int player)
{
	char a = ' ';
	int b = -1;
	printf("Player %d: enter move [row/col]: ", player);
	while (!(a >= 'A' && a <= 'Z')) {
		scanf("%c", &a);
	}
	while (!(b >= 0 && b <= 25)) {
		scanf("%d", &b);
	}
	*row = (int)(a - 'A');
	*col = b;
}

// Takes two "shots" arrays as input (which are the visible parts of the game shown to players) and formats them into a string for printing
// The player currently to move is highlighted
void GetDisplayMapString(int shots1[MAP_SIZE][MAP_SIZE], int shots2[MAP_SIZE][MAP_SIZE], int player, char *boardString)
{
	int i, j;
	char numbers[10];
	// Intialise board string to be empty:
	boardString[0] = '\0';
	strcat(boardString, "  ");

	for (i = 0; i < MAP_SIZE; i++) {
		sprintf(numbers, "%d", i%10);
		strcat(boardString, numbers);
	}
	strcat(boardString, "|");
	for (i = 0; i < MAP_SIZE; i++) {
		sprintf(numbers, "%d", i%10);
		strcat(boardString, numbers);
	}
	strcat(boardString, "\n  ");

	for (i = 0; i < MAP_SIZE; i++) {
		strcat(boardString, "-");
	}
	strcat(boardString, "|");
	for (i = 0; i < MAP_SIZE; i++) {
		strcat(boardString, "-");
	}
	strcat(boardString, "\n");

	for (i = 0; i < MAP_SIZE; i++) {
		int len = strlen(boardString);
		boardString[len] = (char)('A' + i);
		boardString[len+1] = '\0';
		strcat(boardString, "|");
		for (j = 0; j < MAP_SIZE; j++) {
			if (shots1[i][j] / HIT > 0) {
				if (shots1[i][j] / HIT == 1) {
					strcat(boardString, "X");
				} else {
					sprintf(numbers, "%d", shots1[i][j] / HIT);
					strcat(boardString, numbers);
				}
			} else if (shots1[i][j] > 0) {
				strcat(boardString, ".");
			} else {
				strcat(boardString, " ");
			}
		}
		strcat(boardString, "|");
		for (j = 0; j < MAP_SIZE; j++) {
			if (shots2[i][j] / HIT > 0) {
				if (shots2[i][j] / HIT == 1) {
					strcat(boardString, "X");
				} else {
					sprintf(numbers, "%d", shots2[i][j] / HIT);
					strcat(boardString, numbers);
				}
			} else if (shots2[i][j] > 0) {
				strcat(boardString, ".");
			} else {
				strcat(boardString, " ");
			}
		}
		strcat(boardString, "|");
		len = strlen(boardString);
		boardString[len] = (char)('A' + i);
		boardString[len+1] = '\0';
		strcat(boardString, "\n");
	}
	if (player == 1) {
		strcat(boardString, "  P1");
		for (i = 0; i < MAP_SIZE-2; i++) {
			strcat(boardString, "*");
		}
	} else {
		for (i = 0; i < MAP_SIZE; i++) {
			strcat(boardString, " ");
		}
		strcat(boardString, "   P2");
		for (i = 0; i < MAP_SIZE-2; i++) {
			strcat(boardString, "*");
		}
	}
	strcat(boardString, "\n");
}

// Plays one game of Battleships, beginning with the specified starting player
// Game type = 1 (human vs human) or 2 (human vs bot) or 3 (bot vs bot)
int PlayOneGame(int startingPlayer, int gameType)
{
	int row, col, player, gameOver;

	// String to display the boards
	char displayBoardString[(2*MAP_SIZE+5)*(MAP_SIZE+5)];

	// The maps containing the locations of the ships
	int mapPlayer1[MAP_SIZE][MAP_SIZE] = {0};
	int mapPlayer2[MAP_SIZE][MAP_SIZE] = {0};

	// The locations of the shots
	int shotsPlayer1[MAP_SIZE][MAP_SIZE] = {0};
	int shotsPlayer2[MAP_SIZE][MAP_SIZE] = {0};

	player = startingPlayer;
	gameOver = 0;

	// Create random maps for each player
	InitialiseRandomMap(mapPlayer1);
	InitialiseRandomMap(mapPlayer2);

	// Display the board if a human is playing
	if (gameType != 3) {
		GetDisplayMapString(shotsPlayer1, shotsPlayer2, player, displayBoardString);
		printf("%s", displayBoardString);
	}

	// Process one move for the current player
	while (!gameOver) {
		if (gameType == 1) {
			GetMoveHuman(&row, &col, player);
			if (player == 1) {
				FireShot(shotsPlayer1, mapPlayer2, row, col);
				gameOver = CheckGameOver(shotsPlayer1, mapPlayer2);
			} else {
				FireShot(shotsPlayer2, mapPlayer1, row, col);
				gameOver = CheckGameOver(shotsPlayer2, mapPlayer1);
			}
		} else if (gameType == 2) {
			if (player == 1) {
				GetMoveHuman(&row, &col, player);
				FireShot(shotsPlayer1, mapPlayer2, row, col);
				gameOver = CheckGameOver(shotsPlayer1, mapPlayer2);
			} else {
				GetMoveBot1(shotsPlayer2, &row, &col);
				printf("Player 2 (bot) moves: %c%d\n", (char)(row+'A'), col);
				FireShot(shotsPlayer2, mapPlayer1, row, col);
				gameOver = CheckGameOver(shotsPlayer2, mapPlayer1);
			}
		} else {
			if (player == 1) {
				GetMoveBot1(shotsPlayer1, &row, &col);
				FireShot(shotsPlayer1, mapPlayer2, row, col);
				gameOver = CheckGameOver(shotsPlayer1, mapPlayer2);
			} else {
				GetMoveBot2(shotsPlayer2, &row, &col);
				FireShot(shotsPlayer2, mapPlayer1, row, col);
				gameOver = CheckGameOver(shotsPlayer2, mapPlayer1);
			}
		}

		// Swap players
		if (!gameOver) {
			player = 3 - player;
		}
		if (gameType != 3) {
			GetDisplayMapString(shotsPlayer1, shotsPlayer2, player, displayBoardString);
			printf("%s", displayBoardString);
		}
	}
	return player;
}

// Play a Battleships tournament
// If the tournament is between a human and a bot, only one game is played
// If the tournament is between two bots, the number of games is requested
void PlayBattleships(void)
{
	int gameType, numberOfGames, result;
	int i, wins1, wins2, player;

	// Get play options:
	printf("Options:\n");
	printf(" [1] = Human vs. Human\n");
	printf(" [2] = Human vs. Bot1\n");
	printf(" [3] = Bot1 vs. Bot2\n");
	printf("Choose game type: ");
	scanf("%d", &gameType);
	numberOfGames = 1;
	result = 0;

	// If two bots are playing a tournament, let the user choose how many games
	if (gameType == 3) {
		printf("Number of games: ");
		scanf("%d", &numberOfGames);
	}

	// Set win counts to zero
	wins1 = 0;
	wins2 = 0;

	// Player 1 will always start the first game
	// If a tournament is played (between two bots), the starting player alternates
	player = 1;

	for (i = 0; i < numberOfGames; i++) {
		result = PlayOneGame(player, gameType);
		if (result == 1) {
			wins1++;
		} else {
			wins2++;
		}
		// Switch the starting player for the next game
		player = 3 - player;
	}

	// Show the outcome (of a single game or tournament)
	if (numberOfGames == 1) {
		printf("\nGame over! Congratulations! Winner is Player %d\n\n", result);
	} else {
		printf("\nTournament over! Games played = %d\nPlayer 1 wins = %d / Player 2 wins = %d\n\n", numberOfGames, wins1, wins2);
	}
}

// The main function for the Battleships program
int main(void)
{
	// Initialise the seed for the random number generator
	srand((unsigned int)time(NULL));

	printf("ENGGEN131 - C Project 2023\n");
	printf("                          ... presents ...\n");
	printf(" ___    __   _____ _____  _     ____  __   _     _   ___   __  \n");
	printf("| |_)  / /\\   | |   | |  | |   | |_  ( (` | |_| | | | |_) ( (` \n");
	printf("|_|_) /_/--\\  |_|   |_|  |_|__ |_|__ _)_) |_| | |_| |_|   _)_) \n");
	printf("\n                __/___            \n");
	printf("          _____/______|           \n");
	printf("  _______/_____\\_______\\_____     \n");
	printf("  \\              < < <       |    \n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("\n");

	PlayBattleships();

	return 0;
}

/******************************************************************************
*******************************************************************************
*******************************************************************************
// Implementations of the two bots can be provided below
// The existing implementations are naive:
// - Bot 1 simply takes random shots (without checking for repeated shots)
// - Bot 2 also takes random shots (but makes a single test on each turn for a repeated shot)
*******************************************************************************
*******************************************************************************
******************************************************************************/

// Strategy for Bot 1
void GetMoveBot1(int shots[MAP_SIZE][MAP_SIZE], int *row, int *col)
{
	int rand_row;
	int rand_col;

	rand_row = shots[0][0]; // to prevent compiler warning (because otherwise "shots" array is not accessed)
	rand_row = rand() % MAP_SIZE;
	rand_col = rand() % MAP_SIZE;

	*row = rand_row;
	*col = rand_col;
}

// Strategy for Bot 2
void GetMoveBot2(int shots[MAP_SIZE][MAP_SIZE], int *row, int *col)
{
	int rand_row;
	int rand_col;

	rand_row = shots[0][0]; // to prevent compiler warning (because otherwise "shots" array is not accessed)
	rand_row = rand() % MAP_SIZE;
	rand_col = rand() % MAP_SIZE;

	// If the shot has been tried before, try one more time
	if (shots[rand_row][rand_col] > 0) {
		rand_row = rand() % MAP_SIZE;
		rand_col = rand() % MAP_SIZE;
	}

	*row = rand_row;
	*col = rand_col;
}













