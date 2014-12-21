#include "GameObject.h"

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
