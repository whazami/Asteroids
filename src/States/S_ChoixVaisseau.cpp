#include "../../include/States/S_ChoixVaisseau.h"

#include <iostream>

#include <SFML/Graphics.hpp>

using namespace std;

S_ChoixVaisseau::S_ChoixVaisseau()
{
    // Chargement + Init Font
    if (!this->retroFont.loadFromFile("Assets/Fonts/Retro Gaming.ttf"))
        cout << "Erreur du chargement de la police Retro Gaming" << endl;

    this->textChoose.setFont(this->retroFont);
    this->textChoose.setString("CHOOSE YOUR SPACESHIP");
    this->textChoose.setCharacterSize(45 * screenScaleX);
    this->textChoose.setOrigin(this->textChoose.getLocalBounds().width / 2.f, this->textChoose.getLocalBounds().height / 2.f);
    this->textChoose.setFillColor(sf::Color::Red);
    this->textChoose.setStyle(sf::Text::Bold);
}

/// MAIN FUNCTIONS ///
void S_ChoixVaisseau::init()
{
    // Get Nbr of Unlocked Ships
    this->nbVaisseauxDebloques = getNbrOfUnlockedElem("Ships");

    /// INIT DIMENSIONS DU MENU ///
    // CONSTANTES D'AFFICHAGE //
    /* Coefficient d'affichage */
    const float coeffTaille = (this->nbVaisseauxDebloques >= 4 && this->nbVaisseauxDebloques <= NB_SHIPS_PER_LINE) ? 4.f / this->nbVaisseauxDebloques :                                                        // coeffTaille = 1 si il y a 4 vaisseaux ou moins, 4/(NB_SHIPS_PER_LINE - 1) si (NB_SHIPS_PER_LINE - 1) vaisseaux
        (this->nbVaisseauxDebloques < 4) ? 1 : 4.f / (float)NB_SHIPS_PER_LINE;                                                                                                                                                    // et 4/NB_SHIPS_PER_LINE si NB_SHIPS_PER_LINE vaisseaux ou plus (plus de NB_SHIPS_PER_LINE vaisseaux = plusieurs lignes)

    /* Constantes d'affichage arbitraires, elles sont toutes multipliées par coeffTaille 
     * (dimensions plus petites quand il y a plus de vaisseaux à afficher)
     * et par coeffWidthEcran ou coeffHeightEcran pour la portabilità d'un écran à l'autre. */
    const float widthRectSelec = 350.f * coeffTaille * screenScaleX;
    const float heightRectSelec = 390.f * coeffTaille * screenScaleY;
    const float ecartXentreVaisseaux = 440.f * coeffTaille * screenScaleX;
    const float ecartYentreVaisseaux = 550.f * coeffTaille * screenScaleY;
    const float espaceVideXDansRectSelec = 100.f * coeffTaille * screenScaleX;                                                                                                                                      // Les Vaisseaux ne sont pas collés aux bords de 'rectSelec', il y a a donc un "espace vide" sur la largeur
    const float espaceVideYDansRectSelec = 40.f * coeffTaille * screenScaleY;                                                                                                                                       // et sur la longueur
    const float rectSelecThickness = 5.f * coeffTaille * screenScaleX;
    const float posYRectSelecUneLigne = 650.f * screenScaleY;                                                                                                                                                       // Composante Y de la position de 'rectSelec' dans le cas d'une seule ligne de Vaisseaux (= NB_SHIPS_PER_LINE Vaisseaux ou moins)

    // DIMENSIONS RECTANGLE DE SELECTION //
    this->rectSelec = sf::RectangleShape(sf::Vector2f(widthRectSelec, heightRectSelec));
    this->rectSelec.setOrigin(this->rectSelec.getLocalBounds().width / 2.f, this->rectSelec.getLocalBounds().height / 2.f);                                                                                         // On set l'origine du 'rectSelec' au milieu de celui-ci
    this->rectSelec.setPosition((screenSize.x - ecartXentreVaisseaux * ((this->nbVaisseauxDebloques < NB_SHIPS_PER_LINE) ? (this->nbVaisseauxDebloques - 1) : NB_SHIPS_PER_LINE - 1)) / 2.f,                                 // On place le 'rectSelec' de façon à centrer l'ensemble des vaisseaux en fonction du nombre de ceux-ci
        posYRectSelecUneLigne - (ecartYentreVaisseaux * (int)((this->nbVaisseauxDebloques - 1) / NB_SHIPS_PER_LINE)) / 2.f);                                                                                                 // et à les centrer aussi autour de 'posYRextSelecUneLigne'
    this->rectSelec.setFillColor(sf::Color::Transparent);
    this->rectSelec.setOutlineThickness(rectSelecThickness);
    this->rectSelec.setOutlineColor(sf::Color::Red);
    this->rectClock.restart();
    this->textChoose.setPosition(screenSize.x / 2.f, (this->rectSelec.getPosition().y - (this->rectSelec.getLocalBounds().height / 2.f)) / 2.f);                                                       // On positionne le texte au milieu de l'écran et entre le haut de l'écran et le premier vaisseau

    // PLACEMENT DES VAISSEAUX //
    for (unsigned i = 0; i < this->nbVaisseauxDebloques; i++)
    {
        if(i) this->rectSelec.move((i % NB_SHIPS_PER_LINE) ? ecartXentreVaisseaux : -(NB_SHIPS_PER_LINE - 1) * ecartXentreVaisseaux, (i % NB_SHIPS_PER_LINE) ? 0 : ecartYentreVaisseaux);                                                                                 // On move le 'rectSelec' là oà sera le prochain Vaisseau qu'on va placer (on ne le move pas à (i==0) puisqu'il est déjà à la bonne place)
        float coeffScale = (Vaisseau::TouslesVaisseaux[i]->getSprite()->getLocalBounds().height <= 1.4 * Vaisseau::TouslesVaisseaux[i]->getSprite()->getLocalBounds().width) ?                                      // 1.4 = ('heightRectSelec' - 'espaceVideYDansRectSelec') / ('widthRectSelec' - 'espaceVideXDansRectSelec')
            (this->rectSelec.getLocalBounds().width - 2.f * this->rectSelec.getOutlineThickness() - espaceVideXDansRectSelec) / Vaisseau::TouslesVaisseaux[i]->getSprite()->getLocalBounds().width :                // On scale le Vaisseau dans le 'rectSelec' en fonction de sa width
            (this->rectSelec.getLocalBounds().height - 2.f * this->rectSelec.getOutlineThickness() - espaceVideYDansRectSelec) / Vaisseau::TouslesVaisseaux[i]->getSprite()->getLocalBounds().height;               // ou en fonction de sa height
        Vaisseau::TouslesVaisseaux[i]->getSprite()->setScale(coeffScale, coeffScale);
        Vaisseau::TouslesVaisseaux[i]->getSprite()->setOrigin(Vaisseau::TouslesVaisseaux[i]->getSprite()->getLocalBounds().width / 2.f, Vaisseau::TouslesVaisseaux[i]->getSprite()->getLocalBounds().height / 2.f); // Origine au milieu du sprite du Vaisseau
        Vaisseau::TouslesVaisseaux[i]->getSprite()->setPosition(this->rectSelec.getPosition());                                                                                                                     // Et enfin on positionne le Vaisseau à l'emplacement du 'rectSelec'
    }

    this->rectSelec.move(-ecartXentreVaisseaux * ((this->nbVaisseauxDebloques - 1) % NB_SHIPS_PER_LINE), -ecartYentreVaisseaux * (((int)this->nbVaisseauxDebloques - 1) / NB_SHIPS_PER_LINE));                                        // On remet le 'rectSelec' à sa position initiale
    
    this->vaisseauChoisi = 0;

    // Music
    this->setMusic("Assets/Musiques/States/Menu.ogg");
}

void S_ChoixVaisseau::update(float frameTime)                                                                                                                                                                 // Frame time non utilisé
{
    sf::Event eventMoveRect = sf::Event();                                                                                                                                                                          // On crée un objet sf::Event local pour la fct "moveRectSelec" puisque cette dernière doit être appelée en dehors de la boucle d'event

    while (this->window->pollEvent(this->e) || wl::Joystick::pollEvent(this->e))
    {
        if (this->e.type == sf::Event::Closed)
            this->window->close();

        if (this->e.type == sf::Event::MouseMoved)
            this->changeVaisseauChoisiWithMouse(this->e.mouseMove);

        if (this->e.type == sf::Event::JoystickButtonPressed || this->e.type == sf::Event::JoystickMoved)
            this->cheatCode();

        if (this->e.type == sf::Event::JoystickMoved || this->e.type == sf::Event::KeyPressed)
            eventMoveRect = this->e;

        if ((this->e.type == sf::Event::JoystickButtonPressed && this->e.joystickButton.button == DualShock::Croix)
            || (this->e.type == sf::Event::KeyPressed && this->e.key.code == sf::Keyboard::Enter)
            || (this->e.type == sf::Event::MouseButtonPressed && this->e.mouseButton.button == sf::Mouse::Left && this->rectSelec.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition())))
            State::nextState = NextState::InGame;

        if ((this->e.type == sf::Event::JoystickButtonPressed && this->e.joystickButton.button == DualShock::Rond)
            || (this->e.type == sf::Event::KeyPressed && this->e.key.code == sf::Keyboard::Escape))
            State::nextState = NextState::ChoixNiveau;
    }

    this->moveRectSelec(eventMoveRect);
}

void S_ChoixVaisseau::show()
{
    this->window->draw(this->background);
    this->window->draw(this->textChoose);
    for (unsigned i = 0; i < this->nbVaisseauxDebloques; i++)
        Vaisseau::TouslesVaisseaux[i]->show(this->window);
    window->draw(this->rectSelec);
}


/// FONCTIONS ANNEXES ///
void S_ChoixVaisseau::moveRectSelec(sf::Event e)
{
    static sf::Clock clk;                                                                                                                                                                                           // 'clk' temporise l'animation de défilement créée lorsque l'utilisateur dirige le joystick gauche pendant un certain temps (tpsAvantDefilement)
    static float iterationsPdtDeplacement; const int tpsAvantDefilement = 500; const int tpsPdtDefilement = 25;                                                                                                     // On compte les itérations pendant le défilement pour la temporisation. 'tpsPdtDefilement' correspond au temps entre chaque action de défilement.
    static int oldVaisseauChoisi = this->vaisseauChoisi;

    if ((wl::Joystick::getAxisPosition(sf::Joystick::X) == 100 && clk.getElapsedTime().asMilliseconds() >= tpsAvantDefilement + iterationsPdtDeplacement * tpsPdtDefilement)                                     // Si le joystick est vers la droite et que la temporisation l'autorise
        || (e.joystickMove.axis == sf::Joystick::PovX && e.joystickMove.position == 100) || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Right))                                                 // ou si on a appuyà sur la flèche droite de la manette ou du clavier
    {
        this->vaisseauChoisi = ((this->vaisseauChoisi + 1) % NB_SHIPS_PER_LINE == 0) ? this->vaisseauChoisi - (NB_SHIPS_PER_LINE - 1) : this->vaisseauChoisi + 1;                                                                                         // On incrémente 'vaisseauChoisi' si on n'est pas au bout de la ligne (dans ce cas-là  on le décrémente de (NB_SHIPS_PER_LINE - 1) pour revenir au début de la ligne)
        if (this->vaisseauChoisi >= this->nbVaisseauxDebloques) this->vaisseauChoisi -= this->nbVaisseauxDebloques % NB_SHIPS_PER_LINE;                                                                               // Si on dépasse le nombre de Vaisseaux, on revient au début de la ligne (dont sa taille vaut "this->nbVaisseauxDebloques % NB_SHIPS_PER_LINE")
    }
    if ((wl::Joystick::getAxisPosition(sf::Joystick::X) == -100 && clk.getElapsedTime().asMilliseconds() >= tpsAvantDefilement + iterationsPdtDeplacement * tpsPdtDefilement)                                    // Si le joystick est vers la gauche et que la temporisation l'autorise
        || (e.joystickMove.axis == sf::Joystick::PovX && e.joystickMove.position == -100) || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Left))                                                 // ou si on a appuyà sur la flèche gauche de la manette ou du clavier
    {
        this->vaisseauChoisi = (this->vaisseauChoisi % NB_SHIPS_PER_LINE == 0) ? this->vaisseauChoisi + (NB_SHIPS_PER_LINE - 1) : this->vaisseauChoisi - 1;                                                                                               // On décrémente 'vaisseauChoisi' si on n'est pas au début de la ligne (dans ce cas-là  on l'incrémente de (NB_SHIPS_PER_LINE - 1) pour se placer au bout de la ligne)
        if (this->vaisseauChoisi >= this->nbVaisseauxDebloques) this->vaisseauChoisi = this->nbVaisseauxDebloques - 1;                                                                                // Si on dépasse le nombre de Vaisseaux, on se place sur le dernier Vaisseau de la liste
    }
    if ((wl::Joystick::getAxisPosition(sf::Joystick::Y) == 100 && clk.getElapsedTime().asMilliseconds() >= tpsAvantDefilement + iterationsPdtDeplacement * tpsPdtDefilement)                                     // Si le joystick est vers le bas et que la temporisation l'autorise
        || (e.joystickMove.axis == sf::Joystick::PovY && e.joystickMove.position == -100) || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Down))                                                 // ou si on a appuyà sur la flèche du bas de la manette ou du clavier
    {
        this->vaisseauChoisi += NB_SHIPS_PER_LINE;                                                                                                                                                                                  // On incrémente 'vaisseauChoisi' de NB_SHIPS_PER_LINE (+1 ligne)
        if (this->vaisseauChoisi >= this->nbVaisseauxDebloques) this->vaisseauChoisi %= NB_SHIPS_PER_LINE;                                                                                                                   // Si on dépasse le nombre de Vaisseaux, on se place sur le 'vaisseauChoisi' correspondant sur la première ligne
    }
    if ((wl::Joystick::getAxisPosition(sf::Joystick::Y) == -100 && clk.getElapsedTime().asMilliseconds() >= tpsAvantDefilement + iterationsPdtDeplacement * tpsPdtDefilement)                                    // Si le joystick est vers le haut et que la temporisation l'autorise
        || (e.joystickMove.axis == sf::Joystick::PovY && e.joystickMove.position == 100) || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Up))                                                    // ou si on appuyà sur la flèche du haut de la manette ou du clavier
    {
        this->vaisseauChoisi = (this->vaisseauChoisi % NB_SHIPS_PER_LINE == this->vaisseauChoisi) ? this->vaisseauChoisi + ((int)(this->nbVaisseauxDebloques - 1) / NB_SHIPS_PER_LINE) * NB_SHIPS_PER_LINE : this->vaisseauChoisi - NB_SHIPS_PER_LINE;                       // On décrémente 'vaisseauChoisi' de NB_SHIPS_PER_LINE (-1 ligne) si on n'est pas sur la première ligne (dans ce cas-là  on l'incrémente de "nb_de_lignes * NB_SHIPS_PER_LINE" pour se placer au 'vaisseauChoisi' correspondant sur la dernière ligne)
        if (this->vaisseauChoisi >= this->nbVaisseauxDebloques) this->vaisseauChoisi = this->nbVaisseauxDebloques - 1;                                                                                // Si on dépasse le nombre de Vaisseaux, on se place sur le dernier Vaisseau
    }

    if(this->vaisseauChoisi != oldVaisseauChoisi)
    {
        this->rectSelec.setPosition(Vaisseau::TouslesVaisseaux[this->vaisseauChoisi]->getSprite()->getPosition());                                                                                                  // On met le rectSelec sur le nouveau vaisseau choisi
        this->rectSelec.setOutlineColor(sf::Color::Red);
        this->rectClock.restart();
        iterationsPdtDeplacement = (iterationsPdtDeplacement == -(float)tpsAvantDefilement / tpsPdtDefilement) ? 0 : (int)iterationsPdtDeplacement + 1;                                                             // On set 'iterationsPdtDeplacement' à 0 si on est au début du défilement sinon on l'incrémente
    
        oldVaisseauChoisi = this->vaisseauChoisi;
    }

    if (wl::Joystick::getAxisPosition(sf::Joystick::X) != 100 && wl::Joystick::getAxisPosition(sf::Joystick::X) != -100                                                                                       // Si on n'utilise pas le joystick gauche,
        && wl::Joystick::getAxisPosition(sf::Joystick::Y) != 100 && wl::Joystick::getAxisPosition(sf::Joystick::Y) != -100)
    {
        clk.restart();                                                                                                                                                                                              // on restart 'clk' chargée de la temporisation du défilement
        iterationsPdtDeplacement = -(float)tpsAvantDefilement / tpsPdtDefilement;                                                                                                                                   // et on set 'iterationsPdtDeplacement' de manière à ce que ça annule 'tpsAvantDefilement' (--> 0 attente dès qu'on bouge le joystick gauche)
    }

    // Changement de couleur du 'rectSelec'
    if (this->rectClock.getElapsedTime().asMilliseconds() >= 500)
    {
        this->rectSelec.setOutlineColor((this->rectSelec.getOutlineColor() == sf::Color::Red) ? sf::Color::White : sf::Color::Red);
        this->rectClock.restart();
    }
}

void S_ChoixVaisseau::changeVaisseauChoisiWithMouse(sf::Event::MouseMoveEvent posMouse)
{
    for (unsigned i = 0; i < this->nbVaisseauxDebloques; i++)
    {
        sf::Vector2f pos = Vaisseau::TouslesVaisseaux[i]->getSprite()->getPosition();
        float demiWidth = this->rectSelec.getLocalBounds().width / 2.f;
        float demiHeight = this->rectSelec.getLocalBounds().height / 2.f;

        if (posMouse.x <= pos.x + demiWidth && posMouse.x >= pos.x - demiWidth && posMouse.y <= pos.y + demiHeight && posMouse.y >= pos.y - demiHeight)
            this->vaisseauChoisi = i;
    }
}

/* Change la musique en fonction du cheatcode réalisé
 * On n'utilise volontairement pas la fonction 'State::setMusic'
 * parce qu'on veut un changement immédiat de la musique. */
void S_ChoixVaisseau::cheatCode()
{
    static sf::Clock clk;
    static int cpt;

    if (clk.getElapsedTime().asSeconds() <= 2)
    {
        switch (this->e.type)
        {
        case sf::Event::JoystickButtonPressed:
            switch ((DualShock)this->e.joystickButton.button)
            {
            case DualShock::L1:
                cpt = (cpt == 0) ? cpt + 1 : 0;
                break;

            case DualShock::L2:
                cpt = (cpt == 1) ? cpt + 1 : 0;
                break;

            case DualShock::R1:
                cpt = (cpt == 2) ? cpt + 1 : 0;
                break;

            case DualShock::R2:
                if (cpt == 8) {
                    State::affichePopup(Vaisseau::TouslesVaisseaux.back()->getSprite(), "Cheat code activé !", "Tous les vaisseaux sont débloqués.");
                    for (int i = getNbrOfUnlockedElem("Ships"); i < Vaisseau::TouslesVaisseaux.size(); i++)
                        incrementUnlockedElem("Ships");
                    State::nextState = NextState::ChoixVaisseau;
                    cpt = 0;
                }
                if (cpt == 2 && this->cheatMusic)
                {
                    State::affichePopup(Icone::Musique, "Cheat code désactivé", "Musique par défaut en lecture.");
                    this->music->stop();
                    if (!this->music->openFromFile("Assets/Musiques/States/Menu.ogg"))
                        cout << "Erreur chargement musique du menu" << endl;
                    this->music->play();
                    this->cheatMusic = false;
                    cpt = 0;
                }
                cpt = (cpt == 3) ? cpt + 1 : 0;
                break;

            case DualShock::Triangle:
                cpt = (cpt == 1) ? cpt + 1 : 0;
                break;

            case DualShock::Carre:
                cpt = (cpt == 0) ? cpt + 1 : 0;
                break;

            default:
                cpt = 0;
                break;
            }
            clk.restart();
            break;

        case sf::Event::JoystickMoved:
            if (this->e.joystickMove.axis == sf::Joystick::PovY)
            {
                switch ((int)this->e.joystickMove.position)
                {
                case 100: // Haut
                    cpt = (cpt == 4 || cpt == 2 || cpt <= 1) ? cpt + 1 : 0;
                    break;

                case -100: // Bas
                    cpt = (cpt == 5 || cpt == 3 || cpt == 2) ? cpt + 1 : 0;
                    break;
                }
                clk.restart();
            }
            else if (this->e.joystickMove.axis == sf::Joystick::PovX)
            {
                switch ((int)this->e.joystickMove.position)
                {
                case -100: // Gauche
                    cpt = (cpt >= 4 && cpt <= 7) ? cpt + 1 : 0;
                    break;

                case 100: // Droite
                    if (cpt == 7)
                    {
                        State::affichePopup(Icone::Musique, "Cheat code activé !", "Pour revenir à la musique par défaut, appuyez sur 'Carré', 'Triangle', 'R2' rapidement.");
                        this->music->stop();
                        if (!this->music->openFromFile("Assets/Musiques/Cheat/Zola - Baby Boy.ogg"))
                            cout << "Erreur chargement ZOLA Baby Boy" << endl;
                        this->music->setPlayingOffset(sf::seconds(9.5f));
                        this->music->play();
                        this->cheatMusic = true;
                        cpt = 0;
                    }
                    else if (cpt == 5)
                    {
                        State::affichePopup(Icone::Musique, "Cheat code activé !", "Pour revenir à la musique par défaut, appuyez sur 'Carré', 'Triangle', 'R2' rapidement.");
                        this->music->stop();
                        if (!this->music->openFromFile("Assets/Musiques/Cheat/Zola - L1 L2.ogg"))
                            cout << "Erreur chargement ZOLA L1 L2" << endl;
                        this->music->setPlayingOffset(sf::seconds(21.5f));
                        this->music->play();
                        this->cheatMusic = true;
                        cpt = 0;
                    }
                    cpt = (cpt == 4 || cpt == 6) ? cpt + 1 : 0;
                    break;
                }
                clk.restart();
            }
            break;
        }
    }
    else
    {
        cpt = 0;
        clk.restart();
    }
}
