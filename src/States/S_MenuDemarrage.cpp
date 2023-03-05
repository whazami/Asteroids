#include "../../include/States/S_MenuDemarrage.h"

const string S_MenuDemarrage::controllerMsgs[2] = { "Press any button!", "" };
const string S_MenuDemarrage::keyboardMsgs[2] = { "Press Enter!", "(Or connect a controller)" };

S_MenuDemarrage::S_MenuDemarrage()
{
	// Background decorations
	if (!this->xwingT.loadFromFile(assetsPath + "/Images/Vaisseaux/X-Wing.png"))
		cout << "Erreur du chargement de l'image de x-wing." << endl;
	this->xwing.setTexture(this->xwingT);
	this->xwing.setOrigin(this->xwing.getLocalBounds().width / 2.f, this->xwing.getLocalBounds().height / 2.f);
	this->xwing.setScale(1.5f * screenScaleX, 1.5f * screenScaleX);
	this->xwing.setPosition(250 * screenScaleX, 170 * screenScaleY);
	this->xwing.setRotation(105);

	if (!this->lasersT.loadFromFile(assetsPath + "/Images/Tirs/X-Wing.png"))
		cout << "Erreur du chargement de l'image de tir du x-wing." << endl;
	const sf::Vector2f offset(100.f * screenScaleX, 26.f * screenScaleY);
	for (int i = 0; i < 6; i++) {
		this->lasers[i].setTexture(this->lasersT);
		this->lasers[i].setOrigin(this->lasers[i].getLocalBounds().width / 2.f, this->lasers[i].getLocalBounds().height / 2.f);
		this->lasers[i].setScale(1.5f * screenScaleX, 1.5f * screenScaleX);
		float tang = tan(this->xwing.getRotation() * PI / 180.f);
		this->lasers[i].setPosition(this->xwing.getPosition() + (i % 2 ? offset.y : -offset.y) * sf::Vector2f(1, tang)
																+ (i / 2 + 1) * offset.x * sf::Vector2f(-tang, 1));
		this->lasers[i].setRotation(this->xwing.getRotation());
	}

	if (!this->explosionT.loadFromFile(assetsPath + "/Images/Animations/Explosion.png"))
		cout << "Erreur du chargement de l'image d'explosion." << endl;
	this->explosion.setTexture(this->explosionT);
	this->explosion.setTextureRect(sf::IntRect(800, 100, 100, 100));
	this->explosion.setOrigin(this->explosion.getLocalBounds().width / 2.f, this->explosion.getLocalBounds().height / 2.f);
	this->explosion.setScale(3.2f * screenScaleX, 3.2f * screenScaleX);
	this->explosion.setPosition(1750 * screenScaleX, 570 * screenScaleY);

	if (!this->asteroidsT.loadFromFile(assetsPath + "/Images/Asteroid.png"))
		cout << "Erreur du chargement de l'image d'Asteroid" << endl;
	for (int i = 0; i < 3; i++) {
		this->asteroids[i].setTexture(this->asteroidsT);
		this->asteroids[i].setOrigin(this->asteroids[i].getLocalBounds().width / 2.f, this->asteroids[i].getLocalBounds().height / 2.f);
	}
	this->asteroids[0].setScale(0.8f * screenScaleX, 0.8f * screenScaleX);
	this->asteroids[0].setPosition(200 * screenScaleX, 800 * screenScaleY);
	this->asteroids[1].setScale(0.2f * screenScaleX, 0.2f * screenScaleX);
	this->asteroids[1].setPosition(500 * screenScaleX, 1000 * screenScaleY);
	this->asteroids[2].setScale(0.5f * screenScaleX, 0.5f * screenScaleX);
	this->asteroids[2].setPosition(1200 * screenScaleX, 800 * screenScaleY);

	// Fonts & Texts
	this->retroFont.loadFromFile(assetsPath + "/Fonts/Retro Gaming.ttf");
	this->vintageFont.loadFromFile(assetsPath + "/Fonts/Vintage.ttf");

	this->Asteroids.setFont(this->vintageFont);
	this->Asteroids.setString("Asteroids");
	this->Asteroids.setPosition(screenSize.x / 2.05f, 200.f * screenScaleY);
	this->Asteroids.setOutlineThickness(3.f * screenScaleX);
	this->Asteroids.setOutlineColor(sf::Color::White);
	this->Asteroids.setFillColor(sf::Color::Red);
	this->Asteroids.setCharacterSize(150 * screenScaleX);
	this->Asteroids.setOrigin(this->Asteroids.getGlobalBounds().width / 2.f, this->Asteroids.getGlobalBounds().height / 2.f);

	for (int i = 0; i < 2; i++) {
		this->texts[i].setFont(this->retroFont);
		this->texts[i].setFillColor(sf::Color::Red);
	}
	this->texts[0].setPosition(screenSize / 2.f + sf::Vector2f(0, 10.f * screenScaleY));
	this->texts[0].setCharacterSize(40 * screenScaleX);
	this->texts[1].setPosition(this->texts[0].getPosition() + sf::Vector2f(0, this->texts[0].getCharacterSize() * screenScaleY));
	this->texts[1].setCharacterSize(30 * screenScaleX);
}

void S_MenuDemarrage::init()
{
	// Text
	for (int i = 0; i < 2; i++) {
		this->texts[i].setString(sf::Joystick::isConnected(0) ? S_MenuDemarrage::controllerMsgs[i] : S_MenuDemarrage::keyboardMsgs[i]);
		this->texts[i].setOrigin(this->texts[i].getGlobalBounds().width / 2.f, this->texts[i].getGlobalBounds().height / 2.f);
	}

	// Music
	this->setMusic(assetsPath + "/Musiques/States/Menu.ogg");
}

void S_MenuDemarrage::update(float frameTime)																							// Frame time non utilisé
{
	while (this->window->pollEvent(this->e) || wl::Joystick::pollEvent(this->e))
	{
		if (this->e.type == sf::Event::Closed)
			this->window->close();

		if ((this->e.type == sf::Event::KeyPressed && this->e.key.code == sf::Keyboard::Enter)
			|| this->e.type == sf::Event::JoystickButtonPressed)
			State::nextState = NextState::ChoixNiveau;

		if (this->e.type == sf::Event::JoystickConnected) {
			for (int i = 0; i < 2; i++) {
				this->texts[i].setString(S_MenuDemarrage::controllerMsgs[i]);
				this->texts[i].setOrigin(this->texts[i].getGlobalBounds().width / 2.f, this->texts[i].getGlobalBounds().height / 2.f);
			}
		}

		if (this->e.type == sf::Event::JoystickDisconnected) {
			for (int i = 0; i < 2; i++) {
				this->texts[i].setString(S_MenuDemarrage::keyboardMsgs[i]);
				this->texts[i].setOrigin(this->texts[i].getGlobalBounds().width / 2.f, this->texts[i].getGlobalBounds().height / 2.f);
			}
		}
	}
}

void S_MenuDemarrage::show()
{
	this->window->draw(this->background);

	// Background decorations
	for (int i = 0; i < 6; i++)
		this->window->draw(this->lasers[i]);
	this->window->draw(this->xwing);
	for (int i = 0; i < 3; i++)
		this->window->draw(this->asteroids[i]);
	this->window->draw(this->explosion);

	this->window->draw(this->Asteroids);
	for (int i = 0; i < 2; i++)
		this->window->draw(this->texts[i]);
}
