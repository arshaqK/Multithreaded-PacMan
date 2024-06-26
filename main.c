#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <SOIL/SOIL.h>


const int WIDTH = 25;
const int HEIGHT = 25;
int cellSize = 30;
int score = 0;
float PI = 3.14159265358979323846;
float playerX = WIDTH / 2.0f;
float playerY = (HEIGHT / 2.0f)-2;
GLuint texture; // Add this line at the top of your file



char board[25][25] = {
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

void* display(void *arg){

	float pacmanX = playerX * cellSize;
    float pacmanY = (HEIGHT - 1 - playerY) * cellSize;

	

	glClear(GL_COLOR_BUFFER_BIT);
	
  	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2i(0, 0);
	glVertex2i(WIDTH * cellSize, 0);
	glVertex2i(WIDTH * cellSize, HEIGHT * cellSize);
	glVertex2i(0, HEIGHT * cellSize);
	glEnd();
	
	
  
	glColor3f(0.1f, 0.1f, 0.5f); 

       for (int i = 0; i < HEIGHT; i++) {
       	for (int x = 0; x < WIDTH; x++) {
			if (board[i][x] == '#') {
			  	int left = x * cellSize;
				int top = (HEIGHT - i) * cellSize; 

				glBegin(GL_QUADS);
				glVertex2i(left, top);
				glVertex2i(left + cellSize, top);
				glVertex2i(left + cellSize, top - cellSize);
				glVertex2i(left, top - cellSize);
				glEnd();
			}
			
		}
	}

	
	
	
	glColor3f(1.3f, 1.3f, 0.2f); 
	float foodRadius = cellSize / 7.0f;

	for (int i = 0; i < HEIGHT; i++) {
		for (int x = 0; x < WIDTH; x++) {	
			if (board[i][x] == '.') {				    
				float centerX = (x * cellSize) + (cellSize / 2.0f);
				float centerY = (HEIGHT - 1 - i) * cellSize + (cellSize / 2.0f); // Corrected for top-down display
				int numVertices = 30; // Adjust for circle smoothness

				glBegin(GL_POLYGON);
				for (int i = 0; i < numVertices; i++) {
				    float angle = 2 * PI * i / numVertices;
				    float xPos = centerX + foodRadius * cos(angle);
				    float yPos = centerY + foodRadius * sin(angle);
				    glVertex2f(xPos, yPos);
				}
				glEnd();		
			}
		}
	}

	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
    // Adjust texture coordinates as needed
    glTexCoord2f(0.0f, 0.0f); glVertex2f(pacmanX, pacmanY);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(pacmanX + cellSize, pacmanY);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(pacmanX + cellSize, pacmanY - cellSize);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(pacmanX, pacmanY - cellSize);
    glEnd();

    glDisable(GL_TEXTURE_2D);


	glFlush();

}
int isWall(float x, float y) {
    int gridX = (int)(x);
    int gridY = (int)(y);

    if (gridX < 0 || gridX >= WIDTH || gridY < 0 || gridY >= HEIGHT) {
        printf("Out of bounds\n");
        return 1; 
    }

    return board[HEIGHT - 1 - gridY][gridX] == '#';
}




void reshape(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WIDTH * cellSize, 0, HEIGHT * cellSize);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, width, height);
}

void *gameEngine(void *arg){

	while(1){
		pthread_t uiThread;
		pthread_create(&uiThread, NULL, &display, NULL);
		//pthread_join(uiThread, NULL);


		sleep(1000);
	}
}

void keyboard(int key, int x, int y) {
    float nextX = playerX;
    float nextY = playerY;

    switch (key) {
        case GLUT_KEY_RIGHT:
            nextX += 0.2f;
            break;
        case GLUT_KEY_LEFT:
            nextX -= 0.2f;
            break;
        case GLUT_KEY_DOWN:
            nextY += 0.2f;
            break;
        case GLUT_KEY_UP:
            nextY -= 0.2f;
            break;
    }

    // Check if the new position hits a wall
    if (!isWall(nextX, nextY)) {
        playerX = nextX;
        playerY = nextY;
    }
    
    glutPostRedisplay();
}


void initOpenGL(){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 300, 300, 0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 300, 300, 0);
    glMatrixMode(GL_MODELVIEW);

    
    int width, height;
    unsigned char* image = SOIL_load_image("pacman.png", &width, &height, 0, SOIL_LOAD_RGB);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SOIL_free_image_data(image);
}

int main(int argc, char** argv) {
	
	pthread_t game_engine;
	
	pthread_create(&game_engine, NULL, &gameEngine, NULL);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WIDTH*cellSize, HEIGHT*cellSize);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Multithreaded PacMan");
	initOpenGL();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(keyboard);
	glutMainLoop();

	
	
	//pthread_join(game_engine, NULL);
	
	
	return 0;

}
