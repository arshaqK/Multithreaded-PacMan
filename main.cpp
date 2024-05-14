#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <mutex>

using namespace std;
using namespace sf;

#include <cstring> // Add this line

char board[26][26] = {
    "#########################",
    "#.......... # ..........#",
    "#####.....#####.....#####",
    "#.......................#",
    "#.####.####.#.####.####.#",
    "#....#.#....#....#.#....#",
    "#.####.####.#.####.####.#",
    "#.......................#",
    "#####.#..#######..#.#####",
    "#.....#.....#.....#.....#",
    "#.#.#.#.###- -###.#.#.#.#",
    "#.#.#.#.#  P    #.#.#.#.#",
    "#.#.#.#.#  - -  #.#.#.#.#",
    "#.#.#.#.#########.#.#.#.#",
    "#...#.#...........#.#...#",
    "#####.#.#########.#.#####",
    "#...............#.......#",
    "#.######.######.#..######",
    "#.......................#",
    "#.#.#.#.#...#...#.#.#.#.#",
    "#.#.#.#.###...###.#.#.#.#",
    "#.#.#.#...........#.#.#.#",
    "#.#.#.#####.#.##### #.#.#",
    "#...#...............#...#",
    "#########################",
};

void loadPNGImage(string filename, Texture &texture, Sprite &sprite)
{
    if (!texture.loadFromFile(filename))
    {
        cout << "Error loading image: " << filename << endl;
        cout << "Error: " << strerror(errno) << endl; // Add this line
        exit(1);
    }
    sprite.setTexture(texture);
}

// function to load a font

void loadFont(string filename, Font &font)
{
    if (!font.loadFromFile(filename))
    {
        cout << "Error loading font: " << filename << endl;
        exit(1);
    }
}

// collision between player and walls

bool isCollision(int x, int y)
{
    if (board[y / 20][x / 20] == '#')
    {
        return true;
    }
    return false;
}

int PlayerX = 20;
int PlayerY = 20;
int score = 0;
int lives = 3;

int ghostX[1] = {20};
int ghostY[1] = {20};

RenderWindow window(VideoMode(720, 520), "Pacman Game");

/*
Game Engine Thread:
● Create a dedicated thread for the game engine, which is responsible for coordinating the
overall game flow, handling input from players, updating the game state, and rendering
graphics.
● This thread will execute the main game loop, continuously updating the game state based
on user input and Ghost movement controller.*/

void* gameEngineThread(void *arg)
{
    while (window.isOpen())
    {
        Event event;
        while(window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }

            if (event.type == Event::KeyPressed)
            {

                if(board[PlayerY / 20][PlayerX / 20] == '.')
                {
                    board[PlayerY / 20][PlayerX / 20] = ' ';
                    score += 10;
                }
                if (event.key.code == Keyboard::Right)
                {
                    if (!isCollision(PlayerX + 20, PlayerY))
                    {
                        PlayerX += 20;
                    }
                }
                if (event.key.code == Keyboard::Left)
                {
                    if (!isCollision(PlayerX - 20, PlayerY))
                    {
                        PlayerX -= 20;
                    }
                }
                if (event.key.code == Keyboard::Up)
                {
                    if (!isCollision(PlayerX, PlayerY - 20))
                    {
                        PlayerY -= 20;
                    }
                }
                if (event.key.code == Keyboard::Down)
                {
                    if (!isCollision(PlayerX, PlayerY + 20))
                    {
                        PlayerY += 20;
                    }
                }
            }
        }
 

        // scoreTriggerMutex.signal()

        // use mutex and wait for score thread to check for player collision
        // scoreMutex.wait()



        window.clear();

        // draw ghost here
        Texture ghostTexture;
        Sprite ghostSprite;
        loadPNGImage("pacman.png", ghostTexture, ghostSprite);
        ghostSprite.setPosition(ghostX[0], ghostY[0]);
        ghostSprite.scale(0.05, 0.05);
        window.draw(ghostSprite);

        for (int i = 0; i < 26; i++)
        {
            for (int j = 0; j < 26; j++)
            {
                if (board[i][j] == '#')
                {
                    RectangleShape wall(Vector2f(20, 20));
                    wall.setFillColor(Color::Blue);
                    wall.setPosition(j * 20, i * 20);
                    window.draw(wall);
                }
                if (board[i][j] == '.')
                {
                    CircleShape coin(5);
                    coin.setFillColor(Color::Yellow);
                    coin.setPosition(j * 20 + 7, i * 20 + 7);
                    window.draw(coin);
                }
                if (board[i][j] == 'P')
                {
                    // load a png
                    Texture texture;
                    Sprite sprite;
                    loadPNGImage("pacman.png", texture, sprite);
                    sprite.setPosition(PlayerX, PlayerY);
                    sprite.scale(0.05, 0.05);
                    window.draw(sprite);

                    
                }
            }
        }

       
        Font font;
        loadFont("arial.ttf", font);
        Text text;
        text.setFont(font);
        text.setString("Score: " + to_string(score));
        text.setCharacterSize(24);
        text.setFillColor(Color::Red);
        text.setPosition(window.getSize().x - text.getLocalBounds().width - 10, 10);
        window.draw(text);

       
        Text livesText;
        livesText.setFont(font);
        livesText.setString("Lives: "+ to_string(lives) );
        livesText.setCharacterSize(24);
        livesText.setFillColor(Color::Red);
        livesText.setPosition(window.getSize().x - livesText.getLocalBounds().width - 10, window.getSize().y - livesText.getLocalBounds().height - 10);
        window.draw(livesText);

        if(PlayerX == ghostX[0] && PlayerY == ghostY[0])
        {
            lives -= 1;
            if(lives == 0)
            {
                cout << "Game Over" << endl;
                //window.close();
            }
        }


        window.display();

        
    }
    return NULL;
}

/*
User Interface Thread:
● Implement a separate thread to manage the user interface (UI) components of the game,
including menus, scoreboards, and any Heads-up display (HUD) elements.
● This thread will handle input events from the player, such as keyboard or mouse input, and
communicate them to the game engine thread for processing.
*/
void *userInterfaceThread(void *arg)
{
     while (window.isOpen())
     {
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            cout << "Game Paused" << endl;
        }    
     }
     return NULL;
}

/*
scoreThread
{
    while True:
        scoreSignalMutex.wait()
        if pl collides with pellet:
            score += 10
        scoreMutex.signal()
}
*/

void *moveGhostThread(void *arg)
{
    int ghostNum = 0;//*((int *)arg);
    int lastDirection = 0; // 0 = right, 1 = left, 2 = up, 3 = down
    bool justMovedBackward = false;

    srand(time(NULL)); // Initialize random number generator

    while (window.isOpen())
    {
        if (!justMovedBackward && !isCollision(ghostX[ghostNum] + 20, ghostY[ghostNum]) && lastDirection != 1) {
            ghostX[ghostNum] += 20;
            lastDirection = 0;
        } else if (!justMovedBackward && !isCollision(ghostX[ghostNum] - 20, ghostY[ghostNum]) && lastDirection != 0) {
            ghostX[ghostNum] -= 20;
            lastDirection = 1;
        } else if (!isCollision(ghostX[ghostNum], ghostY[ghostNum] + 20)) {
            ghostY[ghostNum] += 20;
            lastDirection = 3;
            justMovedBackward = false;
        } else if (!isCollision(ghostX[ghostNum], ghostY[ghostNum] - 20)) {
            ghostY[ghostNum] -= 20;
            lastDirection = 2;
            justMovedBackward = false;
        } else {
            // If all directions are blocked, move back in the opposite direction
            switch (lastDirection) {
                case 0:
                    ghostX[ghostNum] -= 20;
                    break;
                case 1:
                    ghostX[ghostNum] += 20;
                    break;
                case 2:
                    ghostY[ghostNum] += 20;
                    break;
                case 3:
                    ghostY[ghostNum] -= 20;
                    break;
            }
            justMovedBackward = true;
        }

        sleep(seconds(1));
    }
    return NULL;
}


int main()
{
    window.setActive(false);
    pthread_t thread;
    pthread_t thread2;
    pthread_create(&thread, NULL, gameEngineThread, NULL);
    pthread_create(&thread2, NULL, moveGhostThread, NULL);
    //pthread_create(&thread2, NULL, userInterfaceThread, NULL);

    pthread_join(thread, NULL);
    return 0;   
}