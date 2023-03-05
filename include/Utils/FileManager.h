#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <SFML/Graphics.hpp>

#include <windows.h>

#include <iostream>
#include <string>
#include <vector>

#include <fstream>

#include "Constantes.h"

using namespace std;

enum class LvlMode { Asteroids, Ships, AsteroidsTime, ShipsTime };
typedef struct
{
    int numLvl;
    LvlMode mode;
    float asteroidFrequency;
    vector<vector<int>> waves;
    int nbrOfAsteroidsToDestroy;
    sf::Time time;
} Level;

int	getNbrOfUnlockedElem(string filenameAndElem);
void fillLevel(Level* lvl);
int getIndexNewShip(int numLvl);
void incrementUnlockedElem(string filenameAndElem);
void cryptOrDecryptFile(string filename);
void displayFileError();

#endif // FILE_MANAGER_H