#include "../../include/States/S_MenuPause.h"

S_MenuPause::S_MenuPause() : rectMenu(screenSize / 2.f), rectSon(sf::Vector2f(75.f * screenScaleX, 75.f * screenScaleX)), rectMusique(sf::Vector2f(75.f * screenScaleX, 75.f * screenScaleX)), rectVibrations(sf::Vector2f(75.f * screenScaleX, 75.f * screenScaleX))
{
	// Rect du menu
	this->rectMenu.setOrigin(this->rectMenu.getSize().x / 2.f, this->rectMenu.getSize().y / 2.f);
	this->rectMenu.setPosition(screenSize / 2.f);
	this->rectMenu.setOutlineThickness(5.f * screenScaleX);
	this->rectMenu.setOutlineColor(sf::Color::Red);
	this->rectMenu.setFillColor(sf::Color::Black);

	// Textes
	this->retroFont.loadFromFile(assetsPath + "/Fonts/Retro Gaming.ttf");

	this->textes[0].setFont(this->retroFont);
	this->textes[0].setString("PAUSE");
	this->textes[0].setCharacterSize(60 * screenScaleX);
	this->textes[0].setOrigin(this->textes[0].getGlobalBounds().width / 2.f, this->textes[0].getGlobalBounds().height / 2.f);
	this->textes[0].setPosition(this->rectMenu.getPosition().x, this->rectMenu.getGlobalBounds().top + (75.f * screenScaleY));
	this->textes[0].setFillColor(sf::Color::Red);
	this->textes[0].setStyle(sf::Text::Bold);

	this->textes[1].setFont(this->retroFont);
	this->textes[1].setString("Resume");
	this->textes[1].setCharacterSize(40 * screenScaleX);
	this->textes[1].setOrigin(this->textes[1].getGlobalBounds().width / 2.f, this->textes[1].getGlobalBounds().height / 2.f);
	this->textes[1].setPosition(this->rectMenu.getPosition().x, this->textes[0].getGlobalBounds().top + (140.f * screenScaleY));
	this->textes[1].setFillColor(sf::Color::Red);

	this->textes[2].setFont(this->retroFont);
	this->textes[2].setString("Retry");
	this->textes[2].setCharacterSize(40 * screenScaleX);
	this->textes[2].setOrigin(this->textes[2].getGlobalBounds().width / 2.f, this->textes[2].getGlobalBounds().height / 2.f);
	this->textes[2].setPosition(this->rectMenu.getPosition().x, this->textes[1].getGlobalBounds().top + (90 * screenScaleY));
	this->textes[2].setFillColor(sf::Color::Red);

	this->textes[3].setFont(this->retroFont);
	this->textes[3].setString("Main Menu");
	this->textes[3].setCharacterSize(40 * screenScaleX);
	this->textes[3].setOrigin(this->textes[3].getGlobalBounds().width / 2.f, this->textes[3].getGlobalBounds().height / 2.f);
	this->textes[3].setPosition(this->rectMenu.getPosition().x, this->textes[2].getGlobalBounds().top + (90 * screenScaleY));
	this->textes[3].setFillColor(sf::Color::Red);

	this->textes[4].setFont(this->retroFont);
	this->textes[4].setString("Quit Game");
	this->textes[4].setCharacterSize(40 * screenScaleX);
	this->textes[4].setOrigin(this->textes[4].getGlobalBounds().width / 2.f, this->textes[4].getGlobalBounds().height / 2.f);
	this->textes[4].setPosition(this->rectMenu.getPosition().x, this->textes[3].getGlobalBounds().top + (90 * screenScaleY));
	this->textes[4].setFillColor(sf::Color::Red);

	// Rectangles
	this->rectSon.setOutlineThickness(1.f * screenScaleX);
	this->rectSon.setOutlineColor(sf::Color::Red);
	this->rectSon.setFillColor(sf::Color::Black);
	this->rectSon.setOrigin(this->rectSon.getSize().x / 2.f, this->rectSon.getSize().y / 2.f);
	this->rectSon.setPosition(this->rectMenu.getGlobalBounds().left + this->rectSon.getSize().x / 2.f + (25.f * screenScaleX), this->rectMenu.getGlobalBounds().top + this->rectSon.getSize().y / 2.f + (25.f * screenScaleY));

	this->rectMusique.setOutlineThickness(1.f * screenScaleX);
	this->rectMusique.setOutlineColor(sf::Color::Red);
	this->rectMusique.setFillColor(sf::Color::Black);
	this->rectMusique.setOrigin(this->rectMusique.getSize().x / 2.f, this->rectMusique.getSize().y / 2.f);
	this->rectMusique.setPosition(this->rectSon.getPosition().x + this->rectSon.getSize().x + (25.f * screenScaleX), this->rectMenu.getGlobalBounds().top + this->rectMusique.getSize().y / 2.f + (25.f * screenScaleY));

	this->rectVibrations.setOutlineThickness(1.f * screenScaleX);
	this->rectVibrations.setOutlineColor(sf::Color::Red);
	this->rectVibrations.setFillColor(sf::Color::Black);
	this->rectVibrations.setOrigin(this->rectVibrations.getSize().x / 2.f, this->rectVibrations.getSize().y / 2.f);
	this->rectVibrations.setPosition(this->rectMusique.getPosition().x + this->rectMusique.getSize().x + (25.f * screenScaleX), this->rectMenu.getGlobalBounds().top + this->rectVibrations.getSize().y / 2.f + (25.f * screenScaleY));

	// Images
	float scale = 0.75f * this->rectSon.getSize().x / 150.f;																		// Les rectangles sont des carrés, il en est de même pour les icônes (dont leurs côtés = 150px)

	this->sonT.loadFromFile(assetsPath + "/Images/Icones/Son.png");
	this->son.setTexture(this->sonT);
	this->son.setOrigin(this->son.getGlobalBounds().width / 2.f, this->son.getGlobalBounds().height / 2.f);
	this->son.setPosition(this->rectSon.getPosition());
	this->son.setScale(scale, scale);

	this->sonCoupeT.loadFromFile(assetsPath + "/Images/Icones/Son Coupe.png");
	this->sonCoupe.setTexture(this->sonCoupeT);
	this->sonCoupe.setOrigin(this->sonCoupe.getGlobalBounds().width / 2.f, this->sonCoupe.getGlobalBounds().height / 2.f);
	this->sonCoupe.setPosition(this->rectSon.getPosition());
	this->sonCoupe.setScale(scale, scale);

	this->musiqueT.loadFromFile(assetsPath + "/Images/Icones/Musique.png");
	this->musique.setTexture(this->musiqueT);
	this->musique.setOrigin(this->musique.getGlobalBounds().width / 2.f, this->musique.getGlobalBounds().height / 2.f);
	this->musique.setPosition(this->rectMusique.getPosition());
	this->musique.setScale(scale, scale);

	this->musiqueCoupeeT.loadFromFile(assetsPath + "/Images/Icones/Musique Coupee.png");
	this->musiqueCoupee.setTexture(this->musiqueCoupeeT);
	this->musiqueCoupee.setOrigin(this->musiqueCoupee.getGlobalBounds().width / 2.f, this->musiqueCoupee.getGlobalBounds().height / 2.f);
	this->musiqueCoupee.setPosition(this->rectMusique.getPosition());
	this->musiqueCoupee.setScale(scale, scale);

	this->vibrationsT.loadFromFile(assetsPath + "/Images/Icones/Vibrations.png");
	this->vibrations.setTexture(this->vibrationsT);
	this->vibrations.setOrigin(this->vibrations.getGlobalBounds().width / 2.f, this->vibrations.getGlobalBounds().height / 2.f);
	this->vibrations.setPosition(this->rectVibrations.getPosition() - sf::Vector2f(1 * screenScaleX, 0));
	this->vibrations.setScale(scale * 1.1f, scale * 1.1f);

	this->vibrationsCoupeesT.loadFromFile(assetsPath + "/Images/Icones/Vibrations Coupees.png");
	this->vibrationsCoupees.setTexture(this->vibrationsCoupeesT);
	this->vibrationsCoupees.setOrigin(this->vibrationsCoupees.getGlobalBounds().width / 2.f, this->vibrationsCoupees.getGlobalBounds().height / 2.f);
	this->vibrationsCoupees.setPosition(this->rectVibrations.getPosition());
	this->vibrationsCoupees.setScale(scale, scale);

	float widthBoutons = 128.f;																																// Les images des boutons sont carrées
	float scaleBoutons = (25.f * screenScaleX) / widthBoutons;																								// et on veut qu'elles fassent 28px de côté

	this->boutonCarreT.loadFromFile(assetsPath + "/Images/Boutons/Carre.png");
	this->toucheS.loadFromFile(assetsPath + "/Images/Boutons/S.png");
	this->boutonCarre.setOrigin(widthBoutons / 2.f, widthBoutons / 2.f);
	this->boutonCarre.setPosition(this->rectSon.getGlobalBounds().left + this->rectSon.getGlobalBounds().width, this->rectSon.getGlobalBounds().top + this->rectSon.getGlobalBounds().height);
	this->boutonCarre.setScale(scaleBoutons, scaleBoutons);

	this->boutonTriangleT.loadFromFile(assetsPath + "/Images/Boutons/Triangle.png");
	this->toucheD.loadFromFile(assetsPath + "/Images/Boutons/D.png");
	this->boutonTriangle.setOrigin(widthBoutons / 2.f, widthBoutons / 2.f);
	this->boutonTriangle.setPosition(this->rectMusique.getGlobalBounds().left + this->rectMusique.getGlobalBounds().width, this->rectMusique.getGlobalBounds().top + this->rectMusique.getGlobalBounds().height);
	this->boutonTriangle.setScale(scaleBoutons, scaleBoutons);

	this->boutonR3T.loadFromFile(assetsPath + "/Images/Boutons/R3.png");
	this->boutonR3.setTexture(this->boutonR3T);
	this->boutonR3.setOrigin(widthBoutons / 2.f, widthBoutons / 2.f);
	this->boutonR3.setPosition(this->rectVibrations.getGlobalBounds().left + this->rectVibrations.getGlobalBounds().width, this->rectVibrations.getGlobalBounds().top + this->rectVibrations.getGlobalBounds().height);
	this->boutonR3.setScale(scaleBoutons, scaleBoutons);

	// Reglage volume
	this->rectVolumeOn = sf::RectangleShape(sf::Vector2f(0.f, 6.75f * screenScaleY));
	this->rectVolumeOn.setOrigin(0.f, this->rectVolumeOn.getSize().y / 2.f);
	this->initPosrectVolumeOn = sf::Vector2f(this->rectSon.getPosition() + sf::Vector2f(-this->rectSon.getSize().x / 2.f, this->rectSon.getSize().y / 2.f + 20.f * screenScaleY));
	this->rectVolumeOn.setPosition(this->initPosrectVolumeOn);
	this->rectVolumeOn.setFillColor(sf::Color::Cyan);

	this->rectVolumeOff = sf::RectangleShape(sf::Vector2f(this->rectSon.getSize().x, 6.75f * screenScaleY));
	this->rectVolumeOff.setOrigin(0.f, this->rectVolumeOff.getSize().y / 2.f);
	this->rectVolumeOff.setPosition(this->rectVolumeOn.getPosition() + sf::Vector2f(this->rectSon.getSize().x, 0.f));
	this->rectVolumeOff.setRotation(180.f);
	this->rectVolumeOff.setFillColor(sf::Color::White);

	this->volumeCursor = sf::CircleShape(10.f * screenScaleX);
	this->volumeCursor.setOrigin(this->volumeCursor.getRadius(), this->volumeCursor.getRadius());
	this->volumeCursor.setPosition(this->rectVolumeOn.getPosition());
	this->volumeCursor.setFillColor(sf::Color(125, 125, 125));
}

void S_MenuPause::init()
{
	this->backgroundT.create(screenSize.x, screenSize.y);
	this->backgroundT.update(*this->window);
	this->background.setTexture(this->backgroundT);
	this->textSelec = 1;
	this->oldTextSelec = this->textSelec;
	this->volumeSelec = ChangeVolume::None;
	this->mouseOnText = false;

	// Les images des boutons dépendent de l'état de connexion de la manette
	this->boutonCarre.setTexture(sf::Joystick::isConnected(0) ? this->boutonCarreT : this->toucheS);
	this->boutonTriangle.setTexture(sf::Joystick::isConnected(0) ? this->boutonTriangleT : this->toucheD);
}

void S_MenuPause::update(float frameTime)																																											// Frame time non utilisé
{
	sf::Event eventMoveCursor = sf::Event();

	while (this->window->pollEvent(this->e) || wl::Joystick::pollEvent(this->e))
	{
		if (this->e.type == sf::Event::Closed)
			this->window->close();

		if (this->e.type == sf::Event::JoystickMoved || this->e.type == sf::Event::KeyPressed || this->e.type == sf::Event::MouseMoved)
		{
			this->chooseOption();
			eventMoveCursor = this->e;
		}

		if (this->e.type == sf::Event::JoystickButtonPressed || this->e.type == sf::Event::KeyPressed || this->e.type == sf::Event::MouseButtonPressed)
		{
			if (this->e.joystickButton.button == DualShock::Croix || this->e.key.code == sf::Keyboard::Enter || (this->e.mouseButton.button == sf::Mouse::Left && this->mouseOnText))
				this->selectOption();

			if (this->e.joystickButton.button == DualShock::Options || this->e.joystickButton.button == DualShock::Rond
				|| this->e.key.code == sf::Keyboard::Escape)
			{
				State::nextState = NextState::BackToGame;
				State::startTimeGame += (time(NULL) - State::startTimePause);
			}

			if (this->e.joystickButton.button == DualShock::R3) {
				if (wl::Joystick::isRumbleActivated())
					wl::Joystick::deactivateRumble();
				else
					wl::Joystick::activateRumble();
				this->changeUserSettings(false);
			}

			this->changeVolumeSelec();
		}

		// Affichage en fonction de l'état de connexion de la Manette
		if (this->e.type == sf::Event::JoystickConnected)
		{
			this->boutonCarre.setTexture(this->boutonCarreT);
			this->boutonTriangle.setTexture(this->boutonTriangleT);
		}
		else if (this->e.type == sf::Event::JoystickDisconnected)
		{
			this->boutonCarre.setTexture(this->toucheS);
			this->boutonTriangle.setTexture(this->toucheD);
		}
	}

	// Gestion Volume
	this->moveVolumeCursor(eventMoveCursor);

	// Clignotement du texte sélectionné
	if (this->blinkTextClk.getElapsedTime().asMilliseconds() > 400)
	{
		this->textes[this->textSelec].setFillColor(this->textes[this->textSelec].getFillColor() == sf::Color::Red ? sf::Color::White : sf::Color::Red);
		this->blinkTextClk.restart();
	}
}

void S_MenuPause::show()
{
	this->window->draw(this->background);
	this->window->draw(this->rectMenu);

	// Textes
	for (sf::Text& text : this->textes)
		this->window->draw(text);

	// Rectangles
	this->window->draw(this->rectSon);
	this->window->draw(this->rectMusique);
	if (sf::Joystick::isConnected(0))
		this->window->draw(this->rectVibrations);

	// Images
	this->window->draw(State::volumeSon ? this->son : this->sonCoupe);
	this->window->draw(this->boutonCarre);
	this->window->draw(State::volumeMusic ? this->musique : this->musiqueCoupee);
	this->window->draw(this->boutonTriangle);
	if (sf::Joystick::isConnected(0)) {
		this->window->draw(wl::Joystick::isRumbleActivated() ? this->vibrations : this->vibrationsCoupees);
		this->window->draw(this->boutonR3);
	}

	// Niveau volume
	if (this->volumeSelec != ChangeVolume::None)
	{
		this->window->draw(this->rectVolumeOn);
		this->window->draw(this->rectVolumeOff);
		this->window->draw(this->volumeCursor);
	}
}

void S_MenuPause::release()
{
	this->textes[this->textSelec].setFillColor(sf::Color::Red);
}

void S_MenuPause::chooseOption()
{
	if (this->e.joystickMove.axis == sf::Joystick::Axis::Y && this->e.joystickMove.position == 100
		|| this->e.joystickMove.axis == sf::Joystick::Axis::PovY && this->e.joystickMove.position == -100
		|| this->e.key.code == sf::Keyboard::Down)
		this->textSelec = (this->textSelec != 4) ? this->textSelec + 1 : 1;
	else if (this->e.joystickMove.axis == sf::Joystick::Axis::Y && this->e.joystickMove.position == -100
		|| this->e.joystickMove.axis == sf::Joystick::Axis::PovY && this->e.joystickMove.position == 100
		|| this->e.key.code == sf::Keyboard::Up)
		this->textSelec = (this->textSelec != 1) ? this->textSelec - 1 : 4;

	if (this->textSelec != this->oldTextSelec)
	{
		this->textes[oldTextSelec].setFillColor(sf::Color::Red);
		this->textes[this->textSelec].setFillColor(sf::Color::White);
		this->blinkTextClk.restart();

		this->oldTextSelec = this->textSelec;
	}
	
	// Choose Option with Mouse
	if (this->e.type == sf::Event::MouseMoved)
	{
		for (unsigned i = 1; i < 5; i++)
		{
			sf::Vector2f posMouse(this->e.mouseMove.x, this->e.mouseMove.y);
			sf::Vector2f pos = this->textes[i].getPosition();
			float demiWidth = this->textes[i].getGlobalBounds().width / 2.f;
			float demiHeight = this->textes[i].getGlobalBounds().height / 2.f;

			if (posMouse.x <= pos.x + demiWidth && posMouse.x >= pos.x - demiWidth && posMouse.y <= pos.y + 2.f * demiHeight && posMouse.y >= pos.y - demiHeight)														// La hitbox des textes recouvre que la moitié supérieures de ceux-ci, c'est pour cela qu'on a un "2.f * demiHeight"
			{
				this->textSelec = i;
				this->mouseOnText = true;
				return;
			}
		}

		this->mouseOnText = false;
	}
}

void S_MenuPause::selectOption()
{
	switch (this->textSelec)
	{
	case 1:
		State::nextState = NextState::BackToGame;
		State::startTimeGame += (time(NULL) - State::startTimePause);
		break;

	case 2:
		State::sameState = true;
		State::nextState = NextState::InGame;
		State::music->play();
		break;

	case 3:
		State::sameState = false;
		State::nextState = NextState::MenuDemarrage;
		break;

	case 4:
		this->window->close();
		break;
	}
}

void S_MenuPause::changeVolumeSelec()
{
	if ((this->e.type == sf::Event::JoystickButtonPressed && this->e.joystickButton.button == DualShock::Carre) || (this->e.type == sf::Event::KeyPressed && !sf::Joystick::isConnected(0) && this->e.key.code == sf::Keyboard::S) || (this->e.type == sf::Event::KeyPressed && sf::Joystick::isConnected(0) && this->e.key.code == sf::Keyboard::C)
		|| this->e.type == sf::Event::MouseButtonPressed && this->e.mouseButton.button == sf::Mouse::Left && this->rectSon.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition()))
	{
		this->volumeSelec = (this->volumeSelec == ChangeVolume::Sound) ? ChangeVolume::None : ChangeVolume::Sound;
		this->rectVolumeOn.setPosition(this->initPosrectVolumeOn);
		this->rectVolumeOn.setSize(sf::Vector2f(this->volumeSon * this->rectSon.getSize().x / 100.f, this->rectVolumeOn.getSize().y));
		this->rectVolumeOff.setPosition(this->rectVolumeOn.getPosition() + sf::Vector2f(this->rectSon.getSize().x, 0.f));
		this->rectVolumeOff.setSize(sf::Vector2f((100 - this->volumeSon) * this->rectSon.getSize().x / 100.f, this->rectVolumeOff.getSize().y));
		this->volumeCursor.setPosition(this->rectVolumeOn.getPosition() + sf::Vector2f(this->volumeSon * this->rectSon.getSize().x / 100.f, 0.f));
	}
	else if ((this->e.type == sf::Event::JoystickButtonPressed && this->e.joystickButton.button == DualShock::Triangle) || (this->e.type == sf::Event::KeyPressed && !sf::Joystick::isConnected(0) && this->e.key.code == sf::Keyboard::D) || (this->e.type == sf::Event::KeyPressed && sf::Joystick::isConnected(0) && this->e.key.code == sf::Keyboard::T)
		|| this->e.type == sf::Event::MouseButtonPressed && this->e.mouseButton.button == sf::Mouse::Left && this->rectMusique.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition()))
	{
		this->volumeSelec = (this->volumeSelec == ChangeVolume::Music) ? ChangeVolume::None : ChangeVolume::Music;
		this->rectVolumeOn.setPosition(this->initPosrectVolumeOn + (this->rectMusique.getPosition() - this->rectSon.getPosition()));
		this->rectVolumeOn.setSize(sf::Vector2f(this->volumeMusic * this->rectMusique.getSize().x / 100.f, this->rectVolumeOn.getSize().y));
		this->rectVolumeOff.setPosition(this->rectVolumeOn.getPosition() + sf::Vector2f(this->rectMusique.getSize().x, 0.f));
		this->rectVolumeOff.setSize(sf::Vector2f((100 - this->volumeMusic) * this->rectMusique.getSize().x / 100.f, this->rectVolumeOff.getSize().y));
		this->volumeCursor.setPosition(this->rectVolumeOn.getPosition() + sf::Vector2f(this->volumeMusic * this->rectMusique.getSize().x / 100.f, 0.f));
	}
}

void S_MenuPause::moveVolumeCursor(sf::Event& e)
{
	static sf::Clock clk;                                                                                                                                                                                               // 'clk' temporise l'animation de défilement créée lorsque l'utilisateur dirige le joystick gauche pendant un certain temps (tpsAvantDefilement)
	static float iterationsPdtDeplacement; const int tpsAvantDefilement = 300; const int tpsPdtDefilement = 15;                                                                                                         // On compte les itérations pendant le défilement pour la temporisation. 'tpsPdtDefilement' correspond au temps entre chaque action de défilement.
	
	if ((wl::Joystick::getAxisPosition(sf::Joystick::X) == 100 && clk.getElapsedTime().asMilliseconds() >= tpsAvantDefilement + iterationsPdtDeplacement * tpsPdtDefilement)                                         // Si le joystick est vers la droite et que la temporisation l'autorise
		|| (e.joystickMove.axis == sf::Joystick::PovX && e.joystickMove.position == 100) || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Right))                                                     // ou si on a appuyà sur la flèche droite de la manette ou du clavier
	{
		if (this->volumeSelec == ChangeVolume::Sound && this->volumeSon < 100 || this->volumeSelec == ChangeVolume::Music && this->volumeMusic < 100)
		{
			this->volumeSelec == ChangeVolume::Sound ? this->volumeSon++ : this->volumeMusic++;
			this->changeUserSettings(true);
			this->rectVolumeOn.setSize(this->rectVolumeOn.getSize() + sf::Vector2f(this->rectSon.getSize().x / 100.f, 0.f));
			this->rectVolumeOff.setSize(this->rectVolumeOff.getSize() - sf::Vector2f(this->rectSon.getSize().x / 100.f, 0.f));
			this->volumeCursor.move(this->rectSon.getSize().x / 100.f, 0.f);
		}

		iterationsPdtDeplacement = (iterationsPdtDeplacement == -(float)tpsAvantDefilement / tpsPdtDefilement) ? 0 : (int)iterationsPdtDeplacement + 1;																	// On set 'iterationsPdtDeplacement' à 0 si on est au début du défilement sinon on l'incrémente
	}
	if ((wl::Joystick::getAxisPosition(sf::Joystick::X) == -100 && clk.getElapsedTime().asMilliseconds() >= tpsAvantDefilement + iterationsPdtDeplacement * tpsPdtDefilement)                                        // Si le joystick est vers la gauche et que la temporisation l'autorise
		|| (e.joystickMove.axis == sf::Joystick::PovX && e.joystickMove.position == -100) || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Left))                                                     // ou si on a appuyà sur la flèche gauche de la manette ou du clavier
	{
		if (this->volumeSelec == ChangeVolume::Sound && this->volumeSon > 0 || this->volumeSelec == ChangeVolume::Music && this->volumeMusic > 0)
		{
			this->volumeSelec == ChangeVolume::Sound ? this->volumeSon-- : this->volumeMusic--;
			this->changeUserSettings(true);
			this->rectVolumeOn.setSize(this->rectVolumeOn.getSize() - sf::Vector2f(this->rectSon.getSize().x / 100.f, 0.f));
			this->rectVolumeOff.setSize(this->rectVolumeOff.getSize() + sf::Vector2f(this->rectSon.getSize().x / 100.f, 0.f));
			this->volumeCursor.move(-this->rectSon.getSize().x / 100.f, 0.f);
		}

		iterationsPdtDeplacement = (iterationsPdtDeplacement == -(float)tpsAvantDefilement / tpsPdtDefilement) ? 0 : (int)iterationsPdtDeplacement + 1;																	// On set 'iterationsPdtDeplacement' à 0 si on est au début du défilement sinon on l'incrémente
	}

	if (wl::Joystick::getAxisPosition(sf::Joystick::X) != 100 && wl::Joystick::getAxisPosition(sf::Joystick::X) != -100                                                                                           // Si on n'utilise pas le joystick gauche,
		&& wl::Joystick::getAxisPosition(sf::Joystick::Y) != 100 && wl::Joystick::getAxisPosition(sf::Joystick::Y) != -100)
	{
		clk.restart();                                                                                                                                                                                                  // on restart 'clk' chargée de la temporisation du défilement
		iterationsPdtDeplacement = -(float)tpsAvantDefilement / tpsPdtDefilement;                                                                                                                                       // et on set 'iterationsPdtDeplacement' de manière à ce que ça annule 'tpsAvantDefilement' (--> 0 attente dès qu'on bouge le joystick gauche)
	}

	// Changement du curseur du volume avec la souris
	static bool leftClicking;

	if (this->volumeSelec != ChangeVolume::None)
	{
		sf::Vector2f posMouse(sf::Mouse::getPosition());

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2f vectDiff = posMouse - this->volumeCursor.getPosition();

			if (this->rectVolumeOn.getGlobalBounds().contains(posMouse) || this->rectVolumeOff.getGlobalBounds().contains(posMouse)
				|| sqrt(pow(vectDiff.x, 2) + pow(vectDiff.y, 2)) <= this->volumeCursor.getRadius())
				leftClicking = true;
		}
		else
			leftClicking = false;

		if (leftClicking)
		{
			if (posMouse.x < this->rectVolumeOn.getPosition().x) posMouse.x = this->rectVolumeOn.getPosition().x;
			if (posMouse.x > this->rectVolumeOff.getPosition().x) posMouse.x = this->rectVolumeOff.getPosition().x;

			this->volumeCursor.setPosition(posMouse.x, this->volumeCursor.getPosition().y);
			(this->volumeSelec == ChangeVolume::Sound ? this->volumeSon : this->volumeMusic) = (posMouse.x - this->rectVolumeOn.getPosition().x) * 100.f / this->rectSon.getSize().x;
			this->rectVolumeOn.setSize(sf::Vector2f(posMouse.x - this->rectVolumeOn.getPosition().x, this->rectVolumeOn.getSize().y));
			this->rectVolumeOff.setSize(sf::Vector2f(this->rectVolumeOff.getPosition().x - posMouse.x, this->rectVolumeOff.getSize().y));
		}
	}
}

// Changes the user settings in the file assetsPath + "/Game Data/Options.txt"
void S_MenuPause::changeUserSettings(bool volume)
{
	ifstream iOptions(assetsPath + "/Game Data/Options.txt");
	if (!iOptions) {
		cout << "Error: Could not open file + \"" + assetsPath + "/Game Data/Options.txt\"" << endl;
		return;
	}

	int newStrI = -1;
	vector<string> content;
	string line;
	for (int i = 0; !iOptions.eof(); i++) {
		getline(iOptions, line);
		if ((volume && (this->volumeSelec == ChangeVolume::Sound && line.find("Son") != string::npos
			|| this->volumeSelec == ChangeVolume::Music && line.find("Musique") != string::npos))
			|| (!volume && line.find("Vibrations") != string::npos))
			newStrI = i;
		content.push_back(line);
	}
	if (newStrI == -1)
		return;
	iOptions.close();

	ofstream oOptions(assetsPath + "/Game Data/Options.txt");
	for (auto it = content.begin(); it != content.end(); it++) {
		int i = it - content.begin();
		if (i == newStrI) {
			if (volume)
				oOptions << (this->volumeSelec == ChangeVolume::Sound ? "Son : " + to_string(State::volumeSon)
					: "Musique : " + to_string(State::volumeMusic)) << endl;
			else
				oOptions << "Vibrations : " << (wl::Joystick::isRumbleActivated() ? "on" : "off") << endl;
		}
		else
			oOptions << *it << endl;
	}
	oOptions.close();
}