#ifndef BOT_H
#define BOT_H

#include "Vaisseau.h"

#include <random>

/* Un bot peut prendre 3 niveaux différents :
 * - Facile : N'a pas une visée parfaite, sa cadence de tir est divisée par 3
 * - Normal : Est tourné en permanence vers le joueur, s'arrête de tirer quand il y a un Asteroid entre lui et le joueur,
 *            évite d'être dans la ligne de mire du joueur, tire non-stop et ne met jamais ses canons en surchauffe
 * - Difficile : A les mêmes caractéristiques qu'un bot normal mais est capable d'anticiper les mouvements du joueur et donc de s'orienter 
 *               en conséquence, ne peut pas recevoir de dégâts d'autres bots 
 */

class Bot : public Vaisseau
{
public:
	Bot(const Vaisseau* v, int niveau, const vector<Asteroid*>& asteroids = vector<Asteroid*>(), const vector<Bot*>& otherBots = vector<Bot*>());

	void update(vector<Asteroid*>& asteroids, Vaisseau* joueur, vector<Bot*>& otherBots, int volumeSon, float frameTime);

private:
    // Fonctions de mouvement
    void calcMoveForce(vector<Asteroid*>& asteroids, Vaisseau* joueur);
    void pivote(Vaisseau* joueur, vector<Asteroid*>& asteroids);
    void move();

    bool feu;                                                               // Indique si le bot doit tirer

    int niveau;                                                             // Niveau du bot (1 : Facile, 2 : Normal et 3 : Difficile)

    // Variables nécessaires à la fonction 'calcMoveForce'
    std::default_random_engine generator;
    std::normal_distribution<float> randLoiNormale;
    int cptFrame;
    float oldFrameTime;

    // Variables nécessaires à la fonction 'pivote'
    float decalageAngle;
    float oldAngle;
    sf::Vector2f oldPosJoueur;

    // Fonction annexe
    bool aFiniSonEntree();
};

// Fonction outil
bool isColliding(const sf::RectangleShape& rect, const sf::CircleShape& circle);

#endif // BOT_H