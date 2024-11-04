#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

float score = 0;
int row, col, dy = 0;
int jumpCoord[] = {3, 5, 6, 6, 5, 3, 0};
int jumpStep = 0;
int cactusX; // x coordinate of cactus
// dx = 4
char *cactusShape[] = {" ^ ", "<#>", "<#>", " # "};

bool gameOver = false;
bool jump = false;
bool needNewСactus = true;

void drawMenu();

void drawGround();

int keyPress();

void drawCactus();

void moveCactus();

void dinoJump();

void drawDino();

void drawGame();

int kbhit();

void updateGame();

void refreshScreen();

void initializeScreen();

void waitingForStartGame();
enum Commands
{
    NOPE,
    JUMP,
    EXIT
};

enum Commands scanCommand();
void handleCommand(enum Commands command);

void playGame();

void finishGame();

void releaseScreen();

int main()
{
    initializeScreen();
    waitingForStartGame();
    playGame();
    finishGame();
    releaseScreen();
    return 0;
}

void releaseScreen()
{
    endwin();
}

void finishGame()
{
    clear();
    if (gameOver == true)
    {
        mvprintw(row / 2, col / 2 - 5, "Game Over");
    }
    else
    {
        mvprintw(row / 2, col / 2 - 5, "Good bye!");
    }
    refresh();
    sleep(2);
}

void playGame()
{
    while (gameOver == false)
    {
        drawGame();
        enum Commands command = scanCommand();
        handleCommand(command);
        updateGame();
        refreshScreen();
    }
}

void handleCommand(enum Commands command)
{
    switch (command)
    {
    case JUMP:
        jump = true;
        break;
    case EXIT:
        gameOver = true;
        break;
    default:
        break;
    }
}

enum Commands scanCommand()
{
    nodelay(stdscr, TRUE); // enable key press
    if (kbhit())
    {
        char ch = getch();
        if (ch == 'q')
        {
            return EXIT;
        }
        else if (ch == ' ')
        {
            return JUMP;
        }
    }
    return NOPE;
}

void waitingForStartGame()
{
    mvprintw(row / 2, col / 2 - 13, "press any key to continue\n");
    getch();
}

void initializeScreen()
{
    initscr();
    curs_set(0); // disable cursor
    noecho();
    getmaxyx(stdscr, row, col);
}

void refreshScreen()
{
    refresh();
    usleep(100000);
    score = score + 0.1;
    clear();
}

void updateGame()
{
    moveCactus();
    dinoJump(); // check if dino is jumping and jump
}

void drawGame()
{
    drawMenu();
    drawGround();
    drawDino();
    drawCactus();
}

void drawMenu()
{
    mvprintw(0, 0, "press 'q' to quit");
    mvprintw(0, col - 10, "score: %d", (int)score);
    mvprintw(0, col / 2 - 5, "<Dino Game>");
}

void drawGround()
{
    for (int i = 0; i < col; i++)
    {
        mvaddch(row - 1, i, ' ' | A_STANDOUT);
    }
}

void drawDino()
{
    mvaddch(row - dy - 2, 4, 'D' | A_BOLD);
}

void dinoJump()
{
    if (jump == true)
    {
        jumpStep++;
        dy = jumpCoord[jumpStep];
        if (jumpStep == 6)
        {
            jump = false;
            jumpStep = 0;
        }
    }
    else
    {
        dy = 0;
    }
}

int keyPress() // check if key is pressed
{
    nodelay(stdscr, TRUE); // enable key press
    char ch;
    if (kbhit())
    {
        ch = getch();
        if (ch == 'w' || ch == ' ')
        {
            jump = true;
        }
        else if (ch == 'q')
        {
            return 0;
        }
    }
    return 1;
}

int kbhit()
{
    int ch = getch();
    if (ch != ERR)
    {
        ungetch(ch);
        return 1;
    }
    else
    {
        return 0;
    }
}

void moveCactus()
{
    if (needNewСactus)
    {
        needNewСactus = false;
        cactusX = col;
    }
    else
    {
        if (cactusX < 1)
        {
            needNewСactus = true;
        }
        else
        {
            cactusX = cactusX - 2;
        }
        bool isCollapsed = (cactusX <= 5 && cactusX > 3) && dy < 5;
        if (isCollapsed)
        {
            gameOver = true;
        }
    }
    // int waitingScoreForCactus;
    // if (needСactus)
    // {
    //     needСactus = false;
    //     srandom(time(NULL));
    //     int r = 7 + rand() % 9;
    //     waitingScoreForCactus = (int)score + r;
    // }

    // if ((int)score >= waitingScoreForCactus)
    // {
    //     waitingScoreForCactus = 0;
    //     createCactus();
    // }
    // mvprintw(1, 0, "waiting score for cactus: %d", waitingScoreForCactus);
}

void drawCactus()
{
    if (cactusX < 0)
    {
        return;
    }
    mvaddch(row - 2, cactusX, 't' | A_BOLD);
    mvaddch(row - 3, cactusX, 'c' | A_BOLD);
    mvaddch(row - 4, cactusX, 'a' | A_BOLD);
    mvaddch(row - 5, cactusX, 'c' | A_BOLD);
}