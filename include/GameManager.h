#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "States/S_MenuDemarrage.h"
#include "States/S_ChoixNiveau.h"
#include "States/S_ChoixVaisseau.h"
#include "States/S_InGame.h"
#include "States/S_MenuPause.h"

#include <iostream>
#include <stack>

#include <SFML/Graphics.hpp>

using namespace std;

class GameManager
{
public:
    // Constructor
    GameManager();

    // Main Loop
    void run();

private:
    /* Objet window du jeu (qui va être bind
     * vers la variable statique 'State::window'). */
    sf::RenderWindow* window;

    // Tous les states
    S_MenuDemarrage menuDemarrage;
    S_ChoixNiveau choixNiveau;
    S_ChoixVaisseau choixVaisseau;
    S_InGame inGame;
    S_MenuPause menuPause;

    // Stack stockant les states sauvegardés et l'actuel
    stack<State*> states;

    // Clock qui cadence tout le jeu
    sf::Clock clkFrame;

    /* Fct appelée dans le main loop qui permet de
     * gérer les transitions d'un state à l'autre
     * en fct de l'actual state et des inputs. */
    void gestionStates();
    
    /// FCTS GERANT LE STACK DE STATES ///
    // Renvoie le State actuel
    State* getActualState() { return (!states.empty()) ? states.top() : NULL; }

    // Push un nouveau State sur le stack (l'ancien n'est pas supprimé)
    void pushState(State& state);

    // Set un nouveau State (l'ancien est donc supprimé)
    void setState(State& state);

    // Supprime le state actuel (à faire qd un state précédent a étà sauvegardé)
    void popState();
};

#endif // GAMEMANAGER_H