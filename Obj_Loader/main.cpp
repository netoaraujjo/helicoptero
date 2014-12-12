#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <ctime>
#include "GameObject.h"
#include <GL/freeglut.h>
#define PI 3.1415
using namespace std;

GLint WIDTH = 800;
GLint HEIGHT = 600;

GLfloat obs[3]={0.0,3.5,0.0};
GLfloat look[3]={0.0,3.0,0.0};
GLfloat axisxz=0;
GLfloat radiusxz=30;

GameObject helicoptero;
GameObject helice;
GameObject torpedo1;
GameObject torpedo2;
GameObject torpedo3;
GameObject torpedo4;

GLfloat helicopteroRotate = 0.0;
GLfloat helicopteroY = 0.0;
GLfloat helicopteroX = 0.0;
GLfloat helicopteroZ = 0.0;
GLfloat heliceRotate = 0.0;


void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void special_keyboard(int key, int x, int y);
void rotacionaHelice(void);

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA | GLUT_MULTISAMPLE);

    glutInitContextVersion (3, 0);
    glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);

    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Helicoptero");
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)	{
        printf("Erro: %s", glewGetErrorString(err));
        printf("Eh necessario uma placa de video com suport a OpenGL 3.0 e Shader GLSL 1.5");
        exit(EXIT_FAILURE);
    } else {
        if (GLEW_VERSION_3_0)
        {
            printf("Driver supports OpenGL");
            printf("Using GLEW %s", glewGetString(GLEW_VERSION));
            printf("Vendor: %s", glGetString (GL_VENDOR));
            printf("Renderer: %s", glGetString (GL_RENDERER));
            printf("Version: %s", glGetString (GL_VERSION));
            printf("GLSL: %s", glGetString (GL_SHADING_LANGUAGE_VERSION));
        }
    }

    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keyboard);
    glutDisplayFunc(display);

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    glDisable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
//

    helicoptero.load("helicoptero_final.obj", "helicoptero_textura.tga", 0);
    helice.load("helice.obj", "helice.tga", 0);
    torpedo1.load("torpedo.obj", "torpedo.tga", 0);
    torpedo2.load("torpedo.obj", "torpedo.tga", 0);
    torpedo3.load("torpedo.obj", "torpedo.tga", 0);
    torpedo4.load("torpedo.obj", "torpedo.tga", 0);

    srand(time(NULL));

    glutMainLoop();

    return EXIT_SUCCESS;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	/* observer position */
    obs[0]=radiusxz*cos(2*PI*axisxz/360);
    obs[2]=radiusxz*sin(2*PI*axisxz/360);
    gluLookAt(obs[0],obs[1],obs[2],look[0],look[1],look[2],0.0,1.0,0.0);

    //glRotatef(180, 0, 1, 0);
    //glRotatef(-10, 1, 0, 0);

    glPushMatrix(); // Contem o helicoptero inteiro

        glRotatef(helicopteroRotate, 0, 1, 0);
        glTranslatef(helicopteroX, helicopteroY, helicopteroZ);

        glPushMatrix(); // Contem o corpo do helicoptero
            glTranslatef(0, 0,0);
            helicoptero.render();
        glPopMatrix();

        glPushMatrix(); // Contem a helice
            glTranslatef(0, 4, 3);
            glRotatef(heliceRotate, 0, 1, 0);
            helice.render();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(4, -3, 3);
            torpedo1.render();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(5.7, -3.1, 3);
            torpedo2.render();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-4.4, -3.1, 3);
            torpedo3.render();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-6.3, -3.1, 3);
            torpedo4.render();
        glPopMatrix();

    glPopMatrix(); // contem o helicoptero inteiro

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    float ar = (float) width / (float) height;
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 1.0, 650.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void rotacionaHelice() {
    heliceRotate += 2.0;
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
        case 27:
            exit(0);
            break;
        case 'r':
            helicopteroRotate += 2.5;
            break;
        case 'e':
            helicopteroRotate -= 2.5;
            break;
        case 'i':
            glutIdleFunc(rotacionaHelice);
            break;
        case 'I':
            glutIdleFunc(NULL);
            break;
	}
	glutPostRedisplay();
}

void special_keyboard(int key, int x, int y) {
    switch (key)
    {
        case GLUT_KEY_UP:
            helicopteroY += 0.3;
            break;
        case GLUT_KEY_DOWN:
            helicopteroY -= 0.3;
            break;
        case GLUT_KEY_LEFT:
            helicopteroZ += 0.3;
            break;
        case GLUT_KEY_RIGHT:
            helicopteroZ -= 0.3;
    }
    glutPostRedisplay();
}
