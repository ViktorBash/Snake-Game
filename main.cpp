// Snake game implemented in c++

#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <winuser.h>

using namespace std;

class SnakeGame {

    bool left = true;
    bool right = false;
    bool up = false;
    bool down = false;

    // 1 = left, 2 = right, 3 = up, 4 = down
    int prevDir = 1;

    bool gameFinished = false;

    // height and width of board
    static const int height = 15;
    static const int width = 35;

    // Snake positions (X, Y) held in 2 vectors. The head of the snake (O) is always the 1st item in the array
    vector<int> tailX{12, 12};
    vector<int> tailY{5, 6};

    int initialFruitPos[2] = {11, 4};

    string game_board[height][width];

    // Create the initial board (including the borders around it)
    void setInitialBoard() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                // Make the border or set tile to space
                if (i == 0 || i + 1 == height) {
                    game_board[i][j] = "#";
                } else if (j == 0 || j + 1 == width) {
                    game_board[i][j] = "#";
                } else {
                    game_board[i][j] = " ";
                }
            }
        }
    }

    // Change the board array to have O/o where the snake is
    void paintSnake() {
        for (int i = 0; i < tailX.size(); i++) {
            int posY = tailY[i];
            int posX = tailX[i];

            if (i == 0) {
                game_board[posY][posX] = "O";
            } else {
                game_board[posY][posX] = "o";

            }
        }
    }
    // Change the board array to have * where the fruit is
    void paintFruit(const int posArr[]) {
        int posX = posArr[1];
        int posY = posArr[0];
        game_board[posY][posX] = "*";
    }

    // Print out the whole board
    void printBoard() {
        for (auto & i : game_board) {
            for (auto & j : i) {
                cout << j << ends;
            }
            cout << "" << endl;

        }
    }

    int getFutureXHead() {
        int futureXHead;
        // dir can only be UP, RIGHT, LEFT, or DOWN
        if(up){
            futureXHead = tailX.at(0);
        }
        else if(right){
            futureXHead = tailX.at(0) + 1;
        }
        else if(left){
            futureXHead = tailX.at(0) - 1;
        }
        else if (down){
            futureXHead = tailX.at(0);
        }
        return futureXHead;
    }

    int getFutureYHead(){
        int futureYHead;

        // dir can only be UP, RIGHT, LEFT, or DOWN
        if(up){
            futureYHead = tailY.at(0) - 1;
        }
        else if(right) {
            futureYHead = tailY.at(0);
        }
        else if(left){
            futureYHead = tailY.at(0);
        }
        else if (down){
            futureYHead = tailY.at(0) + 1;
        }
        return futureYHead;
    }


    void checkGame() {
        // By this point the user has chosen a direction (dir) to move
        // We need to:
        // ☑ See if the move is valid
        //      - Did the user move into an empty space
        //      - Did the user move into themselves
        //      - Did the user move into a fruit
        //      - Did the user move into a wall
        //
        // ☑ Move the snake, remove the last tail (or not)
        bool ateFruit = false;

        int futureXHead = getFutureXHead();
        int futureYHead = getFutureYHead();
//        if(right){
//            cout << "RIGHT" << endl;
//        }
//        if(left){
//            cout << "LEFT" << endl;
//        }
//        if(down) {
//            cout << "DOWN" << endl;
//        }
//        if(up){
//            cout << "UP" << endl;
//        }

        // Now we will do checks on whether the move is valid and set the previous head to a lowercase o
        if(game_board[futureYHead][futureXHead] == " "){
            game_board[futureYHead][futureXHead] = "O";
            int prevYHead = tailY.at(0);
            int prevXHead = tailX.at(0);
            game_board[prevYHead][prevXHead] = "o";
        }
        else if(game_board[futureYHead][futureXHead] == "o"){
            gameFinished = true;
        }
        else if(game_board[futureYHead][futureXHead] == "#"){
            gameFinished = true;

        }
        else if(game_board[futureYHead][futureXHead] == "*"){
            ateFruit = true;
            game_board[futureYHead][futureXHead] = "O";
        }

        // Now add the tail to the vectors
        tailX.insert(tailX.begin() + 0, futureXHead);
        tailY.insert(tailY.begin() + 0, futureYHead);


        // If we did not eat the fruit we need to remove the last piece of the tail
        if(!ateFruit){
            int endTailX = tailX.back();
            int endTailY = tailY.back();
            game_board[endTailY][endTailX] = " ";
            game_board[tailY.back()][tailX.back()] = " ";
            tailX.pop_back();
            tailY.pop_back();
        }


        if(ateFruit){
            bool done = false;
            while(!done){
                int randY = (rand() % height - 1) + 1;
                int randX = (rand() % width - 1) + 1;
                if(game_board[randY][randX] == " "){
                    game_board[randY][randX] = "*";
                    done = true;
                }
            }

        }


    }

    void getInput() {
        if(GetAsyncKeyState(VK_UP) && !down) {
//            cout << "UP" << endl;
            down = false;
            right = false;
            left = false;
            up = true;
        }
        if(GetAsyncKeyState(VK_DOWN) && !up) {
//            cout << "DOWN" << endl;
            down = true;
            right = false;
            left = false;
            up = false;
        }
        if(GetAsyncKeyState(VK_LEFT) && !right) {
//            cout << "LEFT" << endl;
            left = true;
            right = false;
            down = false;
            up = false;
        }
        if(GetAsyncKeyState(VK_RIGHT) && !left) {
//            cout << "RIGHT" << endl;
            right = true;
            left = false;
            down = false;
            up = false;
        }
    }


public:
    void startGame() {
        cout << "Starting Game" << endl;
        Sleep(1000);

        setInitialBoard();
        paintSnake();
        paintFruit(initialFruitPos);


        while (!gameFinished) {
            // print board --> sleep (so the user has time to react) --> get input --> check game w/ input --> clear
            // the screen and immediately start over again by printing board...
            printBoard();
            Sleep(100); // Time in between moves
            getInput();
            checkGame();
            system("cls");
        }

        // User lost, allow them to exit
        cout << "" << endl;
        cout << "You lost!" << endl;
        Sleep(2000);
        exit(0);
    }
};

int main() {
    SnakeGame gameObj{};
    gameObj.startGame();
    return 0;
}