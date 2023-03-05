#include "../../include/GameElements/Tir.h"

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <math.h>

using namespace std;

map<string, sf::Texture*> Tir::textures;

Tir::Tir(string nomVaisseau, sf::Vector2f Vpos, float Vangle, float yOffset, sf::SoundBuffer* bufferPew, int volume, float pitch, bool myShip)
{
    if (this->textures[nomVaisseau] == NULL)
    {
        this->textures[nomVaisseau] = new sf::Texture;
        if (!this->textures[nomVaisseau]->loadFromFile("Assets/Images/Tirs/" + nomVaisseau + ".png"))
            cout << "Erreur chargement du tir de " << nomVaisseau << endl;
    }

    this->sprite = new sf::Sprite;
    this->sprite->setTexture(*this->textures[nomVaisseau]);

    const float maxWidth = 20.f, maxHeight = 100.f;
    if (this->sprite->getGlobalBounds().width > maxWidth || this->sprite->getGlobalBounds().height > maxHeight)
    {
        float ratio = this->sprite->getGlobalBounds().height / this->sprite->getGlobalBounds().width;
        float scale = (ratio <= 5.f) ? maxWidth / this->sprite->getGlobalBounds().width : maxHeight / this->sprite->getGlobalBounds().height;
        this->sprite->setScale(scale * screenScaleX, scale * screenScaleX);
    }
    this->sprite->setOrigin(this->sprite->getLocalBounds().width / 2.f, this->sprite->getLocalBounds().height);                 // On place l'origine en bas du sprite pour que le tir soit orientà vers l'avant par rapport au vaisseau
    float angle = (90.f - Vangle) * PI / 180.f;                                                                                 // Conversion angle vaisseau (degrés) en radians
    sf::Vector2f offset(cos(angle), -sin(angle));
    yOffset -= this->sprite->getGlobalBounds().height / 2.f;
    this->sprite->setPosition(Vpos + yOffset * offset);
    this->sprite->setRotation(Vangle);

    /// CALCUL DIRECTION ///
    float coeffDir = -tan(angle);                                                                                               // "-" pcq les y sont inversés (console vs vrai maths)
    this->vitesse.x = vitesseTirs / sqrt(1 + pow(coeffDir, 2));                                                                 // Calcul de la "valeur absolue" de l'offset en X
    if (cos(angle) < 0) this->vitesse.x = -this->vitesse.x;                                                                     // On set donc le signe ici
    this->vitesse.y = this->vitesse.x * coeffDir;

    /// PARTIE SON ///
    this->pew = new sf::Sound;
    this->pew->setBuffer(*bufferPew);                                                                                           // On met le buffer dans l'objet son
    this->pew->setPitch(pitch);                                                                                                 // On set la fréquence du son
    this->pew->setVolume(pitch * volume);                                                                                       // On adapte le son de celui-ci en fct de sa fréquence
    this->pew->setAttenuation(0.01f);
    this->pew->setMinDistance(100.f);
    if (myShip)
    {
        this->pew->setRelativeToListener(true);
        this->pew->setPosition(0.f, 0.f, 0.f);
    }
    else
        this->pew->setPosition(Vpos.x, 0.f, Vpos.y);
    this->pew->play();
}

Tir::~Tir()
{
    delete this->sprite;
    delete this->pew;
}

// Fonction move
void Tir::move(float frameTime)
{
    this->sprite->move(this->vitesse * frameTime);
}

// Fonction show
void Tir::show(sf::RenderWindow* window)
{
    window->draw(*this->sprite);
}

// Test collision avec Asteroid
bool Tir::toucheAsteroid(const Asteroid* A)
{
    sf::Vector2f pt = sf::Vector2f(this->sprite->getGlobalBounds().left + this->sprite->getGlobalBounds().width/2,              // Milieu du laser (centre du rect formé par getGlobalBounds)
        this->sprite->getGlobalBounds().top + this->sprite->getGlobalBounds().height / 2.f);
    sf::Vector2f cA(A->getCenter());

    sf::Vector2f vectDiff(cA-pt);

    if (sqrt(pow(vectDiff.x, 2) + pow(vectDiff.y, 2)) <= A->getRadius() && this->sprite->getColor() != sf::Color::Transparent)  // Si le tir collisionne l'Asteroid et le tir est actif (visible à l'écran / n'a pas déjà collisionnà un Asteroid),
    {
        this->sprite->setColor(sf::Color::Transparent);                                                                         // on le rend transparent, ce qui témoigne de son inactivité
        return true;
    }
    return false;
}

// Get functions
sf::Sprite* Tir::getSprite() const
{
    return this->sprite;
}


/// FONCTION ANNEXES ///

bool Tir::estDansLecran()
{
    const int err = 300;                                                                                                        // On rajoute cette constante au test pour être large

    if (this->sprite->getGlobalBounds().left > -(this->sprite->getLocalBounds().width + err)
        && this->sprite->getGlobalBounds().left < screenSize.x + err
        && this->sprite->getGlobalBounds().top > -(this->sprite->getLocalBounds().height + err)
        && this->sprite->getGlobalBounds().top < screenSize.y + err)
        return true;

    return false;
}
