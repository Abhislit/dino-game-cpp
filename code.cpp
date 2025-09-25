#include <iostream>
#include <unistd.h>   // for usleep()
#include <termios.h>  // for non-blocking input
#include <fcntl.h>
using namespace std;

bool gameOver;
int dinoY, obstacleX;
const int groundLevel = 10;
bool isJumping = false;

// set terminal to non-blocking mode
int kbhit()
{
    termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void draw()
{
    system("clear"); // works on Linux/Mac

    for (int i = 0; i <= groundLevel; i++)
    {
        for (int j = 0; j < 40; j++)
        {
            if (i == groundLevel)
                cout << "_"; // ground
            else if (i == dinoY && j == 5)
                cout << "D"; // Dino
            else if (i == groundLevel - 1 && j == obstacleX)
                cout << "|"; // Cactus
            else
                cout << " ";
        }
        cout << endl;
    }
}

void input()
{
    if (kbhit())
    {
        char ch = getchar();
        if (ch == ' ')
        {
            if (!isJumping)
                isJumping = true;
        }
        else if (ch == 'q')
        {
            gameOver = true;
        }
    }
}

void logic()
{
    static int jumpCounter = 0;

    if (isJumping)
    {
        if (jumpCounter < 3)
            dinoY--;
        else if (jumpCounter < 6)
            dinoY++;
        jumpCounter++;

        if (jumpCounter >= 6)
        {
            jumpCounter = 0;
            isJumping = false;
        }
    }

    obstacleX--;
    if (obstacleX < 0)
        obstacleX = 39;

    if (obstacleX == 5 && dinoY == groundLevel - 1)
        gameOver = true;
}

int main()
{
    gameOver = false;
    dinoY = groundLevel - 1;
    obstacleX = 30;

    while (!gameOver)
    {
        draw();
        input();
        logic();
        usleep(100000); // 100 ms delay
    }

    cout << "\nGame Over! Press q to quit.\n";
    return 0;
}
