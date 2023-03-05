#ifndef CHOIXNIVEAU_H
#define CHOIXNIVEAU_H

#include "State.h"

#include <windows.h>

#include <SFML/Graphics.hpp>

#include <fstream>

#include <string>
#include <vector>

class S_ChoixNiveau : public State
{
public:
	S_ChoixNiveau();
	void init();
	void update(float frameTime);
	void show();
	void release();

private:
	// Affichage des niveaux
	int nbNiveauxDebloques;
    vector<sf::RectangleShape> niveaux;
    vector<sf::Text> numNiveaux;
    sf::Font retroFont;
    sf::Text textChoose;
	sf::Clock clicloc;

	// Affichage de la fleche de retour
	sf::Texture flecheT;
	sf::Sprite fleche;
	sf::RectangleShape rectFleche;
	sf::Texture rondT;
	sf::Sprite rond;

	const int NB_NIVEAUX_COLONNE, NB_MAX_NIVEAUX;

    // Fcts annexes
    void moveRectSelec(sf::Event& e);
	void changeNiveauChoisiWithMouse(sf::Event::MouseMoveEvent posMouse);
};

#endif // CHOIXNIVEAU_H