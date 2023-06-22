#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <fstream>
#include <limits>

struct Cell {
    int value;
    bool canModify;
};

std::vector<std::vector<Cell>> gameBoard(9, std::vector<Cell>(9, {0, true}));
int playerOneScore = 0, playerTwoScore = 0;
bool playerOneTurn = true, playerOneError = false, playerTwoError = false;

void initializeBoard(bool emptyBoard);
bool isValid(int row, int column, int value);
void renderBoard();
void saveGame(const std::string &winner, int score);
int getNumber();

int main() {
    std::string playerOne, playerTwo;

    std::cout << "Enter Player 1 name: ";
    std::cin >> playerOne;

    std::cout << "Enter Player 2 name: ";
    std::cin >> playerTwo;

    char input;
    std::cout << "Do you want an empty board? (y/n): ";
    std::cin >> input;
    initializeBoard(input == 'y');

    renderBoard();

    while (true) {
        std::cout << "It's " << (playerOneTurn ? playerOne : playerTwo) << "'s turn!\n";
        int row, column, value;
        std::cout << "Enter row number (1-9): ";
        row = getNumber();
        std::cout << "Enter column number (1-9): ";
        column = getNumber();
        std::cout << "Enter value (1-9): ";
        value = getNumber();

        if (isValid(row - 1, column - 1, value) && gameBoard[row - 1][column - 1].canModify) {
            gameBoard[row - 1][column - 1].value = value;
            if (playerOneTurn) {
                playerOneScore++;
                playerOneError = false;
            } else {
                playerTwoScore++;
                playerTwoError = false;
            }
        } else {
            if (playerOneTurn) {
                playerOneError = true;
            } else {
                playerTwoError = true;
            }

            if (playerOneError && playerTwoError) {
                std::string winner;
                int score;
                if (playerOneScore > playerTwoScore) {
                    winner = playerOne;
                    score = playerOneScore;
                } else {
                    winner = playerTwo;
                    score = playerTwoScore;
                }
                saveGame(winner, score);
                break;
            }
        }

        playerOneTurn = !playerOneTurn;
        renderBoard();
    }
    return 0;
}

void initializeBoard(bool emptyBoard) {
    if (!emptyBoard) {
        std::vector<std::pair<int, int>> positions;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                positions.push_back(std::make_pair(i, j));
            }
        }

        std::shuffle(positions.begin(), positions.end(), std::default_random_engine(std::random_device{}()));

        for (int i = 0; i < 40; i++) {
            int x = positions[i].first;
            int y = positions[i].second;
            int randomValue = std::rand() % 9 + 1;
            if(isValid(x, y, randomValue)) {
                gameBoard[x][y].value = randomValue;
                gameBoard[x][y].canModify = false;
            }
        }
    }
}

bool isValid(int row, int column, int value) {
    for(int i = 0; i < 9; i++) {
        if((i != column && gameBoard[row][i].value == value) || (i != row && gameBoard[i][column].value == value))
            return false;
    }

    int startRow = row - row % 3;
    int startCol = column - column % 3;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if((startRow + i != row || startCol + j != column) && gameBoard[startRow + i][startCol + j].value == value)
                return false;
        }
    }

    return true;
}

void renderBoard() {
    for(int i = 0; i < 9; i++) {
        if (i != 0 && i % 3 == 0)
            std::cout << "---------+---------+---------" << std::endl;

        for(int j = 0; j < 9; j++) {
            if (j != 0 && j % 3 == 0)
                std::cout << " | ";

            std::cout << " " << (gameBoard[i][j].value == 0 ? '0' : char(gameBoard[i][j].value + '0')) << " ";
        }
        std::cout << std::endl;
    }
}

void saveGame(const std::string &winner, int score) {
    std::ofstream outFile;
    outFile.open("sudoku.txt");

    outFile << "winner,score\n";
    outFile << winner << "," << score;

    outFile.close();
}

int getNumber() {
    int number;
    while (!(std::cin >> number) || number < 1 || number > 9) {
        std::cout << "Invalid input. Please enter a number between 1 and 9: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return number;
}
