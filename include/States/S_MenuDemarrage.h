#ifndef MENUDEMARRAGE_H
#define MENUDEMARRAGE_H

#include "State.h"

class S_MenuDemarrage : public State
{
public:
	S_MenuDemarrage();

	// Main Functions
	void init();
	void update(float frameTime);
	void show();
	void release() {}

private:
	// Background decorations
	sf::Texture xwingT, lasersT, explosionT, asteroidsT;
	sf::Sprite xwing, lasers[6], explosion, asteroids[3];

	sf::Font vintageFont, retroFont;
	sf::Text Asteroids, texts[2];

	static const string controllerMsgs[2], keyboardMsgs[2];
};

#endif // MENUDEMARRAGE_H