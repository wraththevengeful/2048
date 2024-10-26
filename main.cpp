#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime>   //for random seed
#include <cstdlib> //for random generator
#include <thread>    // for std::this_thread::sleep_for

// Main class that creates a game object
class Game2048
{

public:
    Game2048()
    {
        std::srand(std::time(0));
        resetBoard();
        randomSpawn(); // Spawn initial tiles
        randomSpawn();
    }

private:
    // the main game board where the game will be played in
    std::vector<std::vector<int>> board = {
        {1, 1, 0, 0},
        {2, 0, 2, 0},
        {2, 2, 4, 4},
        {0, 2, 0, 4}};

    void playBaseFunction()
    { // Basic function that builds on other functions (Basically plays left when raw)
        for (int k = 0; k < 4; k++)
        {
            std::vector<int> &singleRow = board[k];
            sortRow(singleRow);
            std::vector<int> newRow;
            int i = 0; // while condition iterator

            while (i < singleRow.size())
            {
                if (i < singleRow.size() - 1 && singleRow[i] == singleRow[i + 1])
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

            board[k] = newRow; // overwrite OG row with our result
        }
    }

    // Sort rows with zeroes behind
    void sortRow(std::vector<int> &singleRow)
    {
        // sort the array to the zero elements last and non zero first
        singleRow.erase(std::remove(singleRow.begin(), singleRow.end(), 0), singleRow.end());
        while (singleRow.size() < 4)
        {
            singleRow.push_back(0);
        }
    }

    void transposeOfBoard()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = i + 1; j < 4; j++)
            {
                std::swap(board[i][j], board[j][i]);
            }
        }
    }

    void reversalOfBoard()
    {
        for (int i = 0; i < 4; i++)
        {
            std::reverse(board[i].begin(), board[i].end());
        }
    }

    void resetBoard()
    {
        board = {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}};
    }

    int generateTileValue()
    {
        // Generate a random number between 0 and 9; 0-8 generates 2, 9 generates 4.
        return (std::rand() % 10 < 9) ? 2 : 4;
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
        // Only spawn if there are empty tiles available
        if (!zeroIndices.empty())
        {
            int randomIndex = std::rand() % zeroIndices.size(); // choose a random index from the list of tiles
            board[zeroIndices[randomIndex].first][zeroIndices[randomIndex].second] = generateTileValue();
        }
    }

public:
    bool checkWin()
    {
        for (const auto &row : board)
        {
            for (int tile : row)
            {
                if (tile == 2048)
                {
                    return true; // Win condition met
                }
            }
        }
        return false;
    }

    bool checkLose()
    {
        // Check for empty tiles
        for (const auto &row : board)
        {
            for (int tile : row)
            {
                if (tile == 0)
                {
                    return false; // There are still empty tiles, so not lost
                }
            }
        }

        // Check for possible merges
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                if ((j < 3 && board[i][j] == board[i][j + 1]) || // Horizontal check
                    (i < 3 && board[i][j] == board[i + 1][j]))
                {                 // Vertical check
                    return false; // Possible merge exists, so not lost
                }
            }
        }
        return true; // No moves available
    }

    void showBoard()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                std::cout << board[i][j] << "       ";
            }
            std::cout << "\n\n";
        }
    }

    void playLeft()
    {
        playBaseFunction();
        randomSpawn();
    }

    void playRight()
    {
        reversalOfBoard();
        playBaseFunction();
        reversalOfBoard();
        randomSpawn();
    }

    void playUp()
    {
        transposeOfBoard();
        playBaseFunction();
        transposeOfBoard();
        randomSpawn();
    }

    void playDown()
    {
        transposeOfBoard();
        reversalOfBoard();
        playBaseFunction();
        reversalOfBoard();
        transposeOfBoard();
        randomSpawn();
    }

    void test()
    {
        return transposeOfBoard();
    }

    void poorni(){
        std::cout<<"Hello, poorni! (Imagine there is a heart emoji and a leaf emoji is here). Also, yes I added a cheat, you won! The message will stay on screen for 10 seconds\n\n";
        board[0][0] = 2048;
    }

    void newGame(){
        resetBoard();
        randomSpawn();
        randomSpawn();
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

int main()
{
    Game2048 game; // Create an instance of the Game2048 class
    bool gameActive = false;
    char choice;

    while (true)
    {
        clearConsole(); // Clear the console
        std::cout << "=== 2048 Game Menu ===\n";
        std::cout << "1. New Game\n";
        std::cout << "2. Rules\n";
        std::cout << "3. Credits\n";
        std::cout << "4. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        switch (choice)
        {
        case '1':
            game.newGame();
            gameActive = true;
            break;

        case '2':
            clearConsole();
            std::cout << "=== Rules ===\n";
            std::cout << "The objective of the game is to slide numbered tiles on a grid to combine them to create a tile with the number 2048.\n";
            std::cout << "Use the arrow keys (or W/A/S/D) to move the tiles in that direction.\n";
            std::cout << "When two tiles with the same number touch, they merge into one!\n";
            std::cout << "Press any key to return to the menu...\n";
            std::cin.ignore(); // To clear the newline character left in the input buffer
            std::cin.get();
            break;

        case '3':
            clearConsole();
            std::cout << "=== Credits ===\n";
            std::cout << "Developed by elanchezhiyan2440@gmail\nGame designed by Gabriele Cirulli under the MIT License.\n";
            std::cout << "Press any key to return to the menu...\n";
            std::cin.ignore();
            std::cin.get();
            break;

        case '4':
            std::cout << "Thank you for playing! Goodbye!\n";
            return 0;

        default:
            std::cout << "Invalid choice. Please try again.\n";
            std::cin.ignore();
            std::cin.get();
            break;
        }

        // Game loop
        while (gameActive)
        {
            clearConsole();
            game.showBoard(); // Display the game board
            if (game.checkWin())
            {
                std::cout << "Congratulations! You've reached 2048!\n";
                std::cout << "Press any key to return to the menu...\n";
                std::cin.ignore();
                std::cin.get();
                gameActive = false;
                break;
            }
            if (game.checkLose())
            {
                std::cout << "Game Over! No moves left.\n";
                std::cout << "Press any key to return to the menu...\n";
                std::cin.ignore();
                std::cin.get();
                gameActive = false;
                break;
            }

            std::cout << "Enter your move (W/A/S/D for up/left/down/right, E to exit): ";
            char move;
            std::cin >> move;

            switch (move)
            {
            case 'W':
            case 'w':
                game.playUp();
                break;
            case 'A':
            case 'a':
                game.playLeft();
                break;
            case 'S':
            case 's':
                game.playDown();
                break;
            case 'D':
            case 'd':
                game.playRight();
                break;
            case 'E':
            case 'e':
                gameActive = false; // Exit game
                break;
            case 'p':
            case 'P':
                clearConsole();
                game.poorni();
                game.showBoard();
                std::this_thread::sleep_for(std::chrono::seconds(10));
                break;
            default:
                std::cout << "Invalid move! Please try again.\n";
                std::cin.ignore();
                std::cin.get();
                break;
            }
        }
    }

    return 0;
}
