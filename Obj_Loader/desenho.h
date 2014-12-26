#ifndef DESENHO_H_
#define DESENHO_H_

void desenhaPlanoDeFundo(GameObject* fundo);
void desenhaSolo(GameObject* solo);
void desenhaAlvo(GameObject* alvo);
void desenhaTorpedos(Torpedo* torpedos, Helicoptero* helicoptero);

void desenhaHelicoptero(Helicoptero* helicoptero, GameObject* mira, GameObject* helice, GameObject* rotor_cauda, GameObject* janela, GLfloat heliceRotate);

void desenhaPersonagem(Personagem* personagem);


void desenhaTextoStroke(void *font, char *string);
void desenhaTexto(char* num_torp, char* num_met);

#endif // DESENHO_H_
