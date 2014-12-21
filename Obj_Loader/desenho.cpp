#include "GameObject.h"
#include "estruturas.h"
#include "constantes.h"

void desenhaPlanoDeFundo(GameObject* fundo) {
    // plano de fundo - montanhas
    glPushMatrix();
        glScalef(1.0, 30.0, 90.0);
        glTranslatef(-600.0, 12.0, 0.0);
        glRotatef(90.0, 0, 1, 0);
        glRotatef(70.0, 1, 0, 0);
        glRotatef(180.0, 0, 1, 0);
        glRotatef(-30.0, 1, 0, 0);
        (*fundo).render();
    glPopMatrix();
}

void desenhaSolo(GameObject* solo) {
    // Solo
    glPushMatrix();
        glTranslatef(0, -5.2, 0);
        glScalef(40.0, 1.0, 50.0);
        glRotatef(-19, 1, 0, 0);
        (*solo).render();
    glPopMatrix();
}

void desenhaAlvo(GameObject* alvo) {
    // alvo
    glPushMatrix();
        glTranslatef(-70.0, 10.0, 10.0);
        glRotatef(90, 0, 0, 1);
        (*alvo).render();
    glPopMatrix();
}


void desenhaProjeteis(Projetil* projeteis, int projeteisDisparados) {
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
}


void desenhaTorpedos(Torpedo* torpedos, Helicoptero* helicoptero) {
    int i;
    for (i = 0; i < NUM_TORPEDOS; i++) {
        glPushMatrix();
            if (torpedos[i].disparado == OFF) {
                glTranslatef(helicoptero->x, helicoptero->y, helicoptero->z);
                glRotatef(helicoptero->rotateY, 0, 1, 0);
                glTranslatef(torpedos[i].eixoX, -3.0, 3.0);
            } else {
                glTranslatef(torpedos[i].translateX, torpedos[i].translateY, torpedos[i].translateZ);
                glRotatef(torpedos[i].rotateY, 0, 1, 0);
                glTranslatef(torpedos[i].eixoX, -3.0, 3.0);
                glTranslatef(0.0, 0.0, torpedos[i].deslocamentoZ);
            }
            torpedos[i].torpedo.render();

        glPopMatrix();
    }
}
