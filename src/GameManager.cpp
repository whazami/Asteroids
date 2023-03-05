#include "../include/GameManager.h"

#include <SFML/Graphics.hpp>

#include <Windows.h>

// Constructor
GameManager::GameManager()
{
    SetConsoleTitle(L"SFML Debug");
    this->window = new sf::RenderWindow(sf::VideoMode::getFullscreenModes()[0], "Asteroids!", sf::Style::Fullscreen);
    this->window->setVerticalSyncEnabled(true);     // FPS de la fenêtre égales à celles de l'écran
    State::mainInit(this->window);

    this->setState(this->menuDemarrage);
}

// Main Loop
void GameManager::run()
{
    while (this->window->isOpen())
    {
        // Update
        this->getActualState()->update(this->clkFrame.restart().asSeconds());
        State::gestionMute();

        // Show
        this->window->clear();
        this->getActualState()->show();
        State::mainShow();
        this->window->display();

        // Transitions de states
        this->gestionStates();
    }
}

// Fct qui gère les transitions d'un state à l'autre
void GameManager::gestionStates()
{
    switch (State::nextState)
    {
    case NextState::MenuDemarrage:
        if (this->getActualState() == &this->menuPause)
            this->popState();
        this->setState(this->menuDemarrage);
        break;

    case NextState::ChoixNiveau:
        this->setState(this->choixNiveau);
        break;

    case NextState::ChoixVaisseau:
        this->setState(this->choixVaisseau);
        break;

    case NextState::InGame:
        if (this->getActualState() == &this->menuPause)
            this->popState();
        this->setState(this->inGame);
        break;

    case NextState::BackToGame:
        this->popState();
        break;

    case NextState::MenuPause:
        this->pushState(this->menuPause);
        break;
    }

    State::nextState = NextState::None;
}


/// FCTS GERANT LE STACK DE STATES ///
void GameManager::pushState(State& state)
{
    // Set l'actual State
    this->states.push(&state);
    this->states.top()->init();
}

void GameManager::setState(State& state)
{
    // Supprime l'actual State (s'il y en a un)
    this->popState();

    // Ajoute le nouveau State
    this->pushState(state);
}

void GameManager::popState()
{
    if (!this->states.empty())
    {
        states.top()->release();
        this->states.pop();
    }
}