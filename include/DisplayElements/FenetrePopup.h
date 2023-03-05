#ifndef FENETREPOPUP_H
#define FENETREPOPUP_H

#include "../Utils/Constantes.h"

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;

enum class Icone { Musique, RedCross };

class FenetrePopup
{
public:
	// Constructor
	FenetrePopup();

	// Fonction de demande d'affichage d'une fenêtre
	void affiche(Icone icone, string titre, string message, int volume, sf::Color contourColor = sf::Color::White, sf::Color textColor = sf::Color::White, sf::Color backgroundColor = sf::Color::Black);
	void affiche(sf::Sprite* icone, string titre, string message, int volume, sf::Color contourColor = sf::Color::White, sf::Color textColor = sf::Color::White, sf::Color backgroundColor = sf::Color::Black);

	// Show
	void show(sf::RenderWindow* window);

private:
	// Fenêtre
	sf::RectangleShape fenetre;
	const sf::Vector2f windowSize;
	const sf::Vector2f posFixe;						// Position de le fenêtre lors de son affichage fixe

	// Icone
	sf::Texture iconeT;
	sf::Sprite icone;

	// Textes
	sf::Font retroFont;
	sf::Text titre, message;

	// Sound
	sf::SoundBuffer soundBuffer;
	sf::Sound sound;

	// Gestion de la durée d'affichage
	sf::Clock clk;

	// Fonctions de mouvement
	void setPosition(sf::Vector2f pos);
	void move(float offsetX, float offsetY);

	// Fonction annexe
	string getStrIcone(Icone icone);
};

#endif // FENETREPOPUP_H