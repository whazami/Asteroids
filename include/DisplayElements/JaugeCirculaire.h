#ifndef JAUGERONDE_H
#define JAUGERONDE_H

#include "../Utils/Constantes.h"

#include <SFML/Graphics.hpp>

class JaugeCirculaire
{
public:
	JaugeCirculaire();
	~JaugeCirculaire();

	void setPercentage(float percentage);	// percentage = float à une décimale
	float getPercentage();

	void setPosition(sf::Vector2f pos);
	void move(sf::Vector2f offset);

	void show(sf::RenderWindow* window);

	void makeInvisible();					// Rend la jauge invisible, elle redevient visible à l'utilisation de 'setPercentage'

private:
	sf::CircleShape* cercle;				// Son contour est la jauge
	sf::RectangleShape* tabLignes[1000];	// Les lignes recouvrent le contour du cercle (on gère donc le pourcentage en rendant ces lignes noires au lieu de transparentes et vice-versa)

	float percentage;						// Le pourcentage ne peut pas avoir plus qu'une décimale (il y a 1000 lignes)

	// Fct annexe
	void moveLineOnTheJauge(sf::RectangleShape& line);
};

#endif // JAUGERONDE_H