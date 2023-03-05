#include "../../include/DisplayElements/FenetrePopup.h"

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include <Windows.h>

using namespace std;

// Init dimensions + positions éléments de la fenêtre
FenetrePopup::FenetrePopup() : windowSize(650.f * screenScaleX, 160.f * screenScaleY), posFixe(sf::Vector2f(screenSize.x - windowSize.x - (30.f * screenScaleX), 30.f * screenScaleY))
{
	// Support fenêtre
	this->fenetre.setSize(windowSize);
	this->fenetre.setOutlineThickness(3.f * screenScaleX);
	this->fenetre.setPosition(-this->fenetre.getSize() - sf::Vector2f(this->fenetre.getOutlineThickness(), this->fenetre.getOutlineThickness()));	// On met la fenêtre en dehors de l'écran

	// Icone
	this->icone.setPosition(this->fenetre.getPosition() + sf::Vector2f(40.f * screenScaleX, 40.f * screenScaleY));

	// Textes
	if (!this->retroFont.loadFromFile("Assets/Fonts/Retro Gaming.ttf"))
		cout << "Erreur chargement de la font." << endl;
	this->titre.setFont(this->retroFont);
	this->titre.setCharacterSize(25 * screenScaleX);
	this->titre.setPosition(this->icone.getPosition() + sf::Vector2f(40 * screenScaleX, -15 * screenScaleY));

	this->message.setFont(this->retroFont);
	this->message.setCharacterSize(20 * screenScaleX);
	this->message.setPosition(titre.getPosition() + sf::Vector2f(0, 40 * screenScaleY));

	// Sound
	if (!this->soundBuffer.loadFromFile("Assets/Sons/Notification.wav"))
		cout << "Erreur du chargement du son de la notification de la fanêtre popup." << endl;
	this->sound.setBuffer(this->soundBuffer);
}

// Fonction de demande d'affichage
void FenetrePopup::affiche(sf::Sprite* icone, string titre, string message, int volume, sf::Color contourColor, sf::Color textColor, sf::Color backgroundColor)
{
	this->fenetre.setFillColor(backgroundColor);
	this->fenetre.setOutlineColor(contourColor);

	this->icone = *icone;
	this->icone.setColor(sf::Color::White);
	this->icone.setOrigin(this->icone.getLocalBounds().width / 2.f, this->icone.getLocalBounds().height / 2.f);
	this->icone.setPosition(this->fenetre.getPosition() + sf::Vector2f(40.f * screenScaleX, 40.f * screenScaleY));
	this->icone.setScale((40.f * screenScaleX) / this->icone.getLocalBounds().width, (40.f * screenScaleX) / this->icone.getLocalBounds().width);		// Il faut que l'icône fasse 40 pixels de largeur

	if (titre.length() > 30)
		titre = "* Erreur : titre trop long. *";
	this->titre.setString(titre);
	this->titre.setFillColor(textColor);

	if (message.length() > 36)
		message[message.find_last_of(' ', 36)] = '\n';
	if (message.length() > 72)
		message[message.find_last_of(' ', 72)] = '\n';
	if (message.length() > 108)
		message = "* Erreur : message trop long. *";
	this->message.setString(message);
	this->titre.setFillColor(textColor);

	this->clk.restart();
	this->setPosition(this->posFixe - sf::Vector2f(0, this->fenetre.getSize().y - this->fenetre.getOutlineThickness()));								// On met la fenêtre juste au-dessus de 'this->pos' (en dehors de l'écran)
	
	this->sound.setVolume(volume);
	this->sound.play();
}

void FenetrePopup::affiche(Icone icone, string titre, string message, int volume, sf::Color contourColor, sf::Color textColor, sf::Color backgroundColor)
{
	if (!this->iconeT.loadFromFile("Assets/Images/Icones/" + this->getStrIcone(icone) + ".png"))
		cout << "Erreur de chargement de l'icone " << this->getStrIcone(icone) << endl;
	this->icone.setTexture(iconeT);

	this->affiche(&this->icone, titre, message, volume, contourColor, textColor, backgroundColor);
}

// Affichage de la fenetre dans un thread
void FenetrePopup::show(sf::RenderWindow* window)
{
	if (this->clk.getElapsedTime().asSeconds() < 4)
	{
		if (this->fenetre.getPosition().y < this->posFixe.y)																							// Animation d'apparition de la fenêtre
			this->move(0, 10.f * screenScaleY);

		window->draw(this->fenetre);
		window->draw(this->icone);
		window->draw(this->titre);
		window->draw(this->message);
	}
}

// Set la position du sf::RectangleShape et les autres éléments de la fenêtre suivent
void FenetrePopup::setPosition(sf::Vector2f pos)
{
	sf::Vector2f vectMove = this->fenetre.getPosition();
	this->fenetre.setPosition(pos);
	vectMove = this->fenetre.getPosition() - vectMove;

	this->icone.move(vectMove);
	this->titre.move(vectMove);
	this->message.move(vectMove);
}

void FenetrePopup::move(float offsetX, float offsetY)
{
	this->fenetre.move(offsetX, offsetY);
	this->icone.move(offsetX, offsetY);
	this->titre.move(offsetX, offsetY);
	this->message.move(offsetX, offsetY);
}

// Fonction annexe
string FenetrePopup::getStrIcone(Icone icone)
{
	switch (icone)
	{
	case Icone::Musique:
		return "Musique Violet";

	case Icone::RedCross:
		return "Red Cross";
	}
}