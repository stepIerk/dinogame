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
bool needСactus = true;

void drawMenu();

void drawGround();

int keyPress();

void drawCactus();

void cactusInit();

void dinoJump();

void drawDino();

void paint();

int kbhit();

int main()
{
    initscr();
    curs_set(0); // disable cursor

    noecho();
    getmaxyx(stdscr, row, col);
    mvprintw(row / 2, col / 2 - 13, "press any key to continue\n");
    getch();
    printw("Game in progress\n");
    while (gameOver == false)
    {
        if (!keyPress()) // check which key is pressed, if 'q' is pressed, return 0
        {
            break; // break the loop
        }
        paint(); // paint the screen

        refresh();
        usleep(100000);
        score = score + 0.1;
        clear();
    }
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
    endwin();
    return 1;
}

void paint()
{
    drawMenu();
    drawGround();
    dinoJump(); // check if dino is jumping and jump
    drawDino();
    cactusInit();
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

void cactusInit()
{
    if (needСactus)
    {
        needСactus = false;
        cactusX = col;
    }
    else
    {

        if (cactusX < 1)
        {
            needСactus = true;
        }
        else
        {
            cactusX = cactusX - 2;
            drawCactus();
        }
        if ((cactusX <= 5 && cactusX > 3) && dy < 5)
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
    mvaddch(row - 2, cactusX, 't' | A_BOLD);
    mvaddch(row - 3, cactusX, 'c' | A_BOLD);
    mvaddch(row - 4, cactusX, 'a' | A_BOLD);
    mvaddch(row - 5, cactusX, 'c' | A_BOLD);
}