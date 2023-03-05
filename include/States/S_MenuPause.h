#ifndef MENUPAUSE_H
#define MENUPAUSE_H

#include "State.h"

#include <SFML/Graphics.hpp>

#include <fstream>

class S_MenuPause : public State
{
public:
	S_MenuPause();

	// Main functions
	void init();
	void update(float frameTime);
	void show();
	void release();

private:
	// Fonctions annexes
	void chooseOption();
	void selectOption();
	void changeVolumeSelec();
	void moveVolumeCursor(sf::Event& e);
	void changeUserSettings(bool volume);

	sf::Texture backgroundT;
	sf::Sprite background;
	sf::RectangleShape rectMenu;

	// Gestion Options du Menu Pause
	sf::Font retroFont;
	sf::Text textes[5];												// Les textes du menu de haut en bas
	int textSelec;
	int oldTextSelec;
	sf::Clock blinkTextClk;
	bool mouseOnText;

	// Images
	sf::Texture sonT, sonCoupeT, musiqueT, musiqueCoupeeT, vibrationsT, vibrationsCoupeesT,
		boutonCarreT, boutonTriangleT, boutonR3T, toucheS, toucheD;
	sf::Sprite son, sonCoupe, musique, musiqueCoupee, vibrations, vibrationsCoupees,
		boutonCarre, boutonTriangle, boutonR3;

	// Rectangles d'icones
	sf::RectangleShape rectSon, rectMusique, rectVibrations;

	// Changement de volume
	sf::RectangleShape rectVolume, rectVolumeOn, rectVolumeOff;
	sf::CircleShape volumeCursor;
	enum class ChangeVolume {None, Sound, Music} volumeSelec;
	sf::Vector2f initPosrectVolumeOn;
};

#endif // MENUPAUSE_H