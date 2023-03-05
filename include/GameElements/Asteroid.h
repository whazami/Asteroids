#ifndef ASTEROID_H
#define ASTEROID_H

#include "../Utils/Constantes.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Asteroid
{
public:
    // Constructor & Destructor
    Asteroid();
    ~Asteroid();

    // Update & Show
    void update(int volumeExplosion, bool explosionArtifice, float frameTime);
    void show(sf::RenderWindow* window);

    // Get functions
    sf::Vector2f getCenter() const;
    int getRadius() const;

    // Collisions
    bool touche(Asteroid* otherA);
    void gestionRebond(Asteroid* otherA);                                           // Si l'Asteroid en touche un autre, ils rebondissent
    bool gestionRebond(const sf::Sprite* rectParoi, const sf::Vector2f& speedParoi,
        const float& rotationSpeedParoi, bool justTest = false);                    // Rebond sur une paroi en mouvement si 2e parametre (utilisà pour la collision avec le Vaisseau)
    void estTouche(int degats);

    // Fct annexe
    bool isTransparent();                                                           // Synonyme de "n'est pas encore détruit ?" puisque pendant l'animation d'explosion, l'Asteroid est transparent
    bool isDestroyed() { return this->pv <= 0; }
    static void loadFiles();                                                        // Charger les textures à chaque création d'Asteroid prend un temps considérable

    // Si l'Asteroid doit être delete
    bool effacer;

private:
    // Variables
    static sf::Texture* texture;
    sf::Sprite* sprite;

    int radius;
    sf::Vector2f speed;                                                             // Pour le déplacement de l'Asteroid
    float rotateSpeed;                                                              // Vitesse de rotation en degrés/images
    int masse;                                                                      // Masse de l'asteroid pour gérer les transferts d'energie pendant une collision
    int pv;

    const int rayonMax;
    const int vitesseMin;
    const int vitesseMax;

    // Variables de collisions pour éviter les bugs
    sf::Clock clkRebondAsteroid;
    sf::Clock clkHorizontal, clkVertical, clkCoin;                                  // Pour chaque partie du rectParoi
    int cptMovesObligatoires;

    bool dansEcran;

    // Variables animations
    static sf::Texture *explosionT, *feudArtificeT;
    sf::Sprite *explosionSprite, *feudArtificeSprite;
    float cptAnim;                                                                  // Un compteur qui s'incrémente pdt l'animation, correspondant au numéro de l'image à afficher
    int couleurArtifice;
    bool explosionArtifice;

    // Variables update
    sf::Clock clkRed;
    float oldFrameTime;

    // Variables sons
    static sf::SoundBuffer *bufferExplosion, *bufferArtifice;
    sf::Sound* explosionSound;
    bool explosionSoundPlayed;

    // Fonction animation explosion
    void animationExplosion(float frameTime);

    // Fonctions annexes
    bool estDansLecran();
};

/// Fonctions supplémentaire pour gérer les sf::Vector2f ///
// Produit scalaire
float operator*(sf::Vector2f v1, sf::Vector2f v2);
// Valeur absolue
sf::Vector2f fabs(const sf::Vector2f& v);

#endif // ASTEROID_H
