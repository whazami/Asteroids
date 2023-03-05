#ifndef INGAME_H
#define INGAME_H

#include "State.h"
#include "../GameElements/Bot.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

#include <ctime>

using namespace std;

class S_InGame : public State
{
public:
    S_InGame();

    // Main functions
    void init();
    void update(float frameTime);
    void show();
    void release();

private:
    Vaisseau* joueur;
    vector<Asteroid*> asteroids;                                    // Ensemble des Asteroids à l'écran
    vector<Bot*> bots;
    Level currLvl;

    // Gestion affichage objectif
    bool beforeGame;
    sf::Clock clkObj;
    sf::Text level, objectif;

    // Variables annexes à update
    bool objectifAtteint;
    int waveNum;
    int asteroidsDestroyed;

    // Variables Game Over
    enum class GameOver { Null, Retry, DontRetry } gameOverState;   // Null : Le jeu n'est pas en Game Over, Retry : Le curseur est sur 'Yes', DontRetry : le curseur est sur 'No'
    sf::Texture gameOverTexture;
    sf::Sprite gameOverText;
    sf::Font retroFont;
    sf::Text retry, yes, no;
    sf::Clock clkBlinkYesNo;

    // Variables Victory
    sf::Texture victoryTexture;
    sf::Sprite victoryText;
    sf::Clock victoryTime;
    sf::RectangleShape blackRect;
    bool musicStarted;

    // Textes informatifs
    sf::Text timeT, objectifProgress;

    // Fcts annexes
    void updateVaisseaux(float frameTime);
    bool createNewAsteroid();
    void updateAsteroids(float frameTime);
    void updateObjectif();
    void updateTextsInfo();
    void setObjectifString();
    void gameOver(float frameTime);
    void victory(float frameTime);
};

#endif // INGAME_H