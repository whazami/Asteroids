#include "../../include/GameElements/Vaisseau.h"

#include <iostream>
#include <string>
#include <vector>
#include <bitset>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <math.h>

using namespace std;

// Fichiers lourds en static pour ne pas à avoir à les charger plusieurs fois
vector<Vaisseau*> Vaisseau::TouslesVaisseaux;
map<string, sf::Texture*> Vaisseau::skins;
sf::Texture* Vaisseau::explosionT;
map<string, sf::SoundBuffer*> Vaisseau::buffersPew; 
sf::SoundBuffer* Vaisseau::bufferSurchauffe;
sf::SoundBuffer* Vaisseau::bufferExplosion;
sf::SoundBuffer* Vaisseau::bufferImpact;
sf::SoundBuffer* Vaisseau::bufferLaserImpact;

Vaisseau::Vaisseau(string nom, int pv, int degats, int nbTirsSimul, float cadenceTir, int nbTirsSemiAuto, float tauxSurchauffe, float vitesse, sf::Vector2f center, int distCanons, float yOffsetTir) : nom(nom), pv(pv), degats(degats), nbTirsSimul(nbTirsSimul), cadenceTir(cadenceTir), nbTirsSemiAuto(nbTirsSemiAuto), tauxSurchauffe(tauxSurchauffe), vitesse(vitesse* screenScaleX), center(center), distCanons(distCanons), yOffsetTir(yOffsetTir), pvMax(pv)
{
    /// PARTIE IMAGE ///
    this->skins[nom] = new sf::Texture;

    string nameFichierI = "Assets/Images/Vaisseaux/" + nom + ".png";
    if (!this->skins[nom]->loadFromFile(nameFichierI))
        cout << "Erreur chargement de " << nameFichierI << endl;

    this->sprite = new sf::Sprite;
    this->sprite->setTexture(*this->skins[nom]);

    this->jaugeSurchauffe = new JaugeCirculaire;
    this->jaugeSurchauffe->makeInvisible();                                                                                                          // Initialement, la jauge de surchauffe est invisible pour l'affichage du vaisseau

    this->barrePv = new sf::RectangleShape;
    this->barrePvNoire = new sf::RectangleShape;

    /// PARTIE SONS ///
    this->buffersPew[nom] = new sf::SoundBuffer;
    string nameFichierB = "Assets/Sons/Tirs/" + nom + ".wav";
    if (!this->buffersPew[nom]->loadFromFile(nameFichierB))
        cout << "Erreur chargement du son de " << nom << endl;

    this->bufferSurchauffe = new sf::SoundBuffer;
    if (!this->bufferSurchauffe->loadFromFile("Assets/Sons/Surchauffe Canons.wav"))
        cout << "Erreur chargement du son de surchauffe de canon" << endl;

    this->bufferExplosion = new sf::SoundBuffer;
    if (!this->bufferExplosion->loadFromFile("Assets/Sons/Explosions/Explosion.wav"))
        cout << "Erreur chargement du son d'explosion." << endl;

    this->bufferImpact = new sf::SoundBuffer;
    if (!this->bufferImpact->loadFromFile("Assets/Sons/Impacts/Impact.wav"))
        cout << "Erreur chargement du son d'impact." << endl;

    this->bufferLaserImpact = new sf::SoundBuffer;
    if (!this->bufferLaserImpact->loadFromFile("Assets/Sons/Impacts/Laser Impact.wav"))
        cout << "Erreur chargement du son d'impact de laser." << endl;

    /// PARTIE ANIMATION ///
    this->explosionT = new sf::Texture;
    if (!this->explosionT->loadFromFile("Assets/Images/Animations/Explosion.png"))
        cout << "Erreur chargement de la Texture d'explosion." << endl;

    this->explosionSprite = new sf::Sprite;
}

Vaisseau::Vaisseau(const Vaisseau* v) : nom(v->nom), pv(v->pv), degats(v->degats), nbTirsSimul(v->nbTirsSimul), cadenceTir(v->cadenceTir), nbTirsSemiAuto(v->nbTirsSemiAuto), tauxSurchauffe(v->tauxSurchauffe), vitesse(v->vitesse), center(v->center), distCanons(v->distCanons), yOffsetTir(v->yOffsetTir), pvMax(v->pvMax)
{
    /// PARTIE IMAGE ///
    this->sprite = new sf::Sprite;
    this->sprite->setTexture(*this->skins[nom]);

    /// PARTIE SONS ///
    this->surchauffe = new sf::Sound;
    this->explosionSound = new sf::Sound;
    this->impact = new sf::Sound;
    this->laserImpact = new sf::Sound;
    this->surchauffe->setBuffer(*this->bufferSurchauffe);
    this->explosionSound->setBuffer(*this->bufferExplosion);
    this->impact->setBuffer(*this->bufferImpact);
    this->laserImpact->setBuffer(*this->bufferLaserImpact);
    this->surchauffe->setAttenuation(0.01f);
    this->surchauffe->setMinDistance(100.f);
    this->explosionSound->setAttenuation(0.01f);
    this->explosionSound->setMinDistance(100.f);
    this->impact->setAttenuation(0.01f);
    this->impact->setMinDistance(100.f);
    this->laserImpact->setAttenuation(0.01f);
    this->laserImpact->setMinDistance(100.f);

    /// PARTIE ANIMATION ///
    this->explosionSprite = new sf::Sprite;
    this->explosionSprite->setTexture(*this->explosionT);
    this->explosionSprite->setTextureRect(sf::IntRect(-100, -100, 100, 100));                                                                        // On met le sprite en dehors de la Texture à l'initialisation (le sprite est donc transparent)
    this->explosionSprite->setOrigin(this->explosionSprite->getLocalBounds().width / 2.f, this->explosionSprite->getLocalBounds().height / 2.f);

    /// INIT JEU ///
    const float maxWidth = 120.f;
    float ratio = this->sprite->getGlobalBounds().height / this->sprite->getGlobalBounds().width;
    float scale = (ratio <= 1.9f) ? maxWidth / this->sprite->getGlobalBounds().width : 1.9f * maxWidth / this->sprite->getGlobalBounds().height;
    this->sprite->setScale(scale * screenScaleX, scale * screenScaleX);
    this->sprite->setOrigin(this->center);
    this->sprite->setPosition(screenSize / 2.f);                                                                                                    // C'est la position par défaut d'un Vaisseau (celle du joueur), lorsqu'il s'agit d'un bot, elle est modifiée dans son constructeur
    
    this->explosionSprite->setPosition(sf::Vector2f(this->sprite->getGlobalBounds().left, this->sprite->getGlobalBounds().top) + sf::Vector2f(this->sprite->getGlobalBounds().width / 2.f, this->sprite->getGlobalBounds().height / 2.f));
    this->cptAnim = 0;
    this->cptClignotement = 0;
    this->impactType = Impact::None;

    this->jaugeSurchauffe = new JaugeCirculaire;
    this->setRotationJauge(0.f);                                                                                                                      // On positionne la jauge de surchauffe au bon endroit
    this->jaugeSurchauffe->setPercentage(100.f);                                                                                                    // On la rend visible
    
    this->barrePv = new sf::RectangleShape(sf::Vector2f(this->sprite->getGlobalBounds().width, (3.f * screenScaleY)));
    this->barrePv->setFillColor(sf::Color::Red);
    this->barrePv->setOrigin(0.f, this->barrePv->getSize().y / 2.f);
    this->barrePv->setPosition(this->sprite->getGlobalBounds().left, this->sprite->getPosition().y - this->sprite->getGlobalBounds().height / 2.f - (3.f * screenScaleX));
    this->barrePvNoire = new sf::RectangleShape(sf::Vector2f(0.f, 3.f * screenScaleY));
    this->barrePvNoire->setFillColor(sf::Color::Black);
    this->barrePvNoire->setOrigin(0.f, this->barrePvNoire->getSize().y / 2.f);
    this->barrePvNoire->setPosition(this->barrePv->getPosition() + sf::Vector2f(this->sprite->getGlobalBounds().width, 0.f));
    this->barrePvNoire->setRotation(180.f);
}

Vaisseau::~Vaisseau()
{
    delete this->sprite;
    delete this->explosionSprite;
    for (Tir* tir : this->tirs)
        delete tir;
    this->tirs.clear();

    delete this->jaugeSurchauffe;
    delete this->barrePv;
    delete this->barrePvNoire;

    delete this->surchauffe;
    delete this->impact;
    delete this->laserImpact;
    delete this->explosionSound;
}

/// C'EST ICI QU'ON CRÉE LES VAISSEAUX DU JEU ///
void Vaisseau::CreationVaisseaux()
{
    /* PARAMETRES : Nom des fichiers associés au Vaisseau (sans leur extension), Points de vie, Dégâts qu'inflige un laser
                    Nombre de tirs simultanés, Cadence de tir (en tir/s), Nombre de tirs par phase semi-automatique,
                    % de surchauffe en moins à chaque tir, Vitesse de déplacement (en px/s), Centre de rotation, Distance entre les canons, vertical offset tirs */
    Vaisseau::TouslesVaisseaux =
    {
        new Vaisseau("Faucon Millenium",     400,  65, 1,   7.5f, 0,   6.f, 800, sf::Vector2f(75, 133),   0,   0),
        new Vaisseau("X-Wing",               350,  60, 2,   4.5f, 0,  7.5f, 625, sf::Vector2f(75, 126), 135,  60),
        new Vaisseau("A-Wing",               350,  65, 2,    4.f, 0,   5.f, 500, sf::Vector2f(75, 111), 140,  30),
        new Vaisseau("Jedi Starfighter",     300,  30, 2,    9.f, 0,   5.f, 500, sf::Vector2f(75, 123),  17, 120),
        new Vaisseau("Voiture des nuages",   350,  40, 2,    5.f, 0,   3.f, 700, sf::Vector2f(75,  92), 100,  90),
        new Vaisseau("Y-Wing",               350,  65, 2,    4.f, 0,   5.f, 500, sf::Vector2f(75, 176),  10, 176),
        new Vaisseau("N-1 Starfighter",      350,  30, 2,    8.f, 0,   5.f, 700, sf::Vector2f(75, 110),  18,  90),
        new Vaisseau("U-Wing",               350,  85, 2,   3.5f, 0,   5.f, 400, sf::Vector2f(75, 245),  32, 105),
        new Vaisseau("Naboo Royal Starship", 300,  55, 2,    5.f, 0,   5.f, 600, sf::Vector2f(75, 201),  10, 175),
        new Vaisseau("Radiant VII",          800, 115, 2,    2.f, 0, 33.3f, 300, sf::Vector2f(75, 160),  10, 150),
        new Vaisseau("Lambda Shuttle",       350,  50, 2,   7.5f, 2,   5.f, 500, sf::Vector2f(75,  50),  45,  25),
        new Vaisseau("Tie Fighter",          300,  45, 2,    5.f, 0,  7.5f, 625, sf::Vector2f(75,  71),  16,  20),
        new Vaisseau("Tie Bomber",           400,  40, 2,    9.f, 0,  10.f, 300, sf::Vector2f(75,  44),  47,   0),
        new Vaisseau("Slave 1",              300,  30, 2, 11.25f, 3,   6.f, 500, sf::Vector2f(73,  39),  15,  35),
        new Vaisseau("Cimeterre",            500,  15, 2,   13.f, 0,   5.f, 700, sf::Vector2f(75, 189), 140,   0),
        new Vaisseau("Destroyer",            999, 999, 1,    1.f, 0, 100.f, 200, sf::Vector2f(75, 231),   0, 165)
    };
}

// Fonction update (Gestion du mouvement, de la rotation et des tirs du Vaisseau)
void Vaisseau::update(vector<Asteroid*>& asteroids, vector<Vaisseau*>& adversaires, int volumeSon, bool asteroidsHurt, float frameTime)
{
    this->frameTime = frameTime;

    if (this->estVivant())
    {
        this->calcMoveForce();
        this->pivote();
        this->tire(wl::Joystick::isButtonPressed(DualShock::R2) || sf::Mouse::isButtonPressed(sf::Mouse::Left), volumeSon);

        this->gestionImpactsAsteroids(asteroids, asteroidsHurt);
        for (Vaisseau* adversaire : adversaires)
            this->gestionImpactsAdversaire(adversaire);
        this->animationImpact();

        this->move();
    }
    else
        this->animationExplosion();

    this->updateTirs(asteroids, asteroidsHurt);
    this->updateSons(volumeSon);
    this->updateRumble();
}

// Fonction show
void Vaisseau::show(sf::RenderWindow* window)
{
    for(unsigned i=0;i<this->tirs.size();i++)
        this->tirs[i]->show(window);
    window->draw(*this->sprite);
    this->jaugeSurchauffe->show(window);
    window->draw(*this->barrePv);
    window->draw(*this->barrePvNoire);
    window->draw(*this->explosionSprite);
}

/// FONCTIONS PRINCIPALES VAISSEAU ///
void Vaisseau::calcMoveForce()
{
    float coeffDirJoystick, coeffVitesse;
    float Xoffset, Yoffset;

    if (fabs(wl::Joystick::getAxisPosition(sf::Joystick::X)) > 15 || fabs(wl::Joystick::getAxisPosition(sf::Joystick::Y)) > 15)               // Si le joystick gauche a suffisamment bougé
    {
        if (fabs(wl::Joystick::getAxisPosition(sf::Joystick::X)) < 15)                                                                           // Pour éviter d'avoir un coeffDirJoystick qui tend vers l'infini
        {
            Xoffset = 0;
            Yoffset = this->vitesse * fabs(wl::Joystick::getAxisPosition(sf::Joystick::Y) / 100);                                                // Vitesse multipliée par un coeff entre 0 et 1 (dépendant d'à quel point le joystick a bougé)
        }
        else
        {
            coeffDirJoystick = wl::Joystick::getAxisPosition(sf::Joystick::Y) / wl::Joystick::getAxisPosition(sf::Joystick::X);
            coeffVitesse = sqrt(pow(wl::Joystick::getAxisPosition(sf::Joystick::X), 2)                                                           // Norme vecteur Joystick sur 100 : on obtient un coeff entre 0 et 1
                + pow(wl::Joystick::getAxisPosition(sf::Joystick::Y), 2)) / 100.f;
            Xoffset = coeffVitesse * this->vitesse / (sqrt(1 + pow(coeffDirJoystick, 2)));                                                          // Seulement les "valeurs absolues" des offsets sont calculées
            Yoffset = Xoffset * fabs(coeffDirJoystick);
        }
        if (wl::Joystick::getAxisPosition(sf::Joystick::X) < 0) Xoffset = -Xoffset;                                                              // C'est pourquoi on les rend négatifs ici si besoin
        if (wl::Joystick::getAxisPosition(sf::Joystick::Y) < 0) Yoffset = -Yoffset;

        this->force = sf::Vector2f(Xoffset, Yoffset) * this->frameTime;
    }
    else if (!sf::Joystick::isConnected(0))                                                                                                         // Si la manette n'est pas connectée, le Vaisseau se déplace avec le clavier
    {
        this->force = sf::Vector2f(0.f, 0.f);                                                                                                       // Dans ce cas, on réinitialise la force

        // Azerty and Qwerty compatibility at the same time
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))    this->force.x += 1.f;                                                                      // Et on met une unité dans chaque direction correspondant à chaque touche pressée
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) this->force.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))    this->force.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) this->force.y -= 1.f;

        if (this->force != sf::Vector2f(0.f, 0.f))
        {
            this->force /= (float)sqrt(pow(this->force.x, 2) + pow(this->force.y, 2));                                                                     // On normalise le vecteur force
            this->force *= this->vitesse * this->frameTime;                                                                                                    // Et on set sa norme à la vitesse maximale du Vaisseau
        }
    }
    else
        this->force = sf::Vector2f(0, 0);

    this->instantSpeed = this->force;                                                                                                               // Si la fonction 'gestionImpacts' ne modifie pas la vitesse, elle restera égale à la force
}

void Vaisseau::move()
{
    this->sprite->move(this->instantSpeed);                                                                                                         // Bouge le sprite du vaisseau
    this->jaugeSurchauffe->move(this->instantSpeed);
    this->explosionSprite->move(this->instantSpeed);
    this->barrePv->move(this->instantSpeed);
    this->barrePvNoire->move(this->instantSpeed);

    this->resteDansLecran();

    this->instantSpeed = sf::Vector2f(0.f, 0.f);
}

void Vaisseau::pivote()
{
    float angle = this->oldAngle;

    if (fabs(wl::Joystick::getAxisPosition(sf::Joystick::Z)) > 50 || fabs(wl::Joystick::getAxisPosition(sf::Joystick::R)) > 50)               // Si le joystick gauche a suffisamment bougé (Z : X du joystick droit et R : Y du joystick droit)
    {
        float module = sqrt(pow(wl::Joystick::getAxisPosition(sf::Joystick::Z), 2) + pow(wl::Joystick::getAxisPosition(sf::Joystick::R), 2)); // Calcul du module pour scale le paramètre de "acos" entre -1 et 1
        angle = acos(wl::Joystick::getAxisPosition(sf::Joystick::Z) / module);                                                                   // Comme l'ensemble d'arrivée de arccos est [0;PI], en l'utilisant on obtient la "valeur absolue" de l'angle
        if (wl::Joystick::getAxisPosition(sf::Joystick::R) > 0) angle = -angle;                                                                  // C'est pourquoi on rend l'angle négatif ici si nécessaire
        angle *= (180.f / PI);                                                                                                                      // Conversion de l'angle en radians en degrés
        angle = 90.f - angle;                                                                                                                       // Conversion pcq le vaisseau est initialement vers le haut et en trigo l'angle 0 est orientà vers la droite
    }
    else if (!sf::Joystick::isConnected(0))                                                                                                         // Si la manette n'est pas connectée, le Vaisseau pivote en fonction de la position de la souris
    {
        sf::Vector2f vectDiff((sf::Vector2f)sf::Mouse::getPosition() - this->sprite->getPosition());
        angle = 90.f - atan2f(-vectDiff.y, vectDiff.x) * 180.f / PI;
    }

    this->instantRotationSpeed = angle - this->oldAngle;
    this->sprite->setRotation(angle);
    this->setRotationJauge(angle);                                                                                                                  // On effectue aussi la rotation de la jauge de surchauffe

    this->oldAngle = angle;
}

void Vaisseau::tire(bool tire, int volumeTirs)
{
    const float vitesseRechargementSurchauffe = 62.5f;                                                                                              // Nombre de pourcentages récupérés par seconde

    if (tire && this->tirsClock.getElapsedTime().asSeconds() >= 1 / this->cadenceTir && this->jaugeSurchauffe->getPercentage() != 0)                // Si le temps correspondant à la cadence de tir s'est écoulé et si les canons ne sont pas totalement surchauffés
    {
        if (!this->nbTirsSemiAuto || this->cptTirsSemiAuto < this->nbTirsSemiAuto)                                                                  // Si le Vaisseau n'a pas de tir semi-auto ou s'il n'a pas atteint son quotat de ce dernier
        {
            this->jaugeSurchauffe->setPercentage(this->jaugeSurchauffe->getPercentage() - this->tauxSurchauffe);

            if (this->jaugeSurchauffe->getPercentage() == 0 && this->tauxSurchauffe != 100.f)                                                       // Si les canons surchauffent totalement,
                this->surchauffe->play();                                                                                                           // on joue un son pour l'indiquer

            for (int cptTirsSimultanes = 0; cptTirsSimultanes < this->nbTirsSimul; cptTirsSimultanes++)                                             // Boucle qui crée les tirs simultanés du Vaisseau
            {
                Tir* nouveauTir = new Tir(this->nom, this->sprite->getPosition(), this->sprite->getRotation(), this->yOffsetTir * this->sprite->getScale().x, this->buffersPew[nom],
                    cptTirsSimultanes ? 0 : volumeTirs, (this->jaugeSurchauffe->getPercentage() <= 33.3f) ? 0.85f + this->jaugeSurchauffe->getPercentage() * 0.15f / 50.f : 1.f, !this->isBot());

                float fact = this->calculFact(cptTirsSimultanes);                                                                                   // Voir fct accessoire
                nouveauTir->getSprite()->setOrigin(nouveauTir->getSprite()->getOrigin().x + fact * this->distCanons / 2
                    * (float)this->sprite->getScale().x / nouveauTir->getSprite()->getScale().x, nouveauTir->getSprite()->getOrigin().y);           // Met le tir au milieu, à droite ou à gauche du vaisseau (dépendant de la distance entre les deux canons du Vaisseau) (sert aux Vaisseaux ayant plusieurs tirs simultanés)

                this->tirs.push_back(nouveauTir);

                if (cptTirsSimultanes == 0)
                    this->rumble(RumbleMode::Tir);
            }
            if (this->nbTirsSemiAuto)                                                                                                               // S'il a des tirs semi-auto
                this->cptTirsSemiAuto++;

            this->tirsClock.restart();
        }
        else if (this->tirsClock.getElapsedTime().asSeconds() >= 4 / this->cadenceTir)                                                              // Temps entre deux "sessions" de semi-auto
            this->cptTirsSemiAuto = 0;
    }
    else if (!tire && ((this->jaugeSurchauffe->getPercentage() != 0 && this->tirsClock.getElapsedTime().asSeconds() >= 0.3f)                        // Si on n'appuie pas sur la gachette de tir et que les canons ne sont pas totalement surchauffés et après un petit laps de tps
        || (this->jaugeSurchauffe->getPercentage() == 0 && this->tirsClock.getElapsedTime().asSeconds() >= 1.3f)))                                  // ou que les canons sont totalement surchauffés et après un laps de temps plus long,
        this->jaugeSurchauffe->setPercentage(this->jaugeSurchauffe->getPercentage() + vitesseRechargementSurchauffe * this->frameTime);                                                           // on recharge la jauge (= les canons se refroidissent)
    else if (tire && this->jaugeSurchauffe->getPercentage() == 0)                                                                                   // Tant qu'on appuie sur la gachette et que la jauge est vide,
        this->tirsClock.restart();                                                                                                                  // on restart la clock pour attendre le tps exigé une fois la gachette lâchée
}

// asteroidsHurt = false if victory ==> more damage to asteroids when victory screen
void Vaisseau::updateTirs(vector<Asteroid*>& asteroids, bool asteroidsHurt)
{
    // On move tous les tirs du Vaisseau + on les delete qd ils sont en dehors de l'écran
    for (unsigned i = 0; i < this->tirs.size(); i++)
    {
        tirs[i]->move(this->frameTime);
        if (!this->tirs[i]->estDansLecran() && this->tirs[i]->isSoundFinished())
        {
            delete this->tirs[i];
            this->tirs.erase(this->tirs.begin() + i);
        }
    }

    // Collision Tir/Asteroids
    for (Tir* tir : this->tirs)
        for (Asteroid* asteroid : asteroids)
            if (!asteroid->isTransparent() && tir->toucheAsteroid(asteroid))                                                                      // Test sur la transparence de l'Asteroid nécessaire puisqu'il l'est pendant son animation de destruction
                asteroid->estTouche(this->degats + 2.f * !asteroidsHurt * this->degats);
}

void Vaisseau::gestionImpactsAsteroids(vector<Asteroid*>& asteroids, bool asteroidsHurt)
{
    // Impacts avec Asteroids
    for (Asteroid* asteroid : asteroids)
        if (asteroid->gestionRebond(this->sprite, this->instantSpeed * 0.8f, this->instantRotationSpeed * 0.2f) && this->impactType != Impact::Invincible)
        {
            if (asteroidsHurt)
            {
                this->impact->play();
                this->estTouche(asteroid->getRadius() / 2.f);
                this->impactType = Impact::Normal;

                this->rumble(RumbleMode::Impact);
            }
        }
}

void Vaisseau::gestionImpactsAdversaire(Vaisseau* adversaire)
{
    // Impact avec Vaisseau adverse (entraîne une phase d'invincibilité)
    if (this->collides(adversaire->sprite))
    {
        if (adversaire->sprite->getColor() != sf::Color::Transparent)
        {
            if (signOf(adversaire->force.x) == -signOf(adversaire->sprite->getPosition().x - this->sprite->getPosition().x))
                this->instantSpeed.x = this->force.x + adversaire->force.x;
            if (signOf(adversaire->force.y) == -signOf(adversaire->sprite->getPosition().y - this->sprite->getPosition().y))
                this->instantSpeed.y = this->force.y + adversaire->force.y;
        }

        if (this->impactType != Impact::Invincible && adversaire->cptAnim < 45)
        {
            this->impact->play();
            this->estTouche(adversaire->pvMax / 10.f);
            this->impactType = Impact::Invincible;

            this->rumble(RumbleMode::Impact);
        }
    }

    // Impact avec Tirs
    for (Tir* tir : adversaire->tirs)
        if (this->collides(tir->getSprite()) && tir->getSprite()->getColor() != sf::Color::Transparent)
        {
            tir->getSprite()->setColor(sf::Color::Transparent);

            if (this->impactType != Impact::Invincible)
            {
                this->laserImpact->play();
                this->estTouche(adversaire->degats);
                this->impactType = Impact::Normal;

                this->rumble(RumbleMode::Impact);
            }
        }
}

void Vaisseau::estTouche(int degats)
{
    this->pv -= degats;
    this->barrePvNoire->setSize(sf::Vector2f(this->barrePvNoire->getSize() + sf::Vector2f(this->barrePv->getSize().x * degats / this->pvMax, 0.f)));
    this->sprite->setColor(sf::Color::Red);
    this->clkInvincible.restart();

    if (this->pv <= 0)
    {
        this->impactType = Impact::None;
        this->explosionSound->play();
        float scale = (this->sprite->getGlobalBounds().width > this->sprite->getGlobalBounds().height) ?
            1.35f * this->sprite->getGlobalBounds().width / 100.f :
            1.35f * this->sprite->getGlobalBounds().height / 100.f;
        this->explosionSprite->setScale(scale, scale);
    }
}

void Vaisseau::animationImpact()
{
    // Impact classique
    if (this->impactType == Impact::Normal)
    {
        if (this->sprite->getColor() == sf::Color::Red && this->clkInvincible.getElapsedTime().asMilliseconds() >= 100)
        {
            this->sprite->setColor(sf::Color::White);
            this->impactType = Impact::None;
        }
    }

    // Phase d'invincibilité
    if (this->impactType == Impact::Invincible)
    {
        if (this->sprite->getColor() == sf::Color::Red)
        {
            if (this->clkInvincible.getElapsedTime().asMilliseconds() > 200)
            {
                this->sprite->setColor(sf::Color::White);
                this->clkInvincible.restart();
            }
        }
        else if (this->sprite->getColor() == sf::Color::White)
        {
            if (this->clkInvincible.getElapsedTime().asMilliseconds() > 200)
            {
                this->sprite->setColor(sf::Color::Transparent);
                this->clkInvincible.restart();
            }
        }
        else if (this->sprite->getColor() == sf::Color::Transparent)
        {
            if (this->clkInvincible.getElapsedTime().asMilliseconds() > 200)
            {
                this->sprite->setColor(sf::Color::White);
                this->clkInvincible.restart();
                this->cptClignotement++;
                if (this->cptClignotement == 2)
                {
                    this->impactType = Impact::None;
                    this->cptClignotement = 0;
                }
            }
        }
    }
}

void Vaisseau::animationExplosion()
{
    const float vitesseAnimation = 66.6f;                                                                                                           // = fps

    this->explosionSprite->setTextureRect(sf::IntRect((int)this->cptAnim % 9 * 100, (int)this->cptAnim / 9 * 100, 100, 100));                       // A noter que le résultat de la division du 2e paramètre est forcément un int (int/int)

    if (this->cptAnim >= 14)
    {
        this->sprite->setColor(sf::Color::Transparent);
        this->jaugeSurchauffe->makeInvisible();
        this->barrePv->setFillColor(sf::Color::Transparent);
        this->barrePvNoire->setFillColor(sf::Color::Transparent);
    }

    if (this->cptAnim == 0)
        this->rumble(RumbleMode::Explosion);

    this->cptAnim += vitesseAnimation * this->frameTime;
}

void Vaisseau::updateSons(int volume)
{
    this->surchauffe->setVolume(volume);
    this->impact->setVolume(volume);
    this->laserImpact->setVolume(volume);
    this->explosionSound->setVolume(volume);

    this->surchauffe->setRelativeToListener(!this->isBot());
    this->impact->setRelativeToListener(!this->isBot());
    this->laserImpact->setRelativeToListener(!this->isBot());
    this->explosionSound->setRelativeToListener(!this->isBot());

    this->surchauffe->setPosition(!this->isBot() ? 0.f : this->sprite->getPosition().x, 0.f, !this->isBot() ? 0.f : this->sprite->getPosition().y);
    this->impact->setPosition(!this->isBot() ? 0.f : this->sprite->getPosition().x, 0.f, !this->isBot() ? 0.f : this->sprite->getPosition().y);
    this->laserImpact->setPosition(!this->isBot() ? 0.f : this->sprite->getPosition().x, 0.f, !this->isBot() ? 0.f : this->sprite->getPosition().y);
    this->explosionSound->setPosition(!this->isBot() ? 0.f : this->sprite->getPosition().x, 0.f, !this->isBot() ? 0.f : this->sprite->getPosition().y);
}

void Vaisseau::updateRumble()
{
    if (this->rumbleMode == RumbleMode::Tir && this->clkRumble.getElapsedTime().asSeconds() >= 0.5f / this->cadenceTir) {
        wl::Joystick::rumble(0);
        this->rumbleMode = RumbleMode::None;
    }

    if (this->rumbleMode == RumbleMode::Explosion && this->clkRumble.getElapsedTime().asSeconds() >= 1.f) {
        wl::Joystick::rumble(0);
        this->rumbleMode = RumbleMode::None;
    }

    if (this->rumbleMode == RumbleMode::Impact && this->clkRumble.getElapsedTime().asMilliseconds() >= 100) {
        wl::Joystick::rumble(0);
        this->rumbleMode = RumbleMode::None;
    }
}

void Vaisseau::rumble(RumbleMode mode)
{
    if (!this->isBot()) {
        wl::Joystick::rumble(100);
        this->rumbleMode = mode;
        this->clkRumble.restart();
    }
}

// Teste si le Vaisseau est en collision avec 'otherSprite'
bool Vaisseau::collides(const sf::Sprite* otherSprite)
{
    vector<sf::Vector2f> axesRectangles{                                                                                                            // On stocke dans ce vecteur les axes associés aux côtés du haut et de gauche des deux sprites (Nous allons utiliser le théorème de l'axe séparateur)
        getCorner(this->sprite, 0) - getCorner(this->sprite, 1),
        getCorner(this->sprite, 0) - getCorner(this->sprite, 2),
        getCorner(otherSprite, 0) - getCorner(otherSprite, 1),
        getCorner(otherSprite, 0) - getCorner(otherSprite, 2)
    };

    for (sf::Vector2f& axe : axesRectangles)
    {
        float r1scalaires[4];                                                                                                                       // On stocke dans ce tableau les coins de notre sprite projetés sur l'axe
        for (int i = 0; i < 4; i++)
            r1scalaires[i] = getCorner(this->sprite, i) * axe;

        float r2scalaires[4];                                                                                                                       // Idem pour le sprite du Vaisseau adverse
        for (int i = 0; i < 4; i++)
            r2scalaires[i] = getCorner(otherSprite, i) * axe;

        float r1scalairesMin = *min_element(r1scalaires, r1scalaires + 4);
        float r1scalairesMax = *max_element(r1scalaires, r1scalaires + 4);
        float r2scalairesMin = *min_element(r2scalaires, r2scalaires + 4);
        float r2scalairesMax = *max_element(r2scalaires, r2scalaires + 4);

        if (r1scalairesMax < r2scalairesMin || r1scalairesMin > r2scalairesMax)                                                                     // Dans ce cas, les segments créés par les projections des deux sprites ne se chevauchent pas, donc les Vaisseaux ne sont pas en collision
            return false;
    }

    return true;
}

// Fonction accessoire à updateTirs (détermine si le tir est au milieu, sur le canon droit ou gauche du vaisseau) (fct à revoir/améliorer pour plus de possibilités de tirs simultanés)
float Vaisseau::calculFact(int cptTirsSimultanes) const
{
    if(this->nbTirsSimul%2 && !cptTirsSimultanes)
        return 0;

    else if(cptTirsSimultanes == this->nbTirsSimul%2)
        return 1;

    else if(cptTirsSimultanes == this->nbTirsSimul%2 + 1)
        return -1;
}


// Get functions
sf::Sprite* Vaisseau::getSprite() const
{
    return this->sprite;
}


/// FONCTIONS ANNEXES ///
void Vaisseau::resteDansLecran()
{
    bool toucheLimite = false;
    float rayon = this->sprite->getScale().x * (this->sprite->getLocalBounds().width + this->sprite->getLocalBounds().height) / 4.f;

    if (this->sprite->getPosition().x < rayon)
    {
        this->sprite->setPosition(rayon, this->sprite->getPosition().y);
        this->explosionSprite->setPosition(rayon, this->sprite->getPosition().y);
        toucheLimite = true;
    }
    if (this->sprite->getPosition().x > screenSize.x - rayon)
    {
        this->sprite->setPosition(screenSize.x - rayon, this->sprite->getPosition().y);
        this->explosionSprite->setPosition(screenSize.x - rayon, this->sprite->getPosition().y);
        toucheLimite = true;
    }
    if (this->sprite->getPosition().y < rayon)
    {
        this->sprite->setPosition(this->sprite->getPosition().x, rayon);
        this->explosionSprite->setPosition(this->sprite->getPosition().x, rayon);
        toucheLimite = true;
    }
    if (this->sprite->getPosition().y > screenSize.y - rayon)
    {
        this->sprite->setPosition(this->sprite->getPosition().x, screenSize.y - rayon);
        this->explosionSprite->setPosition(this->sprite->getPosition().x, screenSize.y - rayon);
        toucheLimite = true;
    }

    if (toucheLimite)
    {
        this->setRotationJauge(this->sprite->getRotation());
        // On met l'angle du sprite temporairement à zéro pour bien placer les vies
        float oldAngle = this->sprite->getRotation();
        this->sprite->setRotation(0);
        this->barrePv->setPosition(this->sprite->getGlobalBounds().left, this->sprite->getPosition().y - this->sprite->getGlobalBounds().height / 2.f - (3.f * screenScaleX));
        this->barrePvNoire->setPosition(this->barrePv->getPosition() + sf::Vector2f(this->sprite->getGlobalBounds().width, 0.f));
        this->sprite->setRotation(oldAngle);
    }
}

void Vaisseau::setRotationJauge(float angle)
{
    sf::RectangleShape line(sf::Vector2f(0.1f, 0.7f * sqrt(pow(this->sprite->getLocalBounds().width, 2)
        + pow(this->sprite->getLocalBounds().height, 2)) / 2.f * this->sprite->getScale().x));                                                      // On set la longueur de la ligne (= Distance entre emplacement de la jauge et du centre de rotation du Vaisseau) 
    line.setRotation((180.f - 35.f) + angle);
    int termex = (line.getRotation() < 180.f) ? 0 : line.getGlobalBounds().width;															            // Ligne sur la partie gauche du cercle
    int termey = (line.getRotation() > 90.f && line.getRotation() < 270.f) ? 0 : line.getGlobalBounds().height;									        // Ligne sur la partie haute du cercle
    this->jaugeSurchauffe->setPosition(this->sprite->getPosition() + sf::Vector2f(line.getGlobalBounds().left + termex,
        line.getGlobalBounds().top + termey));                                                                                                      // On déplace la jauge au bout de la ligne
}

// Fonctions outils
// corner arg: 0 -> haut gauche, 1 -> haut droite, 2 -> bas gauche, 3 -> bas droite
sf::Vector2f getCorner(const sf::Sprite* rect, int corner)
{
    return rect->getTransform().transformPoint(sf::Vector2f((bitset<2>(corner)[0]) ? rect->getLocalBounds().width : 0, (bitset<2>(corner)[1]) ? rect->getLocalBounds().height : 0));
}

int signOf(float val)
{
    return val / abs(val);
}