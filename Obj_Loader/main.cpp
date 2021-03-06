#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <assert.h>
#include <ctime>
#include <time.h>
#include "PewGL.h"
#include "GameObject.h"
#include "estruturas.h"
#include "constantes.h"
#include "desenho.h"
#include <GL/freeglut.h>
#include <string.h>
#include <fmod.h>

#define PI 3.1415

using namespace std;

void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void special_keyboard(int key, int x, int y);
void controlaAnimacoes(void);
void inicializaVariaveis(void);
void carregaObjetos(void);

void controleHelicoptero(unsigned char key);
void controleEspecialHelicoptero(int key);
void controlePersonagem(unsigned char key);
void controleEspecialPersonagem(int key);
void movimentaBracos(void);
void movimentaPernas(void);
int todosOsAlvosAtingidos();
void detectaColisao(GLfloat z, GLfloat y, int tipo);
bool carregaSom(char *currentSound);
bool carregaSomHelice(char *currentSound);
bool carregaSomHeliceDesligado(char *currentSound_helice);
void calculaBonus();

/*********************************************
    CONTROLES DOS TORPEDOS E PROJ�TEIS
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
    CONTROLES DA H�LICE E ROTOR DE CAUDA
********************************************/
GameObject helice;
GLfloat heliceRotate;
GLfloat heliceRotateIncrement;
GLint heliceState;
/*******************************************/

Helicoptero helicoptero;

GameObject solo;
GameObject rotor_cauda;
GameObject janela;
GameObject fundo;
GameObject mira;
GameObject torpedoImg;
GameObject balaImg;
GameObject relogioImg;

GLint WIDTH = 800;
GLint HEIGHT = 600;

GLfloat obs[3]={0.0,3.5,0.0};
GLfloat look[3]={0.0,3.0,0.0};
GLfloat axisxz=0;
GLfloat radiusxz=30;

Personagem personagem;
Relogio relogio;
Alvo alvo[NUM_ALVOS];

FMOD_RESULT result;
FMOD_SYSTEM * fmodsystem;
FMOD_SOUND * sound;
FMOD_CHANNEL * channel;

FMOD_RESULT result_helice;
FMOD_SYSTEM * fmodsystem_helice;
FMOD_SOUND * sound_helice;
FMOD_CHANNEL * channel_helice;

int acabou;
int vitoria;
char pontuacao[10];

int main(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA | GLUT_MULTISAMPLE);

    glutInitContextVersion (3, 0);
    glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);

//    glutInitWindowSize(WIDTH, HEIGHT);
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

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	/* observer position */
    obs[0]=radiusxz*cos(2*PI*axisxz/360);
    obs[2]=radiusxz*sin(2*PI*axisxz/360);
    gluLookAt(obs[0],obs[1],obs[2],look[0],look[1],look[2],0.0,1.0,0.0);

    desenhaPlanoDeFundo(&fundo);
    desenhaSolo(&solo);
    desenhaHelicoptero(&helicoptero, &mira, &helice, &rotor_cauda, &janela, heliceRotate);
    desenhaImagens(&torpedoImg, &balaImg, &relogioImg);

    for (int i = 0; i < NUM_ALVOS; i++) {
        if (alvo[i].atingido == OFF)
            desenhaAlvo(&alvo[i].objeto, alvo[i].escala, alvo[i].translateX, alvo[i].translateY, alvo[i].translateZ);
    }

    desenhaTexto(num_torp, num_met, relogio.tempo);

/**************************************************************************************************************
    DESENHA OS PROJ�TEIS DA METRALHADORA
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

    if (personagem.embarcado == OFF) {
        desenhaPersonagem(&personagem);
    }

    if (vitoria == OFF) {
        desenhaGameOver(pontuacao);
    } else if (vitoria == ON) {
        desenhaWin(pontuacao);
    }

    glutSwapBuffers();
}

void reshape(int width, int height) {
    float ar = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 1.0, 650.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void finaliza() {
    glutIdleFunc(NULL);
    acabou = ON;
    calculaBonus();
}

void controlaAnimacoes() {

    if (todosOsAlvosAtingidos() == ON) {
        vitoria = ON;
        finaliza();
    }

    if (strcmp(num_torp, "0") == 0 && strcmp(num_met, "0") == 0) {
        if (torpedos[0].deslocamentoZ > 110
                && torpedos[2].deslocamentoZ > 110
                && projeteis[NUM_PROJETEIS - 1].eixoZ > 110
                && todosOsAlvosAtingidos() == OFF) {
            vitoria = OFF;
            finaliza();
        }
    }

    if (relogio.contando == ON) {
        if (relogio.instante1 == 0) { // Pega o instante inicial
            relogio.instante1 = (float)clock()/(float)CLOCKS_PER_SEC;
        }
        sprintf(relogio.tempo, "%.0f", TEMPO_TOTAL - (relogio.instante2 - relogio.instante1));
        if (relogio.instante2 - relogio.instante1 < TEMPO_TOTAL) {
            relogio.instante2 = (float)clock()/(float)CLOCKS_PER_SEC;
        } else {
            strcpy(relogio.tempo, "0");
            vitoria = OFF;
            finaliza();
        }
    }

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
        if (torpedos[1].eixoX > 0) {
            torpedos[1].eixoX -= TORPEDO_INCREMENT_HORIZONTAL;
        }
        if (torpedos[1].eixoY > TORPEDO_POSITION_Y) {
            torpedos[1].eixoY -= TORPEDO_INCREMENT_VERTICAL;
        }
        detectaColisao(torpedos[1].deslocamentoZ,
                       projeteis[1].eixoY,
                       TORPEDO);
    }
    if (numTorpedoEsquerda > 1) {
        torpedos[0].deslocamentoZ += TORPEDO_INCREMENT;
        if (torpedos[0].eixoX > 0) {
            torpedos[0].eixoX -= TORPEDO_INCREMENT_HORIZONTAL;
        }
        if (torpedos[0].eixoY > TORPEDO_POSITION_Y) {
            torpedos[0].eixoY -= TORPEDO_INCREMENT_VERTICAL;
        }
        detectaColisao(torpedos[0].deslocamentoZ,
                       projeteis[0].eixoY,
                       TORPEDO);
    }
    if (numTorpedoDireita > 0) {
        torpedos[3].deslocamentoZ += TORPEDO_INCREMENT;
        if (torpedos[3].eixoX < 0) {
            torpedos[3].eixoX += TORPEDO_INCREMENT_HORIZONTAL;
        }
        if (torpedos[3].eixoY > TORPEDO_POSITION_Y) {
            torpedos[3].eixoY -= TORPEDO_INCREMENT_VERTICAL;
        }
        detectaColisao(torpedos[3].deslocamentoZ,
                       projeteis[3].eixoY,
                       TORPEDO);
    }
    if (numTorpedoDireita > 1) {
        torpedos[2].deslocamentoZ += TORPEDO_INCREMENT;
        if (torpedos[2].eixoX < 0) {
            torpedos[2].eixoX += TORPEDO_INCREMENT_HORIZONTAL;
        }
        if (torpedos[2].eixoY > TORPEDO_POSITION_Y) {
            torpedos[2].eixoY -= TORPEDO_INCREMENT_VERTICAL;
        }
        detectaColisao(torpedos[2].deslocamentoZ,
                       projeteis[2].eixoY,
                       TORPEDO);
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
        detectaColisao(projeteis[i].eixoZ,
                       projeteis[i].translateY + projeteis[i].eixoY,
                       PROJETIL);
    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
        case 27:
            exit(0);
            break;
        default:
            if (acabou == OFF) {
                if (personagem.embarcado == OFF) {
                    controlePersonagem(key);
                } else {
                    controleHelicoptero(key);
                }
            }
            break;
	}
	glutPostRedisplay();
}

void special_keyboard(int key, int x, int y) {
    if (acabou == OFF) {
        if (personagem.embarcado == OFF) {
            controleEspecialPersonagem(key);
        } else {
            controleEspecialHelicoptero(key);
        }
    }
    glutPostRedisplay();
}

void inicializaVariaveis() {
    GLint i;
    GLint j = 0;
    GLint aux;

    acabou = OFF;
    vitoria = 2;

    strcpy(num_torp, "4");
    strcpy(num_met, "200");
    strcpy(pontuacao, "0");

    itoa(TEMPO_TOTAL, relogio.tempo, 10);

    relogio.instante1 = 0;
    relogio.instante2 = 0;
    relogio.contando = OFF;

    srand( (unsigned)time(NULL) );
    for (i = 0; i < NUM_ALVOS; i++) {
        alvo[i].atingido = OFF;
        alvo[i].projeteis = 0;
        alvo[i].torpedos = 0;
        alvo[i].escala = 0.7;
        alvo[i].translateX = -100;
        alvo[i].translateY = (rand() % 801) / 10;
        alvo[i].translateZ = (rand() % 1601) / 10;
        aux = rand() % 2;
        if (aux == 0) {
            alvo[i].translateZ = alvo[i].translateZ * -1;
        }
    }

    helicoptero.rotate = -90.0;
    helicoptero.x = -10.0;
    helicoptero.y = 0.0;
    helicoptero.z = 0.0;

    heliceState = OFF;

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
    personagem.rotateTronco = 0.0;
    personagem.x = 23.0;
    personagem.y = 0.0;
    personagem.z = 0.0;
    personagem.embarcado = OFF;
    personagem.inverteBracoDir = 0;
    personagem.inverteBracoEsq = 0;
    personagem.rotatePernaDir = 0;
    personagem.rotatePernaEsq = 0;
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
    torpedoImg.load("piso.obj", "torpedo2.tga", 0);
    balaImg.load("piso.obj", "bala.tga", 0);
    relogioImg.load("piso.obj", "relogio.tga", 0);

    for (int i = 0; i < NUM_ALVOS; i++) {
        alvo[i].objeto.load("Alvo.obj", "Alvo.tga", 0);
    }

}

void controleHelicoptero(unsigned char key) {
    switch (key) {
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
            relogio.contando = ON;
            carregaSomHelice("sound/sons/helice ligando.mp3");
            break;
        case 'I':
            if (helicoptero.y == 0) {
                heliceState = OFF;
                carregaSomHeliceDesligado("sound/sons/helice desligando.mp3");
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
                carregaSom("sound/sons/torpedo.mp3");
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
                carregaSom("sound/sons/torpedo.mp3");
            }
            break;
        case 'm':
        case 'M':
            if (projeteisDisparados < NUM_PROJETEIS) {
//                carregaSom("sound/sons/projetil.mp3");
                projeteis[projeteisDisparados].rotateY = helicoptero.rotate;
                projeteis[projeteisDisparados].translateX = helicoptero.x;
                projeteis[projeteisDisparados].translateY = helicoptero.y;
                projeteis[projeteisDisparados].translateZ = helicoptero.z;
                projeteis[projeteisDisparados].disparado = ON;
                projeteisDisparados++;
                itoa(NUM_PROJETEIS - projeteisDisparados, num_met, 10);
            }
            break;
        case 'e':
        case 'E':
            if (helicoptero.y == 0) {
                personagem.embarcado = OFF;
            }
            break;
	}
}

void controleEspecialHelicoptero(int key) {
    switch (key) {
        case GLUT_KEY_UP:
            if (heliceState == ON) helicoptero.y += 0.7;
            break;
        case GLUT_KEY_DOWN:
            if (helicoptero.y > 0.0) {
                helicoptero.y -= 0.7;
            } else {
                helicoptero.y = 0.0;
            }
            break;
        case GLUT_KEY_LEFT:
            if (heliceState == ON) helicoptero.z += 0.7;//axisxz+=0.2;
            break;
        case GLUT_KEY_RIGHT:
            if (heliceState == ON) helicoptero.z -= 0.7;//axisxz-=0.2;
            break;
    }
}

void controlePersonagem(unsigned char key) {
    switch(key) {
        case 'e':
        case 'E':
            personagem.embarcado = ON;
            break;
    }
}

void controleEspecialPersonagem(int key) {
    switch (key) {
        case GLUT_KEY_LEFT:
            personagem.rotateTronco = -90.0;
            personagem.z += 0.1;
            movimentaBracos();
            movimentaPernas();
            break;
        case GLUT_KEY_RIGHT:
            personagem.rotateTronco = 90.0;
            personagem.z -= 0.1;
            movimentaBracos();
            movimentaPernas();
            break;
        case GLUT_KEY_DOWN:
            personagem.rotateTronco = 0.0;
            personagem.x += 0.1;
            movimentaBracos();
            movimentaPernas();
            break;
        case GLUT_KEY_UP:
            personagem.rotateTronco = 180.0;
            personagem.x -= 0.1;
            movimentaBracos();
            movimentaPernas();
            break;
    }
}

void movimentaBracos() {
    if (personagem.inverteBracoDir == 0) {
        personagem.rotateBracoDir += 6.0;
        if (personagem.rotateBracoDir > 60.0) {
            personagem.inverteBracoDir = 1;
        }
    } else {
        personagem.rotateBracoDir -= 6.0;
        if (personagem.rotateBracoDir < -60.0) {
            personagem.inverteBracoDir = 0;
        }
    }

    if (personagem.inverteBracoEsq == 0) {
        personagem.rotateBracoEsq -= 6.0;
        if (personagem.rotateBracoEsq < -60.0) {
            personagem.inverteBracoEsq = 1;
        }
    } else {
        personagem.rotateBracoEsq += 6.0;
        if (personagem.rotateBracoEsq > 60.0) {
            personagem.inverteBracoEsq = 0;
        }
    }
}

void movimentaPernas() {
    if (personagem.invertePernaEsq == 0) {
        personagem.rotatePernaEsq += 6.0;
        if (personagem.rotatePernaEsq > 50.0) {
            personagem.invertePernaEsq = 1;
        }
    } else {
        personagem.rotatePernaEsq -= 6.0;
        if (personagem.rotatePernaEsq < -50.0) {
            personagem.invertePernaEsq = 0;
        }
    }

    if (personagem.invertePernaDir == 0) {
        personagem.rotatePernaDir -= 6.0;
        if (personagem.rotatePernaDir < -50.0) {
            personagem.invertePernaDir = 1;
        }
    } else {
        personagem.rotatePernaDir += 6.0;
        if (personagem.rotatePernaDir > 50.0) {
            personagem.invertePernaDir = 0;
        }
    }
}

int todosOsAlvosAtingidos() {
    for (int i = 0; i < NUM_ALVOS; i++) {
        if (alvo[i].atingido == OFF) {
            return OFF;
        }
    }
    return ON;
}

void detectaColisao(GLfloat z, GLfloat y, int tipo) {
    int i;
    for (i = 0; i < NUM_ALVOS; i++) {
        if (alvo[i].atingido == OFF) {
            if ((z * -1) <= alvo[i].translateX) {
                if (y >= (alvo[i].translateY - 2) && y <= (alvo[i].translateY + 2)) {
                    printf("\n\n\n\tY = %f", y);
                    printf("\nTRANSLATE Y = %f", alvo[i].translateY);
                    alvo[i].atingido = ON;
                    if (tipo == TORPEDO)
                        alvo[i].torpedos++;
                    else if (tipo == PROJETIL)
                        alvo[i].projeteis++;
                }
            }
        }
//        if (PewGL::colisionDetection(xA, yA, zA, raioA, (double)alvo[i].translateX, (double)alvo[i].translateY, (double)alvo[i].translateZ, 7.0) == true) {
//            alvo[i].atingido = ON;
//            desenhaWin();
//        }
    }
}

bool carregaSom(char *currentSound) {
    result = FMOD_System_Create(&fmodsystem);
    if (result != FMOD_OK)
        return false;
    result = FMOD_System_Init(fmodsystem,2, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK)
        return false;
    result = FMOD_Sound_Release(sound);
    result = FMOD_System_CreateStream(fmodsystem,currentSound, FMOD_SOFTWARE, 0, &sound);
    if (result != FMOD_OK)
       return false;
    result = FMOD_System_PlaySound(fmodsystem,FMOD_CHANNEL_FREE, sound, false, &channel);
    FMOD_Channel_SetMode(channel,FMOD_INIT_NORMAL);
}

bool carregaSomHelice(char *currentSound_helice) {
    result_helice = FMOD_System_Create(&fmodsystem_helice);
    if (result_helice != FMOD_OK)
        return false;
    result_helice = FMOD_System_Init(fmodsystem_helice,2, FMOD_INIT_NORMAL, 0);
    if (result_helice != FMOD_OK)
        return false;
    result_helice = FMOD_Sound_Release(sound_helice);
    result_helice = FMOD_System_CreateStream(fmodsystem_helice,currentSound_helice, FMOD_SOFTWARE, 0, &sound_helice);
    if (result_helice != FMOD_OK)
       return false;
    result_helice = FMOD_System_PlaySound(fmodsystem_helice,FMOD_CHANNEL_FREE, sound_helice, false, &channel_helice);
    FMOD_Channel_SetMode(channel_helice,FMOD_LOOP_NORMAL);
}

bool carregaSomHeliceDesligado(char *currentSound_helice) {
    result_helice = FMOD_System_Create(&fmodsystem_helice);
    if (result_helice != FMOD_OK)
        return false;
    result_helice = FMOD_System_Init(fmodsystem_helice,2, FMOD_INIT_NORMAL, 0);
    if (result_helice != FMOD_OK)
        return false;
    result_helice = FMOD_Sound_Release(sound_helice);
    result_helice = FMOD_System_CreateStream(fmodsystem_helice,currentSound_helice, FMOD_SOFTWARE, 0, &sound_helice);
    if (result_helice != FMOD_OK)
       return false;
    result_helice = FMOD_System_PlaySound(fmodsystem_helice,FMOD_CHANNEL_FREE, sound_helice, false, &channel_helice);
    FMOD_Channel_SetMode(channel_helice,FMOD_INIT_NORMAL);
}

void calculaBonus() {
    int proj, tempo, torp, pont, alv = 0;

    for (int i = 0; i < NUM_ALVOS; i++) {
        if (alvo[i].atingido == ON) {
            alv++;
        }
    }

    alv = alv * 50;
    pont = alv;

    if (vitoria == ON) {
        proj = atoi(num_met);
        tempo = atoi(relogio.tempo);
        torp = atoi(num_torp);

        proj = proj * 10;
        tempo = tempo * 20;
        torp = torp * 30;

        pont = proj + tempo + torp + alv;
    }

    itoa(pont, pontuacao, 10);
}
