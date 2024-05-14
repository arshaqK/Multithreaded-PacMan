#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <mutex>
#include <unistd.h>
#include <semaphore.h>
#include <queue>

using namespace std;
using namespace sf;

#include <cstring> // Add this line

sem_t ghostSemaphore;
sem_t speedBoostSemaphore;

Clock clock1;

float timer = 0;
float timer2 = 0;
float speedBoostTime = 0;
float invincibility = 0;
bool eatEnemy = false;
bool canMove[] = {false, true, false, false};
int PlayerX = 20;
int PlayerY = 460;
int score = 0;
int lives = 3;
void displaymenu();
void gameOver();
int ghostX[] = {220,240,240,260};
int ghostY[] = {220,220,220,220};
pthread_mutex_t displayMenuMutex;



char board[26][26] = {
    "#########################",
    "#.......... # ..........#",
    "#####.....#####.....#####",
    "#...../.................#",
    "#.####.####.#.####.####.#",
    "#....#.#....#....#.#....#",
    "#.####.####.#.####.####.#",
    "#................./.....#",
    "#####.#..#######..#.#####",
    "#.....#.....#.....#.....#",
    "#.#.#.#.###   ###.#.#.#.#",
    "#.#.#.#.#  P    #.#.#.#.#",
    "#-#.#.#.#       #.#.#.#.#",
    "#.#.#.#.#########.#.#.#.#",
    "#...#.#...........#.#...#",
    "#####.#.#########.#.#####",
    "#.../...........#.......#",
    "#.######.######.#..######",
    "#.......................#",
    "#.#.#.#.#...#...#.#.#.#.#",
    "#.#.#.#.###...###.#.#.#.#",
    "#.#.#.#...........#-#.#.#",
    "#.#.#.#####.#.#####.#.#.#",
    "#...#...............#...#",
    "#########################",
};
	

RenderWindow window(VideoMode(720, 520), "Pacman Game");

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

bool clockBool = false;

/*
Game Engine Thread:
● Create a dedicated thread for the game engine, which is responsible for coordinating the
overall game flow, handling input from players, updating the game state, and rendering
graphics.
● This thread will execute the main game loop, continuously updating the game state based
on user input and Ghost movement controller.*/

void gameOver(){
    
   
    window.clear(Color::Black);
    // text
 	Texture gameOverT;
	Sprite gameOver;
	loadPNGImage("gameOver.png", gameOverT, gameOver);
	gameOver.setPosition(0,0);
	gameOver.scale(1, 1);
	window.draw(gameOver);
	window.display();
    while (window.isOpen())
    {
        Event event;
        while(window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                exit(1);
            }

            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Enter)
                {
                    exit(0);
                    
                }
            }
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::R)
                {
              
                    return;

                    
                }
            }
        }
    
    }
}


void* gameEngineThread(void *arg)
{
    while (window.isOpen())
    {
        Event event;
        if(clockBool == false){
   		clock1.restart();
   		clockBool = true;
   	}
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
                if(board[PlayerY / 20][PlayerX / 20] == '-')
                {
                	eatEnemy = true;
                	board[PlayerY / 20][PlayerX / 20] = ' ';
                	timer2 = 0;
					
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
                if(score == 3030){
                	gameOver();
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
        loadPNGImage("ghost1.png", ghostTexture, ghostSprite);
        ghostSprite.setPosition(ghostX[0], ghostY[0]);
        ghostSprite.scale(0.4, 0.4);
        window.draw(ghostSprite);

        //  ghost
        Texture ghostTexture2;
        Sprite ghostSprite2;
        loadPNGImage("ghost2.png", ghostTexture2, ghostSprite2);
        ghostSprite2.setPosition(ghostX[1], ghostY[1]);
        ghostSprite2.scale(0.4, 0.4);
        window.draw(ghostSprite2);

        //  ghost
        Texture ghostTexture3;
        Sprite ghostSprite3;
        loadPNGImage("ghost3.png", ghostTexture3, ghostSprite3);
        ghostSprite3.setPosition(ghostX[2], ghostY[2]);
        ghostSprite3.scale(0.4, 0.4);
        window.draw(ghostSprite3);

        // ghost
        Texture ghostTexture4;
        Sprite ghostSprite4;
        loadPNGImage("ghost4.png", ghostTexture4, ghostSprite4);
        ghostSprite4.setPosition(ghostX[3], ghostY[3]);
        ghostSprite4.scale(0.4, 0.4);
        window.draw(ghostSprite4);



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
                if (board[i][j] == '/'){
                    CircleShape coin(5);
                    coin.setFillColor(Color::Green);
                    coin.setPosition(j * 20 + 7, i * 20 + 7);
                    window.draw(coin);
                }
                if (board[i][j] == '.')
                {
                    CircleShape coin(5);
                    coin.setFillColor(Color::Yellow);
                    coin.setPosition(j * 20 + 7, i * 20 + 7);
                    window.draw(coin);
                }
                if (board[i][j] == '-')
                {
                    CircleShape coin(5);
                    coin.setFillColor(Color::Red);
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

       // draw score text on top left corner
        Font font;
        loadFont("Crackman Front.otf", font);
        Text text;
        text.setFont(font);
        text.setString("Score: " + to_string(score));
        text.setCharacterSize(30);
        text.setFillColor(Color::Red);
        text.setPosition(520, 240);
        window.draw(text);

        // draw the lives text on bottom right corner
        Text livesText;
        livesText.setFont(font);
        livesText.setString("Lives: "+ to_string(lives) );
        livesText.setCharacterSize(30);
        livesText.setFillColor(Color::Red);
        livesText.setPosition(520, 280);
        window.draw(livesText);
     window.display();
	for(int i=0; i<4; i++){
		if(PlayerX == ghostX[i] && PlayerY == ghostY[i] && eatEnemy == false)
		{
		    lives -= 1;
		    PlayerX = 20;
		    PlayerY = 460;
		    if(lives <= 0)
		    {
			
			gameOver();
		    }
		}
	}
	
   

        
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
    int ghostNum = 0;
    int lastDirection = 0; // 0 = right, 1 = left, 2 = up, 3 = down
    bool justMovedBackward = false;
    bool hasSpeedBoost = false;
   

    srand(time(NULL)); // Initialize random number generator

    while (window.isOpen())
    {
        float time = clock1.getElapsedTime().asSeconds();

        clock1.restart();
        timer += time;
        timer2 += time;
        int val = 0;
        sem_getvalue(&ghostSemaphore, &val);
	

        if(timer >= 4 && val < 2){
            sem_post(&ghostSemaphore);
            timer = 0;
        }
        
        if(PlayerX == ghostX[0] && PlayerY == ghostY[0] && eatEnemy == true)
	{
		ghostX[0] = 220;
		ghostY[0] = 220;
						
	}
	
	if(timer2 >= 10){
		sem_post(&ghostSemaphore);
		timer2 = 0;
		eatEnemy = false;		
	}
        
        if(board[ghostY[ghostNum] / 20][ghostX[ghostNum] / 20] == '/')
        {
            sem_getvalue(&speedBoostSemaphore, &val);
            if(val<2&&!hasSpeedBoost){
                sem_post(&speedBoostSemaphore);
                hasSpeedBoost = true;
                speedBoostTime = timer; // Change the type of speedBoostTime
                cout<<"Speed Boost"<<endl;
                exit(0);

            }
        }

        if(hasSpeedBoost && time - speedBoostTime > 5){ // Use std::chrono::system_clock::now() for comparison
            sem_post(&speedBoostSemaphore);
            hasSpeedBoost = false;
        }
	  if (!isCollision(ghostX[ghostNum], ghostY[ghostNum] - 20)) {
            ghostY[ghostNum] -= 20;
            lastDirection = 2;
            justMovedBackward = false;
        } else if (!justMovedBackward && !isCollision(ghostX[ghostNum] - 20, ghostY[ghostNum]) && lastDirection != 0) {
            ghostX[ghostNum] -= 20;
            lastDirection = 1;
        } else if (!isCollision(ghostX[ghostNum], ghostY[ghostNum] + 20)) {
            ghostY[ghostNum] += 20;
            lastDirection = 3;
            justMovedBackward = false;
        } else if (!justMovedBackward && !isCollision(ghostX[ghostNum] + 20, ghostY[ghostNum]) && lastDirection != 1) {
            ghostX[ghostNum] += 20;
            lastDirection = 0;
        } else {
            // If all directions are blocked, move back in the opposite direction
            switch (lastDirection) {
                case 0:
                    if (!isCollision(ghostX[ghostNum], ghostY[ghostNum] - 20)) {
                        ghostY[ghostNum] -= 20;
                        lastDirection = 2;
                    } else {
                        ghostX[ghostNum] -= 20;
                    }
                    break;
                case 1:
                    if (!isCollision(ghostX[ghostNum], ghostY[ghostNum] - 20)) {
                        ghostY[ghostNum] -= 20;
                        lastDirection = 2;
                    } else {
                        ghostX[ghostNum] += 20;
                    }
                    break;
                case 2:
                    ghostY[ghostNum] += 20;
                    break;
                case 3:
                    if (!isCollision(ghostX[ghostNum] - 20, ghostY[ghostNum])) {
                        ghostX[ghostNum] -= 20;
                        lastDirection = 1;
                    } else if (!isCollision(ghostX[ghostNum] + 20, ghostY[ghostNum])) {
                        ghostX[ghostNum] += 20;
                        lastDirection = 0;
                    } else {
                        ghostY[ghostNum] -= 20;
                    }
                    break;
            }
            justMovedBackward = true;
        }

        sleep(seconds(1));
    }
    return NULL;

}



void *moveGhostThread2(void *arg)
{
    int ghostNum = 1;//*((int *)arg);
    int lastDirection = 0; // 0 = right, 1 = left, 2 = up, 3 = down
    bool justMovedBackward = false;

    srand(time(NULL)); // Initialize random number generator


    while (window.isOpen())
    {
    
    	float time = clock1.getElapsedTime().asSeconds();

	clock1.restart();
	timer += time;
	timer2 += time;
	int val = 0;
	sem_getvalue(&ghostSemaphore, &val);
	
	if(timer >= 4 && val < 2){
		sem_post(&ghostSemaphore);
		timer = 0;
	}
	
	
	if(PlayerX == ghostX[1] && PlayerY == ghostY[1] && eatEnemy == true)
	{
		cout << "THIS ISS IN THE CODE: " << endl;
		ghostX[1] = 220;
		ghostY[1] = 220;
						
	}
	
	if(timer2 >= 10){
		sem_post(&ghostSemaphore);
		timer2 = 0;
		eatEnemy = false;		
	}
	
	cout << "Ghost 2: Timer1: " << timer << " Timer2: " << timer2 << endl;


	 if (!isCollision(ghostX[ghostNum], ghostY[ghostNum] - 20)) {
            ghostY[ghostNum] -= 20;
            lastDirection = 2;
            justMovedBackward = false;
        } else if (!justMovedBackward && !isCollision(ghostX[ghostNum] - 20, ghostY[ghostNum]) && lastDirection != 0) {
            ghostX[ghostNum] -= 20;
            lastDirection = 1;
        } else if (!isCollision(ghostX[ghostNum], ghostY[ghostNum] + 20)) {
            ghostY[ghostNum] += 20;
            lastDirection = 3;
            justMovedBackward = false;
        } else if (!justMovedBackward && !isCollision(ghostX[ghostNum] + 20, ghostY[ghostNum]) && lastDirection != 1) {
            ghostX[ghostNum] += 20;
            lastDirection = 0;
        } else {
            // If all directions are blocked, move back in the opposite direction
            switch (lastDirection) {
                case 0:
                    if (!isCollision(ghostX[ghostNum], ghostY[ghostNum] - 20)) {
                        ghostY[ghostNum] -= 20;
                        lastDirection = 2;
                    } else {
                        ghostX[ghostNum] -= 20;
                    }
                    break;
                case 1:
                    if (!isCollision(ghostX[ghostNum], ghostY[ghostNum] - 20)) {
                        ghostY[ghostNum] -= 20;
                        lastDirection = 2;
                    } else {
                        ghostX[ghostNum] += 20;
                    }
                    break;
                case 2:
                    ghostY[ghostNum] += 20;
                    break;
                case 3:
                    if (!isCollision(ghostX[ghostNum] - 20, ghostY[ghostNum])) {
                        ghostX[ghostNum] -= 20;
                        lastDirection = 1;
                    } else if (!isCollision(ghostX[ghostNum] + 20, ghostY[ghostNum])) {
                        ghostX[ghostNum] += 20;
                        lastDirection = 0;
                    } else {
                        ghostY[ghostNum] -= 20;
                    }
                    break;
            }
            justMovedBackward = true;
        }


        sleep(seconds(1));
	
     }
    return NULL;
}



void *moveGhostThread3(void *arg)
{
    int ghostNum = 2;//*((int *)arg);
    int lastDirection = 0; // 0 = right, 1 = left, 2 = up, 3 = down
    bool justMovedBackward = false;

    srand(time(NULL)); // Initialize random number generator

    while (window.isOpen())
    {
	float time = clock1.getElapsedTime().asSeconds();

	clock1.restart();
	timer += time;
	timer2 += time;
	int val = 0;
	sem_getvalue(&ghostSemaphore, &val);

	if(timer >= 8 && val < 2){
		sem_post(&ghostSemaphore);
		timer = 0;
	}
	
	if(PlayerX == ghostX[2] && PlayerY == ghostY[2] && eatEnemy == true)
	{
		ghostX[2] = 220;
		ghostY[2] = 220;
						
	}
	
	if(timer2 >= 10){
		sem_post(&ghostSemaphore);
		timer2 = 0;
		eatEnemy = false;		
	}
	
	
	cout << "Ghost 1 Timer1: " << timer << " Timer2: " << timer2 << endl;

	
        if (!isCollision(ghostX[ghostNum], ghostY[ghostNum] - 20)) {
            ghostY[ghostNum] -= 20;
            lastDirection = 2;
            justMovedBackward = false;
        } else if (!justMovedBackward && !isCollision(ghostX[ghostNum] - 20, ghostY[ghostNum]) && lastDirection != 0) {
            ghostX[ghostNum] -= 20;
            lastDirection = 1;
        } else if (!isCollision(ghostX[ghostNum], ghostY[ghostNum] + 20)) {
            ghostY[ghostNum] += 20;
            lastDirection = 3;
            justMovedBackward = false;
        } else if (!justMovedBackward && !isCollision(ghostX[ghostNum] + 20, ghostY[ghostNum]) && lastDirection != 1) {
            ghostX[ghostNum] += 20;
            lastDirection = 0;
        } else {
            // If all directions are blocked, move back in the opposite direction
            switch (lastDirection) {
                case 0:
                    if (!isCollision(ghostX[ghostNum], ghostY[ghostNum] - 20)) {
                        ghostY[ghostNum] -= 20;
                        lastDirection = 2;
                    } else {
                        ghostX[ghostNum] -= 20;
                    }
                    break;
                case 1:
                    if (!isCollision(ghostX[ghostNum], ghostY[ghostNum] - 20)) {
                        ghostY[ghostNum] -= 20;
                        lastDirection = 2;
                    } else {
                        ghostX[ghostNum] += 20;
                    }
                    break;
                case 2:
                    ghostY[ghostNum] += 20;
                    break;
                case 3:
                    if (!isCollision(ghostX[ghostNum] - 20, ghostY[ghostNum])) {
                        ghostX[ghostNum] -= 20;
                        lastDirection = 1;
                    } else if (!isCollision(ghostX[ghostNum] + 20, ghostY[ghostNum])) {
                        ghostX[ghostNum] += 20;
                        lastDirection = 0;
                    } else {
                        ghostY[ghostNum] -= 20;
                    }
                    break;
            }
            justMovedBackward = true;
        }

        sleep(seconds(1));
    }
    return NULL;
}

void *moveGhostThread4(void *arg)
{
    int ghostNum = 3;//*((int *)arg);
    int lastDirection = 0; // 0 = right, 1 = left, 2 = up, 3 = down
    bool justMovedBackward = false;

    srand(time(NULL)); // Initialize random number generator

	

    while (window.isOpen())
    {
 	float time = clock1.getElapsedTime().asSeconds();

	clock1.restart();
	timer += time;
	timer2 += time;
	int val = 0;
	sem_getvalue(&ghostSemaphore, &val);

	if(timer >= 8 && val < 2){
		sem_post(&ghostSemaphore);
		timer = 0;
	}
	
	if(PlayerX == ghostX[3] && PlayerY == ghostY[3] && eatEnemy == true)
	{
		ghostX[3] = 220;
		ghostY[3] = 220;
						
	}
	
	if(timer2 >= 10){
		sem_post(&ghostSemaphore);
		timer2 = 0;
		eatEnemy = false;		
	}
	
	
        if (!isCollision(ghostX[ghostNum], ghostY[ghostNum] - 20) && lastDirection != 3)
        {
            ghostY[ghostNum] -= 20;
            lastDirection = 2;
            justMovedBackward = false;
        }
        else if (!justMovedBackward && !isCollision(ghostX[ghostNum] + 20, ghostY[ghostNum]) && lastDirection != 1)
        {
            ghostX[ghostNum] += 20;
            lastDirection = 0;
        }
        else if (!isCollision(ghostX[ghostNum], ghostY[ghostNum] + 20) && lastDirection != 2)
        {
            ghostY[ghostNum] += 20;
            lastDirection = 3;
            justMovedBackward = false;
        }
        else if (!justMovedBackward && !isCollision(ghostX[ghostNum] - 20, ghostY[ghostNum]) && lastDirection != 0)
        {
            ghostX[ghostNum] -= 20;
            lastDirection = 1;
        }
        else
        {
            // If all directions are blocked, move back in the opposite direction
            switch (lastDirection)
            {
            case 0:
                if (!isCollision(ghostX[ghostNum], ghostY[ghostNum] - 20))
                {
                    ghostY[ghostNum] -= 20;
                    lastDirection = 2;
                }
                else
                {
                    ghostX[ghostNum] -= 20;
                }
                break;
            case 1:
                if (!isCollision(ghostX[ghostNum], ghostY[ghostNum] - 20))
                {
                    ghostY[ghostNum] -= 20;
                    lastDirection = 2;
                }
                else
                {
                    ghostX[ghostNum] += 20;
                }
                break;
            case 2:
                ghostY[ghostNum] += 20;
                break;
            case 3:
                if (!isCollision(ghostX[ghostNum] - 20, ghostY[ghostNum]))
                {
                    ghostX[ghostNum] -= 20;
                    lastDirection = 1;
                }
                else if (!isCollision(ghostX[ghostNum] + 20, ghostY[ghostNum]))
                {
                    ghostX[ghostNum] += 20;
                    lastDirection = 0;
                }
                else
                {
                    ghostY[ghostNum] -= 20;
                }
                break;
            }
            justMovedBackward = true;
        }

        sleep(seconds(1));
    }
    return NULL;
}


void displaymenu()
{
    // Load and display the menu image
    Texture menuT;
    Sprite menu;
	loadPNGImage("menu.png", menuT, menu);
    menu.setPosition(0, 0);
    menu.setScale(1, 1);
    window.draw(menu);
    window.display();

    // Event loop to wait for the Enter key or window close
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                exit(1);
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)
            {
               
                return;
            }
        }
    }
}



int main()
{
    window.setActive(false);

    pthread_mutex_init(&displayMenuMutex, NULL);

    pthread_mutex_lock(&displayMenuMutex);

    displaymenu();

    pthread_mutex_unlock(&displayMenuMutex);

    pthread_t thread;
    pthread_t thread2, thread3, thread4, thread5;
    pthread_create(&thread, NULL, gameEngineThread, NULL);


    sem_init(&ghostSemaphore, 0, 2);
	sem_init(&speedBoostSemaphore, 0, 2);

    sem_wait(&ghostSemaphore);
    pthread_create(&thread2, NULL, moveGhostThread, NULL);

    sem_wait(&ghostSemaphore);
    pthread_create(&thread3, NULL, moveGhostThread2, NULL);

    sem_wait(&ghostSemaphore);
    pthread_create(&thread4, NULL, moveGhostThread3, NULL);

    sem_wait(&ghostSemaphore);
    pthread_create(&thread5, NULL, moveGhostThread4, NULL);

    // Wait for the game engine thread to finish
    pthread_join(thread, NULL);

    // Destroy the mutex after use
    pthread_mutex_destroy(&displayMenuMutex);

    return 0;
}
