#ifndef CONSTANTES_H
#define CONSTANTES_H

#include <SFML/Graphics.hpp>

constexpr float PI = 3.1415926535897932f;

// Pour la compatibilité entre les écrans
const sf::Vector2f screenSize(sf::VideoMode::getFullscreenModes()[0].width, sf::VideoMode::getFullscreenModes()[0].height);
const float screenScaleX = screenSize.x / 1920.f;																// Les valeurs en pixels de ce projet ont été imaginées sur un écran 1920x1080
const float screenScaleY = screenSize.y / 1080.f;																// ces deux coefficients permettent donc de convertir ces dernières si besoin (dans le cas d'un écran plus petit ou plus grand)

// La vitesse de déplacement des Tirs est la même pour tous les Vaisseaux
const float vitesseTirs = 1250.f * screenScaleX;

#endif // CONSTANTES_H