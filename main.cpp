#include <stdlib.h>
#include <iostream>
#include <vector>    //for std::vectors
#include <algorithm> //for reversing the vectors
#include <ctime>     //for random seed
#include <cstdlib>   //for random generator
#include <utility>   //for pairs
class Game2048
{
private:
    std::vector<std::vector<int>> board = {
        {1024, 1025, 1, 2},
        {1020, 4, 5, 6},
        {7, 8, 9, 10},
        {11, 12, 13, 14}};

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

public:
    void getBoard()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                std::cout << board[i][j] << " ";
            }
            std::cout << std::endl;
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
    }

    void playTop()
    {
        transposeOfBoard(); // so it can do playleft
        playLeft();
        transposeOfBoard(); // get it back to normal order
    }

    void playBottom()
    {
        transposeOfBoard();
        playRight();
        transposeOfBoard();
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

    void test()
    {
        return randomSpawn();
    }
};

int main()
{
    // std::cout << "hi" << std::endl;

    Game2048 newGame;

    // newGame.getBoard();
    std::cout << std::endl;
    bool doI = newGame.checkGameStatus();
    std::cout << doI;
    // newGame.getBoard();

    std::cout << std::endl;
    return 1;
};