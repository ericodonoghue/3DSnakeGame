#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <GL/glut.h>

// movement detection
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define MAX_LENGTH 100
#define HEAD 0
#define _x 0
#define _z 1

// game variables
GLint points = 0;
GLbyte game_state = false;
GLint platform_width = 150;
GLint platform_length = 150;

// snake variables
GLint body_pos[MAX_LENGTH][2] = { {} }; // for each body segement x = body_pos[i][_x], z = body_pos[i][_z]. head of snake stored at body_pos[HEAD][_x/_z]
GLint prev_x[2] = {};
GLint prev_z[2] = {};
GLint direction = 0;
GLint length = 1;
GLfloat rotation = 90.0f;

// food variables
GLint food[2] = {};

// graphics variables
GLint display_width = 800;
GLint display_height = 800;
GLfloat view_rotx = 45.0F;
GLfloat view_roty = 0.0F;
GLfloat view_rotz = 0.0F;
GLfloat zoom = -300.0f;


void reset_game() 
{
	body_pos[HEAD][_x] = 5;
	body_pos[HEAD][_z] = 10;
	direction = 0;
	points = 0;
	length = 1;
	view_rotx = 45.0F;
	view_roty = 0.0F;
	view_rotz = 0.0F;
	rotation = 90.0F;
}

void ManipulateViewAngle()
{
	glRotatef(view_rotx, 1.0, 0.0, 0.0);//Rotate Arround X axis
	glRotatef(view_roty, 0.0, 1.0, 0.0);//Rotate Arround Y axis
	glRotatef(view_rotz, 0.0, 0.0, 1.0);//Rotate Arround Z axis
}
void resize(int w, int h) //TODO: understand why
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1, 800.0);
}
void WriteLine(const char* s)
{
	while (*s)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *s++);
}

void DrawStart()
{
	glClearColor(0.157f, 0.329f, 0.141f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glRasterPos2f(-40, 20);
	glColor3f(1, 1, 1);
	WriteLine("Welcome! Press enter to begin game.");
}

void DrawPlatform()
{
	glPushMatrix();
		ManipulateViewAngle();
		glColor3f(0.157f, 0.329f, 0.141f);
		glTranslatef(60.0, -16.0, 35.0);
		glScalef(platform_width, 5.0, platform_length);
		glutSolidCube(1);
	glPopMatrix();
}

void DrawSnake()
{
	glPushMatrix();
		ManipulateViewAngle();
		glColor3f(1, 0, 0);
		glTranslatef(body_pos[HEAD][_x], -10.0f, body_pos[HEAD][_z]);
		glScalef(10, 10, 10);
		glutSolidCube(1);
	glPopMatrix();

	int i;
	for (i = 1; i < length; i++) { // iteration starts at 1 as the head is stored at index 0
		glPushMatrix();
			ManipulateViewAngle();
			glColor3f(0, 1, 0);
			glTranslatef(body_pos[i][_x], -10.0f, body_pos[i][_z]);
			glScalef(9.8, 9.8, 9.8);
			glutSolidCube(1);
		glPopMatrix();
	}
}

void DrawFood()
{
	glPushMatrix();
		ManipulateViewAngle();
		glColor3f(0, 0, 1.0);
		glTranslatef(food[_x], -10.0f, food[_z]);
		glScalef(0.3, 0.3, 0.3);
		glutSolidSphere(10, 20, 20);
	glPopMatrix();
}

void Draw()
{
	glClearColor(0.36f, 0.619f, 0.98f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-60.0, 40.0, zoom);

	if (game_state) {
		DrawPlatform();
		DrawSnake();
		DrawFood();
		// TODO: draw score
	}
	else
		DrawStart();

	// update screen
	glutPostRedisplay();
	glutSwapBuffers();
}


bool check_collsion()
{
	int i;
	for (i = 0; i < length; i++)
	{
		// TODO: detect collisions with snake body
		if (body_pos[i][_x] <= -20 || body_pos[i][_x] >= platform_width - 8 || body_pos[i][_z] <= -45 || body_pos[i][_z] >= platform_length - 35)
			return true; 
	}
	return false;
}

void generate_food()
{
	if  ((body_pos[HEAD][_x] == food[_x]  && body_pos[HEAD][_z] == food[_z]) ||
		((body_pos[HEAD][_x] >= food[_x]) && (body_pos[HEAD][_x] <= food[_x] + 4) && (body_pos[HEAD][_z] >= food[_z]) && (body_pos[HEAD][_z] <= food[_z] + 4)) ||
		((body_pos[HEAD][_x] <= food[_x]) && (body_pos[HEAD][_x] >= food[_x] - 4) && (body_pos[HEAD][_z] <= food[_z]) && (body_pos[HEAD][_z] >= food[_z] - 4)) ||
		((body_pos[HEAD][_x] <= food[_x]) && (body_pos[HEAD][_x] >= food[_x] - 4) && (body_pos[HEAD][_z] >= food[_z]) && (body_pos[HEAD][_z] <= food[_z] + 4)) ||
		((body_pos[HEAD][_x] >= food[_x]) && (body_pos[HEAD][_x] <= food[_x] + 4) && (body_pos[HEAD][_z] <= food[_z]) && (body_pos[HEAD][_z] >= food[_z] - 4))) {
		points++;
		length++;

		int max; int min;
		max = platform_width - 12;
		min = -10;
		food[_x] = (rand() % (max - min)) + min;
		max = platform_length - 45;
		min = -20;
		food[_z] = (rand() % (max - min)) + min;
	}	
}

void Play(int v)
{
	prev_x[1] = body_pos[HEAD][_x];
	prev_z[1] = body_pos[HEAD][_z];

	switch (direction)
	{
		case UP:
			body_pos[HEAD][_z] += 5;
			break;
		case DOWN:
			body_pos[HEAD][_z] -= 5;
			break;
		case LEFT:
			body_pos[HEAD][_x] -= 5;
			break;
		case RIGHT:
			body_pos[HEAD][_x] += 5;
			break;
	}

	if (check_collsion())
		game_state = false;

	generate_food();

	int i;
	for (i = 1; i < length; i++) {
		prev_x[0] = prev_x[1];
		prev_z[0] = prev_z[1];
		prev_x[1] = body_pos[i][_x];
		prev_z[1] = body_pos[i][_z];
		body_pos[i][_x] = prev_x[0];
		body_pos[i][_z] = prev_z[0];
	}

	glutTimerFunc(130, Play, 0);
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w' :
			if (direction != UP)
				direction = DOWN;
			break;
		case 's':
			if (direction != DOWN)
				direction = UP;
			break;
		case 'a':
			if (direction != RIGHT)
				direction = LEFT;
			break;
		case 'd':
			if (direction != LEFT)
				direction = RIGHT;
			break;
		case 13: // enter key
			game_state = true;
			reset_game();
			glutPostRedisplay();
			break;
		case 27: // esc key
			exit(0);
			break;
		default:
			break;
	}
}

int main(int argc, char** argv)
{
	// window setup
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(display_width, display_height);
	glutInitWindowPosition(80, 80);
	glutCreateWindow("3D Snake");

	// setup background color
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.36f, 0.619f, 0.98f, 1.0f);

	// setup key detection
	glutKeyboardFunc(keyboard);

	glutDisplayFunc(Draw);
	glutReshapeFunc(resize);

	// seed random number generator
	time_t seconds;
	time(&seconds);
	srand((unsigned int)seconds);

	generate_food();
	Play(0);

	glutMainLoop();
}