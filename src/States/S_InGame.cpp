#include "../../include/States/S_InGame.h"

#include <iostream>

#include <SFML/Graphics.hpp>

using namespace std;

S_InGame::S_InGame()
{
    Asteroid::loadFiles();

    // Chargement + Init textes
    if (!this->gameOverTexture.loadFromFile("Assets/Images/Textes/Game Over.png"))
        cout << "Erreur du chargement de l'images texte 'Game Over'." << endl;

    this->gameOverText.setTexture(this->gameOverTexture);
    this->gameOverText.setOrigin(this->gameOverText.getLocalBounds().width / 2.f, this->gameOverText.getLocalBounds().height / 2.f);
    this->gameOverText.setPosition(screenSize / 2.f - sf::Vector2f(0, 100.f * screenScaleY));
    this->gameOverText.setScale(0.4f * screenScaleX, 0.4f * screenScaleX);
    this->gameOverText.setColor(sf::Color::White - sf::Color(0, 0, 0, 255));

    if (!this->victoryTexture.loadFromFile("Assets/Images/Textes/Victory.png"))
        cout << "Erreur du chargement de l'images texte 'Victory'." << endl;

    this->victoryText.setTexture(this->victoryTexture);
    this->victoryText.setOrigin(this->victoryText.getLocalBounds().width / 2.f, this->victoryText.getLocalBounds().height / 2.f);
    this->victoryText.setPosition(screenSize / 2.f - sf::Vector2f(0, 100.f * screenScaleY));
    this->victoryText.setScale(0.4f * screenScaleX, 0.4f * screenScaleX);
    this->victoryText.setColor(sf::Color::White - sf::Color(0, 0, 0, 255));

    if (!this->retroFont.loadFromFile("Assets/Fonts/Retro Gaming.ttf"))
        cout << "Erreur du chargement de la font retro dans 'InGame'." << endl;

    this->retry.setFont(retroFont);
    this->yes.setFont(retroFont);
    this->no.setFont(retroFont);
    this->level.setFont(retroFont);
    this->objectif.setFont(retroFont);
    this->timeT.setFont(retroFont);
    this->objectifProgress.setFont(retroFont);

    this->retry.setString("Try again?");
    this->retry.setCharacterSize(45 * screenScaleX);
    this->retry.setOrigin(this->retry.getLocalBounds().width / 2.f, this->retry.getLocalBounds().height / 2.f);
    this->retry.setPosition(screenSize / 2.f);
    this->retry.setFillColor(sf::Color::Red - sf::Color(0, 0, 0, 255));
    this->retry.setStyle(sf::Text::Bold);

    this->yes.setString("Yes");
    this->yes.setCharacterSize(30 * screenScaleX);
    this->yes.setOrigin(this->yes.getLocalBounds().width / 2.f, this->yes.getLocalBounds().height / 2.f);
    this->yes.setPosition(screenSize / 2.f + sf::Vector2f(-50.f * screenScaleX, 50.f * screenScaleY));
    this->yes.setFillColor(sf::Color::Red - sf::Color(0, 0, 0, 255));

    this->no.setString("No");
    this->no.setCharacterSize(30 * screenScaleX);
    this->no.setOrigin(this->no.getLocalBounds().width / 2.f, this->no.getLocalBounds().height / 2.f);
    this->no.setPosition(screenSize / 2.f + sf::Vector2f(50.f * screenScaleX, 50.f * screenScaleY));
    this->no.setFillColor(sf::Color::Red - sf::Color(0, 0, 0, 255));

    this->level.setCharacterSize(45 * screenScaleX);
    this->level.setPosition(screenSize / 2.f - sf::Vector2f(0.f, 200.f * screenScaleY));
    this->level.setFillColor(sf::Color::White);

    this->objectif.setCharacterSize(35 * screenScaleX);
    this->objectif.setPosition(screenSize / 2.f - sf::Vector2f(0.f, 100.f * screenScaleY));
    this->objectif.setFillColor(sf::Color::White);

    this->timeT.setString("00:00");
    this->timeT.setCharacterSize(30 * screenScaleX);
    this->timeT.setOrigin(this->timeT.getLocalBounds().width / 2.f, this->timeT.getLocalBounds().height / 2.f);
    this->timeT.setPosition(100.f * screenScaleX, 50.f * screenScaleY);
    this->timeT.setFillColor(sf::Color::Red);

    this->objectifProgress.setString("Vague : 0/0");
    this->objectifProgress.setCharacterSize(30 * screenScaleX);
    this->objectifProgress.setOrigin(this->objectifProgress.getLocalBounds().width / 2.f, this->objectifProgress.getLocalBounds().height / 2.f);
    this->objectifProgress.setPosition(325.f * screenScaleX, 50.f * screenScaleY);
    this->objectifProgress.setFillColor(sf::Color::Red);

    this->blackRect.setSize(screenSize);
    this->blackRect.setOrigin(this->blackRect.getGlobalBounds().width / 2.f, this->blackRect.getGlobalBounds().height / 2.f);
    this->blackRect.setPosition(screenSize / 2.f);
    this->blackRect.setFillColor(sf::Color::Black - sf::Color(0, 0, 0, 255));
}

/// MAIN FUNCTIONS ///
void S_InGame::init()
{
    this->setMusic("Assets/Musiques/States/In Game.ogg");

    // On remplit la structure 'currLvl'
    this->currLvl.numLvl = State::niveauChoisi + 1;
    fillLevel(&this->currLvl);

    if (!State::sameState)
    {
        this->setObjectifString();
        this->beforeGame = true;
        this->clkObj.restart();
    }
    else
        State::startTimeGame = time(NULL);

    this->joueur = new Vaisseau(Vaisseau::TouslesVaisseaux[this->vaisseauChoisi]);
    this->waveNum = 0;
    this->asteroidsDestroyed = 0;
    this->objectifAtteint = false;
    this->musicStarted = false;
}

void S_InGame::update(float frameTime)
{
    while (this->window->pollEvent(this->e) || wl::Joystick::pollEvent(this->e))
    {
        if (this->e.type == sf::Event::Closed)
            this->window->close();

        if (this->gameOverState != GameOver::Null)
            if (this->e.type == sf::Event::JoystickMoved && (this->e.joystickMove.axis == sf::Joystick::X || this->e.joystickMove.axis == sf::Joystick::PovX) && fabs(this->e.joystickMove.position) == 100
                || this->e.type == sf::Event::KeyPressed && (this->e.key.code == sf::Keyboard::Left || this->e.key.code == sf::Keyboard::Right)
                || this->yes.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition()) && this->gameOverState == GameOver::DontRetry
                || this->no.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition()) && this->gameOverState == GameOver::Retry)
                if (this->gameOverState == GameOver::Retry)
                {
                    this->no.setFillColor(sf::Color::White);
                    this->yes.setFillColor(sf::Color::Red);
                    this->gameOverState = GameOver::DontRetry;
                    this->clkBlinkYesNo.restart();
                }
                else
                {
                    this->yes.setFillColor(sf::Color::White);
                    this->no.setFillColor(sf::Color::Red);
                    this->gameOverState = GameOver::Retry;
                    this->clkBlinkYesNo.restart();
                }

        if (!this->beforeGame)
        {
            if (this->gameOverState == GameOver::Null && !this->objectifAtteint)
                if (this->e.type == sf::Event::JoystickButtonPressed && this->e.joystickButton.button == DualShock::Options
                    || (this->e.type == sf::Event::KeyPressed && this->e.key.code == sf::Keyboard::Escape))
                {
                    State::nextState = NextState::MenuPause;
                    State::startTimePause = time(NULL);
                }
        }

        if (this->e.type == sf::Event::JoystickButtonPressed && this->e.joystickButton.button == DualShock::Croix
            || (this->e.type == sf::Event::KeyPressed && this->e.key.code == sf::Keyboard::Enter)
            || (this->e.type == sf::Event::MouseButtonPressed && this->e.mouseButton.button == sf::Mouse::Left
                && (this->yes.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition())
                    || this->no.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition()))))
            if (this->gameOverState != GameOver::Null)      // Game Over
            {
                if (this->gameOverState == GameOver::Retry)
                {
                    State::sameState = true;
                    State::nextState = NextState::InGame;
                }
                else
                {
                    State::sameState = false;
                    State::nextState = NextState::ChoixNiveau;
                }
            }

        if (this->e.type == sf::Event::JoystickButtonPressed && this->e.joystickButton.button == DualShock::Options
            || (this->e.type == sf::Event::KeyPressed && (this->e.key.code == sf::Keyboard::Enter || this->e.key.code == sf::Keyboard::Escape)))
            if (this->victoryText.getColor().a == 255)      // Victory text displayed
                State::nextState = NextState::ChoixNiveau;
    }

    if (!this->beforeGame)
    {
        this->updateVaisseaux(frameTime);
        this->updateAsteroids(frameTime);
        this->updateObjectif();
    }
    this->updateTextsInfo();

    // Transition before - during game
    const float vitesseDeDisparition = 300.f;
    if (this->clkObj.getElapsedTime().asSeconds() >= 1.f)
        if (this->level.getFillColor().a != 0)
        {
            this->level.setFillColor(this->level.getFillColor() - sf::Color(0, 0, 0, vitesseDeDisparition * frameTime));
            this->objectif.setFillColor(this->level.getFillColor() - sf::Color(0, 0, 0, vitesseDeDisparition * frameTime));
        }
        else if (this->beforeGame)
        {
            this->beforeGame = false;
            State::startTimeGame = time(NULL);
        }

    // End of Game
    switch (this->objectifAtteint)
    {
    case true:      // Victory
        this->victory(frameTime);

        if (!this->musicStarted)
        {
            this->setMusic("Assets/Musiques/End of Game/Victory.ogg");
            int indNewShip = getIndexNewShip(this->currLvl.numLvl);
            if (indNewShip == getNbrOfUnlockedElem("Ships"))
            {
                string message = "Vous pouvez désormais jouer avec le " + Vaisseau::TouslesVaisseaux[indNewShip]->getName() + " !";
                State::affichePopup(Vaisseau::TouslesVaisseaux[indNewShip]->getSprite(), "Nouveau vaisseau débloqué !", message);
                incrementUnlockedElem("Ships");
            }
            if (this->currLvl.numLvl == getNbrOfUnlockedElem("Levels"))
                incrementUnlockedElem("Levels");
            
            this->victoryTime.restart();
            this->musicStarted = true;
        }
        break;

    case false:     // Game Over?
        if (!this->joueur->estVivant())
        {
            this->gameOver(frameTime);

            if (!this->musicStarted)
            {
                this->setMusic("Assets/Musiques/End of Game/Game Over.ogg");
                this->musicStarted = true;
            }
        }
        else if (((this->currLvl.mode == LvlMode::AsteroidsTime || this->currLvl.mode == LvlMode::ShipsTime) && time(NULL) - State::startTimeGame >= this->currLvl.time.asSeconds()) && !this->beforeGame)
            this->joueur->meurs();
        break;
    }
}

void S_InGame::show()
{
    if (this->beforeGame)
    {
        this->window->draw(this->level);
        this->window->draw(this->objectif);
    }
    else
    {
        this->window->draw(this->background);
        this->joueur->show(this->window);
        for (Bot* bot : this->bots)
            bot->show(this->window);
        for (Asteroid* asteroid : this->asteroids)
            asteroid->show(this->window);

        // Textes de Game Over
        this->window->draw(this->gameOverText);
        this->window->draw(this->retry);
        this->window->draw(this->yes);
        this->window->draw(this->no);

        // Texte Victory
        this->window->draw(this->victoryText);

        // Textes Info
        this->window->draw(this->timeT);
        this->window->draw(this->objectifProgress);

        this->window->draw(this->blackRect);
    }
}

void S_InGame::release()
{
    delete this->joueur;
    for (Bot* bot : this->bots)
        delete bot;
    this->bots.clear();
    for (Asteroid* asteroid : this->asteroids)
        delete asteroid;
    this->asteroids.clear();

    // Les textes de Game Over redeviennent transparents
    this->gameOverText.setColor(sf::Color::White - sf::Color(0, 0, 0, 255));
    this->retry.setFillColor(sf::Color::Red - sf::Color(0, 0, 0, 255));
    this->yes.setFillColor(sf::Color::Red - sf::Color(0, 0, 0, 255));
    this->no.setFillColor(sf::Color::Red - sf::Color(0, 0, 0, 255));

    // Les éléments de victoire redeviennent transparents
    this->victoryText.setColor(sf::Color::White - sf::Color(0, 0, 0, 255));
    this->blackRect.setFillColor(sf::Color::Black - sf::Color(0, 0, 0, 255));

    // Les textes du before game redeviennent blancs
    this->level.setFillColor(sf::Color::White);
    this->objectif.setFillColor(sf::Color::White);

    // Les textes informatifs redeviennent rouges
    this->timeT.setFillColor(sf::Color::Red);
    this->objectifProgress.setFillColor(sf::Color::Red);

    this->gameOverState = GameOver::Null;
}


// Fonctions annexes
void S_InGame::updateVaisseaux(float frameTime)
{
    vector<Vaisseau*> bots;
    for (Bot* bot : this->bots) bots.push_back(bot);                    // Conversion de vector<Bot*> en vector<Vaisseau*>
    sf::Listener::setPosition(this->joueur->getSprite()->getPosition().x, 0.f, this->joueur->getSprite()->getPosition().y);
    sf::Listener::setDirection(cos((90.f - this->joueur->getSprite()->getRotation()) * PI / 180.f), 0.f, -sin((90.f - this->joueur->getSprite()->getRotation()) * PI / 180.f));
    int asteroidsDestroyedBefore = 0, asteroidsDestroyedAfter = 0;
    for (Asteroid* asteroid : this->asteroids)
        asteroidsDestroyedBefore += asteroid->isDestroyed();
    this->joueur->update(this->asteroids, bots, this->volumeSon, !this->objectifAtteint, frameTime);
    for (Asteroid* asteroid : this->asteroids)
        asteroidsDestroyedAfter += asteroid->isDestroyed();
    this->asteroidsDestroyed += (asteroidsDestroyedAfter - asteroidsDestroyedBefore);

    for (unsigned i = 0; i < this->bots.size(); i++)
    {
        vector<Bot*> otherBots = this->bots;
        otherBots.erase(otherBots.begin() + i);
        this->bots[i]->update(this->asteroids, this->joueur, otherBots, this->volumeSon, frameTime);
                           
        if (this->bots[i]->mustBeDeleted())
        {
            delete this->bots[i];
            this->bots.erase(this->bots.begin() + i);
        }
    }

    if (!this->currLvl.waves.empty())
    {
        if (this->bots.empty() && this->waveNum < this->currLvl.waves.size())
        {
            for (int i = 0; i < this->currLvl.waves[this->waveNum].size(); i++)
                this->bots.push_back(new Bot(Vaisseau::TouslesVaisseaux[this->currLvl.waves[this->waveNum][i] + 10], min(this->currLvl.waves[this->waveNum][i], 3), this->asteroids, this->bots));
            this->waveNum++;
        }
    }
}

bool S_InGame::createNewAsteroid()
{
    Asteroid* newAsteroid = new Asteroid();

    for (Asteroid* asteroid : this->asteroids)
        if (newAsteroid->touche(asteroid))                              // Sécurité contre les bugs de collision
        {
            delete newAsteroid;
            return false;
        }

    this->asteroids.push_back(newAsteroid);
    return true;
}

void S_InGame::updateAsteroids(float frameTime)
{
    // Création
    static sf::Clock timerA;
    if (timerA.getElapsedTime().asSeconds() > 1.f / this->currLvl.asteroidFrequency)
        if (this->createNewAsteroid())
            timerA.restart();

    // Gestion du vector<Asteroid*> VA
    for (unsigned i = 0; i < this->asteroids.size(); ++i)
    {
        this->asteroids[i]->update(this->volumeSon, this->objectifAtteint, frameTime);

        // Gestion Collisions entre Asteroids (if ils se touchent, ils rebondissent)
        for (unsigned j = 0; j < this->asteroids.size(); ++j)
            if (i != j)
                asteroids[i]->gestionRebond(asteroids[j]);

        // Delete si nécessaire
        if (this->asteroids[i]->effacer)
        {
            delete this->asteroids[i];
            this->asteroids.erase(this->asteroids.begin() + i);
        }
    }
}

void S_InGame::updateObjectif()
{
    if (this->joueur->estVivant())
        switch (this->currLvl.mode)
        {
        case LvlMode::Asteroids:
            if (time(NULL) - State::startTimeGame >= this->currLvl.time.asSeconds())
                this->objectifAtteint = true;
            break;

        case LvlMode::Ships:
            if (this->waveNum == this->currLvl.waves.size() && this->bots.empty())
                this->objectifAtteint = true;
            break;

        case LvlMode::AsteroidsTime:
            if (this->asteroidsDestroyed >= this->currLvl.nbrOfAsteroidsToDestroy)
                this->objectifAtteint = true;
            break;

        case LvlMode::ShipsTime:
            if (this->waveNum == this->currLvl.waves.size() && this->bots.empty())
                this->objectifAtteint = true;
            break;
        }
}

void S_InGame::updateTextsInfo()
{
    int minutesVal = max((int)(this->currLvl.time.asSeconds() - (time(NULL) - State::startTimeGame)) / 60, 0);
    int secondsVal = max((int)(this->currLvl.time.asSeconds() - (time(NULL) - State::startTimeGame)) % 60, 0);
    string minutes = ((minutesVal < 10) ? "0" : "") + to_string(minutesVal);
    string seconds = ((secondsVal < 10) ? "0" : "") + to_string(secondsVal);
    string timeStr = minutes + ":" + seconds;
    this->timeT.setString(timeStr);

    string waveNumbStr = to_string(this->waveNum);
    string waveSizeStr = to_string(this->currLvl.waves.size());
    string objectifStr = "Vague : " + waveNumbStr + "/" + waveSizeStr;
    this->objectifProgress.setString(objectifStr);

    string AstDestrStr;

    switch (this->currLvl.mode)
    {
    case LvlMode::Asteroids:
        this->objectifProgress.setFillColor(sf::Color::Transparent);
        break;

    case LvlMode::AsteroidsTime:
        AstDestrStr = to_string(this->asteroidsDestroyed);
        objectifStr = "Astéroïdes détruits : " + AstDestrStr + "/" + to_string(this->currLvl.nbrOfAsteroidsToDestroy);
        this->objectifProgress.setString(objectifStr);
        break;

    case LvlMode::Ships:
        this->timeT.setFillColor(sf::Color::Transparent);
        break;

    case LvlMode::ShipsTime:
        break;
    }
}

void S_InGame::setObjectifString()
{
    string minutes = to_string((int)this->currLvl.time.asSeconds() / 60);
    string seconds = to_string((int)this->currLvl.time.asSeconds() % 60);
    string timeStr = (minutes != "0" ? minutes + " minute" + (minutes != "1" ? "s" : "") + (seconds != "0" ? " et " : "") : "")
        + (seconds != "0" ? seconds + " seconde" + (seconds != "1" ? "s" : "") : "");

    string objectifStr;
    switch (this->currLvl.mode)
    {
    case LvlMode::Asteroids:
        objectifStr = "Survivez à ce champ d'Astéroïdes pendant " + timeStr + " !";
        break;

    case LvlMode::AsteroidsTime:
        objectifStr = "Détruisez " + to_string(this->currLvl.nbrOfAsteroidsToDestroy) + " astéroïdes en moins de " + timeStr + " !";
        break;

    case LvlMode::Ships:
        objectifStr = "Abattez tous les vaisseaux ennemis !";
        break;

    case LvlMode::ShipsTime:
        objectifStr = "Abattez tous les vaisseaux ennemis en moins de " + timeStr + " !";
        break;
    }
    this->objectif.setString(objectifStr);
    this->objectif.setOrigin(this->objectif.getLocalBounds().width / 2.f, this->objectif.getLocalBounds().height / 2.f);

    this->level.setString("Niveau " + to_string(this->currLvl.numLvl));
    this->level.setOrigin(this->level.getLocalBounds().width / 2.f, this->level.getLocalBounds().height / 2.f);
}

void S_InGame::gameOver(float frameTime)
{
    const float vitesseDapparition = 361.f;

    if (this->gameOverText.getColor().a != 255)
        this->gameOverText.setColor(this->gameOverText.getColor() + sf::Color(0, 0, 0, vitesseDapparition * frameTime));
    else if (this->retry.getFillColor().a != 255)
    {
        this->retry.setFillColor(this->retry.getFillColor() + sf::Color(0, 0, 0, vitesseDapparition * frameTime));
        this->yes.setFillColor(this->yes.getFillColor() + sf::Color(0, 0, 0, vitesseDapparition * frameTime));
        this->no.setFillColor(this->no.getFillColor() + sf::Color(0, 0, 0, vitesseDapparition * frameTime));

        if (this->retry.getFillColor().a == 255) this->gameOverState = GameOver::Retry;
    }
    else if(this->clkBlinkYesNo.getElapsedTime().asMilliseconds() > 400)
    {
        if (this->gameOverState == GameOver::Retry)
            this->yes.setFillColor(this->yes.getFillColor() == sf::Color::Red ? sf::Color::White : sf::Color::Red);
        else if(this->gameOverState == GameOver::DontRetry)
            this->no.setFillColor(this->no.getFillColor() == sf::Color::Red ? sf::Color::White : sf::Color::Red);

        this->clkBlinkYesNo.restart();
    }
}

void S_InGame::victory(float frameTime)
{
    const float vitesseDapparitionText = 300.f;

    if (this->victoryText.getColor().a != 255)
        this->victoryText.setColor(this->victoryText.getColor() + sf::Color(0, 0, 0, vitesseDapparitionText * frameTime));
    else if (this->blackRect.getFillColor().a != 255 && this->victoryTime.getElapsedTime().asSeconds() > 10.f && this->victoryTime.getElapsedTime().asMilliseconds() % 1 == 0)
        this->blackRect.setFillColor(this->blackRect.getFillColor() + sf::Color(0, 0, 0, 1));
    else if (this->blackRect.getFillColor().a == 255)
        State::nextState = NextState::ChoixNiveau;
}