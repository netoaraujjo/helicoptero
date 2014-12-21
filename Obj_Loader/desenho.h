#ifndef DESENHO_H_
#define DESENHO_H_



void desenhaPlanoDeFundo(GameObject* fundo);
void desenhaSolo(GameObject* solo);
void desenhaAlvo(GameObject* alvo);
void desenhaTorpedos(Torpedo* torpedos, Helicoptero* helicoptero);
void desenhaProjeteis(Projetil* projeteis, GLint projeteisDisparados);

#endif // DESENHO_H_
