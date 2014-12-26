#include "GameObject.h"
#include "estruturas.h"
#include "constantes.h"

void desenhaPlanoDeFundo(GameObject* fundo) {
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
    glPushMatrix();
        glTranslatef(0, -5.2, 0);
        glScalef(40.0, 1.0, 50.0);
        glRotatef(-19, 1, 0, 0);
        (*solo).render();
    glPopMatrix();
}

void desenhaAlvo(GameObject* alvo) {
    glPushMatrix();
        glTranslatef(-70.0, 10.0, 10.0);
        glRotatef(90, 0, 0, 1);
        (*alvo).render();
    glPopMatrix();
}


void desenhaHelicoptero(Helicoptero* helicoptero, GameObject* mira, GameObject* helice, GameObject* rotor_cauda, GameObject* janela, GLfloat heliceRotate) {
glPushMatrix();
        glTranslatef(helicoptero->x, helicoptero->y, helicoptero->z);
        glRotatef(helicoptero->rotate, 0, 1, 0);

        glPushMatrix(); // corpo do helicoptero
            helicoptero->corpo.render();
            glTranslatef(0.0, -5.0, 50.0);
            glRotatef(90, 1, 0, 0);
            (*mira).render();
        glPopMatrix();

        glPushMatrix(); // helice
            glTranslatef(0.0, 4.0, 3.0);
            glRotatef(heliceRotate, 0, 1, 0);
            (*helice).render();
        glPopMatrix();

        glPushMatrix(); // janela
            glTranslatef(0.0, 0.0, 0.01);
            (*janela).render();
        glPopMatrix();

        glPushMatrix(); // rotor de cauda
            glTranslatef(0.5, 5.0, -22.5);
            glScalef(0.5, 0.16, 0.16);
            glRotatef(-90, 0, 0, 1);
            glRotatef(heliceRotate, 0, 1, 0);
            (*rotor_cauda).render();
        glPopMatrix();

    glPopMatrix();
}



void desenhaTorpedos(Torpedo* torpedos, Helicoptero* helicoptero) {
    int i;
    for (i = 0; i < NUM_TORPEDOS; i++) {
        glPushMatrix();
            if (torpedos[i].disparado == OFF) {
                glTranslatef(helicoptero->x, helicoptero->y, helicoptero->z);
                glRotatef(helicoptero->rotate, 0, 1, 0);
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


void desenhaPersonagem(Personagem* personagem) {
    glPushMatrix();
        glTranslatef(personagem->x, personagem->y, personagem->z);
        glRotatef(personagem->rotateTronco, 0, 1, 0);
        /** Tronco **/
        glPushMatrix();
            glColor3f(0.0, 0.0, 1.0);
            glScalef(0.5, 1.5, 1.0);
            glutSolidCube(1.0);
            glColor3f(1.0, 1.0, 1.0);
        glPopMatrix();

        /** Cabeca **/
        glPushMatrix();
            glColor3f(1.0, 0.0, 1.0);
            glTranslatef(0.0, 1.3, 0.0);
            glutSolidCube(1.0);
            glColor3f(1.0, 1.0, 1.0);
        glPopMatrix();

        /** Bracos **/
        glPushMatrix();
            // Braço direito
            glPushMatrix();
                glColor3f(0.0, 0.0, 1.0);
                glScalef(0.5, 0.5, 0.5);
                glTranslatef(0.0, 1.3, 1.5);
                glRotatef(personagem->rotateBracoDir, 0.0, 0.0, 1.0);
                glTranslatef(0.0, -0.2, 0.0);
                glutSolidCube(1.0);
                glColor3f(1.0, 1.0, 1.0);

                glPushMatrix();
                    glColor3f(1.0, 0.0, 0.0);
                    glScalef(1.0, 2.0, 1.0);
                    glTranslatef(0.0, -0.75, 0.0);
                    glutSolidCube(1.0);
                    glColor3f(1.0, 1.0, 1.0);
                glPopMatrix();
            glPopMatrix();

            // Braço esquerdo
            glPushMatrix();
                glColor3f(0.0, 0.0, 1.0);
                glScalef(0.5, 0.5, 0.5);
                glTranslatef(0.0, 1.3, -1.5);
                glRotatef(personagem->rotateBracoEsq, 0.0, 0.0, 1.0);
                glTranslatef(0.0, -0.2, 0.0);
                glutSolidCube(1.0);
                glColor3f(1.0, 1.0, 1.0);

                glPushMatrix();
                    glColor3f(1.0, 0.0, 0.0);
                    glScalef(1.0, 2.0, 1.0);
                    glTranslatef(0.0, -0.75, 0.0);
                    glutSolidCube(1.0);
                    glColor3f(1.0, 1.0, 1.0);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();

        /** Pernas **/
        glPushMatrix();
            // Perna direita
            glPushMatrix();
                glColor3f(0.0, 1.0, 0.0);
                glTranslatef(0.0, -0.675, 0.25);
                glRotatef(personagem->rotatePernaDir, 0, 0, 1);
                glTranslatef(0.0, -0.675, 0.0);
                glScalef(0.5, 1.2, 0.5);
                glutSolidCube(1.0);
                glColor3f(1.0, 1.0, 1.0);

                glPushMatrix();
                    glColor3f(1.0, 0.3, 0.7);
                    glTranslatef(0.0, -0.6, 0.0);
                    glScalef(1.0, 0.2, 1.0);
                    glutSolidCube(1.0);
                    glColor3f(1.0, 1.0, 1.0);
                glPopMatrix();
            glPopMatrix();

            // Perna esquerda
            glPushMatrix();
                glColor3f(0.0, 1.0, 0.0);
                glTranslatef(0.0, -0.675, -0.25);
                glRotatef(personagem->rotatePernaEsq, 0, 0, 1);
                glTranslatef(0.0, -0.675, 0.0);
                glScalef(0.5, 1.2, 0.5);
                glutSolidCube(1.0);
                glColor3f(1.0, 1.0, 1.0);

                glPushMatrix();
                    glColor3f(1.0, 0.3, 0.7);
                    glTranslatef(0.0, -0.6, 0.0);
                    glScalef(1.0, 0.2, 1.0);
                    glutSolidCube(1.0);
                    glColor3f(1.0, 1.0, 1.0);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();


    glPopMatrix();
}
















// Função que recebe a fonte e um texto por parâmetro para ser exibido na
// tela usando fonte de linhas
void desenhaTextoStroke(void *font, char *string)
{
	// Exibe caractere a caractere
	while(*string)
		glutStrokeCharacter(GLUT_STROKE_ROMAN,*string++);
}

void desenhaTexto(char* num_torp, char* num_met) {
    // Posiciona o texto stroke usando transformações geométricas
	glPushMatrix();
        glTranslatef(27.25,5.8,-4.6);
        glScalef(0.002, 0.002, 0.002); // diminui o tamanho do fonte
        glRotatef(90, 0 , 1 , 0); // rotaciona o texto
        glLineWidth(4); // define a espessura da linha
        glColor3f(0,20,0);
        desenhaTextoStroke(GLUT_STROKE_ROMAN, num_torp);
        glColor3f(1.0, 1.0, 1.0);
	glPopMatrix();

	// Posiciona o texto stroke usando transformações geométricas
	glPushMatrix();
        glTranslatef(27.25,5.4,-4.3);
        glScalef(0.002, 0.002, 0.002); // diminui o tamanho do fonte
        glRotatef(90, 0 , 1 , 0); // rotaciona o texto
        glLineWidth(4); // define a espessura da linha
        glColor3f(0,20,0);
        desenhaTextoStroke(GLUT_STROKE_ROMAN, num_met);
        glColor3f(1.0, 1.0, 1.0);
	glPopMatrix();
}
