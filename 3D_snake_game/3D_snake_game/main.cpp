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
	view_rotx = 45.0F;
	view_roty = 0.0F;
	view_rotz = 0.0F;
	rotation = 90.0F;
}

void ManipulateViewAngle() //TODO: understand why
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

	// TODO: draw body segments
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
	}
	else
		DrawStart();

	// update screen
	glutPostRedisplay();
	glutSwapBuffers();
}

// TODO: generate food


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

	// TODO: save postions of body segments

	glutTimerFunc(130, Play, 0);
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w' :
			direction = DOWN;
			break;
		case 's':
			direction = UP;
			break;
		case 'a':
			direction = LEFT;
			break;
		case 'd':
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

	glutKeyboardFunc(keyboard);

	glutDisplayFunc(Draw);
	glutReshapeFunc(resize);

	Play(0);

	glutMainLoop();
}