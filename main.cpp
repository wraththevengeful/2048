#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
class Game2048{
    private:
        std::vector<std::vector<int> > board = {
            {2,0,2,0},
            {4,4,4,0},
            {0,0,8,0},
            {2,2,16,16}
        };

        // Method to use for left and top plays where rows go backward (right to left <--)
        void backwardPlay(std::vector<int>& singleRow){

            std::vector<int> newRow;
            int i = 0; //while condition iterator

            while(i<singleRow.size()){
                if(singleRow[i]==singleRow[i+1]){ //if i and next element are same
                    newRow.push_back(singleRow[i]+singleRow[i+1]); //add them both and push to new array
                    i+=2; // point at the element after i and i+1
                }
                else{ //if i and i+1 arent same
                    newRow.push_back(singleRow[i]); //push a single element to new array
                    i++; // go to next element
                }
            };

            // padding with 0s to keep it size 4
            while(newRow.size()<4){
                newRow.push_back(0);
            }

            singleRow = newRow; //overwrite OG row with our result
        };

        void sortRows(std::vector<int>& singleRow){
            // sort the array to the zero elements last and non zero first 
            int i = 0;
            while(i < singleRow.size()){
                // base case

                // find a zero element
                while(singleRow[i]!=0 && singleRow[i] != 0){
                    i++;
                }

                int j = i+1;
                // find the next non zero element
                while(singleRow[j]==0 && j<singleRow.size()){
                    j++;
                }

                if(j == singleRow.size()){
                    break;
                }

                // swap now
                singleRow[i] = singleRow[j];
                singleRow[j] = 0;

                i++;
            }
        };

        void transposeOfBoard(){
            std::vector<std::vector<int>> newMatrix(4, std::vector<int>(4,0));
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4;j++){
                    newMatrix[i][j] = board[j][i];
                }
            }
            board = newMatrix;
        }
    
    public:
        void getBoard(){
            for(int i = 0; i<4;i++){
                for(int j = 0; j<4;j++){
                    std::cout << board[i][j] << " ";
                }
                std::cout << std::endl;
            }
        };

        void playLeft(){ //for left play

            for(int i = 0; i<=3;i++){ //to cover every row
                std::vector<int>singleRow = board[i];
                sortRows(singleRow);
                backwardPlay(singleRow);
                board[i] = singleRow;
            }
        }

        void playRight(){ //for right play

            for(int i = 0; i<=3;i++){ //to cover every row
                std::vector<int>singleRow = board[i];
                std::reverse(singleRow.begin(),singleRow.end()); // Reverse vector to accomdate the same as left
                sortRows(singleRow);
                backwardPlay(singleRow);
                std::reverse(singleRow.begin(),singleRow.end()); // OG state
                board[i] = singleRow;
            }
        }

        void playTop(){
            transposeOfBoard(); //so it can do playleft 
            playLeft();
            transposeOfBoard(); //get it back to normal order
        }

        void playBottom(){
            transposeOfBoard();
            playRight();
            transposeOfBoard();
        }

        void test(){
            return transposeOfBoard();
        }
};

int main(){
    // std::cout << "hi" << std::endl;

    Game2048 newGame;

    newGame.getBoard();
    std::cout << std::endl;
    newGame.playBottom();
    newGame.getBoard();

    std::cout << std::endl;
    return 1;
};