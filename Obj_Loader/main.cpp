#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <ctime>
#include "GameObject.h"
#include <GL/freeglut.h>
#define PI 3.1415

#define ON 1
#define OFF 0
#define NUM_PROJETEIS 200

using namespace std;

/**********************************************
    ESTRUTURA DOS TORPEDOS
**********************************************/
typedef struct {
    GameObject torpedo;
    GLint disparado;
    GLfloat translateX;
    GLfloat translateZ;
    GLfloat translateY;
    GLfloat rotateY;
    GLfloat deslocamentoZ;
} Torpedo;
/*******************************************/


/**********************************************
    ESTRUTURA DOS PROJÉTEIS
**********************************************/
typedef struct {
    GLfloat translateX;
    GLfloat translateY;
    GLfloat translateZ;

    GLfloat eixoX;
    GLfloat eixoY;
    GLfloat eixoZ;

    GLfloat rotateY;
    GLint disparado;
} Projetil;

GLint projeteisDisparados;
Projetil projeteis[NUM_PROJETEIS];

/*******************************************/



GLint WIDTH = 800;
GLint HEIGHT = 600;

GLfloat obs[3]={0.0,3.5,0.0};
GLfloat look[3]={0.0,3.0,0.0};
GLfloat axisxz=0;
GLfloat radiusxz=30;

/**********************************************
    CONTROLES DO HELICÓPTERO
**********************************************/
GameObject helicoptero;
GLfloat helicopteroRotateY = 0.0;
GLfloat helicopteroY = 0.0;
GLfloat helicopteroX = -10.0;
GLfloat helicopteroZ = 0.0;
/*******************************************/


/*********************************************
    CONTROLES DOS TORPEDOS
*********************************************/
Torpedo torpedo1;
Torpedo torpedo2;
Torpedo torpedo3;
Torpedo torpedo4;
GLint numTorpedoEsquerda = 0;
GLint numTorpedoDireita = 0;
/*******************************************/


/********************************************
    CONTROLES DA HÉLICE
********************************************/
GameObject helice;
GLfloat heliceRotate = 0.0;
GLfloat heliceRotateIncrement = 0.0;
GLint heliceState = OFF;
/*******************************************/


GameObject solo;
GameObject rotor_cauda;
GameObject janela;
GameObject fundo;

void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void special_keyboard(int key, int x, int y);
void controlaAnimacoes(void);
void initVariables(void);


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA | GLUT_MULTISAMPLE);

    glutInitContextVersion (3, 0);
    glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);

    //glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Helicoptero");

    glutFullScreen();

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

    // inicializa variaveis que controlam os movimentos
    initVariables();

    helicoptero.load("helicoptero_final.obj", "helicoptero_textura.tga", 0);
    helice.load("helice.obj", "helice.tga", 0);
    torpedo1.torpedo.load("torpedo.obj", "torpedo.tga", 0);
    torpedo2.torpedo.load("torpedo.obj", "torpedo.tga", 0);
    torpedo3.torpedo.load("torpedo.obj", "torpedo.tga", 0);
    torpedo4.torpedo.load("torpedo.obj", "torpedo.tga", 0);
    solo.load("piso.obj", "piso.tga", 0);
    rotor_cauda.load("helice.obj", "helice.tga", 0);
    janela.load("janela.obj", "parabrisa.tga", 0);
    fundo.load("piso.obj", "montanhas2.tga", 0);

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

    // plano de fundo
    glPushMatrix();
        glScalef(1.0, 30.0, 90.0);
        glTranslatef(-600.0, 12.0, 0.0);
        glRotatef(90.0, 0, 1, 0);
        glRotatef(70.0, 1, 0, 0);
        glRotatef(180.0, 0, 1, 0);
        glRotatef(-30.0, 1, 0, 0);
        fundo.render();
    glPopMatrix();

    // Solo
    glPushMatrix();
        glTranslatef(0, -5.2, 0);
        glScalef(40.0, 1.0, 50.0);
        glRotatef(-19, 1, 0, 0);
        solo.render();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(helicopteroX, helicopteroY, helicopteroZ);
        glRotatef(helicopteroRotateY, 0, 1, 0);

        glPushMatrix();
            helicoptero.render();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0, 4.0, 3.0);
            glRotatef(heliceRotate, 0, 1, 0);
            helice.render();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0, 0.0, 0.01);
            janela.render();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.5, 5.0, -22.5);
            glScalef(0.5, 0.16, 0.16);
            glRotatef(-90, 0, 0, 1);
            glRotatef(heliceRotate, 0, 1, 0);
            rotor_cauda.render();
        glPopMatrix();

    glPopMatrix();

/**************************************************************************************************************
    DESENHA OS PROJÉTEIS DA METRALHADORA
**************************************************************************************************************/
    int i;
    for (i = 0; i < projeteisDisparados; i++) {
        glPushMatrix();
            glColor3f(1.0, 0.0, 0.0);
            if (projeteis[i].disparado == ON) {
                glTranslatef(projeteis[i].translateX, projeteis[i].translateY, projeteis[i].translateZ);
                glRotatef(projeteis[i].rotateY, 0, 1, 0);
                glTranslatef(projeteis[i].eixoX, projeteis[i].eixoY, projeteis[i].eixoZ);
                glutSolidSphere(0.06, 20, 20);
            }
            glColor3f(1.0, 1.0, 1.0);
        glPopMatrix();
    }
/*************************************************************************************************************/

    // torpedo 1
    glPushMatrix();
        if (torpedo1.disparado == OFF) {
            glTranslatef(helicopteroX, helicopteroY, helicopteroZ);
            glRotatef(helicopteroRotateY, 0, 1, 0);
            glTranslatef(4.0, -3.0, 3.0);
        } else {
            glTranslatef(torpedo1.translateX, torpedo1.translateY, torpedo1.translateZ);
            glRotatef(torpedo1.rotateY, 0, 1, 0);
            glTranslatef(4.0, -3.0, 3.0);
            glTranslatef(0.0, 0.0, torpedo1.deslocamentoZ);
        }
        torpedo1.torpedo.render();
    glPopMatrix();

    // torpedo 2
    glPushMatrix();
        if (torpedo2.disparado == OFF) {
            glTranslatef(helicopteroX, helicopteroY, helicopteroZ);
            glRotatef(helicopteroRotateY, 0, 1, 0);
            glTranslatef(5.7, -3.0, 3.0);
        } else {
            glTranslatef(torpedo2.translateX, torpedo2.translateY, torpedo2.translateZ);
            glRotatef(torpedo2.rotateY, 0, 1, 0);
            glTranslatef(5.7, -3.0, 3.0);
            glTranslatef(0.0, 0.0, torpedo2.deslocamentoZ);
        }
        torpedo1.torpedo.render();
    glPopMatrix();

    // torpedo 3
    glPushMatrix();
        if (torpedo3.disparado == OFF) {
            glTranslatef(helicopteroX, helicopteroY, helicopteroZ);
            glRotatef(helicopteroRotateY, 0, 1, 0);
            glTranslatef(-4.4, -3.0, 3.0);
        } else {
            glTranslatef(torpedo3.translateX, torpedo3.translateY, torpedo3.translateZ);
            glRotatef(torpedo3.rotateY, 0, 1, 0);
            glTranslatef(-4.4, -3.0, 3.0);
            glTranslatef(0.0, 0.0, torpedo3.deslocamentoZ);
        }
        torpedo1.torpedo.render();
    glPopMatrix();

    // torpedo 4
    glPushMatrix();
        if (torpedo4.disparado == OFF) {
            glTranslatef(helicopteroX, helicopteroY, helicopteroZ);
            glRotatef(helicopteroRotateY, 0, 1, 0);
            glTranslatef(-6.3, -3.0, 3.0);
        } else {
            glTranslatef(torpedo4.translateX, torpedo4.translateY, torpedo4.translateZ);
            glRotatef(torpedo4.rotateY, 0, 1, 0);
            glTranslatef(-6.3, -3.0, 3.0);
            glTranslatef(0.0, 0.0, torpedo4.deslocamentoZ);
        }
        torpedo1.torpedo.render();
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    float ar = (float) width / (float) height;
    glViewport(0, 0, width, height);
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
            heliceRotateIncrement += 0.01;
        }
        if (heliceRotate == 360.0) {
            heliceRotate = 0.0;
        }
    } else {
        if (heliceRotateIncrement > 0.0) {
            heliceRotateIncrement -= 0.05;
            heliceRotate += heliceRotateIncrement;
        }
    }

    if (numTorpedoEsquerda > 0) {
        torpedo2.deslocamentoZ += 0.1;
    }
    if (numTorpedoEsquerda > 1) {
        torpedo1.deslocamentoZ += 0.1;
    }
    if (numTorpedoDireita > 0) {
        torpedo4.deslocamentoZ += 0.1;
    }
    if (numTorpedoDireita > 1) {
        torpedo3.deslocamentoZ += 0.1;
    }

    int i;
    for (i = 0; i < projeteisDisparados; i++) {
        projeteis[i].eixoZ += 0.05;
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
        case 'D':
            if (heliceState == ON) helicopteroRotateY += 2.5;
            break;
        case 'a':
        case 'A':
            if (heliceState == ON) helicopteroRotateY -= 2.5;
            break;
        case 's':
        case 'S':
            if (heliceState == ON) helicopteroX += 0.3;
            break;
        case 'w':
        case 'W':
            if (heliceState == ON) helicopteroX -= 0.3;
            break;
        case 'i':
            heliceState = ON;
            break;
        case 'I':
            if (helicopteroY == 0) {
                heliceState = OFF;
            }
            break;
        case 't':
            if (numTorpedoEsquerda < 2) {
                numTorpedoEsquerda++;
                if (numTorpedoEsquerda == 1) {
                    torpedo2.rotateY = helicopteroRotateY;
                    torpedo2.translateX = helicopteroX;
                    torpedo2.translateY = helicopteroY;
                    torpedo2.translateZ = helicopteroZ;
                    torpedo2.disparado = ON;
                } else {
                    torpedo1.rotateY = helicopteroRotateY;
                    torpedo1.translateX = helicopteroX;
                    torpedo1.translateY = helicopteroY;
                     torpedo1.translateZ = helicopteroZ;
                    torpedo1.disparado = ON;
                }
            }
            break;
        case 'T':
            if (numTorpedoDireita < 2) {
                numTorpedoDireita++;
                if (numTorpedoDireita == 1) {
                    torpedo4.rotateY = helicopteroRotateY;
                    torpedo4.translateX = helicopteroX;
                    torpedo4.translateY = helicopteroY;
                    torpedo4.translateZ = helicopteroZ;
                    torpedo4.disparado = ON;
                } else {
                    torpedo3.rotateY = helicopteroRotateY;
                    torpedo3.translateX = helicopteroX;
                    torpedo3.translateY = helicopteroY;
                    torpedo3.translateZ = helicopteroZ;
                    torpedo3.disparado = ON;
                }
            }
            break;
        case 'm':
        case 'M':
            if (projeteisDisparados < NUM_PROJETEIS) {
                projeteis[projeteisDisparados].rotateY = helicopteroRotateY;
                projeteis[projeteisDisparados].translateX = helicopteroX;
                projeteis[projeteisDisparados].translateY = helicopteroY;
                projeteis[projeteisDisparados].translateZ = helicopteroZ;
                projeteis[projeteisDisparados].disparado = ON;
                projeteisDisparados++;
            }
            break;
	}
	glutPostRedisplay();
}

void special_keyboard(int key, int x, int y) {
    switch (key)
    {
        case GLUT_KEY_UP:
            if (heliceState == ON) helicopteroY += 0.3;
            break;
        case GLUT_KEY_DOWN:
            if (helicopteroY > 0.0) {
                helicopteroY -= 0.3;
            } else {
                helicopteroY = 0.0;
            }
            break;
        case GLUT_KEY_LEFT:
            if (heliceState == ON) helicopteroZ += 0.3;
            break;
        case GLUT_KEY_RIGHT:
            if (heliceState == ON) helicopteroZ -= 0.3;
            break;
    }
    glutPostRedisplay();
}

void initVariables() {
    GLint i;
    GLint j = 0;

    for (i = 0; i < NUM_PROJETEIS; i++) {
        projeteis[i].disparado = OFF;
        projeteis[i].eixoZ = 11.9;

        if (j == 0) {
            projeteis[i].eixoX = -0.04;
            projeteis[i].eixoY = -4.78;
        } else if (j == 1) {
            projeteis[i].eixoX = 0.275;
            projeteis[i].eixoY = -4.78;
        } else {
            projeteis[i].eixoX = 0.11;
            projeteis[i].eixoY = -5.0;
        }
        if (j == 2) {
            j = 0;
        } else {
            j++;
        }
    }

    torpedo1.deslocamentoZ = 0.0;
    torpedo2.deslocamentoZ = 0.0;
    torpedo3.deslocamentoZ = 0.0;
    torpedo4.deslocamentoZ = 0.0;

    torpedo1.disparado = OFF;
    torpedo2.disparado = OFF;
    torpedo3.disparado = OFF;
    torpedo4.disparado = OFF;

    projeteisDisparados = 0;
}
