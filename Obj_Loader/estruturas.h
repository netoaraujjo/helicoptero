#ifndef ESTRUTURAS_H_
#define ESTRUTURAS_H_

/**********************************************
    ESTRUTURA DO HELICÓPTERO
**********************************************/
typedef struct {
    GameObject corpo;
    GLfloat rotateY;
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

#endif // ESTRUTURAS_H_
