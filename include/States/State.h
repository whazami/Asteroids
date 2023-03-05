#ifndef STATE_H
#define STATE_H

#include "../Utils/Joystick.h"
#include "../GameElements/Vaisseau.h"
#include "../DisplayElements/FenetrePopup.h"
#include "../Utils/FileManager.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

enum class NextState { None, MenuDemarrage, ChoixNiveau, ChoixVaisseau, InGame, BackToGame, MenuPause };

/* Cette classe est une classe abstraite, elle ne 
 * sert qu'à stocker les variables et fonctions communes 
 * à tous les états et jouer le rôle de template de ceux-ci.
 */

class State
{
public:
    // Main State Functions
    virtual void init() = 0;
    virtual void update(float frameTime) = 0;
    virtual void show() = 0;
    virtual void release() = 0;

    // State Management Functions
    static void mainInit(sf::RenderWindow* window); 
    static void mainShow();
    static void gestionMute();

    // Variable used in GameManager to change state
    static NextState nextState;

protected:
    // Variables communes à tous les états
    static sf::RenderWindow* window;
    static sf::Event e;
    static sf::Sprite background;
    static sf::Music* music;
    static int volumeSon;
    static int volumeMusic;
    static bool cheatMusic;                             // Indique si une musique de cheat code est en lecture
    static unsigned vaisseauChoisi;                     // Index du Vaisseau choisi dans mainMenu et nécessaire pour l'init d'inGame
    static unsigned niveauChoisi;
    static bool sameState;
    static int startTimeGame, startTimePause;

    // Fonctions communes à tous les états
    static void affichePopup(Icone icone, string titre, string message, sf::Color contourColor = sf::Color::Red, sf::Color textColor = sf::Color::White, sf::Color backgroundColor = sf::Color::Black);
    static void affichePopup(sf::Sprite* icone, string titre, string message, sf::Color contourColor = sf::Color::Red, sf::Color textColor = sf::Color::White, sf::Color backgroundColor = sf::Color::Black);
    static void setMusic(string musicFile);

private:
    // Fenetre Pop-up d'affichage d'informations tierces
    static FenetrePopup popup;

    // Variables propre à cette classe
    static sf::Texture backgroundT;
    static sf::Thread* threadChangeMusic;
    static string nameMusicPlaying;

    // Fonction de changement de musique associée au 'threadChangeMusic' (pour une transition sonore agréable non bloquante)
    static void ChangerdeMusique(string musicFile);

    // Init user options
    static int initVolume(bool son);
    static void reWriteOptions();
};

#endif // STATE_H
