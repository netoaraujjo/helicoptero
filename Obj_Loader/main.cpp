#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <ctime>
#include "GameObject.h"
#include "estruturas.h"
#include "constantes.h"
#include "desenho.h"
#include <GL/freeglut.h>
#include <string.h>

#define PI 3.1415

using namespace std;

void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void special_keyboard(int key, int x, int y);
void controlaAnimacoes(void);
void inicializaVariaveis(void);
void carregaObjetos(void);

void keyboardPersonagem(unsigned char key, int x, int y);

/*********************************************
    CONTROLES DOS TORPEDOS E PROJÉTEIS
*********************************************/
Torpedo torpedos[NUM_TORPEDOS];
GLint numTorpedoEsquerda;
GLint numTorpedoDireita;
char num_torp[2];
char num_met[4];
GLint projeteisDisparados;
Projetil projeteis[NUM_PROJETEIS];
/*******************************************/


/********************************************
    CONTROLES DA HÉLICE E ROTOR DE CAUDA
********************************************/
GameObject helice;
GLfloat heliceRotate = 0.0;
GLfloat heliceRotateIncrement = 0.0;
GLint heliceState = OFF;
/*******************************************/

Helicoptero helicoptero;


GameObject solo;
GameObject rotor_cauda;
GameObject janela;
GameObject fundo;
GameObject mira;
GameObject alvo;
GameObject torpedoImg;
GameObject balaImg;

GLint WIDTH = 800;
GLint HEIGHT = 600;

GLfloat obs[3]={0.0,3.5,0.0};
GLfloat look[3]={0.0,3.0,0.0};
GLfloat axisxz=0;
GLfloat radiusxz=30;

Personagem personagem;

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

    //glutKeyboardFunc(keyboardPersonagem);


    glutIdleFunc(controlaAnimacoes);

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    glDisable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    inicializaVariaveis();
    carregaObjetos();

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

    desenhaPlanoDeFundo(&fundo);
    desenhaSolo(&solo);
    desenhaAlvo(&alvo);
    desenhaHelicoptero(&helicoptero, &mira, &helice, &rotor_cauda, &janela, heliceRotate);

    glPushMatrix();
        glTranslatef(27.25, 5.9, -4.3);
        glScalef(0.01, 0.01, 0.01);
        glRotatef(30, 0, 1, 1);
        torpedoImg.render();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(27.25, 5.5, -4);
        glScalef(0.01, 0.01, 0.01);
        glRotatef(-180, 1, 1, 1);
        balaImg.render();
    glPopMatrix();

    desenhaTexto(num_torp, num_met);

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


    desenhaTorpedos(torpedos, &helicoptero);

    //desenhaPersonagem(&personagem);

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
            heliceRotateIncrement += 0.05;
        }
        if (heliceRotate == 360.0) {
            heliceRotate = 0.0;
        }
    } else {
        if (heliceRotateIncrement > 0.0) {
            heliceRotateIncrement -= 0.1;
            heliceRotate += heliceRotateIncrement;
            if (heliceRotate < 0.0) {
                heliceRotate = 0.0;
            }
        }
    }

    if (numTorpedoEsquerda > 0) {
        torpedos[1].deslocamentoZ += TORPEDO_INCREMENT;
        if (torpedos[1].eixoX > torpedos[1].translateZ) {
            torpedos[1].eixoX -= 0.05;
        }
        if (torpedos[1].eixoY < torpedos[1].translateY) {
            torpedos[1].eixoY += 0.035;
        }
    }
    if (numTorpedoEsquerda > 1) {
        torpedos[0].deslocamentoZ += TORPEDO_INCREMENT;
        if (torpedos[0].eixoX > torpedos[0].translateZ) {
            torpedos[0].eixoX -= 0.05;
        }
        if (torpedos[0].eixoY < torpedos[0].translateY) {
            torpedos[0].eixoY += 0.035;
        }
    }
    if (numTorpedoDireita > 0) {
        torpedos[3].deslocamentoZ += TORPEDO_INCREMENT;
        if (torpedos[3].eixoX < torpedos[3].translateZ) {
            torpedos[3].eixoX += 0.05;
        }
        if (torpedos[3].eixoY < torpedos[3].translateY) {
            torpedos[3].eixoY += 0.035;
        }
    }
    if (numTorpedoDireita > 1) {
        torpedos[2].deslocamentoZ += TORPEDO_INCREMENT;
        if (torpedos[2].eixoX < torpedos[2].translateZ) {
            torpedos[2].eixoX += 0.05;
        }
        if (torpedos[2].eixoY < torpedos[2].translateY) {
            torpedos[2].eixoY += 0.035;
        }
    }

    if (numTorpedoDireita + numTorpedoEsquerda == 4) {
        strcpy(num_torp, "0");
    } else if (numTorpedoDireita + numTorpedoEsquerda == 3) {
        strcpy(num_torp, "1");
    } else if (numTorpedoDireita + numTorpedoEsquerda == 2) {
        strcpy(num_torp, "2");
    } else if (numTorpedoDireita + numTorpedoEsquerda == 1) {
        strcpy(num_torp, "3");
    }

    int i;
    for (i = 0; i < projeteisDisparados; i++) {
        projeteis[i].eixoZ += PROJETIL_INCREMENT;
        if (projeteis[i].eixoY < projeteis[i].translateY) {
            projeteis[i].eixoY += 0.1;
        }
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
            if (heliceState == ON) helicoptero.rotate += 2.5;
            break;
        case 'a':
        case 'A':
            if (heliceState == ON) helicoptero.rotate -= 2.5;
            break;
        case 's':
        case 'S':
            if (heliceState == ON) helicoptero.x += 0.3;
            break;
        case 'w':
        case 'W':
            if (heliceState == ON) helicoptero.x -= 0.3;
            break;
        case 'i':
            heliceState = ON;
            break;
        case 'I':
            if (helicoptero.y == 0) {
                heliceState = OFF;
            }
            break;
        case 't':
            if (numTorpedoEsquerda < 2) {
                numTorpedoEsquerda++;
                if (numTorpedoEsquerda == 1) {
                    torpedos[1].rotateY = helicoptero.rotate;
                    torpedos[1].translateX = helicoptero.x;
                    torpedos[1].translateY = helicoptero.y;
                    torpedos[1].translateZ = helicoptero.z;
                    torpedos[1].disparado = ON;
                } else {
                    torpedos[0].rotateY = helicoptero.rotate;
                    torpedos[0].translateX = helicoptero.x;
                    torpedos[0].translateY = helicoptero.y;
                    torpedos[0].translateZ = helicoptero.z;
                    torpedos[0].disparado = ON;
                }
            }
            break;
        case 'T':
            if (numTorpedoDireita < 2) {
                numTorpedoDireita++;
                if (numTorpedoDireita == 1) {
                    torpedos[3].rotateY = helicoptero.rotate;
                    torpedos[3].translateX = helicoptero.x;
                    torpedos[3].translateY = helicoptero.y;
                    torpedos[3].translateZ = helicoptero.z;
                    torpedos[3].disparado = ON;
                } else {
                    torpedos[2].rotateY = helicoptero.rotate;
                    torpedos[2].translateX = helicoptero.x;
                    torpedos[2].translateY = helicoptero.y;
                    torpedos[2].translateZ = helicoptero.z;
                    torpedos[2].disparado = ON;
                }
            }
            break;
        case 'm':
        case 'M':
            if (projeteisDisparados < NUM_PROJETEIS) {
                projeteis[projeteisDisparados].rotateY = helicoptero.rotate;
                projeteis[projeteisDisparados].translateX = helicoptero.x;
                projeteis[projeteisDisparados].translateY = helicoptero.y;
                projeteis[projeteisDisparados].translateZ = helicoptero.z;
                projeteis[projeteisDisparados].disparado = ON;
                projeteisDisparados++;
                itoa(NUM_PROJETEIS - projeteisDisparados, num_met, 10);
            }
            break;
	}
	glutPostRedisplay();
}


void keyboardPersonagem(unsigned char key, int x, int y) {
    switch(key) {
        case 27:
            exit(0);
            break;
        case 'a':
        case 'A':
            personagem.x += 0.5;
            break;
        case 'q':
        case 'Q':
            personagem.rotateBracoDir += 10.0;
            break;
        case 'e':
        case 'E':
            personagem.rotateBracoEsq += 10.0;
            break;
        case 'r':
        case 'R':
            personagem.rotatePernaDir += 10.0;
            break;
        case 'y':
        case 'Y':
            personagem.rotatePernaEsq += 10.0;
            break;
        case 'j':
        case 'J':
            personagem.rotateTronco += 10.0;
            if (personagem.rotateTronco >= 360.0) personagem.rotateTronco = 0.0;
            break;
    }
    glutPostRedisplay();
}


void special_keyboard(int key, int x, int y) {
    switch (key)
    {
        case GLUT_KEY_UP:
            if (heliceState == ON) helicoptero.y += 0.3;
            break;
        case GLUT_KEY_DOWN:
            if (helicoptero.y > 0.0) {
                helicoptero.y -= 0.3;
            } else {
                helicoptero.y = 0.0;
            }
            break;
        case GLUT_KEY_LEFT:
            if (heliceState == ON) helicoptero.z += 0.3;
            break;
        case GLUT_KEY_RIGHT:
            if (heliceState == ON) helicoptero.z -= 0.3;
            break;
    }
    glutPostRedisplay();
}

void inicializaVariaveis() {
    GLint i;
    GLint j = 0;

    strcpy(num_torp, "4");
    strcpy(num_met, "200");

    helicoptero.rotate = 0.0;
    helicoptero.x = -10.0;
    helicoptero.y = 0.0;
    helicoptero.z = 0.0;

    for (i = 0; i < NUM_TORPEDOS; i++) {
        torpedos[i].deslocamentoZ = 0.0;
        torpedos[i].eixoY = -3.0;
        torpedos[i].disparado = OFF;
    }

    projeteisDisparados = 0;

    torpedos[0].eixoX = 4.0;
    torpedos[1].eixoX = 5.7;
    torpedos[2].eixoX = -4.4;
    torpedos[3].eixoX = -6.3;

    numTorpedoEsquerda = 0;
    numTorpedoDireita = 0;

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

    personagem.rotateBracoDir = 0.0;
    personagem.rotateBracoEsq = 0.0;
    personagem.rotatePernaDir = 0.0;
    personagem.rotatePernaEsq = 0.0;
    personagem.rotateTronco = 90.0;
    personagem.x = 27.0;
    personagem.y = 2.0;
    personagem.z = 0.0;
}

void carregaObjetos() {
    helicoptero.corpo.load("helicoptero_final.obj", "helicoptero_textura.tga", 0);
    helice.load("helice.obj", "helice.tga", 0);
    torpedos[0].torpedo.load("torpedo.obj", "torpedo.tga", 0);
    torpedos[1].torpedo.load("torpedo.obj", "torpedo.tga", 0);
    torpedos[2].torpedo.load("torpedo.obj", "torpedo.tga", 0);
    torpedos[3].torpedo.load("torpedo.obj", "torpedo.tga", 0);
    solo.load("piso.obj", "piso.tga", 0);
    rotor_cauda.load("helice.obj", "helice.tga", 0);
    janela.load("janela.obj", "parabrisa.tga", 0);
    fundo.load("piso.obj", "montanhas2.tga", 0);
    mira.load("Mira.obj", "vermelho.tga", 0);
    alvo.load("Alvo.obj", "Alvo.tga", 0);
    torpedoImg.load("piso.obj", "torpedo2.tga", 0);
    balaImg.load("piso.obj", "bala.tga", 0);
}
