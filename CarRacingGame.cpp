#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <time.h>

using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); // this line for adding color

int lives = 3; // for lives
int score = 0; // for score

// my car in 2D array
char car[6][7] = {{' ', ' ', ' ', '#', ' ', ' ', ' '},
                  {' ', ' ', '#', '#', '#', ' ', ' '},
                  {'#', '#', '#', '#', '#', '#', '#'},
                  {' ', '#', '#', '#', '#', '#', ' '},
                  {'#', '#', '#', '#', '#', '#', '#'},
                  {' ', ' ', '#', '#', '#', ' ', ' '}};

int car_x = 27; // my car X-axis
int car_y = 50; // my car Y-axis

int enemyX[7];    // enemy cars X-axis
int enemyY[7];    // enemy cars Y-axis
int enemyFlag[7]; // enemy flags used in program for on / off enemy car

int passed[7] = {0, 0, 0, 0, 0, 0, 0};

int const row = 209, col = 61; // track 2D array row and columns
char track[row][col];          // array for game track

void head();                                // function for main head of game
char mainMenu();                            // function for main menu
void carDesign();                           // function for the car which shown in main menu
void playGame();                            // function for actual playing game
void resetEverything();                     // function for reseting all necessary varaibles before starting the game
void playingGameBox();                      // function for printing the game box along with track
void gameName(int x, int y);                // function for displaying game name name in game box
void items(int x, int y);                   // function for displaying valuse of score and lives
void instructions(int x, int y);            // function for printing instructions of the control of the game
void gameOverMessage(int x, int y);         // finction lose its all three lives than the game over message appears
void readTrack();                           // function for reading track from file
bool gameover();                            // function for checking the collisions of my car anf enemy cars
void rep(int m, int x, int y, int c);       // function for printing space behind the car
void move_car(int car_x, int car_y, int m); // function for moving my car
void gotoxy(int x, int y);                  // function for displaying some thing on specific location
void movingTrack(int x);                    // functio nfor moving track on screen
void GenerateEnemy();                       // function for generating randomly enemy cars
int GenRandomNu();                          // function for generating random numbers from 0 to 6
void drawEnemy(int ind);                    // function for displayong enemy car
void increaseIndex();                       // function for changing the index of enemy car for moving
void resetingIndex();                       // if car reach specific height  than reset the enemy car index
void erase(int x, int y);                   // if enemy car reach the specific loctaion than remove that car from screen
void helpForUser(int x, int y);             // function for help of the user
void countScore();                          // function for counting score

main() // main function
{
    readTrack(); // reading track from file

    char op; // variable for option
    while (true)
    {
        system("cls");
        op = mainMenu(); // game main menu

        if (op == '1') // if option is 1
        {
            playGame(); // start playing game
        }
        else if (op == '2') // if option is 2
        {
            helpForUser(40, 25); // help
        }
        else if (op == '3') // option 3 for exit
        {
            break;
        }
    }
}

// function for   playing game
void playGame()
{
    system("cls");
    char m = 0; // used for removing # sign as car moves
    int x;      // for moving track
    int a = row - 61;
    int loop = 0, killStop = 0, b = 0; // variables used for loop counting

    resetEverything(); // before starting game reset everything
    playingGameBox();  // creating game design
    gameName(73, 3);   // displaying game name

    instructions(77, 50); // displaying instructions

    while (true) // game loop
    {

        gotoxy(70, 50);
        countScore();  // counting score
        items(70, 20); // displaying score and lives on screen

        if (lives == 0) // if lives become equal zero game over
        {
            gameOverMessage(1, 30); // game over message
            break;                  // breaking game loop
        }

        x = a;
        movingTrack(x); // moving game track
        a = a - 1;
        if (a == 0) // if game track ends
        {
            a = row - 61; // restart track
        }

        if (GetAsyncKeyState(VK_LEFT)) // for moving left
        {
            if (car_x > 1) // car cannot move left more if column is 1
            {
                car_x--; // minus 1 from car x axis
                m = 1;   // giving it direction in which car moves
            }
        }
        if (GetAsyncKeyState(VK_RIGHT)) // for moving right
        {
            if (car_x < 52) // car cannot move right more if column is 52
            {
                car_x++; // adding 1 from car x axis
                m = 2;   // giving it direction in which car moves
            }
        }
        if (GetAsyncKeyState(VK_UP)) // for moving up
        {
            if (car_y > 0) // car cannot move up more if row is 0
            {
                car_y--; // minus 1 from car y axis
                m = 3;   // giving it direction in which car moves
            }
        }
        if (GetAsyncKeyState(VK_DOWN)) // for moving down
        {
            if (car_y < 53) // car cannot move down more if row is 53
            {
                car_y++; // adding 1 from car y axis
                m = 4;   // giving it direction in which car moves
            }
        }
        move_car(car_x, car_y, m); // moving my car

        if (GetAsyncKeyState(VK_ESCAPE)) // for escaping from game
        {
            break; // breaking game loop
        }

        if (loop == 5) // generate enemy after every 5th loop of game
        {
            GenerateEnemy(); // generating enemy index
            loop = 0;        // making loop count zero
        }
        loop++; // adding 1

        drawEnemy(0); // enemy car 1
        drawEnemy(1); // enemy car 2
        drawEnemy(2); // enemy car 3
        drawEnemy(3); // enemy car 4
        drawEnemy(4); // enemy car 5
        drawEnemy(5); // enemy car 6
        drawEnemy(6); // enemy car 7

        if (killStop == 0) // if one live lose than disable gameover function
        {
            b = 0;
            if (gameover()) // checking collision of our car with enemy car
            {
                killStop = 25; // disabling for 25 loops
                b = 1;         // turning on disable count
            }
        }
        if (b == 1)
        {
            killStop--; // counting for disable of game over function
        }

        increaseIndex(); // increasing the indexes of enemy cars
        resetingIndex(); // reseting index of the enemy cars when they reach on specific location
        Sleep(50);       // delaying game for 50
    }
    cin.ignore(); // for ignoring keys inputs
}

// function for reseting things before game start
void resetEverything()
{
    lives = 3;  // lives equal to 3
    score = 0;  // score equal to zero
    car_x = 27; // giving car x axis 27
    car_y = 50; // giving car y axis 50
    // making all enemy Flags zero
    enemyFlag[0] = enemyFlag[1] = enemyFlag[2] = enemyFlag[3] = enemyFlag[4] = enemyFlag[5] = enemyFlag[6] = 0;
}

// function for reading track from file
void readTrack()
{
    string line;
    fstream file;

    file.open("GameTrack.txt", ios::in); // opening file

    for (int x = 0; x < row; x++) // reading until x = row
    {
        getline(file, line); // reading line by line

        for (int y = 0; y < col; y++)
        {
            track[x][y] = line[y]; // storing readed file into track array charcterby charcter
        }
    }

    file.close(); // closing file
}

// function for moving game track
void movingTrack(int x)
{

    for (int n = 0; n < 59; n++) // for screen size 59
    {
        gotoxy(0, n);
        cout << track[x]; // displaying line by line from track array

        x = x + 1; // adding in index
    }
}

// function for moving my car
void move_car(int car_x, int car_y, int m)
{
    int n = car_y;           // giving y axis value to n
    rep(m, car_x, car_y, 1); // covering garbage leaved by car

    SetConsoleTextAttribute(h, 10); // for green color
    for (int i = 0; i < 6; i++) // for car rows
    {

        gotoxy(car_x, n);
        for (int y = 0; y < 7; y++) // for car columns
        {
            cout << car[i][y]; // showing car
        }
        n++; // adding in y axis
    }
    SetConsoleTextAttribute(h, 7); // for white color

    rep(m, car_x, car_y, 0); // covering garbage leaved by car
}

// function  covering garbage leaved by car
void rep(int m, int x, int y, int c)
{
    if (m == 1) // if car moves left
    {
        x = x + 7;
        for (int t = 0; t < 6; t++) // add spaces on right side
        {
            gotoxy(x, y + t);
            cout << " ";
        }
    }
    else if (m == 2 && c == 1) // if car moves right
    {
        x--;
        for (int t = 0; t < 6; t++) // add spaces on left side
        {
            gotoxy(x, y + t);
            cout << " ";
        }
    }
    else if (m == 3) // if car moves up
    {
        y = y + 6;
        gotoxy(x, y); // add spaces on down side
        cout << "      ";
    }
    else if (m == 4) // if car moves down
    {
        y--;
        gotoxy(x, y); // add spaces on up side
        cout << "      ";
    }
}

// function for displaying something on screen on specific location
void gotoxy(int x, int y)
{
    COORD coordinates;
    coordinates.X = x;
    coordinates.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}

// function generating random number
int GenRandomNu()
{
    return rand() % (7); // generating 7 numbers from 0 to 6
}

// function for generating enemy cars indexes
void GenerateEnemy()
{
    int random;
    random = GenRandomNu(); // giving random number to variable

    if (random == 0 && enemyFlag[0] == 0) // if random number is 0 and flag also 0
    {
        enemyX[0] = 4;    // enemy 1 x-axis
        enemyY[0] = 0;    // enemy 1 y-axis
        enemyFlag[0] = 1; // enemy 1 flag
        passed[0] = 1;
    }
    else if (random == 1 && enemyFlag[1] == 0) // if random number is 1 and flag also 0
    {
        enemyX[1] = 19;   // enemy 2 x-axis
        enemyY[1] = 0;    // enemy 2 y-axis
        enemyFlag[1] = 1; // enemy 2 flag
        passed[1] = 1;
    }
    else if (random == 2 && enemyFlag[2] == 0) // if random number is 2 and flag also 0
    {
        enemyX[2] = 34;   // enemy 3 x-axis
        enemyY[2] = 0;    // enemy 3 y-axis
        enemyFlag[2] = 1; // enemy 3 flag
        passed[2] = 1;
    }
    else if (random == 3 && enemyFlag[3] == 0) // if random number is 3 and flag also 0
    {
        enemyX[3] = 49;   // enemy 4 x-axis
        enemyY[3] = 0;    // enemy 4 y-axis
        enemyFlag[3] = 1; // enemy 4 flag
        passed[3] = 1;
    }
    else if (random == 4 && enemyFlag[4] == 0) // if random number is 4 and flag also 0
    {
        enemyX[4] = 12;   // enemy 4 x-axis
        enemyY[4] = 0;    // enemy 4 y-axis
        enemyFlag[4] = 1; // enemy 4 flag
        passed[4] = 1;
    }
    else if (random == 5 && enemyFlag[5] == 0) // if random number is 5 and flag also 0
    {
        enemyX[5] = 27;   // enemy 4 x-axis
        enemyY[5] = 0;    // enemy 4 y-axis
        enemyFlag[5] = 1; // enemy 4 flag
        passed[5] = 1;
    }
    else if (random == 6 && enemyFlag[6] == 0) // if random number is 6 and flag also 0
    {
        enemyX[6] = 42;   // enemy 4 x-axis
        enemyY[6] = 0;    // enemy 4 y-axis
        enemyFlag[6] = 1; // enemy 4 flag
        passed[6] = 1;
    }
}

// function for drawing enemy car
void drawEnemy(int ind)
{
    if (enemyFlag[ind] == 1) // if enemy flag equal to 1
    {
        if (enemyY[ind] != 0) // if enemy row is not zero
        {
            gotoxy(enemyX[ind], enemyY[ind] - 1);
            cout << "       "; // add spaces behind the enemy car
        }
        SetConsoleTextAttribute(h, 4); // for red color
        gotoxy(enemyX[ind], enemyY[ind]);
        cout << "*******";
        gotoxy(enemyX[ind], enemyY[ind] + 1);
        cout << " ***** ";
        gotoxy(enemyX[ind], enemyY[ind] + 2);
        cout << "*******";
        gotoxy(enemyX[ind], enemyY[ind] + 3);
        cout << "  ***  ";
        gotoxy(enemyX[ind], enemyY[ind] + 4);
        cout << "   *   ";
        SetConsoleTextAttribute(h, 7); // for white color
    }
}

// function for increasing the indexes of enemy cars
void increaseIndex()
{
    for (int idx = 0; idx < 7; idx++) // for all enemy cars
    {
        if (enemyFlag[idx] == 1) // if enemy flag is 1
        {
            enemyY[idx] = enemyY[idx] + 1; // add one in enemy y-axis
        }
    }
}

// function for reseting the enemy cars indexes
void resetingIndex()
{
    for (int idx = 0; idx < 7; idx++) // for all enemy cars
    {
        if (enemyY[idx] == 54) // if the y-axis becomes equal to 54
        {
            erase(enemyX[idx], enemyY[idx]); // erasing car
            enemyY[idx] = 0;                 // making y-axis 0
            enemyFlag[idx] = 0;              // making enemy flag 0
            // score++;                         // and increasing in the score
        }
    }
}

// function for removing enemy car
void erase(int x, int y)
{
    // putting spaces in all location
    gotoxy(x, y - 1);
    cout << "       ";
    gotoxy(x, y);
    cout << "       ";
    gotoxy(x, y + 1);
    cout << "       ";
    gotoxy(x, y + 2);
    cout << "       ";
    gotoxy(x, y + 3);
    cout << "       ";
    gotoxy(x, y + 4);
    cout << "       ";
}

// function for the game over conditions
bool gameover()
{
    bool flag = false;          // flag variable
    for (int i = 0; i < 7; i++) // for all 7 enemy cars
    {

        for (int x = 0; x < 7; x++) // for car size 7
        {
            // checking car x-axis becomes equal to enemy car x-axis
            if ((car_x + x >= enemyX[i] && car_x + x <= enemyX[i] + 6) && (car_y >= enemyY[i] && car_y <= enemyY[i] + 3))
            {
                flag = true;
            }
        }

        for (int j = 0; j < 3; j++) // for car length 3
        {
            // checking car sides becomes equal to enemy car sides
            if (car_x == enemyX[i] + 6 && car_y + 2 == enemyY[i] + j)
            {
                flag = true;
            }
        }

        for (int j = 0; j < 3; j++) // for car length 3
        {
            // checking car sides becomes equal to enemy car sides
            if (car_x == enemyX[i] + 6 && car_y + 4 == enemyY[i] + j)
            {
                flag = true;
            }
        }

        for (int j = 0; j < 3; j++) // for car length 3
        {
            // checking car sides becomes equal to enemy car sides
            if (car_x + 6 == enemyX[i] && car_y + 2 == enemyY[i] + j)
            {
                flag = true;
            }
        }

        for (int j = 0; j < 3; j++) // for car length 3
        {
            // checking car sides becomes equal to enemy car sides
            if (car_x + 6 == enemyX[i] && car_y + 4 == enemyY[i] + j)
            {
                flag = true;
            }
        }
    }

    if (flag) // if flag is true
    {
        lives--; // minus one live
        // giving car initial values
        car_x = 27; // car x-axis
        car_y = 50; // car y-axis
        return true;
    }
    return false;
}

// function for head display of game
void head()
{
    SetConsoleTextAttribute(h, 3); // for blue color
    cout << "        ----------------------------------------------------------------------------------------------------------------------  " << endl;
    cout << "       [                               _______    _______    _____            _______     _______     ______                  ] " << endl;
    cout << "       [                 |\\       |   |          |          |      \\         |           /       \\   |      \\                 ] " << endl;
    cout << "       [                 | \\      |   |          |          |       \\        |          |         |  |       |                ] " << endl;
    cout << "       [                 |  \\     |   |          |          |        \\       |          |         |  |       |                ] " << endl;
    cout << "       [                 |   \\    |   |_______   |_______   |        |       |_______   |         |  |______/                 ] " << endl;
    cout << "       [                 |    \\   |   |          |          |        |       |          |         |  |\\                       ] " << endl;
    cout << "       [                 |     \\  |   |          |          |        /       |          |         |  |  \\                     ] " << endl;
    cout << "       [                 |      \\ |   |          |          |       /        |          |         |  |    \\                   ] " << endl;
    cout << "       [                 |       \\|   |_______   |_______   |_____ /         |           \\_______/   |      \\                 ] " << endl;
    cout << "       [                                                                                                                      ] " << endl;
    cout << "       [                                                                                                                      ] " << endl;
    cout << "       [           ______     ______     _______    _______    _____                                                          ] " << endl;
    cout << "       [          /      \\   |      \\   |          |          |      \\                                                        ] " << endl;
    cout << "       [         |           |       |  |          |          |       \\                                                       ] " << endl;
    cout << "       [         |           |       |  |          |          |        \\                                                      ] " << endl;
    cout << "       [          \\______    |______/   |_______   |_______   |        |                                                      ] " << endl;
    cout << "       [                 \\   |          |          |          |        |     /    ___  ___   __    ___   ___          ___  \\  ] " << endl;
    cout << "       [                  |  |          |          |          |        /    |    |    |   | |  \\  |   | |   | |\\  /| |      | ] " << endl;
    cout << "       [                  |  |          |          |          |       /     |    |    |___| |__/  |  _  |___| | \\/ | |---   | ] " << endl;
    cout << "       [          \\______/   |          |_______   |_______   |_____ /       \\   |___ |   | |  \\  |_| | |   | |    | |___  /  ] " << endl;
    cout << "       [                                                                                                                      ] " << endl;
    cout << "       [----------------------------------------------------------------------------------------------------------------------]  " << endl;

    for (int i = 0; i < 30; i++) // creating the box
    {
        gotoxy(7, 23 + i); // on left side
        cout << "[";
        gotoxy(126, 23 + i); // on right side
        cout << "]";
    }
    cout << endl;
    cout << "        ----------------------------------------------------------------------------------------------------------------------   " << endl;
    SetConsoleTextAttribute(h, 7); // for white color
}

// function for main menu of the game
char mainMenu()
{
    char option;
    head();
    carDesign();

    gotoxy(30, 25);
    cout << " SELECT ANY ONE OPTION >>>";

    gotoxy(30, 30);
    cout << " 1. PLAY GAME  :) "; // option 1 play game
    gotoxy(30, 31);
    cout << " 2. HELP ! "; // option 2 help
    gotoxy(30, 32);
    cout << " 3. EXIT "; // option 3 exit

    gotoxy(30, 34);
    cout << " YOUR OPTION...: ";
    option = getche(); // taking option input
    return option;     // returning option
}

// function for car design for first page
void carDesign()
{
    SetConsoleTextAttribute(h, 10); // for green color
    gotoxy(25, 38);
    cout << "                         __________________________        " << endl;
    gotoxy(25, 38 + 1);
    cout << "                       /        \\          \\        \\      " << endl;
    gotoxy(25, 38 + 2);
    cout << "                     /          |           |        \\     " << endl;
    gotoxy(25, 38 + 3);
    cout << "            _______/____________|___________|_________\\    " << endl;
    gotoxy(25, 38 + 4);
    cout << "          /_|                _  |         _ |        _ \\   " << endl;
    gotoxy(25, 38 + 5);
    cout << "         |                      |           |       |_| |  " << endl;
    gotoxy(25, 38 + 6);
    cout << "         |        ____          |           |   ____    |  " << endl;
    gotoxy(25, 38 + 7);
    cout << "         |_______/____\\_________|___________|__/____\\___|  " << endl;
    gotoxy(25, 38 + 8);
    cout << "                |      |                      |      |     " << endl;
    gotoxy(25, 38 + 9);
    cout << "                 \\____/                        \\____/      " << endl;
    SetConsoleTextAttribute(h, 7); // for white color
}

// function for game name for playing game box
void gameName(int x, int y)
{
    SetConsoleTextAttribute(h, 11); // for light blue color
    gotoxy(x, y);
    cout << "         ___  ___  __      ___   __    ___  ";
    gotoxy(x, y + 1);
    cout << " |\\   | |    |    |  \\    |     /  \\  |   \\ ";
    gotoxy(x, y + 2);
    cout << " | \\  | |___ |___ |   |   |___ |    | |___/ ";
    gotoxy(x, y + 3);
    cout << " |  \\ | |    |    |   |   |    |    | | \\   ";
    gotoxy(x, y + 4);
    cout << " |   \\| |___ |___ |__/    |     \\__/  |  \\  ";
    gotoxy(x, y + 5);
    cout << "";
    gotoxy(x, y + 6);
    cout << "         ___   ___   ___  ___  __           ";
    gotoxy(x, y + 7);
    cout << "        |   | |   | |    |    |  \\          ";
    gotoxy(x, y + 8);
    cout << "        |___  |___| |___ |___ |   |         ";
    gotoxy(x, y + 9);
    cout << "            | |     |    |    |   |         ";
    gotoxy(x, y + 10);
    cout << "        |___| |     |___ |___ |__/     GAME     ";
    SetConsoleTextAttribute(h, 7); // for white color
}

// function for displaying different items on screen
void items(int x, int y)
{
    SetConsoleTextAttribute(h, 14); // for yellow color
    gotoxy(x, y);
    cout << " SCORE : " << score; // displaying score of user
    gotoxy(x, y + 2);
    cout << " LIVES :          "; // displaying lives of user
    for (int i = 1; i <= lives; i++)
    {
        gotoxy(x + 8 + i, y + 2);
        cout << '\3'; // heart symbol
    }
    SetConsoleTextAttribute(h, 7); // for white color
}

// function for displaying game instructions on the screen
void instructions(int x, int y)
{
    SetConsoleTextAttribute(h, 13); // for pink color
    gotoxy(x + 6, y);
    cout << "<<< CONTROLS OF CAR >>>"; // controls of car
    gotoxy(x, y + 2);
    cout << " FOR MOVING LEFT  : LEFT ARROW KEY ";
    gotoxy(x, y + 3);
    cout << " FOR MOVING RIGHT : RIGHT ARROW KEY ";
    gotoxy(x, y + 4);
    cout << " FOR MOVING UP    : UP ARROW KEY ";
    gotoxy(x, y + 5);
    cout << " FOR MOVING DOWN  : DOWN ARROW KEY ";
    SetConsoleTextAttribute(h, 7); // for white color
}

// function for designing game box
void playingGameBox()
{

    for (int x = 0; x < 126 - 61; x++) // creating box
    {

        if (x < (126 - 61) / 2) // creating box from left side
        {
            gotoxy(61 + x, 0);
            cout << "[";
            gotoxy(61 + x, 16);
            cout << "[";
            gotoxy(61 + x, 48);
            cout << "[";
            gotoxy(61 + x, 58);
            cout << "[";
        }
        else // creating box from right side
        {
            gotoxy(61 + x, 0);
            cout << "]";
            gotoxy(61 + x, 16);
            cout << "]";
            gotoxy(61 + x, 48);
            cout << "]";
            gotoxy(61 + x, 58);
            cout << "]";
        }
    }

    for (int i = 0; i < 59; i++)
    {
        gotoxy(61, i);
        cout << "[[[";
        gotoxy(126, i);
        cout << "]]]";
    }
}

// function for displaying game over message
void gameOverMessage(int x, int y)
{
    SetConsoleTextAttribute(h, 4); // for red color
    gotoxy(x, y - 1);
    cout << "                                                                                                                                          " << endl;
    gotoxy(x, y);
    cout << "           ########     ############    ##         ##    ############        ############    #             #    ############    ########  " << endl;
    gotoxy(x, y + 1);
    cout << "         #         #    #          #    # #       # #    #                   #          #    #             #    #               #       # " << endl;
    gotoxy(x, y + 2);
    cout << "        #          #    #          #    #  #     #  #    #                   #          #    #             #    #               #        #" << endl;
    gotoxy(x, y + 3);
    cout << "       #                #          #    #   #   #   #    #                   #          #    #             #    #               #        #" << endl;
    gotoxy(x, y + 4);
    cout << "      #                 #          #    #    # #    #    #                   #          #     #           #     #               #       # " << endl;
    gotoxy(x, y + 5);
    cout << "      #                 ############    #     #     #    ############        #          #      #         #      ############    ########  " << endl;
    gotoxy(x, y + 6);
    cout << "      #                 #          #    #           #    #                   #          #       #       #       #               ##        " << endl;
    gotoxy(x, y + 7);
    cout << "       #                #          #    #           #    #                   #          #        #     #        #               #  #      " << endl;
    gotoxy(x, y + 8);
    cout << "        #      ####     #          #    #           #    #                   #          #         #   #         #               #    #    " << endl;
    gotoxy(x, y + 9);
    cout << "         #     #  #     #          #    #           #    #                   #          #          # #          #               #      #  " << endl;
    gotoxy(x, y + 10);
    cout << "          ######  #     #          #    #           #    ############        ############           #           ############    #       # " << endl;
    gotoxy(x, y + 11);
    cout << "                                                                                                                                          " << endl;
    SetConsoleTextAttribute(h, 7); // for white color
}

// function for the help of user that how to play game
void helpForUser(int x, int y)
{

    system("cls");
    head();
    gotoxy(x + 4, y);
    SetConsoleTextAttribute(h, 4); // for red color
    cout << " <<<< HOW TO PLAY GAME >>>>";

    instructions(x, y + 2);
    SetConsoleTextAttribute(h, 10); // for green color
    gotoxy(x - 10, y + 9);
    cout << " Your Goal is to protect your car from collision with enemy Cars ";
    SetConsoleTextAttribute(h, 4); // for red color
    gotoxy(x + 7, y + 11);
    cout << " <<< LIVES SYSYTEM >>>";
    SetConsoleTextAttribute(h, 10); // for green color
    gotoxy(x - 1, y + 13);
    cout << " You have three lives at the start of the game. ";
    gotoxy(x - 3, y + 14);
    cout << " You lose your one health when you hits by any enemy car ";
    gotoxy(x - 6, y + 15);
    cout << "  If you lose all your three lives then you lose the game :( ";
    SetConsoleTextAttribute(h, 4); // for red color
    gotoxy(x + 7, y + 17);
    cout << " <<< SCORE SYSTEM >>> ";
    SetConsoleTextAttribute(h, 10); // for green color
    gotoxy(x - 14, y + 19);
    cout << "  Your score increase when you passed one car and score increased by one ";
    gotoxy(x, y + 22);
    SetConsoleTextAttribute(h, 7); // for white color
    cout << " Press Any Key to go back to menu....";
    getch();
}

// function for counting score
void countScore()
{
    for (int i = 0; i < 7; i++) // for all 7 enemy cars
    {
        if (passed[i] == 1) // if passed equal to 1
        {
            if (car_y < enemyY[i]) // if our car passed enemy car
            {
                score++;       // than add one in score
                passed[i] = 0; // making passed variable 0
            }
        }
    }
}