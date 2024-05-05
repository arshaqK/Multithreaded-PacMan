#include <GL/glut.h>
#include <stdio.h>

#define BOARD_WIDTH 25
#define BOARD_HEIGHT 25
#define CELL_SIZE 30

float x = 50.0f;
float y = 50.0f;
float side = 50.0f;



char board[25][25] = {
        "#########################",
        "#..........##........####",
        "#.######.##.########.##.#",
        "#.#  #     ##    # ### .#",
        "#.#  #####    ###  . ####",
        "#.#      ####          .#",
        "#.####.########.####.####",
        "#...........###.........#",
        "#.####.##.#######.#### .#",
        "#.#   ##.#  ### ###   #.#",
        "#.#.####.#  ### #####.#.#",
        "#.#......########.....#.#",
        "#.##.#.###########.#.##.#",
        "#...#.#   #####  #.#. ..#",
        "###.#.##### ##########.##",
        "#   .       #####    .  #",
        "###.#.##.########### #.##",
        "#...#.##.## ####### ... #",
        "#.#.####.## ###  ######.#",
        "#.#...##.##########.....#",
        "#.#####......###..#####.#",
        "#......##.#####.###.....#",
        "#.#####.#      ###.####.#",
        "#.#      ###### ##    #.#",
        "#########################",
       };



void display(){

	glClear(GL_COLOR_BUFFER_BIT);
	
  	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2i(0, 0);
	glVertex2i(BOARD_WIDTH * CELL_SIZE, 0);
	glVertex2i(BOARD_WIDTH * CELL_SIZE, BOARD_HEIGHT * CELL_SIZE);
	glVertex2i(0, BOARD_HEIGHT * CELL_SIZE);
	glEnd();

  
	glColor3f(0.2f, 0.1f, 0.5f); 

       for (int y = 0; y < BOARD_HEIGHT; y++) {
       	for (int x = 0; x < BOARD_WIDTH; x++) {
			if (board[y][x] == '#') {
			  	int left = x * CELL_SIZE;
				int top = (BOARD_HEIGHT - y) * CELL_SIZE; 

				glBegin(GL_QUADS);
				glVertex2i(left, top);
				glVertex2i(left + CELL_SIZE, top);
				glVertex2i(left + CELL_SIZE, top - CELL_SIZE);
				glVertex2i(left, top - CELL_SIZE);
				glEnd();
			}
		}
	}

	glFlush();
    
  }
void keyboard(int key, int xx, int yy) {
	switch (key) {
		case GLUT_KEY_RIGHT:
			x+= 10;
			break;
		case GLUT_KEY_LEFT:
			x -= 10;
			break;
		case GLUT_KEY_UP:
			y += 10;
			break;
		case GLUT_KEY_DOWN:
			y -= 10;
			break;
			
	}
	glutPostRedisplay();
}

void reshape(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, BOARD_WIDTH * CELL_SIZE, 0, BOARD_HEIGHT * CELL_SIZE);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, width, height);
}


int main(int argc, char** argv) {
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(BOARD_WIDTH*CELL_SIZE, BOARD_HEIGHT*CELL_SIZE);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Multithreaded PacMan");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(keyboard);
	glutMainLoop();
	return 0;

}
