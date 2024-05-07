#include <GL/glut.h>
#include <stdio.h>

#define WIDTH 25
#define HEIGHT 25
#define CELL 30

char board[25][25] = {
        "#########################",
        "#.......... # ..........#",
        "#####.    #####     #####",
        "#.......................#",
        "# ####.####.#.####.####.#",
        "#.   #.#    #   .#.#    #",
        "#.####.####.#.####.####.#",
        "#.......................#",
        "#####.#  ####### .# #####",
        "#    .#     #     #     #",
        "# # #.# ###- -### # # # #",
        "# # #.# #  P    #.# # # #",
        "# # #.# #  - -  # # # # #",
        "# # #.# ######### # # # #",
        "#   #.#           # #   #",
        "#####.#  ######## # #####",
        "#.........   .. #.. ....#",
        "# ###### ###### #. ######",
        "# ..    .....        .  #",
        "# # # #.#   #   # #.#.# #",
        "# # # #.###   ### #.#.# #",
        "# #.# #. .... ....#.# # #",
        "#.# # #####.#.##### # # #",
        "#...#...............#. .#",
        "#########################",
       };



void display(){

	glClear(GL_COLOR_BUFFER_BIT);
	
  	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2i(0, 0);
	glVertex2i(WIDTH * CELL, 0);
	glVertex2i(WIDTH * CELL, HEIGHT * CELL);
	glVertex2i(0, HEIGHT * CELL);
	glEnd();

  
	glColor3f(0.1f, 0.1f, 0.5f); 

       for (int i = 0; i < HEIGHT; i++) {
       	for (int x = 0; x < WIDTH; x++) {
			if (board[i][x] == '#') {
			  	int left = x * CELL;
				int top = (HEIGHT - i) * CELL; 

				glBegin(GL_QUADS);
				glVertex2i(left, top);
				glVertex2i(left + CELL, top);
				glVertex2i(left + CELL, top - CELL);
				glVertex2i(left, top - CELL);
				glEnd();
			}
		}
	}

	glFlush();
    
}


void reshape(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WIDTH * CELL, 0, HEIGHT * CELL);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, width, height);
}


int main(int argc, char** argv) {
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WIDTH*CELL, HEIGHT*CELL);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Multithreaded PacMan");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutSpecialFunc(keyboard);
	glutMainLoop();
	return 0;

}
