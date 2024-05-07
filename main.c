#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>


int WIDTH = 25;
int HEIGHT = 25;
int cellSize = 30;
float PI = 3.14159265358979323846;


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

	glFlush();
    
}


void reshape(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WIDTH * cellSize, 0, HEIGHT * cellSize);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, width, height);
}


int main(int argc, char** argv) {
	
	pthread_t uiThread;
	
	pthread_create(&uiThread, NULL, &display, NULL);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WIDTH*cellSize, HEIGHT*cellSize);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Multithreaded PacMan");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutSpecialFunc(keyboard);
	glutMainLoop();
	
	pthread_join(uiThread, NULL);
	
	return 0;

}
