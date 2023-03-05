#include "../../include/States/S_ChoixNiveau.h"

#include <iostream>

#include <SFML/Graphics.hpp>

using namespace std;

const sf::Color gris(125, 125, 125);

S_ChoixNiveau::S_ChoixNiveau() : NB_NIVEAUX_COLONNE(8), NB_MAX_NIVEAUX(NB_NIVEAUX_COLONNE * 3)
{
    // Chargement + Init Font
    if (!this->retroFont.loadFromFile("Assets/Fonts/Retro Gaming.ttf"))
        cout << "Erreur du chargement de la police Retro Gaming" << endl;

    this->textChoose.setFont(this->retroFont);
    this->textChoose.setString("CHOOSE YOUR LEVEL");
    this->textChoose.setCharacterSize(45 * screenScaleX);
    this->textChoose.setOrigin(this->textChoose.getLocalBounds().width / 2.f, this->textChoose.getLocalBounds().height / 2.f);
    this->textChoose.setFillColor(sf::Color(200, 0, 0));
    this->textChoose.setStyle(sf::Text::Bold);

    // Fleche
    if (!this->flecheT.loadFromFile("Assets/Images/Icones/Fleche.png"))
        cout << "Erreur du chargement de l'image de la fleche." << endl;

    this->fleche.setTexture(this->flecheT);
    this->fleche.setScale(1.f / 2.f, 1.f / 2.f);
    this->fleche.setOrigin(this->fleche.getLocalBounds().width / 2.f, this->fleche.getLocalBounds().height / 2.f);
    this->fleche.setPosition(100.f * screenScaleX, 100.f * screenScaleY);
    this->fleche.setColor(gris);

    this->rectFleche.setSize(sf::Vector2f(85.f * screenScaleX, 85.f * screenScaleX));
    this->rectFleche.setOutlineThickness(5.f * screenScaleX);
    this->rectFleche.setOrigin(this->rectFleche.getGlobalBounds().width / 2.f - this->rectFleche.getOutlineThickness(), this->rectFleche.getGlobalBounds().height / 2.f - this->rectFleche.getOutlineThickness());
    this->rectFleche.setPosition(this->fleche.getPosition());
    this->rectFleche.setFillColor(sf::Color::Transparent);
    this->rectFleche.setOutlineColor(gris);

    if (!this->rondT.loadFromFile("Assets/Images/Boutons/Rond.png"))
        cout << "Erreur du chargement de l'image du bouton rond." << endl;

    this->rond.setTexture(this->rondT);
    this->rond.setScale(1.f / 5.f, 1.f / 5.f);
    this->rond.setOrigin(this->rond.getLocalBounds().width / 2.f, this->rond.getLocalBounds().height / 2.f);
    this->rond.setPosition(this->rectFleche.getPosition() + sf::Vector2f(this->rectFleche.getGlobalBounds().width - this->rectFleche.getOutlineThickness(), this->rectFleche.getGlobalBounds().height - this->rectFleche.getOutlineThickness()) / 2.f);
}

void S_ChoixNiveau::init()
{
    /// INIT DIMENSIONS DU MENU ///
    /* Constantes d'affichage arbitraires, elles sont toutes multipliées par coeffTaille
     * (dimensions plus petites quand il y a plus de vaisseaux à afficher)
     * et par coeffWidthEcran ou coeffHeightEcran pour la portabilità d'un écran à l'autre. */
    const float widthRectSelec = 140.f * screenScaleX;
    const float ecartentreNiveaux = 185.f * screenScaleX;
    const float rectSelecThickness = 5.f * screenScaleX;

    // DIMENSIONS RECTANGLE DE SELECTION //
    sf::RectangleShape rectSelec(sf::Vector2f(widthRectSelec, widthRectSelec));                                                                                                                                         // rect qui va servir à placer les rects des niveaux (il est invisible)
    rectSelec.setOrigin(widthRectSelec / 2.f, widthRectSelec / 2.f);                                                                                                                                                    // On set l'origine du 'rectSelec' au milieu de celui-ci
    rectSelec.setPosition((screenSize.x - NB_NIVEAUX_COLONNE * widthRectSelec - (NB_NIVEAUX_COLONNE - 1) * (ecartentreNiveaux - widthRectSelec)) / 2.f + widthRectSelec / 2.f, 390.f * screenScaleY);      // et à les centrer aussi autour de 'posYRextSelecUneLigne'
    rectSelec.setFillColor(sf::Color::Transparent);
    rectSelec.setOutlineThickness(rectSelecThickness);
    rectSelec.setOutlineColor(gris);
    this->textChoose.setPosition(screenSize.x / 2.f, (rectSelec.getPosition().y - (rectSelec.getLocalBounds().height / 2.f)) / 2.f);                                                                       // On positionne le texte au milieu de l'écran et entre le haut de l'écran et le premier vaisseau

    // PLACEMENT DES NIVEAUX //
    this->nbNiveauxDebloques = getNbrOfUnlockedElem("Levels");

    for (unsigned i = 0; i < NB_MAX_NIVEAUX; i++)
    {
        if (i) rectSelec.move((i % NB_NIVEAUX_COLONNE) ? ecartentreNiveaux : -(NB_NIVEAUX_COLONNE - 1) * ecartentreNiveaux, (i % NB_NIVEAUX_COLONNE) ? 0 : ecartentreNiveaux);                                          // On move le 'rectSelec' là oà sera le prochain Vaisseau qu'on va placer (on ne le move pas à (i==0) puisqu'il est déjà à la bonne place)
        this->niveaux.push_back(sf::RectangleShape(rectSelec));
        sf::Text numNiveau;
        numNiveau.setFont(this->retroFont);
        numNiveau.setString(((i + 1) <= this->nbNiveauxDebloques) ? to_string(i + 1) : "?");
        numNiveau.setCharacterSize(35 * screenScaleX);
        numNiveau.setFillColor(gris);
        numNiveau.setOrigin(numNiveau.getGlobalBounds().width / 2.f, numNiveau.getGlobalBounds().height / 2.f);
        numNiveau.setPosition(rectSelec.getPosition() - sf::Vector2f(rectSelecThickness, rectSelecThickness));
        this->numNiveaux.push_back(numNiveau);
    }

    this->niveaux[this->niveauChoisi].setFillColor(gris);
    this->numNiveaux[this->niveauChoisi].setFillColor(sf::Color::Black);

    // Music
    this->setMusic("Assets/Musiques/States/Menu.ogg");
}

void S_ChoixNiveau::update(float frameTime)                                                                                                                                                                       // Frame time non utilisé
{
    sf::Event eventMoveRect = sf::Event();

	while (this->window->pollEvent(this->e) || wl::Joystick::pollEvent(this->e))
	{
		if (this->e.type == sf::Event::Closed)
			this->window->close();

        if (this->e.type == sf::Event::MouseMoved)
            this->changeNiveauChoisiWithMouse(this->e.mouseMove);

        if (this->e.type == sf::Event::JoystickMoved || this->e.type == sf::Event::KeyPressed)
            eventMoveRect = this->e;

        if ((this->e.type == sf::Event::MouseButtonPressed && this->e.mouseButton.button == sf::Mouse::Left && this->rectFleche.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition()))
            || (this->e.type == sf::Event::JoystickButtonPressed && this->e.joystickButton.button == DualShock::Rond)
            || (this->e.type == sf::Event::KeyPressed && this->e.key.code == sf::Keyboard::Escape))
            State::nextState = NextState::MenuDemarrage;

        if (this->e.type == sf::Event::MouseButtonPressed && this->e.mouseButton.button == sf::Mouse::Left && this->niveaux[this->niveauChoisi].getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition())
            || this->e.type == sf::Event::JoystickButtonPressed && this->e.joystickButton.button == DualShock::Croix
            || this->e.type == sf::Event::KeyPressed && this->e.key.code == sf::Keyboard::Enter)
			{
                this->niveaux[this->niveauChoisi].setFillColor(sf::Color::Transparent);
                this->numNiveaux[this->niveauChoisi].setFillColor(gris);
                this->clicloc.restart();
				if (this->niveauChoisi + 1 <= this->nbNiveauxDebloques)
					State::nextState = NextState::ChoixVaisseau;
            }
	}

    if (!this->rectFleche.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition()))
        this->moveRectSelec(eventMoveRect);

    static bool isOut;
    if (this->rectFleche.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition()))
    {
        this->fleche.setColor(sf::Color::Black);
        this->rectFleche.setFillColor(gris);
        this->niveaux[this->niveauChoisi].setFillColor(sf::Color::Transparent);
        this->numNiveaux[this->niveauChoisi].setFillColor(gris);
        isOut = true;
    }
    else
    {
        this->fleche.setColor(gris);
        this->rectFleche.setFillColor(sf::Color::Transparent);
        if (isOut)
        {
            this->niveaux[this->niveauChoisi].setFillColor(gris);
            this->numNiveaux[this->niveauChoisi].setFillColor(sf::Color::Black);
            isOut = false;
        }
    }

    this->rond.setColor(sf::Joystick::isConnected(0) ? sf::Color::White : sf::Color::Transparent);
}

void S_ChoixNiveau::show()
{
    this->window->draw(this->textChoose);

    for (int i = 0; i < NB_MAX_NIVEAUX; i++)
    {
        this->window->draw(this->niveaux[i]);
        this->window->draw(this->numNiveaux[i]);
    }

    this->window->draw(this->rectFleche);
    this->window->draw(this->fleche);
    this->window->draw(this->rond);
}

void S_ChoixNiveau::release()
{
    this->niveaux.clear();
    this->numNiveaux.clear();
}

/// FONCTIONS ANNEXES ///
void S_ChoixNiveau::moveRectSelec(sf::Event& e)
{
    static sf::Clock clk;                                                                                                                                                                                               // 'clk' temporise l'animation de défilement créée lorsque l'utilisateur dirige le joystick gauche pendant un certain temps (tpsAvantDefilement)
    static float iterationsPdtDeplacement; const int tpsAvantDefilement = 500; const int tpsPdtDefilement = 25;                                                                                                         // On compte les itérations pendant le défilement pour la temporisation. 'tpsPdtDefilement' correspond au temps entre chaque action de défilement.
    static int oldniveauChoisi;
    static bool clic;

    if ((wl::Joystick::getAxisPosition(sf::Joystick::X) == 100 && clk.getElapsedTime().asMilliseconds() >= tpsAvantDefilement + iterationsPdtDeplacement * tpsPdtDefilement)                                         // Si le joystick est vers la droite et que la temporisation l'autorise
        || (e.joystickMove.axis == sf::Joystick::PovX && e.joystickMove.position == 100) || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Right))                                                     // ou si on a appuyà sur la flèche droite de la manette ou du clavier
    {
        this->niveauChoisi = ((this->niveauChoisi + 1) % NB_NIVEAUX_COLONNE == 0) ? this->niveauChoisi - (NB_NIVEAUX_COLONNE - 1) : this->niveauChoisi + 1;                                                             // On incrémente 'niveauChoisi' si on n'est pas au bout de la ligne (dans ce cas-là  on le décrémente de 5 pour revenir au début de la ligne)
        if (this->niveauChoisi >= NB_MAX_NIVEAUX) this->niveauChoisi -= NB_MAX_NIVEAUX % NB_NIVEAUX_COLONNE;                                                                                                            // Si on dépasse le nombre de Vaisseaux, on revient au début de la ligne (dont sa taille vaut "NB_MAX_VAISSEAUX % 6")
    }
    if ((wl::Joystick::getAxisPosition(sf::Joystick::X) == -100 && clk.getElapsedTime().asMilliseconds() >= tpsAvantDefilement + iterationsPdtDeplacement * tpsPdtDefilement)                                        // Si le joystick est vers la gauche et que la temporisation l'autorise
        || (e.joystickMove.axis == sf::Joystick::PovX && e.joystickMove.position == -100) || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Left))                                                     // ou si on a appuyà sur la flèche gauche de la manette ou du clavier
    {
        this->niveauChoisi = (this->niveauChoisi % NB_NIVEAUX_COLONNE == 0) ? this->niveauChoisi + (NB_NIVEAUX_COLONNE - 1) : this->niveauChoisi - 1;                                                                   // On décrémente 'niveauChoisi' si on n'est pas au début de la ligne (dans ce cas-là  on l'incrémente de 5 pour se placer au bout de la ligne)
        if (this->niveauChoisi >= NB_MAX_NIVEAUX) this->niveauChoisi = NB_MAX_NIVEAUX - 1;                                                                                                                              // Si on dépasse le nombre de Vaisseaux, on se place sur le dernier Vaisseau de la liste
    }
    if ((wl::Joystick::getAxisPosition(sf::Joystick::Y) == 100 && clk.getElapsedTime().asMilliseconds() >= tpsAvantDefilement + iterationsPdtDeplacement * tpsPdtDefilement)                                         // Si le joystick est vers le bas et que la temporisation l'autorise
        || (e.joystickMove.axis == sf::Joystick::PovY && e.joystickMove.position == -100) || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Down))                                                     // ou si on a appuyà sur la flèche du bas de la manette ou du clavier
    {
        this->niveauChoisi += NB_NIVEAUX_COLONNE;                                                                                                                                                                       // On incrémente 'niveauChoisi' de 6 (+1 ligne)
        if (this->niveauChoisi >= NB_MAX_NIVEAUX) this->niveauChoisi %= NB_NIVEAUX_COLONNE;                                                                                                                             // Si on dépasse le nombre de Vaisseaux, on se place sur le 'vaisseauChoisi' correspondant sur la première ligne
    }
    if ((wl::Joystick::getAxisPosition(sf::Joystick::Y) == -100 && clk.getElapsedTime().asMilliseconds() >= tpsAvantDefilement + iterationsPdtDeplacement * tpsPdtDefilement)                                        // Si le joystick est vers le haut et que la temporisation l'autorise
        || (e.joystickMove.axis == sf::Joystick::PovY && e.joystickMove.position == 100) || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Up))                                                        // ou si on appuyà sur la flèche du haut de la manette ou du clavier
    {
        this->niveauChoisi = (this->niveauChoisi % NB_NIVEAUX_COLONNE == this->niveauChoisi) ? this->niveauChoisi + ((int)(NB_MAX_NIVEAUX - 1) / NB_NIVEAUX_COLONNE) * NB_NIVEAUX_COLONNE : this->niveauChoisi - NB_NIVEAUX_COLONNE;  // On décrémente 'niveauChoisi' de 6 (-1 ligne) si on n'est pas sur la première ligne (dans ce cas-là  on l'incrémente de "nb_de_lignes * 6" pour se placer au 'vaisseauChoisi' correspondant sur la dernière ligne)
        if (this->niveauChoisi >= NB_MAX_NIVEAUX) this->niveauChoisi = NB_MAX_NIVEAUX - 1;                                                                                                                              // Si on dépasse le nombre de Vaisseaux, on se place sur le dernier Vaisseau
    }

    if (this->clicloc.getElapsedTime().asMilliseconds() >= 50)
    {
        if (this->niveauChoisi != oldniveauChoisi || clic)
        {
            this->niveaux[oldniveauChoisi].setFillColor(sf::Color::Transparent);
            this->numNiveaux[oldniveauChoisi].setFillColor(gris);
            this->niveaux[this->niveauChoisi].setFillColor(gris);
            this->numNiveaux[this->niveauChoisi].setFillColor(sf::Color::Black);
            iterationsPdtDeplacement = (iterationsPdtDeplacement == -(float)tpsAvantDefilement / tpsPdtDefilement) ? 0 : (int)iterationsPdtDeplacement + 1;                                                             // On set 'iterationsPdtDeplacement' à 0 si on est au début du défilement sinon on l'incrémente

            oldniveauChoisi = this->niveauChoisi;
            clic = false;
        }
    }
    else
        clic = true;

    if (wl::Joystick::getAxisPosition(sf::Joystick::X) != 100 && wl::Joystick::getAxisPosition(sf::Joystick::X) != -100                                                                                           // Si on n'utilise pas le joystick gauche,
        && wl::Joystick::getAxisPosition(sf::Joystick::Y) != 100 && wl::Joystick::getAxisPosition(sf::Joystick::Y) != -100)
    {
        clk.restart();                                                                                                                                                                                                  // on restart 'clk' chargée de la temporisation du défilement
        iterationsPdtDeplacement = -(float)tpsAvantDefilement / tpsPdtDefilement;                                                                                                                                       // et on set 'iterationsPdtDeplacement' de manière à ce que ça annule 'tpsAvantDefilement' (--> 0 attente dès qu'on bouge le joystick gauche)
    }
}

void S_ChoixNiveau::changeNiveauChoisiWithMouse(sf::Event::MouseMoveEvent posMouse)
{
    for (int i = 0; i < NB_MAX_NIVEAUX; i++)
    {
        float posX = this->niveaux[i].getPosition().x, posY = this->niveaux[i].getPosition().y;
        float demiWidth = this->niveaux[i].getGlobalBounds().width / 2.f;

        if (posMouse.x <= posX + demiWidth && posMouse.x >= posX - demiWidth && posMouse.y <= posY + demiWidth && posMouse.y >= posY - demiWidth)
            this->niveauChoisi = i;
    }
}