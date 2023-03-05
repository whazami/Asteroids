#include "../../include/GameElements/Bot.h"

Bot::Bot(const Vaisseau* v, int niveau, const vector<Asteroid*>& asteroids, const vector<Bot*>& otherBots) : Vaisseau(v), niveau(niveau), feu(false), generator(time(NULL))
{
    // Init
    sf::Clock clk;
    bool collidesSomeThing;
    do
    {
        collidesSomeThing = false;
        sf::Vector2f pos;
        int sel = rand() % 4;

        switch (sel)                                                                                                                                // Pour une position aléatoire en dehors de l'écran
        {
        case 0: // A gauche
            pos.x = -this->sprite->getGlobalBounds().width;
            pos.y = rand() % (int)(screenSize.y + 1);
            break;

        case 1: // En haut
            pos.x = rand() % (int)(screenSize.x + 1);
            pos.y = -this->sprite->getGlobalBounds().height;
            break;

        case 2: // A droite
            pos.x = screenSize.x + this->sprite->getGlobalBounds().width;
            pos.y = rand() % (int)(screenSize.y + 1);
            break;

        case 3: // En bas
            pos.x = rand() % (int)(screenSize.x + 1);
            pos.y = screenSize.y + this->sprite->getGlobalBounds().height;
            break;
        }

        this->sprite->setPosition(pos);

        for (Asteroid* asteroid : asteroids)
            if (asteroid->gestionRebond(this->sprite, sf::Vector2f(0.f, 0.f), 0.f, true))
                collidesSomeThing = true;

        for (Bot* bot : otherBots)
            if (this->collides(bot->sprite))
                collidesSomeThing = true;
    } 
    while (collidesSomeThing && clk.getElapsedTime().asMilliseconds() < 1.f);

    this->explosionSprite->setPosition(sf::Vector2f(this->sprite->getGlobalBounds().left, this->sprite->getGlobalBounds().top) + sf::Vector2f(this->sprite->getGlobalBounds().width / 2.f, this->sprite->getGlobalBounds().height / 2.f));

    this->barrePv->setPosition(this->sprite->getGlobalBounds().left, this->sprite->getPosition().y - this->sprite->getGlobalBounds().height / 2.f - (3.f * screenScaleX));
    this->barrePvNoire->setPosition(this->barrePv->getPosition() + sf::Vector2f(this->sprite->getGlobalBounds().width, 0.f));
    this->setRotationJauge(0.f);                                                                                                                      // On positionne la jauge de surchauffe au bon endroit

    this->vitesse *= 0.5f;

    if (this->niveau == 1)
        this->cadenceTir /= 3.f;
}

void Bot::update(vector<Asteroid*>& asteroids, Vaisseau* joueur, vector<Bot*>& otherBots, int volumeSon, float frameTime)
{
    this->frameTime = frameTime;

    if (this->estVivant())
    {
        this->calcMoveForce(asteroids, joueur);
        this->pivote(joueur, asteroids);
        this->tire(this->feu, volumeSon);

        this->gestionImpactsAsteroids(asteroids, true);
        this->gestionImpactsAdversaire(joueur);
        if (this->niveau < 3)
            for (Bot* bot : otherBots)
                this->gestionImpactsAdversaire(bot);
        this->animationImpact();

        this->move();
    }
    else
        this->animationExplosion();

    this->updateTirs(asteroids, true);
    this->updateSons(volumeSon);
}

void Bot::calcMoveForce(vector<Asteroid*>& asteroids, Vaisseau* joueur)
{
    if (this->cptFrame % 10 == 0)
    {
        sf::Vector2f eviteAsteroid, versMilieu, eviteTirs;

        // Calcul du vecteur 'eviteAsteroid' qui corrrespond au vecteur opposé à l'Asteroid le plus proche
        float distMin = numeric_limits<float>::infinity();                                                                                      // On met l'infini comme ça il existe forcément une distance plus petite
        vector<Asteroid*> asteroidsProches;
        for (Asteroid* asteroid : asteroids)
        {
            sf::Vector2f vectDiff = asteroid->getCenter() - this->sprite->getPosition();
            float dist = sqrt(pow(vectDiff.x, 2) + pow(vectDiff.y, 2));

            if (dist < distMin)
            {
                distMin = dist;
                eviteAsteroid = -vectDiff / dist;
            }

            if (dist < screenSize.x / 4.f)
                asteroidsProches.push_back(asteroid);
        }

        // S'il ya beaucoup d'Asteroids autour du Bot, il faut trouver la meilleure direction pour se sortir de cette situation
        if (asteroidsProches.size() >= 2)
        {
            vector<float> anglesAsteroids;
            for (Asteroid* asteroid : asteroidsProches)
            {
                sf::Vector2f vectDiff = asteroid->getCenter() - this->sprite->getPosition();
                anglesAsteroids.push_back(atan2f(vectDiff.y, vectDiff.x));                                                                      // Donc angles entre -90 et 270
            }

            sort(anglesAsteroids.begin(), anglesAsteroids.end());

            float maxAngle = 0, issueAngle;
            for (int i = 0; i < anglesAsteroids.size() - 1; i++)
                if (anglesAsteroids[i + 1] - anglesAsteroids[i] > maxAngle)
                {
                    maxAngle = anglesAsteroids[i + 1] - anglesAsteroids[i];
                    issueAngle = (anglesAsteroids[i] + anglesAsteroids[i + 1]) / 2.f;
                    eviteAsteroid = sf::Vector2f(cos(issueAngle), sin(issueAngle));
                }

            if ((anglesAsteroids[0] + 2.f * PI) - anglesAsteroids.back() > maxAngle)
            {
                issueAngle = ((anglesAsteroids[0] + 2.f * PI) + anglesAsteroids.back()) / 2.f;
                eviteAsteroid = sf::Vector2f(cos(issueAngle), sin(issueAngle));
            }
        }

        // Calcul du vecteur 'versMilieu' qui est dirigé vers le milieu de l'écran
        versMilieu = screenSize / 2.f - this->sprite->getPosition();
        versMilieu /= (float)sqrt(pow(versMilieu.x, 2) + pow(versMilieu.y, 2));

        // Calcul du vecteur 'eviteTirs'
        if (joueur->estVivant())
        {
            sf::Vector2f vectDiff = joueur->getSprite()->getPosition() - this->sprite->getPosition();
            float angleVectDiff = atan2f(vectDiff.y, vectDiff.x);
            float angleDiff = ((joueur->getSprite()->getRotation() - 90.f) * PI / 180.f) - angleVectDiff;
            if (angleDiff > PI) angleDiff -= 2.f * PI;
            if (angleDiff < -PI) angleDiff += 2.f * PI;
            eviteTirs = sf::Vector2f(signOf(angleDiff) * sin(angleVectDiff), -signOf(angleDiff) * cos(angleVectDiff));
        }

        // Construction de 'vectMove'
        if (this->aFiniSonEntree())
        {
            if (this->niveau == 1)  // Un bot niveau facile n'essaye pas d'éviter pas les tirs du joueur
                this->force = (2.5f * eviteAsteroid + 1.5f * versMilieu) / 4.f;
            else                    // Sinon le vecteur de mouvement est la moyenne pondérée des 3 vecteurs précédents
                this->force = (2.5f * eviteAsteroid + 1.5f * versMilieu + eviteTirs) / 5.f;

            // On ajoute une rotation aléatoire pour plus de réalisme
            this->randLoiNormale = std::normal_distribution<float>(0.f, 5.f);                                                                   // On set le bon 'stddev'
            float rotateAngle = this->randLoiNormale(this->generator) * PI / 180.f;                                                             // L'angle de rotation est déterminé suivant la loi normale (il a peu de chance de beaucoup changer)
            this->force = sf::Vector2f(cos(rotateAngle) * this->force.x - sin(rotateAngle) * this->force.y,
                                        sin(rotateAngle) * this->force.x + cos(rotateAngle) * this->force.y);                                   // On rotate le vecteur de rotateAngle
        }
        else
            this->force = 2.f * ((1.5f * versMilieu + eviteAsteroid) / 2.5f);

        this->force *= this->vitesse * this->frameTime;
    }
    else
        this->force *= this->frameTime / this->oldFrameTime;

    this->oldFrameTime = this->frameTime;

    this->instantSpeed = this->force;                                                                                                           // Si la fonction 'gestionImpacts' ne modifie pas la vitesse, elle restera égale à la force

    this->cptFrame++;
}

void Bot::move()
{
    this->sprite->move(this->instantSpeed);                                                                                                     // Bouge le sprite du vaisseau
    this->jaugeSurchauffe->move(this->instantSpeed);
    this->explosionSprite->move(this->instantSpeed);
    this->barrePv->move(this->instantSpeed);
    this->barrePvNoire->move(this->instantSpeed);

    if (this->aFiniSonEntree())
        this->resteDansLecran();

    this->instantSpeed = sf::Vector2f(0.f, 0.f);
}

void Bot::pivote(Vaisseau* joueur, vector<Asteroid*>& asteroids)
{
    sf::Vector2f vectDiff = joueur->getSprite()->getPosition() - this->sprite->getPosition();

    // Bot orienté vers le joueur
    if (this->niveau == 3)                                                                                                                      // Si le bot est de niveau 3, il est capable d'anticiper le déplacement du joueur
    {
        float nbFramesImpact = sqrt(pow(vectDiff.x, 2) + pow(vectDiff.y, 2)) / (vitesseTirs * this->frameTime);
        sf::Vector2f instantSpeedJoueur = joueur->getSprite()->getPosition() - this->oldPosJoueur;
        sf::Vector2f futureJoueurPos = joueur->getSprite()->getPosition() + nbFramesImpact * instantSpeedJoueur * (float)(1.f - (sqrt(pow(vectDiff.x, 2) + pow(vectDiff.y, 2)) / (float)sqrt(pow(screenSize.x, 2) + pow(screenSize.y, 2))));
        vectDiff = futureJoueurPos - this->sprite->getPosition();
    }

    float angle = 90.f - atan2f(-vectDiff.y, vectDiff.x) * 180.f / PI;

    if (this->niveau == 1)                                                                                                                      // Un niveau 1 ne vise pas très bien
    {
        this->randLoiNormale = std::normal_distribution<float> (0.f, 0.5f);
        this->decalageAngle += this->randLoiNormale(this->generator);
        angle += this->decalageAngle;
    }

    this->instantRotationSpeed = angle - oldAngle;
    this->sprite->setRotation(angle);
    this->setRotationJauge(angle);                                                                                                              // On effectue aussi la rotation de la jauge de surchauffe

    oldAngle = angle;
    this->oldPosJoueur = joueur->getSprite()->getPosition();

    // Calcule si un Asteroid est entre le Bot et le joueur
    bool asteroidDansLeViseur = false;
    if (this->niveau > 1)
        for (Asteroid* asteroid : asteroids)
        {
            vectDiff = joueur->getSprite()->getPosition() - this->sprite->getPosition();
            sf::RectangleShape traineeLaser(sf::Vector2f(this->distCanons * this->sprite->getScale().x,
                sqrt(pow(vectDiff.x, 2) + pow(vectDiff.y, 2))));
            traineeLaser.setOrigin(traineeLaser.getLocalBounds().width / 2.f, traineeLaser.getLocalBounds().height);
            traineeLaser.setPosition(this->sprite->getPosition());
            traineeLaser.setRotation(this->sprite->getRotation());

            sf::CircleShape asteroidCircle(asteroid->getRadius());
            asteroidCircle.setPosition(asteroid->getCenter());

            if (isColliding(traineeLaser, asteroidCircle))
            {
                asteroidDansLeViseur = true;
                break;
            }
        }

    this->feu = (this->jaugeSurchauffe->getPercentage() > this->tauxSurchauffe && !asteroidDansLeViseur && joueur->estVivant() && this->aFiniSonEntree());
}

// Fonction annexe
bool Bot::aFiniSonEntree()
{
    return (this->sprite->getPosition().x > this->sprite->getGlobalBounds().width / 2.f && this->sprite->getPosition().x < screenSize.x - this->sprite->getGlobalBounds().width / 2.f
            && this->sprite->getPosition().y > this->sprite->getGlobalBounds().height / 2.f && this->sprite->getPosition().y < screenSize.y - this->sprite->getGlobalBounds().height / 2.f);
}

// Fonction outil
bool isColliding(const sf::RectangleShape& rect, const sf::CircleShape& circle)
{
    // Les deux repères de calculs
    sf::Vector2f repereRect[2], repereParoi[2];

    // Attributs du rect
    const float widthRect = rect.getLocalBounds().width * rect.getScale().x;
    const float heightRect = rect.getLocalBounds().height * rect.getScale().y;
    const sf::Vector2f posRect(rect.getGlobalBounds().left + rect.getGlobalBounds().width / 2.f,
        rect.getGlobalBounds().top + rect.getGlobalBounds().height / 2.f);

    /// CREATION DU REPERE CENTRﾉ SUR LE RECT ///
    sf::RectangleShape ligneAbscisse(sf::Vector2f(1.f, 0.f));
    float angle = (rect.getRotation() > 180.f) ? rect.getRotation() - 180.f : rect.getRotation();	// angle entre 0 et 180
    ligneAbscisse.setRotation(angle);

    float fact = (angle <= 90.f) ? 1 : -1;
    repereRect[0] = sf::Vector2f(fact * ligneAbscisse.getGlobalBounds().width, ligneAbscisse.getGlobalBounds().height);
    repereRect[1] = sf::Vector2f(repereRect[0].y, -repereRect[0].x);

    /// TEST ///
    sf::Vector2f posCercleRepereRect = circle.getPosition() - posRect;
    posCercleRepereRect = sf::Vector2f(posCercleRepereRect * repereRect[0], posCercleRepereRect * repereRect[1]);
    sf::Vector2f vectCercleBordRect = fabs(posCercleRepereRect) - sf::Vector2f(widthRect / 2.f, heightRect / 2.f);

    if (posCercleRepereRect.x >= -widthRect / 2.f && posCercleRepereRect.x <= widthRect / 2.f)
    {
        if (vectCercleBordRect.y <= circle.getRadius())
            return true;
    }
    else if (posCercleRepereRect.y >= -heightRect / 2.f && posCercleRepereRect.y <= heightRect / 2.f)
    {
        if (vectCercleBordRect.x <= circle.getRadius())
            return true;
    }
    else
    {
        if (sqrt(pow(vectCercleBordRect.x, 2) + pow(vectCercleBordRect.y, 2)) <= circle.getRadius())
            return true;
    }

    return false;
}

// Defining this function here cause it needs to
// know Bot class
bool Vaisseau::isBot()
{
    if (dynamic_cast<Bot*>(this) == nullptr)
        return false;
    return true;
}