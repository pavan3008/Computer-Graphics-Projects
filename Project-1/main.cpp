#include <Windows.h>
#include <freeglut.h>
#include <iostream>

void display(void);
void reshape(int,int);
void timer(int);

void Init_OpenGL()
{  
    glClearColor(0.0, 0.0, 0.0, 0.0); 
    //glShadeModel(GL_FLAT); //No color transition; GL_SMOOTH - Color Trasition occurs;
    glEnable(GL_DEPTH_TEST); 
} 
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(900, 650);
    glutCreateWindow("WELCOME TO MY WORLD - SPACE BOT");
    Init_OpenGL();
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutTimerFunc(1000,timer,0);

    glutMainLoop();
}

float angle = 0.0;

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -8.0); //Changes the location with respect to origin
    glRotatef(angle, 0.0, 1.0, 0.0); //Rotates the objects with respect to axis
    glRotatef(angle, 1.0, 0.0, 0.0);
    //glRotatef(angle, 0.0, 0.0, 1.0);
    glScalef(1.0f, 2.0f, 3.0f);//Enlarging or shrinking (Re-size)
    glColor3f(1.0, 1.0, 1.0);
    
    //Sphere Shield
    glutWireSphere(1.5, 10, 10);
    glPointSize(10.0);

    //EYE at Front
    glBegin(GL_POINTS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-0.2, -0.2, 1.0);
    glVertex3f(0.2, -0.2, 1.0);
    glEnd();

    //Smile at Front
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-0.4, 0.6, 1.0);
    glVertex3f(-0.2, 0.7, 1.0);
    glVertex3f(0.2, 0.7, 1.0);
    glVertex3f(0.4, 0.6, 1.0);
    glEnd();

    //EYE at Back
    glBegin(GL_POINTS);
    glColor3f(1.0, 1.0, -1.0);
    glVertex3f(-0.2, -0.2, -1.0);
    glVertex3f(0.2, -0.2, -1.0);
    glEnd();

    //Smile at Back
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0, 1.0, -1.0);
    glVertex3f(-0.4, 0.6, -1.0);
    glVertex3f(-0.2, 0.7, -1.0);
    glVertex3f(0.2, 0.7, -1.0);
    glVertex3f(0.4, 0.6, -1.0);
    glEnd();

    //BOX as HEAD 
    glBegin(GL_QUADS);

    //front view
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);

    //back view
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(1.0, 1.0, -1.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-1.0, 1.0, -1.0);

    //right view
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(1.0, -1.0, 1.0);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);

    //right view Windows
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(1.0, 0.5, 0.5);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(1.0, -0.5, 0.5);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(1.0, -0.5, -0.5);
    glVertex3f(1.0, 0.5, -0.5);

    //right view Inner Windows
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(1.0, 0.25, 0.25);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(1.0, -0.25, 0.25);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(1.0, -0.25, -0.5);
    glVertex3f(1.0, 0.25, -0.25);

    //left view
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glColor3f(1.0, 1.0, 0.5);
    glVertex3f(-1.0, -1.0, 1.0);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-1.0, 1.0, 1.0);

    //left view Outer Windows
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(-1.0, 0.5, -0.5);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-1.0, -0.5, -0.5);
    glColor3f(1.0, 1.0, 0.5);
    glVertex3f(-1.0, -0.5, 0.5);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-1.0, 0.5, 0.5);

    //left view Inner Windows
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(-1.0, 0.25, -0.25);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-1.0, -0.25, -0.25);
    glColor3f(1.0, 1.0, 0.5);
    glVertex3f(-1.0, -0.25, 0.25);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-1.0, 0.25, 0.25);

    //top view
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(1.0, 1.0, -1.0);

    //top view Outer Windows
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(-0.5, 1.0, -0.5);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-0.5, 1.0, 0.5);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(0.5, 1.0, 0.5);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.5, 1.0, -0.5);

    //top view Inner Windows
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(-0.25, 1.0, -0.25);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-0.25, 1.0, 0.25);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(0.25, 1.0, 0.25);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.25, 1.0, -0.25);

    //bottom view
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(1.0, -1.0, 1.0);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(1.0, -1.0, -1.0);

    //bottom view Outer Windows
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(-0.5, -1.0, -0.5);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-0.5, -1.0, 0.5);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(0.5, -1.0, 0.5);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.5, -1.0, -0.5);

    //bottom view Inner Windows
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(-0.25, -1.0, -0.25);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-0.25, -1.0, 0.25);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(0.25, -1.0, 0.25);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.25, -1.0, -0.25);

    glEnd();
    //30
    //Upper Triangles as HORNS 
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(2.0, 1.0, 1.0);
    glVertex3f(2.0, 2.0, 1.0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(1.0,1.0,-2.0);
    glVertex3f(1.0, 2.0, -2.0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-2.0, 1.0, -1.0);
    glVertex3f(-2.0, 2.0, -1.0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-1.0, 1.0, 2.0);
    glVertex3f(-1.0, 2.0, 2.0);
    glEnd();

    //Lower Traingles as HORNS
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(2.0, -1.0, 1.0);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(2.0, -2.0, 1.0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(1.0, -1.0, -2.0);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(1.0, -2.0, -2.0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-2.0, -1.0, -1.0);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(-2.0, -2.0, -1.0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-1.0, -1.0, 2.0);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(-1.0, -2.0, 2.0);
    glEnd();

    //54
    //Asteroids and Misc. stuff
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(2.5, 2.6, 3.0);
    glVertex3f(2.7, 2.7, 3.0);
    glVertex3f(2.6, 2.7, 3.0);
    glVertex3f(2.4, 2.6, 3.0);
    glVertex3f(2.7, 2.7, 3.0);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-2.5, -2.6, -3.0);
    glVertex3f(-2.7, -2.7, -3.0);
    glVertex3f(-2.6, -2.7, -3.0);
    glVertex3f(-2.4, -2.6, -3.0);
    glVertex3f(-2.7, -2.7, -3.0);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-2.5, 2.6, 3.0);
    glVertex3f(-2.7, 2.7, 3.0);
    glVertex3f(-2.6, 2.7, 3.0);
    glVertex3f(-2.4, 2.6, 3.0);
    glVertex3f(-2.7, 2.7, 3.0);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(2.5, 2.6, -3.0);
    glVertex3f(2.7, 2.7, -3.0);
    glVertex3f(2.6, 2.7, -3.0);
    glVertex3f(2.4, 2.6, -3.0);
    glVertex3f(2.7, 2.7, -3.0);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(2.5, -2.6, 3.0);
    glVertex3f(2.7, -2.7, 3.0);
    glVertex3f(2.6, -2.7, 3.0);
    glVertex3f(2.4, -2.6, 3.0);
    glVertex3f(2.5, -2.6, 3.0);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-2.5, -2.6, 3.0);
    glVertex3f(-2.7, -2.7, 3.0);
    glVertex3f(-2.6, -2.7, 3.0);
    glVertex3f(-2.4, -2.6, 3.0);
    glVertex3f(-2.7, -2.7, 3.0);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(2.5, -2.6, -3.0);
    glVertex3f(2.7, -2.7, -3.0);
    glVertex3f(2.6, -2.7, -3.0);
    glVertex3f(2.4, -2.6, -3.0);
    glVertex3f(2.7, -2.7, -3.0);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-2.5, 2.6, -3.0);
    glVertex3f(-2.7, 2.7, -3.0);
    glVertex3f(-2.6, 2.7, -3.0);
    glVertex3f(-2.4, 2.6, -3.0);
    glVertex3f(-2.7, 2.7, -3.0);
    glEnd();

    //89+5+5
    //Misc. Lines



    glutSwapBuffers(); //Flush giving blank screen; SwapBuffers double renders.
}

//Set the View
void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // Mandatory to refresh the tranformations
    gluPerspective(60.0, 1, 2.0, 50.0); // Can use Ortho2D
    glMatrixMode(GL_MODELVIEW);
}

//Set the movement
void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000/60, timer, 0);
    
    angle += 0.8;
    if (angle > 360.0) {
        angle = angle - 360.0;
    }
}

