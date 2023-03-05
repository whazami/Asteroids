#ifndef CHOIXVAISSEAU_H
#define CHOIXVAISSEAU_H

#include "State.h"

#include <windows.h>

#include <SFML/Graphics.hpp>

#include <fstream>

#include <string>
#include <vector>

#define NB_SHIPS_PER_LINE 8

class S_ChoixVaisseau : public State
{
public:
    S_ChoixVaisseau();

    // Main functions
    void init();
    void update(float frameTime);
    void show();
    void release() {}

private:
    int nbVaisseauxDebloques;
    sf::Font retroFont;
    sf::Text textChoose;
    sf::RectangleShape rectSelec;       // Rectangle de sélection du Vaisseau
    sf::Clock rectClock;                // Pour le clignotement de rectSelec
    
    // Fcts annexes
    void moveRectSelec(sf::Event e);
    void changeVaisseauChoisiWithMouse(sf::Event::MouseMoveEvent posMouse);
    void cheatCode();
};

#endif // CHOIXVAISSEAU_H
