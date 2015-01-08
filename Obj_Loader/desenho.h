#ifndef DESENHO_H_
#define DESENHO_H_

void desenhaPlanoDeFundo(GameObject* fundo);
void desenhaSolo(GameObject* solo);
void desenhaAlvo(GameObject* alvo, GLfloat escala, GLfloat translateX, GLfloat translateY, GLfloat translateZ);
void desenhaTorpedos(Torpedo* torpedos, Helicoptero* helicoptero);

void desenhaHelicoptero(Helicoptero* helicoptero, GameObject* mira, GameObject* helice, GameObject* rotor_cauda, GameObject* janela, GLfloat heliceRotate);
void desenhaImagens(GameObject* torpedoImg, GameObject* balaImg, GameObject* relogioImg);

void desenhaPersonagem(Personagem* personagem);


void desenhaTextoStroke(void *font, char *string);
void desenhaTexto(char* num_torp, char* num_met, char* tempo);
void desenhaGameOver(char* pontuacao);
void desenhaWin(char* pontuacao);

#endif // DESENHO_H_
