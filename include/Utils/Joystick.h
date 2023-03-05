#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <SFML/Graphics.hpp>
#include <JoyShockLibrary.h>

#include <iostream>
#include <type_traits>
#include <io.h>
#include <fcntl.h>
#include <fstream>
#include <thread>
#include <Windows.h>

constexpr float axisDeadZone = 0.1f;

using namespace std;

// Partie gestion boutons manette PS4
enum class DualShock { Carre, Croix, Rond, Triangle, L1, R1, L2, R2, Share, Options, L3, R3, PS, PaveTactile };

class State;
namespace wl
{
	class Joystick
	{
	public:
		static bool pollEvent(sf::Event& e);

		static bool isButtonPressed(const DualShock& sfmlButton);
		static float getAxisPosition(const sf::Joystick::Axis& axis);

		static void rumble(float rumblePercentage);
		static void activateRumble();
		static void deactivateRumble();
		static bool isRumbleActivated();

		static int updateJsl();

	private:
		friend class State;
		static int jslJoystickId;
		static bool initVibrations();

		// Utils
		static int sfmlToJsl(DualShock sfmlButton);
		static DualShock jslToSfml(int JslButton);
		static sf::Joystick::Axis getMovedAxis(const JOY_SHOCK_STATE& state, const JOY_SHOCK_STATE& oldState);

		static bool isBluetooth;
		static bool rumbleOn;
	};
}

bool operator==(const unsigned& bouton1, const DualShock& bouton2);
unsigned operator-(const DualShock& bouton1, const DualShock& bouton2);
void emptyEvent(sf::Event& e);
bool operator!=(const JOY_SHOCK_STATE& s1, const JOY_SHOCK_STATE& s2);

#endif // JOYSTICK_H