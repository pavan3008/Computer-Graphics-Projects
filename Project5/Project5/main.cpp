#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define _USE_MATH_DEFINES
#include <math.h>


#ifdef WIN32
#include <windows.h>
#pragma warning(disable : 4996)
#include "glew.h"
#include<freeglut.h>
#endif

#define TRUE 1
#define FALSE 0
#define PI 3.14159265

#include "glslprogram.h"

#define MS_PER_CYCLE 5000
#define ESCAPE 0x1b

const char* WINDOWTITLE = { "Project 5" };
const char* GLUITITLE = { "This is GLUI" };

//glui package defines as true and false:
const int GLUITRUE = { true };
const int GLUIFALSE = { false };

//window size:
const int INIT_WINDOW_SIZE = { 600 };

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };
const float MINSCALE = { 0.05f };
const int LEFT = { 4 };
const int MIDDLE = { 2 };
const int RIGHT = { 1 };

// projections:

enum Projections
{
	ORTHO,
	PERSP
};

// Buttons:

enum ButtonVals
{
	RESET,
	QUIT
};

const GLfloat BACKCOLOR[] = { 0., 0., 0., 1. };
const GLfloat AXES_WIDTH = { 3. };
enum Colors
{
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA,
	WHITE,
	BLACK
};

char* ColorNames[] =
{
	(char *) "Red",
	(char*) "Yellow",
	(char*) "Green",
	(char*) "Cyan",
	(char*) "Blue",
	(char*) "Magenta",
	(char*) "White",
	(char*) "Black" };

const GLfloat Colors[][3] =
{
	{1., 0., 0.}, // red
	{1., 1., 0.}, // yellow
	{0., 1., 0.}, // green
	{0., 1., 1.}, // cyan
	{0., 0., 1.}, // blue
	{1., 0., 1.}, // magenta
	{1., 1., 1.}, // white
	{0., 0., 0.}, // black
};

// fog parameters:
const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum FOGMODE = { GL_LINEAR };
const GLfloat FOGDENSITY = { 0.30f };
const GLfloat FOGSTART = { 1.5 };
const GLfloat FOGEND = { 4. };

// non-constant global variables:

int ActiveButton;	// current button that is down
GLuint AxesList;	 // list to hold the axes
int AxesOn;			 // != 0 means to draw the axes
int DebugOn;		 // != 0 means to print debugging info
int DepthCueOn;		 // != 0 means to use intensity depth cueing
GLuint RoadList;	 // object display list
int MainWindow;		 // window id for main graphics window
float Scale;		 // scaling factor
int WhichColor;		 // index into Colors[ ]
int WhichProjection; // ORTHO or PERSP
int Xmouse, Ymouse;  // mouse values
float Xrot, Yrot;	// rotation angles in degrees
float uSize;
float Time = 0.;
bool Freeze = TRUE;
int AnimateFragment = 1;
int AnimateVertex = 1;

GLSLProgram* Pattern;

float White[] = { 1., 1., 1., 1. };

void Animate();
void Display();
void DoAxesMenu(int);
void DoColorMenu(int);
void DoDepthMenu(int);
void DoDebugMenu(int);
void DoMainMenu(int);
void DoProjectMenu(int);
void DoRasterString(float, float, float, char*);
void DoStrokeString(float, float, float, float, char*);
float ElapsedSeconds();
void InitGraphics();
void InitLists();
void InitMenus();
void Keyboard(unsigned char, int, int);
void MouseButton(int, int, int, int);
void MouseMotion(int, int);
void Reset();
void Resize(int, int);
void Visibility(int);

void Axes(float);
void HsvRgb(float[3], float[3]);
float Dot(float[3], float[3]);
void Cross(float[3], float[3], float[3]);
float Unit(float[3], float[3]);

float* Array3(float, float, float);
float* MulArray3(float, float[3]);
void SetMaterial(float, float, float, float);
void SetPointLight(int, float, float, float, float, float, float);
void SetSpotLight(int, float, float, float, float, float, float, float, float, float);

float Dot(float v1[3], float v2[3])
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}
void Cross(float v1[3], float v2[3], float vout[3])
{
	float tmp[3];
	tmp[0] = v1[1] * v2[2] - v2[1] * v1[2];
	tmp[1] = v2[0] * v1[2] - v1[0] * v2[2];
	tmp[2] = v1[0] * v2[1] - v2[0] * v1[1];
	vout[0] = tmp[0];
	vout[1] = tmp[1];
	vout[2] = tmp[2];
}
float Unit(float vin[3], float vout[3])
{
	float dist = vin[0] * vin[0] + vin[1] * vin[1] + vin[2] * vin[2];
	if (dist > 0.0)
	{
		dist = sqrt(dist);
		vout[0] = vin[0] / dist;
		vout[1] = vin[1] / dist;
		vout[2] = vin[2] / dist;
	}
	else
	{
		vout[0] = vin[0];
		vout[1] = vin[1];
		vout[2] = vin[2];
	}
	return dist;
}

float*
Array3(float a, float b, float c)
{
	static float array[4];
	array[0] = a;
	array[1] = b;
	array[2] = c;
	array[3] = 1.;
	return array;
}

float*
MulArray3(float factor, float array0[3])
{
	static float array[4];
	array[0] = factor * array0[0];
	array[1] = factor * array0[1];
	array[2] = factor * array0[2];
	array[3] = 1.;
	return array;
}

void SetMaterial(float r, float g, float b, float shininess)
{
	glMaterialfv(GL_BACK, GL_EMISSION, Array3(0., 0., 0.));
	glMaterialfv(GL_BACK, GL_AMBIENT, MulArray3(.4f, White));
	glMaterialfv(GL_BACK, GL_DIFFUSE, MulArray3(1., White));
	glMaterialfv(GL_BACK, GL_SPECULAR, Array3(0., 0., 0.));
	glMaterialf(GL_BACK, GL_SHININESS, 2.f);
	glMaterialfv(GL_FRONT, GL_EMISSION, Array3(0., 0., 0.));
	glMaterialfv(GL_FRONT, GL_AMBIENT, Array3(r, g, b));
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Array3(r, g, b));
	glMaterialfv(GL_FRONT, GL_SPECULAR, MulArray3(.8f, White));
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void SetPointLight(int ilight, float x, float y, float z, float r, float g, float b)
{
	glLightfv(ilight, GL_POSITION, Array3(x, y, z));
	glLightfv(ilight, GL_AMBIENT, Array3(0., 0., 0.));
	glLightfv(ilight, GL_DIFFUSE, Array3(r, g, b));
	glLightfv(ilight, GL_SPECULAR, Array3(r, g, b));
	glLightf(ilight, GL_CONSTANT_ATTENUATION, 1.);
	glLightf(ilight, GL_LINEAR_ATTENUATION, 0.5);
	glLightf(ilight, GL_QUADRATIC_ATTENUATION, 0.);
	glEnable(ilight);
}

void SetSpotLight(int ilight, float x, float y, float z, float xdir, float ydir, float zdir, float r, float g, float b)
{
	glLightfv(ilight, GL_POSITION, Array3(x, y, z));
	glLightfv(ilight, GL_SPOT_DIRECTION, Array3(xdir, ydir, zdir));
	glLightf(ilight, GL_SPOT_EXPONENT, 1.);
	glLightf(ilight, GL_SPOT_CUTOFF, 20.);
	glLightfv(ilight, GL_AMBIENT, Array3(0., 0., 0.));
	glLightfv(ilight, GL_DIFFUSE, Array3(r, g, b));
	glLightfv(ilight, GL_SPECULAR, Array3(r, g, b));
	glLightf(ilight, GL_CONSTANT_ATTENUATION, 1.);
	glLightf(ilight, GL_LINEAR_ATTENUATION, 0.);
	glLightf(ilight, GL_QUADRATIC_ATTENUATION, 0.1);
	glEnable(ilight);
}

int main(int argc, char* argv[])
{
	
	glutInit(&argc, argv);
	InitGraphics();

	InitLists();

	Reset();

	InitMenus();

	glutSetWindow(MainWindow);
	glutMainLoop();
	return 0;
}

void Animate()
{
	int ms = glutGet(GLUT_ELAPSED_TIME);
	ms %= MS_PER_CYCLE;
	Time = (float)ms / (float)(MS_PER_CYCLE - 1);

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void Display()
{
	if (DebugOn != 0)
	{
		fprintf(stderr, "Display\n");
	}

	glutSetWindow(MainWindow);

	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_FLAT);

	GLsizei vx = glutGet(GLUT_WINDOW_WIDTH);
	GLsizei vy = glutGet(GLUT_WINDOW_HEIGHT);
	GLsizei v = vx < vy ? vx : vy;
	GLint xl = (vx - v) / 2;
	GLint yb = (vy - v) / 2;
	glViewport(xl, yb, v, v);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (WhichProjection == ORTHO)
		glOrtho(-3., 3., -3., 3., 0.1, 1000.);
	else
		gluPerspective(90., 1., 0.1, 1000.);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0., 7., 10., 0., 0., 0., 0., 1., 0.);
	glRotatef((GLfloat)Yrot, 0., 1., 0.);
	glRotatef((GLfloat)Xrot, 1., 0., 0.);

	if (Scale < MINSCALE)
		Scale = MINSCALE;
	glScalef((GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale);

	// set the fog parameters:

	if (DepthCueOn != 0)
	{
		glFogi(GL_FOG_MODE, FOGMODE);
		glFogfv(GL_FOG_COLOR, FOGCOLOR);
		glFogf(GL_FOG_DENSITY, FOGDENSITY);
		glFogf(GL_FOG_START, FOGSTART);
		glFogf(GL_FOG_END, FOGEND);
		glEnable(GL_FOG);
	}
	else
	{
		glDisable(GL_FOG);
	}


	if (AxesOn != 0)
	{
		glColor3fv(&Colors[WhichColor][0]);
		glCallList(AxesList);
	}


	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);

	float S0, T0;
	float Ds, Dt;
	float V0, V1, V2;
	float ColorR, ColorG, ColorB;
	float SColorR, SColorG, SColorB;
	float uKa, uKd, uKs;
	float pointx, pointy, pointz;
	float maxdist;

	S0 = 0.;
	T0 = 0.;
	ColorR = 1.0;
	ColorG = 1.0;
	ColorB = 1.0;
	SColorR = 0.0;
	SColorG = 0.5;
	SColorB = 0.0;
	uKa = 0.7;
	uKd = 0.4;
	uKs = 1.0;
	uSize = 1.5;
	pointx = cos(Time * 20) * 3;
	pointy = cos(Time * 15) * 3 ;
	pointz = 2.;
	maxdist = 4.;

	Pattern->Use();
	Pattern->SetUniformVariable((char *) "uS0", S0);
	Pattern->SetUniformVariable((char*) "uT0", T0);
	Pattern->SetUniformVariable((char*)"uColor", ColorR, ColorG, ColorB);
	Pattern->SetUniformVariable((char*) "uSize", uSize);
	Pattern->SetUniformVariable((char*) "uTime", Time);
	Pattern->SetUniformVariable((char*) "uSpecularColor", SColorR, SColorG, SColorB);
	Pattern->SetUniformVariable((char*) "uKa", uKa);
	Pattern->SetUniformVariable((char*) "uKd", uKd);
	Pattern->SetUniformVariable((char*) "uKs", uKs);
	Pattern->SetUniformVariable((char*) "fragPattern", AnimateFragment);
	Pattern->SetUniformVariable((char*) "vertPattern", AnimateVertex);

	//object:
	glShadeModel(GL_FLAT);
	glPushMatrix();
	SetMaterial(1.0, 1.0, 0.0, 1.);
	glutSolidTeapot(2.5);
	//glutSolidTeacup(4);
	//glutSolidTorus(1, 3, 80, 80);
	//glutSolidRhombicDodecahedron();
	//glutSolidCylinder(3, 6, 80, 80);
	glPopMatrix();

	Pattern->Use(0);

	glutSwapBuffers();

	glFlush();
}

void DoAxesMenu(int id)
{
	AxesOn = id;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void DoColorMenu(int id)
{
	WhichColor = id - RED;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void DoDebugMenu(int id)
{
	DebugOn = id;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void DoDepthMenu(int id)
{
	DepthCueOn = id;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void DoMainMenu(int id)
{
	switch (id)
	{
	case RESET:
		Reset();
		break;

	case QUIT:
		glutSetWindow(MainWindow);
		glFinish();
		glutDestroyWindow(MainWindow);
		exit(0);
		break;

	default:
		fprintf(stderr, "404 %d\n", id);
	}

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void DoProjectMenu(int id)
{
	WhichProjection = id;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}


void DoRasterString(float x, float y, float z, char* s)
{
	glRasterPos3f((GLfloat)x, (GLfloat)y, (GLfloat)z);

	char c;
	for (; (c = *s) != '\0'; s++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
}


void DoStrokeString(float x, float y, float z, float ht, char* s)
{
	glPushMatrix();
	glTranslatef((GLfloat)x, (GLfloat)y, (GLfloat)z);
	float sf = ht / (119.05f + 33.33f);
	glScalef((GLfloat)sf, (GLfloat)sf, (GLfloat)sf);
	char c; 
	for (; (c = *s) != '\0'; s++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
	}
	glPopMatrix();
}


float ElapsedSeconds()
{
	int ms = glutGet(GLUT_ELAPSED_TIME);
	return (float)ms / 1000.f;
}


void InitMenus()
{
	glutSetWindow(MainWindow);

	int numColors = sizeof(Colors) / (3 * sizeof(int));
	int colormenu = glutCreateMenu(DoColorMenu);
	for (int i = 0; i < numColors; i++)
	{
		glutAddMenuEntry(ColorNames[i], i);
	}

	int axesmenu = glutCreateMenu(DoAxesMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int depthcuemenu = glutCreateMenu(DoDepthMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int debugmenu = glutCreateMenu(DoDebugMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int projmenu = glutCreateMenu(DoProjectMenu);
	glutAddMenuEntry("Orthographic", ORTHO);
	glutAddMenuEntry("Perspective", PERSP);

	int mainmenu = glutCreateMenu(DoMainMenu);
	glutAddSubMenu("Axes", axesmenu);
	glutAddSubMenu("Colors", colormenu);
	glutAddSubMenu("Depth Cue", depthcuemenu);
	glutAddSubMenu("Projection", projmenu);
	glutAddMenuEntry("Reset", RESET);
	glutAddSubMenu("Debug", debugmenu);
	glutAddMenuEntry("Quit", QUIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void InitGraphics()
{

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(INIT_WINDOW_SIZE, INIT_WINDOW_SIZE);


	MainWindow = glutCreateWindow(WINDOWTITLE);
	glutSetWindowTitle(WINDOWTITLE);

	// set the framebuffer clear values:

	glClearColor(0., .7, 0., BACKCOLOR[3]);

	// setup the callback functions:
	// DisplayFunc -- redraw the window
	// ReshapeFunc -- handle the user resizing the window
	// KeyboardFunc -- handle a keyboard input
	// MouseFunc -- handle the mouse button going down or up
	// MotionFunc -- handle the mouse moving with a button down
	// PassiveMotionFunc -- handle the mouse moving with a button up
	// VisibilityFunc -- handle a change in window visibility
	// EntryFunc -- handle the cursor entering or leaving the window
	// SpecialFunc -- handle special keys on the keyboard
	// SpaceballMotionFunc -- handle spaceball translation
	// SpaceballRotateFunc -- handle spaceball rotation
	// SpaceballButtonFunc -- handle spaceball button hits
	// ButtonBoxFunc -- handle button box hits
	// DialsFunc -- handle dial rotations
	// TabletMotionFunc -- handle digitizing tablet motion
	// TabletButtonFunc -- handle digitizing tablet button hits
	// MenuStateFunc -- declare when a pop-up menu is in use
	// TimerFunc -- trigger something to happen a certain time from now
	// IdleFunc -- what to do when nothing else is going on

	glutSetWindow(MainWindow);
	glutDisplayFunc(Display);
	glutReshapeFunc(Resize);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(NULL);
	glutVisibilityFunc(Visibility);
	glutEntryFunc(NULL);
	glutSpecialFunc(NULL);
	glutSpaceballMotionFunc(NULL);
	glutSpaceballRotateFunc(NULL);
	glutSpaceballButtonFunc(NULL);
	glutButtonBoxFunc(NULL);
	glutDialsFunc(NULL);
	glutTabletMotionFunc(NULL);
	glutTabletButtonFunc(NULL);
	glutMenuStateFunc(NULL);
	glutTimerFunc(-1, NULL, 0);
	glutIdleFunc(Animate);

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "glewInit Error\n");
	}
	else
		fprintf(stderr, "GLEW initialized OK\n");
	fprintf(stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	Pattern = new GLSLProgram();
	bool valid = Pattern->Create((char *) "pattern.vert", (char*)"pattern.frag");
	if (!valid)
	{
		fprintf(stderr, "Shader cannot be created!\n");
		DoMainMenu(QUIT);
	}
	else
	{
		fprintf(stderr, "Shader created.\n");
	}
	Pattern->SetVerbose(false);
}

void InitLists()
{
	glutSetWindow(MainWindow);

	// create the axes:
	AxesList = glGenLists(1);
	glNewList(AxesList, GL_COMPILE);
	glPushMatrix();
	glLineWidth(AXES_WIDTH);
	Axes(1.5);
	glLineWidth(1.);
	glPopMatrix();
	glEndList();
}

// the keyboard callback:

void Keyboard(unsigned char c, int x, int y)
{
	if (DebugOn != 0)
		fprintf(stderr, "Keyboard: '%c' (0x%0x)\n", c, c);

	switch (c)
	{
	case 'o':
	case 'O':
		WhichProjection = ORTHO;
		break;
	case 'p':
	case 'P':
		WhichProjection = PERSP;
		break;
	case 's':
	case 'S':
		Freeze = !(Freeze); //Freezing the entire animation
		if (Freeze)
			glutIdleFunc(NULL);
		else
			glutIdleFunc(Animate);

	case 'b':					// Enable both animations
		AnimateVertex = TRUE;
		AnimateFragment = TRUE;
		break;
	case 'f':					// Freeze both animations
		AnimateVertex = FALSE;
		AnimateFragment = FALSE;
		break;
	case 'F':					// Enable fragment animation and freeze vertex animation
		AnimateFragment = TRUE;
		AnimateVertex = FALSE;
		break;
	case 'V':					// Enable vertex animation and freeze fragment animation
		AnimateVertex = TRUE;
		AnimateFragment = FALSE;
		break;

	case 'q':
	case 'Q':
	case ESCAPE:
		DoMainMenu(QUIT);
		break;

	default:
		fprintf(stderr, "404 (0x%0x)\n", c);
	}

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

//Mouse buttons:

void MouseButton(int button, int state, int x, int y)
{
	int b = 0; // LEFT, MIDDLE, or RIGHT

	if (DebugOn != 0)
		fprintf(stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y);

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		b = LEFT;
		break;

	case GLUT_MIDDLE_BUTTON:
		b = MIDDLE;
		break;

	case GLUT_RIGHT_BUTTON:
		b = RIGHT;
		break;

	case 3: // Scroll up
		Scale += SCLFACT * 2;

		if (Scale < MINSCALE)
			Scale = MINSCALE;
		glutSetWindow(MainWindow);
		glutPostRedisplay();
		break;

	case 4: // Scroll down
		Scale -= SCLFACT * 2;

		if (Scale < MINSCALE)
			Scale = MINSCALE;
		glutSetWindow(MainWindow);
		glutPostRedisplay();
		break;

	default:
		b = 0;
		fprintf(stderr, "Unknown mouse button: %d\n", button);
	}

	if (state == GLUT_DOWN)
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b; // set the proper bit
	}
	else
	{
		ActiveButton &= ~b; // clear the proper bit
	}
}

void MouseMotion(int x, int y)
{
	if (DebugOn != 0)
		fprintf(stderr, "MouseMotion: %d, %d\n", x, y);

	int dx = x - Xmouse; // change in mouse coords
	int dy = y - Ymouse;

	if ((ActiveButton & LEFT) != 0)
	{
		Xrot += (ANGFACT * dy);
		Yrot += (ANGFACT * dx);
	}

	if ((ActiveButton & MIDDLE) != 0)
	{
		Scale += SCLFACT * (float)(dx - dy);

		if (Scale < MINSCALE)
			Scale = MINSCALE;
	}

	Xmouse = x;
	Ymouse = y;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void Reset()
{
	ActiveButton = 0;
	AxesOn = 0;
	DebugOn = 0;
	DepthCueOn = 0;
	Scale = 2.0;
	WhichColor = WHITE;
	WhichProjection = PERSP;
	Xrot = Yrot = 0.;
}

void Resize(int width, int height)
{
	if (DebugOn != 0)
		fprintf(stderr, "ReSize: %d, %d\n", width, height);

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void Visibility(int state)
{
	if (DebugOn != 0)
		fprintf(stderr, "Visibility: %d\n", state);

	if (state == GLUT_VISIBLE)
	{
		glutSetWindow(MainWindow);
		glutPostRedisplay();
	}
}

static float xx[] = {
	0.f, 1.f, 0.f, 1.f };

static float xy[] = {
	-.5f, .5f, .5f, -.5f };

static int xorder[] = {
	1, 2, -3, 4 };

static float yx[] = {
	0.f, 0.f, -.5f, .5f };

static float yy[] = {
	0.f, .6f, 1.f, 1.f };

static int yorder[] = {
	1, 2, 3, -2, 4 };

static float zx[] = {
	1.f, 0.f, 1.f, 0.f, .25f, .75f };

static float zy[] = {
	.5f, .5f, -.5f, -.5f, 0.f, 0.f };

static int zorder[] = {
	1, 2, 3, 4, -5, 6 };

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

// Draw a set of 3D axes:
void Axes(float length)
{
	glBegin(GL_LINE_STRIP);
	glVertex3f(length, 0., 0.);
	glVertex3f(0., 0., 0.);
	glVertex3f(0., length, 0.);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(0., 0., 0.);
	glVertex3f(0., 0., length);
	glEnd();

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 4; i++)
	{
		int j = xorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(base + fact * xx[j], fact * xy[j], 0.0);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 5; i++)
	{
		int j = yorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(fact * yx[j], base + fact * yy[j], 0.0);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 6; i++)
	{
		int j = zorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(0.0, fact * zy[j], base + fact * zx[j]);
	}
	glEnd();
}

void HsvRgb(float hsv[3], float rgb[3])
{
	float h = hsv[0] / 60.f;
	while (h >= 6.)
		h -= 6.;
	while (h < 0.)
		h += 6.;

	float s = hsv[1];
	if (s < 0.)
		s = 0.;
	if (s > 1.)
		s = 1.;

	float v = hsv[2];
	if (v < 0.)
		v = 0.;
	if (v > 1.)
		v = 1.;

	// if sat==0, then is a gray:

	if (s == 0.0)
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}

	// get an rgb from the hue itself:

	float i = floor(h);
	float f = h - i;
	float p = v * (1.f - s);
	float q = v * (1.f - s * f);
	float t = v * (1.f - (s * (1.f - f)));

	float r, g, b; // red, green, blue
	switch ((int)i)
	{
	case 0:
		r = v;
		g = t;
		b = p;
		break;

	case 1:
		r = q;
		g = v;
		b = p;
		break;

	case 2:
		r = p;
		g = v;
		b = t;
		break;

	case 3:
		r = p;
		g = q;
		b = v;
		break;

	case 4:
		r = t;
		g = p;
		b = v;
		break;

	case 5:
		r = v;
		g = p;
		b = q;
		break;
	}

	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}