#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <ctime>
#include "GameObject.h"
#include <GL/freeglut.h>
#define PI 3.1415

#define ON 1
#define OFF 0

using namespace std;

struct Torpedo {
    GameObject torpedo;
    GLint disparado;
    GLfloat translateZ;
    GLfloat translateY;
    GLfloat rotateY;
};

GLint WIDTH = 800;
GLint HEIGHT = 600;

GLfloat obs[3]={0.0,3.5,0.0};
GLfloat look[3]={0.0,3.0,0.0};
GLfloat axisxz=0;
GLfloat radiusxz=30;

/* Controle do helicóptero */
GameObject helicoptero;
GLfloat helicopteroRotateY = 0.0;
GLfloat helicopteroY = 0.0;
GLfloat helicopteroX = 0.0;
GLfloat helicopteroZ = 0.0;

/* Controle dos torpedoss */
struct Torpedo torpedo1;
struct Torpedo torpedo2;
struct Torpedo torpedo3;
struct Torpedo torpedo4;
GLint numTorpedoEsquerda = 0;
GLint numTorpedoDireita = 0;

/* Controle da helice */
GameObject helice;
GLfloat heliceRotate = 0.0;
GLfloat heliceRotateIncrement = 0.0;
GLint heliceState = OFF;

GameObject solo;

void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void special_keyboard(int key, int x, int y);
void controlaAnimacoes(void);

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

    glutIdleFunc(controlaAnimacoes);

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

    torpedo1.translateZ = 3.0;
    torpedo2.translateZ = 3.0;
    torpedo3.translateZ = 3.0;
    torpedo4.translateZ = 3.0;

    torpedo1.translateY = -3.0;
    torpedo2.translateY = -3.0;
    torpedo3.translateY = -3.0;
    torpedo4.translateY = -3.0;

    torpedo1.rotateY = 0.0;
    torpedo2.rotateY = 0.0;
    torpedo3.rotateY = 0.0;
    torpedo4.rotateY = 0.0;

    torpedo1.disparado = OFF;
    torpedo2.disparado = OFF;
    torpedo3.disparado = OFF;
    torpedo4.disparado = OFF;

    helicoptero.load("helicoptero_final.obj", "helicoptero_textura.tga", 0);
    helice.load("helice.obj", "helice.tga", 0);
    torpedo1.torpedo.load("torpedo.obj", "torpedo.tga", 0);
    torpedo2.torpedo.load("torpedo.obj", "torpedo.tga", 0);
    torpedo3.torpedo.load("torpedo.obj", "torpedo.tga", 0);
    torpedo4.torpedo.load("torpedo.obj", "torpedo.tga", 0);
    solo.load("piso.obj", "piso.tga", 0);

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

    // Solo
    glPushMatrix();
        glTranslatef(0, -5.2, 0);
        glScalef(2.0, 1.0, 5.5);
        glRotatef(-19, 1, 0, 0);
        solo.render();
    glPopMatrix();

    glPushMatrix(); // Contem o helicoptero inteiro

        glTranslatef(helicopteroX, helicopteroY, helicopteroZ);
        glRotatef(helicopteroRotateY, 0, 1, 0);

        glPushMatrix(); // Contem o corpo do helicoptero
            helicoptero.render();
        glPopMatrix();

        glPushMatrix(); // Contem a helice
            glTranslatef(0, 4, 3);
            glRotatef(heliceRotate, 0, 1, 0);
            helice.render();
        glPopMatrix();

    glPopMatrix(); // contem o helicoptero inteiro

    // torpedo 1
    glPushMatrix();
        //glRotatef(torpedo1.rotateY, 0, 1, 0);
        glTranslatef(4, torpedo1.translateY, torpedo1.translateZ);
        torpedo1.torpedo.render();
    glPopMatrix();

    // torpedo 2
    glPushMatrix();
        //glRotatef(torpedo2.rotateY, 0, 1, 0);
        glTranslatef(5.7, torpedo2.translateY, torpedo2.translateZ);
        torpedo1.torpedo.render();
    glPopMatrix();

    // torpedo 3
    glPushMatrix();
        //glRotatef(torpedo3.rotateY, 0, 1, 0);
        glTranslatef(-4.4, torpedo3.translateY, torpedo3.translateZ);
        torpedo3.torpedo.render();
    glPopMatrix();

    // torpedo 4
    glPushMatrix();
        //glRotatef(torpedo4.rotateY, 0, 1, 0);
        glTranslatef(-6.3, torpedo4.translateY, torpedo4.translateZ);
        torpedo4.torpedo.render();
    glPopMatrix();

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

void controlaAnimacoes() {
    if (heliceState == ON) {
        heliceRotate += heliceRotateIncrement;
        if (heliceRotateIncrement < 30.0) {
            heliceRotateIncrement += 0.05;
        }
    }

    if (numTorpedoEsquerda > 0) {
        torpedo2.translateZ += 0.1;
    }
    if (numTorpedoEsquerda > 1) {
        torpedo1.translateZ += 0.1;
    }
    if (numTorpedoDireita > 0) {
        torpedo4.translateZ += 0.1;
    }
    if (numTorpedoDireita > 1) {
        torpedo3.translateZ += 0.1;
    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
        case 27:
            exit(0);
            break;
        case 'd':
            helicopteroRotateY += 2.5;
            if (torpedo1.disparado == OFF) torpedo1.rotateY += 2.5;
            if (torpedo2.disparado == OFF) torpedo2.rotateY += 2.5;
            if (torpedo3.disparado == OFF) torpedo3.rotateY += 2.5;
            if (torpedo4.disparado == OFF) torpedo4.rotateY += 2.5;
            break;
        case 'a':
            helicopteroRotateY -= 2.5;
            if (torpedo1.disparado == OFF) torpedo1.rotateY -= 2.5;
            if (torpedo2.disparado == OFF) torpedo2.rotateY -= 2.5;
            if (torpedo3.disparado == OFF) torpedo3.rotateY -= 2.5;
            if (torpedo4.disparado == OFF) torpedo4.rotateY -= 2.5;
            break;
        case 'i':
            heliceState = ON;
            break;
        case 'I':
            if (helicopteroY == 0) {
                heliceRotate = 0.0;
                heliceRotateIncrement = 0.0;
                heliceState = OFF;
            }
            break;
        case 't':
            if (numTorpedoEsquerda < 2) {
                numTorpedoEsquerda++;
                if (numTorpedoEsquerda == 1) {
                    torpedo2.disparado = ON;
                } else {
                    torpedo1.disparado = ON;
                }
            }
            break;
        case 'T':
            if (numTorpedoDireita < 2) {
                numTorpedoDireita++;
                if (numTorpedoDireita == 1) {
                    torpedo4.disparado = ON;
                } else {
                    torpedo3.disparado = ON;
                }
            }
            break;
	}
	glutPostRedisplay();
}

void special_keyboard(int key, int x, int y) {
    switch (key)
    {
        case GLUT_KEY_UP:
            if (heliceState == ON) {
                if (torpedo1.disparado == OFF) torpedo1.translateY += 0.3;
                if (torpedo2.disparado == OFF) torpedo2.translateY += 0.3;
                if (torpedo3.disparado == OFF) torpedo3.translateY += 0.3;
                if (torpedo4.disparado == OFF) torpedo4.translateY += 0.3;
                helicopteroY += 0.3;
            }
            break;
        case GLUT_KEY_DOWN:
            if (helicopteroY > 0.0) {
                helicopteroY -= 0.3;
                if (torpedo1.disparado == OFF) torpedo1.translateY -= 0.3;
                if (torpedo2.disparado == OFF) torpedo2.translateY -= 0.3;
                if (torpedo3.disparado == OFF) torpedo3.translateY -= 0.3;
                if (torpedo4.disparado == OFF) torpedo4.translateY -= 0.3;
            } else {
                helicopteroY = 0.0;
            }
            break;
        case GLUT_KEY_LEFT:
            helicopteroZ += 0.3;
            if (torpedo1.disparado == OFF) torpedo1.translateZ += 0.3;
            if (torpedo2.disparado == OFF) torpedo2.translateZ += 0.3;
            if (torpedo3.disparado == OFF) torpedo3.translateZ += 0.3;
            if (torpedo4.disparado == OFF) torpedo4.translateZ += 0.3;
            break;
        case GLUT_KEY_RIGHT:
            helicopteroZ -= 0.3;
            if (torpedo1.disparado == OFF) torpedo1.translateZ -= 0.3;
            if (torpedo2.disparado == OFF) torpedo2.translateZ -= 0.3;
            if (torpedo3.disparado == OFF) torpedo3.translateZ -= 0.3;
            if (torpedo4.disparado == OFF) torpedo4.translateZ -= 0.3;
            break;
    }
    glutPostRedisplay();
}
