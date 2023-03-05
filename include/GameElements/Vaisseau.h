#ifndef VAISSEAU_H
#define VAISSEAU_H

#include "../Utils/Joystick.h"
#include "Tir.h"
#include "../DisplayElements/JaugeCirculaire.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;

class Vaisseau
{
public:
    Vaisseau(string nom, int pv, int degats, int nbTirsSimul, float cadenceTir, int nbTirsSemiAuto, float tauxSurchauffe, float vitesse, sf::Vector2f center, int distCanons, float yOffsetTir);    // Ce constucteur est uniquement utilisé au tout début du programme puisqu'il load tous les fichiers qui sont associés au Vaisseau
    Vaisseau(const Vaisseau* v);                                                                                                                                                                    // Constructeur de copie
    ~Vaisseau();

    // Fonction qui déclare tous les Vaisseaux
    static void CreationVaisseaux();

    // Tous les vaisseaux
    static vector<Vaisseau*> TouslesVaisseaux;

    // Fonctions update & show
    void update(vector<Asteroid*>& asteroids, vector<Vaisseau*>& bots, int volumeSon, bool asteroidsHurt, float frameTime);
    void show(sf::RenderWindow* window);

    // Get functions
    sf::Sprite* getSprite() const;
    string getName() const { return this->nom; }

    // Fonctions annexes
    void meurs() { this->estTouche(this->pv); }
    bool estVivant() { return (this->pv > 0); }
    bool mustBeDeleted()                                                // Indique si le Vaisseau doit être détruit
    { return (this->cptAnim >= 80 && this->tirs.empty()); }

protected:
    // Fonctions qui gèrent les tirs (aussi utilisées par les bots)
    void tire(bool tire, int volumeTirs);                   // La variable bool 'tire' est à true si le Vaisseau veut tirer
    void updateTirs(vector<Asteroid*>& asteroids, bool asteroidsHurt);

    // Fonction qui update le volume et la position des sons
    void updateSons(int volume);

    // Caractéristiques nécessaires aux bots
    float vitesse;                                          // La vitesse est une caractéristique modifiable uniquement par les bots (elle s'exprime en px/s)
    float cadenceTir;                                       // Nb de tirs par seconde
    const float tauxSurchauffe;                             // Pourcentage de surchauffe infligé par un tir sur un canon (100% = Un seul tir entraîne une surchauffe totale du canon (= impossible de tirer pdt un laps de tps))
    const int distCanons;                                   // Si nbTirsSimul > 1

    // Permet d'eviter de le passer a chaque fonction en ayant besoin
    float frameTime;

    // Affichage
    sf::Sprite* sprite;
    sf::RectangleShape* barrePv, *barrePvNoire;             // La barrePvNoire se met devant la barrePv quand on perd des pv

    // Animation Explosion
    void animationExplosion();
    sf::Sprite* explosionSprite;

    // Vitesses instantanées
    sf::Vector2f instantSpeed;
    float instantRotationSpeed;

    // Force (nécessaire lors d'un impact avec un autre Vaisseau)
    sf::Vector2f force;

    // Fonctions qui, testent, gèrent et affiche les impacts (d'Asteroids, Tirs ou Vaisseau ennemi) avec le Vaisseau
    bool collides(const sf::Sprite* otherSprite);
    void gestionImpactsAsteroids(vector<Asteroid*>& asteroids, bool asteroidsHurt);
    void gestionImpactsAdversaire(Vaisseau* adversaire);
    void animationImpact();

    // Variable attribut
    JaugeCirculaire* jaugeSurchauffe;                       // Lorsqu'on tire trop longtemps, les canons surchauffent (= jaugeSurchauffe à 0%)

    // Fonctions annexes
    void resteDansLecran();
    void setRotationJauge(float angle);
    virtual bool isBot();

private:
    // Caractéristiques
    const string nom;
    int pv;                                                 // Pts de vies du Vaisseau (dès que ce nombre tombe à 0, c'est Game Over)
    const sf::Vector2f center;                              // Centre de rotation
    const int degats;
    const int nbTirsSimul;                                  // Nb de tirs en même temps
    const int nbTirsSemiAuto;
    const float yOffsetTir;

    // Fonctions de mouvement
    void calcMoveForce();
    void move();
    void pivote();

    // Fichiers lourds chargés au début du programme
    static map<string, sf::Texture*> skins;
    static sf::Texture* explosionT;
    static map<string, sf::SoundBuffer*> buffersPew;
    static sf::SoundBuffer* bufferSurchauffe;
    static sf::SoundBuffer* bufferExplosion;
    static sf::SoundBuffer* bufferImpact;
    static sf::SoundBuffer* bufferLaserImpact;

    // Son
    sf::Sound* surchauffe;
    sf::Sound* impact;
    sf::Sound* laserImpact;
    sf::Sound* explosionSound;

    // Vibrations
    enum class RumbleMode { None, Tir, Impact, Explosion };
    RumbleMode rumbleMode;
    sf::Clock clkRumble;
    void updateRumble();
    void rumble(RumbleMode mode);

    // Impact
    void estTouche(int degats);

    // Variables animation
    int cptClignotement;                                    // Compte les phases de clignotement pendant la période d'invincibilité
    float cptAnim;                                          // Un compteur qui s'incrémente pdt l'animation, correspondant au numéro de l'image à afficher
    
    // Variables qui gèrent les tirs
    vector<Tir*> tirs;                                      // Ensemble de tous les tirs associé au vaisseau
    sf::Clock tirsClock;
    int cptTirsSemiAuto;

    // Variables Impacts
    enum class Impact {None, Normal, Invincible} impactType;// None : pas d'impact en cours, Normal : impact classique en cours, Invincible : impact entraînant une phase d'invincibilité (donc si impactType == Impact::Invincible, le Vaisseau est invincible)
    sf::Clock clkInvincible;                                // Clock pour la phase d'invincibilité
    const Asteroid* saveAsteroidColliding;                  // Pour contourner les bugs de collision

    // Variable accessoire à la fonction pivote
    float oldAngle;

    // Constante attribut
    const int pvMax;
    
    // Fonction accessoire
    float calculFact(int) const;
};


// Fonctions outils
sf::Vector2f getCorner(const sf::Sprite* rect, int corner);
int signOf(float val);

#endif // VAISSEAU_H
