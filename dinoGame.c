#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

float score = 0;
int screenHeiht, screenWidth;
int dinoY = 0;
int dinoX = 9;
int jumpCoord[] = {3, 5, 6, 6, 6, 5, 5, 3, 2, 0};
int jumpStep = 0;
int jumpFrames = 9;

int frame = 40000;

#define MAX_CACTUS 5

int cactusPositions[MAX_CACTUS];
int numberOfCactus = 0;

bool gameOver = false;
bool jump = false;
bool needNewСactus = true;

void drawMenu();

void drawGround();

int keyPress();

void drawCactus(int atPosition);

void moveAllCactus();

void checkGameOver();

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
        mvprintw(screenHeiht / 2, screenWidth / 2 - 5, "Game Over");
    }
    mvprintw(screenHeiht / 2 + 2, screenWidth / 2 - 5, "Score: %d", (int)score);

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
    mvprintw(screenHeiht / 2, screenWidth / 2 - 13, "press any key to continue\n");
    getch();
}

void initializeScreen()
{
    initscr();
    curs_set(0); // disable cursor
    noecho();
    getmaxyx(stdscr, screenHeiht, screenWidth);
}

void refreshScreen()
{
    refresh();
    usleep(frame);
    score = score + 0.1;
    clear();
}

void removeOldCactus()
{
    if (numberOfCactus == 0)
    {
        return;
    }
    if (cactusPositions[0] <= 0)
    {
        numberOfCactus--;
        for (int i = 0; i <= numberOfCactus; i++)
        {
            cactusPositions[i] = cactusPositions[i + 1];
        }
    }
}

#define MIN_SPACE_BETWEEN_CACTUS screenWidth / 4

bool isItTimeToCreateNewCactus()
{
    if (numberOfCactus == MAX_CACTUS)
    {
        return false;
    }
    int positionOfLastCactus = cactusPositions[numberOfCactus - 1];
    if (positionOfLastCactus > screenWidth - MIN_SPACE_BETWEEN_CACTUS)
    {
        return false;
    }
    srandom(time(NULL));
    return random() % 100 < 50;
}

void createNewCactus()
{
    if (numberOfCactus < MAX_CACTUS)
    {
        cactusPositions[numberOfCactus] = screenWidth;
        numberOfCactus++;
    }
}

void invalidateCactusList()
{
    removeOldCactus();
    if (isItTimeToCreateNewCactus())
    {
        createNewCactus();
    }
}

void updateGame()
{
    moveAllCactus();
    invalidateCactusList();
    dinoJump(); // check if dino is jumping and jump
    checkGameOver();
}

void drawAllCactus()
{
    for (int i = 0; i < numberOfCactus; i++)
    {
        drawCactus(cactusPositions[i]);
    }
}

void drawGame()
{
    drawMenu();
    drawGround();
    drawDino();
    drawAllCactus();
}

void drawMenu()
{
    mvprintw(0, 0, "press 'q' to quit");
    mvprintw(0, screenWidth - 10, "score: %d", (int)score);
    mvprintw(0, screenWidth / 2 - 5, "<Dino Game>");
}

void drawGround()
{
    for (int i = 0; i < screenWidth; i++)
    {
        mvaddch(screenHeiht - 1, i, '#');
        // mvaddch(screenHeiht - 1, i, ' ' | A_STANDOUT);
    }
}

void drawDino()
{
    mvaddch(screenHeiht - dinoY - 2, dinoX, 'D' | A_BOLD);
}

void dinoJump()
{
    if (jump == true)
    {
        jumpStep++;
        dinoY = jumpCoord[jumpStep];
        if (jumpStep == jumpFrames)
        {
            jump = false;
            jumpStep = 0;
        }
    }
    else
    {
        dinoY = 0;
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
    if (ch == ERR)
    {
        return 0;
    }
    ungetch(ch);
    return 1;
}

void moveAllCactus()
{
    for (int i = 0; i < numberOfCactus; i++)
    {
        cactusPositions[i] = cactusPositions[i] - 1;
    }
}

void checkGameOver()
{
    if (numberOfCactus == 0)
    {
        return;
    }
    int closestCactus = cactusPositions[0];
    bool isCollapsed = (closestCactus <= dinoX + 1 && closestCactus > dinoX - 1) && dinoY < 5;
    if (isCollapsed)
    {
        gameOver = true;
    }
}

void drawCactus(int atPosition)
{
    if (atPosition < 0 || atPosition > screenWidth)
    {
        return;
    }
    mvaddch(screenHeiht - 2, atPosition, '>' | A_BOLD);
    mvaddch(screenHeiht - 3, atPosition, '<' | A_BOLD);
    mvaddch(screenHeiht - 4, atPosition, '>' | A_BOLD);
    mvaddch(screenHeiht - 5, atPosition, '<' | A_BOLD);
}