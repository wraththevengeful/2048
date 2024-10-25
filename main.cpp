#include <stdlib.h>
#include <iostream>
#include <vector>    //for std::vectors
#include <algorithm> //for reversing the vectors
#include <ctime>     //for random seed
#include <cstdlib>   //for random generator
#include <utility>   //for pairs
#include <thread>    // for std::this_thread::sleep_for
#include <chrono>    // for std::chrono::seconds
class Game2048
{
private:
    std::vector<std::vector<int>> board = {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}};

    // Method to use for left and top plays where rows go backward (right to left <--)
    void backwardPlay(std::vector<int> &singleRow)
    {

        std::vector<int> newRow;
        int i = 0; // while condition iterator

        while (i < singleRow.size())
        {
            if (singleRow[i] == singleRow[i + 1])
            {                                                      // if i and next element are same
                newRow.push_back(singleRow[i] + singleRow[i + 1]); // add them both and push to new array
                i += 2;                                            // point at the element after i and i+1
            }
            else
            {                                   // if i and i+1 arent same
                newRow.push_back(singleRow[i]); // push a single element to new array
                i++;                            // go to next element
            }
        };

        // padding with 0s to keep it size 4
        while (newRow.size() < 4)
        {
            newRow.push_back(0);
        }

        singleRow = newRow; // overwrite OG row with our result
    };

    void sortRows(std::vector<int> &singleRow)
    {
        // sort the array to the zero elements last and non zero first
        int i = 0;
        while (i < singleRow.size())
        {
            // base case

            // find a zero element
            while (singleRow[i] != 0 && singleRow[i] != 0)
            {
                i++;
            }

            int j = i + 1;
            // find the next non zero element
            while (singleRow[j] == 0 && j < singleRow.size())
            {
                j++;
            }

            if (j == singleRow.size())
            {
                break;
            }

            // swap now
            singleRow[i] = singleRow[j];
            singleRow[j] = 0;

            i++;
        }
    };

    void transposeOfBoard()
    {
        std::vector<std::vector<int>> newMatrix(4, std::vector<int>(4, 0));
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                newMatrix[i][j] = board[j][i];
            }
        }
        board = newMatrix;
    }

    void randomSpawn()
    {
        // store zero tiles
        std::vector<std::pair<int, int>> zeroIndices;

        // find all zero tiles
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (board[i][j] == 0)
                {
                    zeroIndices.push_back({i, j});
                }
            }
        }

        std::srand(std::time(0));                           // seed random generator with current time
        int randomIndex = std::rand() % zeroIndices.size(); // choose a random index from the list of tiles
        board[zeroIndices[randomIndex].first][zeroIndices[randomIndex].second] = (randomIndex % 2 == 0) ? 2 : 4;
    }

    void resetBoard()
    {
        board = {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}};
    }

public:
    void getBoard()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                std::cout << board[i][j] << "          ";
            }
            std::cout << "\n\n";
        }
    };

    void playLeft()
    { // for left play

        for (int i = 0; i <= 3; i++)
        { // to cover every row
            std::vector<int> singleRow = board[i];
            sortRows(singleRow);
            backwardPlay(singleRow);
            board[i] = singleRow;
        }
        randomSpawn();
    }

    void playRight()
    { // for right play

        for (int i = 0; i <= 3; i++)
        { // to cover every row
            std::vector<int> singleRow = board[i];
            std::reverse(singleRow.begin(), singleRow.end()); // Reverse vector to accomdate the same as left
            sortRows(singleRow);
            backwardPlay(singleRow);
            std::reverse(singleRow.begin(), singleRow.end()); // OG state
            board[i] = singleRow;
        }
        randomSpawn();
    }

    void playTop()
    {
        transposeOfBoard(); // so it can do playleft
        playLeft();
        transposeOfBoard(); // get it back to normal order
        randomSpawn();
    }

    void playBottom()
    {
        transposeOfBoard();
        playRight();
        transposeOfBoard();
        randomSpawn();
    }

    bool checkGameStatus()
    {
        bool hasFreeTiles = false;

        // Check for free tiles
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (board[i][j] == 0)
                {
                    hasFreeTiles = true;
                    break; // Found a free tile, exit inner loop
                }
            }
            if (hasFreeTiles)
                break; // Exit outer loop if a free tile is found
        }

        bool hasAMove = false;

        // Check for possible moves
        if (!hasFreeTiles)
        {
            // Check rows for possible moves
            for (int i = 0; i < 4; i++)
            { // cover every row
                for (int j = 0; j < 3; j++)
                {
                    if (board[i][j] == board[i][j + 1])
                    {
                        hasAMove = true;
                        break; // Found a move, exit inner loop
                    }
                }
                if (hasAMove)
                    break; // Exit outer loop if a move is found
            }

            // Check columns for possible moves
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 3; j++)
                { // Ensure not to go out of bounds
                    if (board[j][i] == board[j + 1][i])
                    {
                        hasAMove = true;
                        break; // Found a move, exit inner loop
                    }
                }
                if (hasAMove)
                    break; // Exit outer loop if a move is found
            }
        }

        return hasAMove || hasFreeTiles; // Return true if there are free tiles or possible moves
    }

    bool checkForAWin()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (board[i][j] == 2048)
                    return true;
            }
        }
        return false;
    }

    void newGame()
    { // reset the board;
        resetBoard();
        randomSpawn();
        randomSpawn();
    }

    void PT()
    {
        board[0][0] = 2048;
        getBoard();
        std::cout << "Well, Poorni...you won!\n\n";
    }
};

// UI code
void clearConsole()
{
#ifdef _WIN32
    system("cls"); // Windows
#else
    system("clear"); // Linux/macOS
#endif
}

void showMenu()
{
    std::cout << "\n1. New Game\n"
              << "2. Rules\n"
              << "3. Credits\n"
              << "4. Exit\n"
              << std::endl
              << std::endl
              << "Enter your option number:  ";
}

int main()
{
    clearConsole();
    Game2048 game;
    bool onGoingGame = false;
    while (true)
    {
        if (!onGoingGame)
        {
            showMenu();
            int choice;
            std::cin >> choice;

            switch (choice)
            {
            case 1:
                clearConsole();
                game.newGame();
                game.getBoard();
                onGoingGame = true;
                break;
            case 2:
                clearConsole();
                std::cout << "\n RULES:\nThe goal of 2048 is to reach the tile with value 2048 by merging tiles on a 4x4 grid.\n"
                          << "Slide tiles in four directions (up, down, left, right), and when two tiles of the same\n"
                          << "value collide, they merge into one with their combined value. After each move, a new 2 or 4\n"
                          << "tile appears. The game ends if no more moves are possible. Win by creating a 2048 tile!\n";
                break;
            case 3:
                clearConsole();
                std::cout << "\n elanchezhiyan2440@github.com\n Copyright (c) 2014 Gabriele Cirulli\n";
                break;
            case 4:
                return 2440;
            default:
                clearConsole();
                std::cout << "\nPlease input a valid option.\n\n";
                break;
            }
        }

        while (onGoingGame)
        {
            if (game.checkForAWin() || !game.checkGameStatus())
            {
                onGoingGame = false;
            }
            clearConsole();
            game.getBoard();
            while (game.checkGameStatus() && !game.checkForAWin())
            {
                std::cout << "\n\n1. Up   2. Down   3. Left   4.Right  5. Quit : ";
                int choice;
                std::cin >> choice;

                switch (choice)
                {
                case 1:
                    clearConsole();
                    game.playTop();
                    game.getBoard();
                    break;
                case 2:
                    clearConsole();
                    game.playBottom();
                    game.getBoard();
                    break;
                case 4:
                    clearConsole();
                    game.playRight();
                    game.getBoard();
                    break;
                case 3: 
                    clearConsole();
                    game.playLeft();
                    game.getBoard();
                    break;
                case 5:
                    onGoingGame = false;
                    break;
                case 89254:
                    clearConsole();
                    game.PT();
                    std::this_thread::sleep_for(std::chrono::seconds(10));
                    clearConsole();
                default:
                    std::cout << "Enter a valid input.";
                    break;
                }
            }
        }
    }
    if (game.checkForAWin())
    {
        clearConsole();
        std::cout << "You Won!\n"
                  << std::endl;
    }
    else if (!game.checkGameStatus())
    {
        std::cout << "You Lost\n\n";
    }
}