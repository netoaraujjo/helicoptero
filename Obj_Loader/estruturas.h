#ifndef ESTRUTURAS_H_
#define ESTRUTURAS_H_

/**********************************************
    ESTRUTURA DO HELICÓPTERO
**********************************************/
typedef struct {
    GameObject corpo;
    GLfloat rotate;
    GLfloat x;
    GLfloat y;
    GLfloat z;
} Helicoptero;
/*******************************************/


/**********************************************
    ESTRUTURA DOS TORPEDOS
**********************************************/
typedef struct {
    GameObject torpedo;
    GLint disparado;
    GLfloat translateX;
    GLfloat translateZ;
    GLfloat translateY;
    GLfloat eixoX;
    GLfloat eixoY;
    GLfloat eixoZ;
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
/*******************************************/


/**********************************************
    ESTRUTURA DO PERSONAGEM
**********************************************/
typedef struct {
    GLfloat rotateTronco;
    GLfloat rotateBracoEsq;
    GLfloat rotateBracoDir;
    GLfloat rotatePernaEsq;
    GLfloat rotatePernaDir;
    GLfloat x, y, z;
    GLint embarcado;
    GLint inverteBracoEsq;
    GLint inverteBracoDir;
    GLint invertePernaDir;
    GLint invertePernaEsq;
} Personagem;
/*******************************************/


/**********************************************
    ESTRUTURA DO RELÓGIO
**********************************************/
typedef struct {
    GLfloat instante1;
    GLfloat instante2;
    int contando;
    char tempo[3];
} Relogio;
/*******************************************/


/**********************************************
    ESTRUTURA DO ALVO
**********************************************/
typedef struct {
    GameObject objeto;
    GLint atingido;
    GLint projeteis;
    GLint torpedos;
    GLfloat escala;
    GLfloat translateX;
    GLfloat translateY;
    GLfloat translateZ;
} Alvo;
/*******************************************/

#endif // ESTRUTURAS_H_
