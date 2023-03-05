#include "../../include/States/State.h"

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;

/// Il faut bien sûr définir les variables statiques ici ///
// public:
NextState State::nextState = NextState::None;
// protected:
sf::RenderWindow* State::window;
sf::Event State::e;
sf::Sprite State::background;
sf::Music* State::music = new sf::Music;
int State::volumeSon = State::initVolume(true);
int State::volumeMusic = State::initVolume(false);
bool State::cheatMusic;
unsigned State::vaisseauChoisi;
unsigned State::niveauChoisi;
bool State::sameState = false;
int State::startTimeGame, State::startTimePause;

// private:
FenetrePopup State::popup;
sf::Texture State::backgroundT;
sf::Thread* State::threadChangeMusic;
string State::nameMusicPlaying;

/* Fonction qui nous sert à charger les fichiers
 * communs à tous les états ou encore réaliser des actions
 * d'initialisation nécessaire à n'importe quel State.
 * De plus, elle bind la window créée dans le GameManager. */
void State::mainInit(sf::RenderWindow* window)
{
    // Objet window pour la fct 'show' des states
    State::window = window;

    // vrai aléatoire
    srand((unsigned)time(NULL));

    // Souris hors de l'écran
    if (sf::Joystick::isConnected(0))
        sf::Mouse::setPosition((sf::Vector2i)screenSize);

    // Test de détection du Joystick par la libraire JSL
    int nbJslDevices = wl::Joystick::updateJsl();
    if (sf::Joystick::isConnected(0) && nbJslDevices == 0)
        cout << "Error: Joystick not detected by JSL." << endl;
    wl::Joystick::jslJoystickId = nbJslDevices - 1;                         // Rend compatible avec des logiciels externes comme DS4Windows

    // Initialisation vibrations et gestion erreur intialisation des options
    if (!wl::Joystick::initVibrations() || State::volumeSon == -1 || State::volumeMusic == -1)
        State::reWriteOptions();

    // Chargement + Init Background
    if (!State::backgroundT.loadFromFile(assetsPath + "/Images/Background.png"))
        cout << "Erreur chargement du background" << endl;

    State::background.setTexture(State::backgroundT);
    State::background.setScale(screenSize.x / State::background.getLocalBounds().width, screenSize.y / State::background.getLocalBounds().height);

    // Musique en boucle
    State::music->setLoop(true);

    // Création des vaisseaux
    Vaisseau::CreationVaisseaux();
}

/* Fct qui affiche/update des potentielles
 * informations communes à tous les états. */
void State::mainShow()
{
    // Si la fenetre Pop-up doit affiché qq chose, elle l'affiche ici.
    State::popup.show(State::window);

    // On remet à jour la JSL si l'état du Joystick change
    static bool joystickConnected = sf::Joystick::isConnected(0);
    if (joystickConnected != sf::Joystick::isConnected(0)) {
        bool oldIsBluetooth = wl::Joystick::isBluetooth;
        wl::Joystick::updateJsl();
        if (!oldIsBluetooth && wl::Joystick::isBluetooth)
            wl::Joystick::jslJoystickId++;
    }
}

/* Demande d'affichage de fenêtre Pop-up. */
void State::affichePopup(Icone icone, string titre, string message, sf::Color contourColor, sf::Color textColor, sf::Color backgroundColor)
{
    State::popup.affiche(icone, titre, message, State::volumeSon, contourColor, textColor, backgroundColor);
}

void State::affichePopup(sf::Sprite* icone, string titre, string message, sf::Color contourColor, sf::Color textColor, sf::Color backgroundColor)
{
    State::popup.affiche(icone, titre, message, State::volumeSon, contourColor, textColor, backgroundColor);
}

/* Demande de changement de musique. */
void State::setMusic(string musicFile)
{
    if (State::nameMusicPlaying != musicFile)
    {
        State::threadChangeMusic = new sf::Thread(&State::ChangerdeMusique, musicFile);     // On set la fonction à exécuter par le thread ainsi que son argument (qui est le nom du fichier de la nouvelle musique à mettre)
        State::threadChangeMusic->launch();                                                 // On lance le thread
        State::nameMusicPlaying = musicFile;
    }
}

/* Fonction s'exécutant dans le thread 'threadChangeMusic'.
 * Baisse le volume de la musique actuelle progressivement 
 * puis change de musique pour celle contenue dans le fichier 'musicFile' */
void State::ChangerdeMusique(string musicFile)
{
    if (!State::cheatMusic)
    {
        for (float i = State::volumeMusic; i >= 0; --i)
        {
            State::music->setVolume(i);
            sf::sleep(sf::milliseconds(2.f * 100.f / State::volumeMusic));                  // "* 100.f / State::volumeMusic" pour que le decrescendo dure tout le temps la même durée peu importe le volume de la musique
        }

        State::music->stop();
        if (!State::music->openFromFile(musicFile))
            cout << "Erreur chargement de la musique " << musicFile << endl;
        State::music->setVolume(State::volumeMusic);
        State::music->play();
    }
}

/* Fonction qui gère le statut de l'objet sf::Music 'music' via la
 * variable bouléenne 'mute' tous les deux communs à tous les états. */
void State::gestionMute()
{
    switch (State::music->getStatus())
    {
    case sf::Music::Playing:
        State::music->setVolume(State::volumeMusic);
        if (!State::volumeMusic)
            State::music->pause();
        break;

    case sf::Music::Paused:
        if (State::volumeMusic)
            State::music->play();
        break;
    }
}

/* Fonctions qui initialise les parametres du jeu inscrits
 * dans assetsPath + "/Game Data/Options.txt". */
int State::initVolume(bool son)
{
    ifstream options(assetsPath + "/Game Data/Options.txt");
    if (!options) {
        cout << "Error: Could not open file + \"" + assetsPath + "/Game Data/Options.txt\"" << endl;
        return 0;
    }

    string line;
    while (!options.eof()) {
        getline(options, line);
        if (line.find(son ? "Son" : "Musique") != string::npos) {
            size_t pos = line.find(':');
            if (pos == string::npos)
                break;
            int volume = stoi(line.substr(pos + 1));
            if (volume < 0 || volume > 100)
                break;
            return volume;
        }
    }
    options.close();

    return -1;
}

void State::reWriteOptions()
{
    if (State::volumeSon == -1)
        State::volumeSon = 50;
    if (State::volumeMusic == -1)
        State::volumeMusic = 50;

    string content = "Son : " + to_string(State::volumeSon)
        + "\nMusique : " + to_string(State::volumeMusic)
        + "\nVibrations : " + (wl::Joystick::isRumbleActivated() ? "on" : "off");

    ofstream options(assetsPath + "/Game Data/Options.txt");
    if (!options) {
        cout << "Error: Could not open file + \"" + assetsPath + "/Game Data/Options.txt\"" << endl;
        return;
    }
    options << content;
}