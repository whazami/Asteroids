#include "../../include/GameElements/Asteroid.h"

#include <iostream>

#include <SFML/Graphics.hpp>

#include <math.h>

using namespace std;

sf::Texture* Asteroid::texture = new sf::Texture;
sf::Texture *Asteroid::explosionT = new sf::Texture, *Asteroid::feudArtificeT = new sf::Texture;
sf::SoundBuffer *Asteroid::bufferExplosion = new sf::SoundBuffer, *Asteroid::bufferArtifice = new sf::SoundBuffer;

Asteroid::Asteroid() : rayonMax(120.f * screenScaleX), vitesseMin(60.f * screenScaleX), vitesseMax(2000.f * screenScaleX), couleurArtifice(-1)
{
    // Variables membres aléatoires
    this->radius = rand() % (int)(101.f * screenScaleX) + (20.f * screenScaleX);                                                                    // Rayon compris entre 25 et 135
    this->pv = this->radius * 2;                                                                                                                    // Points de vie et masse
    this->masse = this->radius;                                                                                                                     // proportionnels au rayon

    // Init animation explosion
    this->explosionSprite = new sf::Sprite;
    this->explosionSprite->setTexture(*Asteroid::explosionT);

    this->explosionSprite->setTextureRect(sf::IntRect(-100, -100, 100, 100));                                                                       // On met le sprite en dehors de la Texture à l'initialisation (le sprite est donc transparent)
    this->explosionSprite->setScale(2.7f * this->radius / 100, 2.7f * this->radius / 100);
    this->explosionSprite->setOrigin(this->explosionSprite->getLocalBounds().width / 2.f, this->explosionSprite->getLocalBounds().height / 2.f);

    this->feudArtificeSprite = new sf::Sprite;
    this->feudArtificeSprite->setTexture(*Asteroid::feudArtificeT);

    this->feudArtificeSprite->setTextureRect(sf::IntRect(-53, -53, 53, 53));                                                                       // On met le sprite en dehors de la Texture à l'initialisation (le sprite est donc transparent)
    this->feudArtificeSprite->setScale(2.7f * this->radius / 53, 2.7f * this->radius / 53);
    this->feudArtificeSprite->setOrigin(this->feudArtificeSprite->getLocalBounds().width / 2.f, this->feudArtificeSprite->getLocalBounds().height / 2.f);

    // Vitesse de rotation inversement proportionnelle au rayon
    this->rotateSpeed = (float)(rand() % rayonMax) / (float)this->radius * (float)this->vitesseMin;

    // Pour la rotation aléa : vers la gauche ou vers la droite
    short sel = rand()%2;
    this->rotateSpeed = (sel) ? this->rotateSpeed : -this->rotateSpeed;

    // Pour l'appartition aléatoire de l'astéroid en dehors de l'ecran :
    int posX = 0, posY = 0;
    sel = rand()%4;
    switch(sel)
    {
    case 0: // A gauche
        posX = -2*this->radius;
        posY = rand() % (int)(screenSize.y + 1);
        break;
    case 1: // En haut
        posX = rand() % (int)(screenSize.x + 1);
        posY = -2*this->radius;
        break;
    case 2: // A droite
        posX = screenSize.x + 2*this->radius;
        posY = rand() % (int)(screenSize.y + 1);
        break;
    case 3: // En bas
        posX = rand() % (int)(screenSize.x + 1);
        posY = screenSize.y + 2*this->radius;
        break;
    }
    // Pour la direction aléatoire plus ou moins vers le centre de l'écran
    int dirX = rand() % (int)(screenSize.x / 2.f) + (screenSize.x / 4.f);
    int dirY = rand() % (int)(screenSize.y / 2.f) + (screenSize.y / 4.f);

    // Sprite
    const float rayonImage = this->texture->getSize().x / 2.f;                                                                                      // Rayon de l'image originale de l'Asteroid
    this->sprite = new sf::Sprite;
    this->sprite->setTexture(*Asteroid::texture);
    float scale_XY = (float)this->radius / rayonImage;                                                                                              // Pour obtenir le bon rayon
    this->sprite->setScale(scale_XY, scale_XY);
    this->sprite->setOrigin(rayonImage, rayonImage);                                                                                                // Centre du cercle
    this->sprite->setPosition((float)posX, (float)posY);                                                                                            // Pour le debug dans la fenetre après avec posX et posY pour un rand juste en dehors de la fenetre.

    // Calcul du vecteur vitesse
    this->speed = sf::Vector2f((float)dirX, (float)dirY) - sf::Vector2f((float)posX, (float)posY);                                                  // Création de la direction du vecteur
    this->speed /= (float)sqrt(pow(this->speed.x, 2) + pow(this->speed.y, 2));
    this->speed *= (float)rayonMax / (float)this->radius * (float)this->vitesseMin;                                                                 // Vitesse élevée plus le rayon est petit
}

Asteroid::~Asteroid()
{
    delete this->sprite;
    delete this->explosionSprite;
    delete this->feudArtificeSprite;

    delete this->explosionSound;
}

void Asteroid::update(int volumeExplosion, bool explosionArtifice, float frameTime)
{
    // Les vitesses dépendent de frameTime
    this->speed *= (this->oldFrameTime != 0) ? frameTime / this->oldFrameTime : frameTime;
    this->rotateSpeed *= (this->oldFrameTime != 0) ? frameTime / this->oldFrameTime : frameTime;
    this->oldFrameTime = frameTime;

    // Si la vitesse de l'Asteroid dépasse 'vitesseMax', on la set à 'vitesseMax'
    float normeSpeed = sqrt(pow(this->speed.x, 2) + pow(this->speed.y, 2));
    if (normeSpeed / frameTime > vitesseMax)
    {
        this->speed /= normeSpeed;
        this->speed *= vitesseMax * frameTime;
    }

    // Moves
    this->sprite->move(this->speed);
    this->sprite->rotate(this->rotateSpeed);
    this->explosionSprite->setPosition(this->sprite->getPosition());
    this->explosionSprite->setRotation(this->sprite->getRotation());
    this->feudArtificeSprite->setPosition(this->sprite->getPosition());
    this->feudArtificeSprite->setRotation(this->sprite->getRotation());

    // Partie animation clignotement rouge
    if (this->sprite->getColor() == sf::Color::Red && this->clkRed.getElapsedTime().asMilliseconds() >= 100)
    {
        this->sprite->setColor(sf::Color::White);
        this->clkRed.restart();
    }

    // Partie animation explosion
    if (this->pv <= 0)
    {
        if (!this->explosionSoundPlayed)
        {
            this->explosionSound = new sf::Sound;
            this->explosionSound->setBuffer(explosionArtifice ? *Asteroid::bufferArtifice : *Asteroid::bufferExplosion);
            this->explosionSound->setVolume(volumeExplosion);
            this->explosionSound->setAttenuation(0.01f);
            this->explosionSound->setMinDistance(100.f);
            this->explosionSound->setPosition(this->sprite->getPosition().x, 0.f, this->sprite->getPosition().y);
            this->explosionSound->play();
            this->explosionArtifice = explosionArtifice;
            this->explosionSoundPlayed = true;
        }

        this->animationExplosion(frameTime);
    }

    // Gestion effacement de l'Asteroid
    if(this->estDansLecran())
        dansEcran = true;
    else if (dansEcran == true)
        this->effacer = true;
}

void Asteroid::show(sf::RenderWindow* window)
{
    window->draw(*this->sprite);
    window->draw(*this->explosionSprite);
    window->draw(*this->feudArtificeSprite);
}

// Get functions
sf::Vector2f Asteroid::getCenter() const { return this->sprite->getPosition(); }
int Asteroid::getRadius() const { return this->radius; }


/// FONCTIONS DE COLLISIONS ///

bool Asteroid::touche(Asteroid* otherA)
{
    if (!this->isTransparent() && !otherA->isTransparent())
    {
        sf::Vector2f c1(this->sprite->getPosition());
        sf::Vector2f c2(otherA->sprite->getPosition());

        sf::Vector2f vectDiff(c2 - c1);

        if (sqrt(pow(vectDiff.x, 2) + pow(vectDiff.y, 2)) <= this->radius + otherA->radius)
            return true;
    }
    return false;
}

void Asteroid::gestionRebond(Asteroid* otherA)
{
    if (this->cptAnim == 0)
    {
        if (this->touche(otherA) && (this->clkRebondAsteroid.getElapsedTime().asMilliseconds() >= 50 || otherA->clkRebondAsteroid.getElapsedTime().asMilliseconds() >= 50))  // Temporisation pour éviter les bugs de collision
        {
            sf::Vector2f repere[2];                                                                                                                         // Repère orthonormé (origine : centre de 'this->sprite' et orientà vers le centre de 'otherA->sprite')
            repere[1] = otherA->sprite->getPosition() - this->sprite->getPosition();                                                                        // repere[1] est le vecteur qui correspond à l'axe des ordonnées

            repere[1] /= (float)sqrt(pow(repere[1].x, 2) + pow(repere[1].y, 2));                                                                                   // On le normalise
            repere[0] = sf::Vector2f(-repere[1].y, repere[1].x);                                                                                            // repere[0] est le vecteur qui correspond à l'axe des abscisses (donc perpendiculaire à repere[1])

            sf::Vector2f thisSpeedVuedeOtherA = this->speed - otherA->speed, otherASpeedVuedeOtherA;                                                        // Nous allons traiter les vitesses par rapport au référentiel de otherA (sa vitesse est donc nulle avant l'impact)
            sf::Vector2f thisSpeedVuedeOtherAsurRepere;                                                                                                     // Nous allons ensuite projeter ces vitesses sur le repère que nous avons créé
            sf::Vector2f thisSpeedVuedeOtherAsurRepereApresImpact, otherASpeedVuedeOtherAsurRepereApresImpact;                                              // Et enfin, nous allons calculer la valeur de ces vitesses après l'impact

            thisSpeedVuedeOtherAsurRepere.x = thisSpeedVuedeOtherA * repere[0];                                                                             // On projete donc la composante x de la vitesse de cet Asteroid vue par 'otherA' sur notre repère
            thisSpeedVuedeOtherAsurRepere.y = thisSpeedVuedeOtherA * repere[1];                                                                             // Puis nous faisons de même pour sa composante y

            thisSpeedVuedeOtherAsurRepereApresImpact.x = thisSpeedVuedeOtherAsurRepere.x;                                                                   // Nous procédons maintenant aux calculs des composantes x et y
            thisSpeedVuedeOtherAsurRepereApresImpact.y = thisSpeedVuedeOtherAsurRepere.y * (this->masse - otherA->masse) / (this->masse + otherA->masse);   // des deux vecteurs vitesse vus de 'otherA' après l'impact.
            otherASpeedVuedeOtherAsurRepereApresImpact.x = 0;                                                                                               // Ces calculs sont directement issus du principe de
            otherASpeedVuedeOtherAsurRepereApresImpact.y = thisSpeedVuedeOtherAsurRepere.y * (2 * this->masse) / (this->masse + otherA->masse);             // la conservation de la quantità de mouvement.

            thisSpeedVuedeOtherA = thisSpeedVuedeOtherAsurRepereApresImpact.x * repere[0] + thisSpeedVuedeOtherAsurRepereApresImpact.y * repere[1];         // On retrouve ensuite les deux vecteurs vitesse toujours vus de 'otherA', après l'impact
            otherASpeedVuedeOtherA = otherASpeedVuedeOtherAsurRepereApresImpact.x * repere[0] + otherASpeedVuedeOtherAsurRepereApresImpact.y * repere[1];   // mais maintenant sur le repère de base, celui de la fenêtre

            this->speed = thisSpeedVuedeOtherA + otherA->speed;                                                                                             // Enfin, on additionne 'otherA->speed' à ces deux vecteurs
            otherA->speed = otherASpeedVuedeOtherA + otherA->speed;                                                                                         // pour revenir sur le référentiel de base, celui de la fenêtre

            this->clkRebondAsteroid.restart();
            otherA->clkRebondAsteroid.restart();
        }
    }
}

bool Asteroid::gestionRebond(const sf::Sprite* rectParoi, const sf::Vector2f& speedParoi, const float& rotationSpeedParoi, bool justTest)
{
    // On considère qu'à partir de l'image 45 de l'animation d'explosion, l'Asteroid ne peut plus avoir de collisions
    if (this->cptAnim >= 45) return false;

    // 'cptMovesObligatoire' empêche les bugs de collision
    if (this->cptMovesObligatoires > 0 && !justTest)
    {
        this->cptMovesObligatoires--;
        return false;
    }

    // Les deux repères de calculs
    sf::Vector2f repereRect[2], repereParoi[2];

    // Attributs du sprite
    const float widthRectParoi = rectParoi->getLocalBounds().width * rectParoi->getScale().x;
    const float heightRectParoi = rectParoi->getLocalBounds().height * rectParoi->getScale().y;
    const sf::Vector2f posRectParoi(rectParoi->getGlobalBounds().left + rectParoi->getGlobalBounds().width / 2.f,
        rectParoi->getGlobalBounds().top + rectParoi->getGlobalBounds().height / 2.f);

    // Calcul vitesse angulaire
    short rotationSign = rotationSpeedParoi / abs(rotationSpeedParoi);
    float LinearRotationSpeedRect = (rotationSpeedParoi * (PI / 180.f)) * (widthRectParoi / 2.f);
    sf::Vector2f rotationSpeedRectRepereParoi(0, LinearRotationSpeedRect);

    /// CREATION DU REPERE CENTRÉ SUR LE RECT ///
    sf::RectangleShape ligneAbscisse(sf::Vector2f(1.f, 0.f));
    float angle = (rectParoi->getRotation() > 180) ? rectParoi->getRotation() - 180 : rectParoi->getRotation();	// angle entre 0 et 180
    ligneAbscisse.setRotation(angle);

    float fact = (angle <= 90) ? 1 : -1;
    repereRect[0] = sf::Vector2f(fact * ligneAbscisse.getGlobalBounds().width, ligneAbscisse.getGlobalBounds().height);
    repereRect[1] = sf::Vector2f(repereRect[0].y, -repereRect[0].x);

    /// TEST + GESTION COLLISION ///
    sf::Vector2f posCercleRepereRect = this->sprite->getPosition() - posRectParoi;
    posCercleRepereRect = sf::Vector2f(posCercleRepereRect * repereRect[0], posCercleRepereRect * repereRect[1]);
    sf::Vector2f vectCercleBordRect = fabs(posCercleRepereRect) - sf::Vector2f(widthRectParoi / 2.f, heightRectParoi / 2.f);

    // On détermine de quel type de collision il s'agit
    bool collisionHorizontale = false, collisionVerticale = false, collisionCoin = false;

    if (posCercleRepereRect.x >= -widthRectParoi / 2.f && posCercleRepereRect.x <= widthRectParoi / 2.f && (this->clkHorizontal.getElapsedTime().asMilliseconds() > 100 || justTest))
    {
        if (vectCercleBordRect.y <= this->radius)
            collisionHorizontale = true;
    }
    else if (posCercleRepereRect.y >= -heightRectParoi / 2.f && posCercleRepereRect.y <= heightRectParoi / 2.f && (this->clkVertical.getElapsedTime().asMilliseconds() > 100 || justTest))
    {
        if (vectCercleBordRect.x <= this->radius)
            collisionVerticale = true;
    }
    else if (this->clkCoin.getElapsedTime().asMilliseconds() > 100 || justTest)
    {
        if (sqrt(pow(vectCercleBordRect.x, 2) + pow(vectCercleBordRect.y, 2)) <= this->radius)
            collisionCoin = true;
    }

    // Et on fait les calculs en conséquence
    if (collisionHorizontale || collisionVerticale || collisionCoin)
    {
        if (justTest) return true;

        if (this->cptAnim == 0)
        {
            if (collisionHorizontale)
            {
                // Création du repère propre à la paroi de collision
                repereParoi[0] = (posCercleRepereRect.y > 0) ? repereRect[0] : -repereRect[0];
                repereParoi[1] = (posCercleRepereRect.y > 0) ? repereRect[1] : -repereRect[1];

                this->clkHorizontal.restart();
            }
            else if (collisionVerticale)
            {
                // Création du repère propre à la paroi de collision
                repereParoi[0] = (posCercleRepereRect.x > 0) ? -repereRect[1] : repereRect[1];
                repereParoi[1] = (posCercleRepereRect.x > 0) ? repereRect[0] : -repereRect[0];

                this->clkVertical.restart();
            }
            if (collisionHorizontale || collisionVerticale)
            {
                // Calcul de la vitesse de rotation de la paroi sur le repère de celle-ci
                sf::Vector2f posCercleRepereParoi = this->sprite->getPosition() - posRectParoi;
                posCercleRepereParoi = sf::Vector2f(posCercleRepereParoi * repereParoi[0], posCercleRepereParoi * repereParoi[1]);
                rotationSpeedRectRepereParoi = !((posCercleRepereParoi.x < 0) ^ (rotationSign > 0)) ? rotationSpeedRectRepereParoi : sf::Vector2f(0, 0);
                rotationSpeedRectRepereParoi.y *= abs(posCercleRepereParoi.x) / (widthRectParoi / 2.f);
            }
            else if (collisionCoin)
            {
                // Création du repère propre à la paroi de collision
                sf::Vector2f vectCercleRect = this->sprite->getPosition() - posRectParoi;
                repereParoi[1] = vectCercleRect / (float)sqrt(pow(vectCercleRect.x, 2) + pow(vectCercleRect.y, 2));
                repereParoi[0] = sf::Vector2f(-repereParoi[1].y, repereParoi[1].x);

                // Calcul de la vitesse de rotation de la paroi sur le repère de celle-ci
                rotationSpeedRectRepereParoi = sf::Vector2f(0, LinearRotationSpeedRect);

                this->clkCoin.restart();
            }

            // Calcul de la nouvelle speed
            sf::Vector2f speedRectRepereParoi(speedParoi* repereParoi[0], speedParoi* repereParoi[1]);
            speedRectRepereParoi += rotationSpeedRectRepereParoi;

            sf::Vector2f speedCircleRepereParoi(this->speed* repereParoi[0], this->speed* repereParoi[1]);
            sf::Vector2f speedCircleRepereParoiRefRect = speedCircleRepereParoi - speedRectRepereParoi;

            sf::Vector2f newSpeedRepereParoiRefRect(speedCircleRepereParoiRefRect.x, -speedCircleRepereParoiRefRect.y);

            speedCircleRepereParoi = newSpeedRepereParoiRefRect + speedRectRepereParoi;
            this->speed = speedCircleRepereParoi.x * repereParoi[0] + speedCircleRepereParoi.y * repereParoi[1];

            /// ANTI BUG COLLISIONS ///
            // On calcule combien de frames l'Asteroid va être en collision avec la paroi et on met le résultat dans 'cptMovesObligatoires'
            this->sprite->move(this->speed);
            while (this->gestionRebond(rectParoi, speedParoi, rotationSpeedParoi, true))
            {
                this->cptMovesObligatoires++;
                this->sprite->move(this->speed);
            }

            // On remet l'Asteroid à sa place
            this->sprite->move(-this->speed * (float)(this->cptMovesObligatoires + 1));

            // S'il y aura un bug de collision, l'Asteroid prend une trajectoire qui s'éloigne du Vaisseau après l'impact (et est non collisionnable pendant 'cptMovesObligatoires' frames)
            if (this->cptMovesObligatoires != 0)
            {
                this->cptMovesObligatoires += 20;                                                                           // Sécurité
                this->speed = this->sprite->getPosition() - rectParoi->getPosition();
                this->speed /= (float)sqrt(pow(this->speed.x, 2) + pow(this->speed.y, 2));
                float normeSpeed = sqrt(pow(this->speed.x, 2) + pow(this->speed.y, 2));
                float normeSpeedParoi = sqrt(pow(speedParoi.x, 2) + pow(speedParoi.y, 2));
                this->speed *= 1.5f * max(normeSpeed, normeSpeedParoi);
            }
        }

        return true;
    }

    return false;
}

void Asteroid::estTouche(int degats)
{
    if(this->sprite->getColor() == sf::Color::White)
    {
        this->sprite->setColor(sf::Color::Red);
        this->clkRed.restart();
    }

    this->pv -= degats;
}

void Asteroid::animationExplosion(float frameTime)
{
    float vitesseAnimation;                                                                                                         // = fps
    if (!this->explosionArtifice)
    {
        vitesseAnimation = 66.6f;

        this->explosionSprite->setTextureRect(sf::IntRect((int)this->cptAnim % 9 * 100, (int)this->cptAnim / 9 * 100, 100, 100));   // A noter que le résultat de la division du 2e paramètre est forcément un int (int/int)
        this->cptAnim += vitesseAnimation * frameTime;

        if (this->cptAnim >= 15)
            this->sprite->setColor(sf::Color::Transparent);
        if (this->cptAnim >= 74)
            this->effacer = true;
    }
    else
    {
        vitesseAnimation = 10.5f;

        if (this->couleurArtifice == -1)
            this->couleurArtifice = rand() % 3;
        this->feudArtificeSprite->setTextureRect(sf::IntRect(this->couleurArtifice * 53, (int)this->cptAnim * 53, 53, 53));         // A noter que le résultat de la division du 2e paramètre est forcément un int (int/int)
        this->cptAnim += vitesseAnimation * frameTime;

        if (this->cptAnim >= 2)
            this->sprite->setColor(sf::Color::Transparent);
        if (this->cptAnim >= 5)
            this->effacer = true;
    }

    // Effet de ralentissement sur la vitesse de déplacement et de rotation
    this->speed -= 0.015f * this->speed * vitesseAnimation * frameTime;
    this->rotateSpeed -= 0.03f * this->rotateSpeed * vitesseAnimation * frameTime;
}


/// FONCTION ANNEXES ///

bool Asteroid::estDansLecran()
{
    if (this->sprite->getGlobalBounds().left > -this->sprite->getGlobalBounds().width && this->sprite->getGlobalBounds().left < screenSize.x
        && this->sprite->getGlobalBounds().top > -this->sprite->getGlobalBounds().height && this->sprite->getGlobalBounds().top < screenSize.y)
        return true;

    return false;
}

bool Asteroid::isTransparent()
{
    return (this->sprite->getColor() == sf::Color::Transparent);
}

void Asteroid::loadFiles()
{
    if (!Asteroid::texture->loadFromFile(assetsPath + "/Images/Asteroid.png"))
        cout << "Erreur chargement image asteroid." << endl;

    if (!Asteroid::explosionT->loadFromFile(assetsPath + "/Images/Animations/Explosion.png"))
        cout << "Erreur chargement image animation explosion asteroid." << endl;

    if (!Asteroid::feudArtificeT->loadFromFile(assetsPath + "/Images/Animations/Feu d'Artifice.png"))
        cout << "Erreur chargement image animation feu d'artifice." << endl;

    if (!Asteroid::bufferExplosion->loadFromFile(assetsPath + "/Sons/Explosions/Explosion.wav"))
        cout << "Erreur chargement son explosion." << endl;

    if (!Asteroid::bufferArtifice->loadFromFile(assetsPath + "/Sons/Explosions/Feu D'Artifice.wav"))
        cout << "Erreur chargement son feu d'artifice." << endl;
}


/// FCTS POUR sf::Vector2f ///
// Produit scalaire entre deux sf::Vector2f
float operator*(sf::Vector2f v1, sf::Vector2f v2)
{
    return (v1.x * v2.x) + (v1.y * v2.y);
}

// Valeur absolue sf::Vector2f
sf::Vector2f fabs(const sf::Vector2f& v)
{
    return sf::Vector2f(v.x < 0 ? -v.x : v.x, v.y < 0 ? -v.y : v.y);
}