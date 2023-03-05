#include "../../include/DisplayElements/JaugeCirculaire.h"

#include <SFML/Graphics.hpp>

#include <iostream>

using namespace std;

JaugeCirculaire::JaugeCirculaire() : percentage(100.f)
{
	this->cercle = new sf::CircleShape(10.f * screenScaleX);
	this->cercle->setOutlineThickness(5.f * screenScaleX);
	this->cercle->setFillColor(sf::Color::Transparent);																// Le rond à l'intérieur de la jauge est transparent									
	this->cercle->setOutlineColor(sf::Color::Green);																// La jauge est initialement à 100%
	this->cercle->setOrigin(this->cercle->getRadius(), this->cercle->getRadius());

	for (int i = 0; i < 1000; i++)
	{
		this->tabLignes[i] = new sf::RectangleShape;
		this->tabLignes[i]->setPosition(this->cercle->getPosition());
		this->tabLignes[i]->setSize(sf::Vector2f(1.f, this->cercle->getRadius() - 1));
		this->tabLignes[i]->setRotation(180);																		// Pour que la ligne 0 soit vers le haut
		this->tabLignes[i]->rotate(i * 360.f / 1000.f);																// Jusqu'ici, les lignes recouvrent parfaitement le 'cercle' (mais pas son contour)
		this->moveLineOnTheJauge(*this->tabLignes[i]);																// On set la position des lignes sur le bout de celles-ci, c'est-à dire sur la partie intérieure du contour
		this->tabLignes[i]->setSize(sf::Vector2f(1.f, this->cercle->getOutlineThickness() + 1));					// Enfin, la longueur des lignes est égale à l'épaisseur du contour (le -1 un peu au-dessus et le +1 ici sont pour les pixels qui dépassent un peu)
		this->tabLignes[i]->setFillColor(sf::Color::Transparent);													// La jauge est initialement à 100%
	}
}

JaugeCirculaire::~JaugeCirculaire()
{
	delete this->cercle;
	for (sf::RectangleShape* ligne : this->tabLignes)
		delete ligne;
}

void JaugeCirculaire::setPercentage(float percentage)
{
	if (percentage < 0.f) percentage = 0.f;																			// Sécurités
	if (percentage > 100.f) percentage = 100.f;

	for (int i = 0; i < 1000 - (10.f * percentage); i++)
		this->tabLignes[i]->setFillColor(sf::Color::Black);															// On rend noires les lignes non-comprises dans le pourcentage

	for (int i = 999; i >= 1000 - (10.f * percentage); i--)
		this->tabLignes[i]->setFillColor(sf::Color::Transparent);													// Les autres on les rend transparentes pour laisser apparaître la couleur du contour

	this->cercle->setOutlineColor(sf::Color((100.f - percentage) * 255.f / 100.f, percentage * 255.f / 100.f, 0));	// RVB : Plus le pourcentage est bas, plus c'est rouge ; Plus le pourcentage est haut, plus c'est vert ; Pas de composante bleue

	this->percentage = percentage;
}

float JaugeCirculaire::getPercentage()
{
	return this->percentage;
}

void JaugeCirculaire::setPosition(sf::Vector2f pos)
{
	sf::Vector2f moveLignes = this->cercle->getPosition();															// On enregistre l'ancienne position du 'cercle' puisque les lignes vont move du même vecteur que celui-ci
	this->cercle->setPosition(pos);
	moveLignes = this->cercle->getPosition() - moveLignes;															// Le vecteur déplacement des lignes vaut donc nouvellePosCercle - oldPosCercle

	for (int i = 0; i < 1000; i++)
		this->tabLignes[i]->move(moveLignes);
}

void JaugeCirculaire::move(sf::Vector2f offset)
{
	this->cercle->move(offset);
	
	for (int i = 0; i < 1000; i++)
		this->tabLignes[i]->move(offset);
}

void JaugeCirculaire::show(sf::RenderWindow* window)
{
	window->draw(*this->cercle);
	for (int i = 0; i < 1000; i++)
		window->draw(*this->tabLignes[i]);
}

void JaugeCirculaire::makeInvisible()
{
	for (int i = 0; i < 1000; i++)
		this->tabLignes[i]->setFillColor(sf::Color::Transparent);
	this->cercle->setOutlineColor(sf::Color::Transparent);
}

// Fct annexe
void JaugeCirculaire::moveLineOnTheJauge(sf::RectangleShape& line)
{
	int factx = (line.getRotation() < 180) ? -1 : 1;																// Ligne sur la partie gauche du cercle
	int facty = (line.getRotation() > 90 && line.getRotation() < 270) ? -1 : 1;										// Ligne sur la partie haute du cercle

	line.setSize(sf::Vector2f(0.01f, line.getSize().y));															// On rend la ligne plus fine pour plus de précision dans les calculs

	line.move(sf::Vector2f(factx * line.getGlobalBounds().width, facty * line.getGlobalBounds().height));			// On déplace la ligne au point au bout de celle-ci, ce qui correspond à la partie intérieure du contour du cercle (= jauge)

	line.setSize(sf::Vector2f(1.f, line.getSize().y));																// On remet l'épaisseur originelle de la ligne
}
