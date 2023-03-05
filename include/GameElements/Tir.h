#ifndef TIR_H
#define TIR_H

#include "Asteroid.h"

#include <iostream>
#include <string>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;

class Tir
{
public:
    Tir(string nomVaisseau, sf::Vector2f Vpos, float angle, float yOffset, sf::SoundBuffer* bufferPew, int volume, float pitch, bool myShip);
    ~Tir();

    // Fonction move
    void move(float frameTime);

    // Fonction show
    void show(sf::RenderWindow* window);

    // Test collision avec Asteroid
    bool toucheAsteroid(const Asteroid* A);

    // Get functions
    sf::Sprite* getSprite() const;

    // Fonctions annexes
    bool estDansLecran();
    bool isSoundFinished() { return this->pew->getStatus() == sf::Sound::Status::Stopped; }

private:
    static map<string, sf::Texture*> textures;
    sf::Sprite* sprite;

    sf::Sound* pew;

    sf::Vector2f vitesse;
};

#endif // TIR_H
