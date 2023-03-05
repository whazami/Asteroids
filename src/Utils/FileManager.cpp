#include "../../include/Utils/FileManager.h"

int	getNbrOfUnlockedElem(string filenameAndElem)
{
	// On d�crypte le fichier
	cryptOrDecryptFile(filenameAndElem);

	ifstream file(assetsPath + "/Game Data/" + filenameAndElem);
	if (!file)
	{
		cout << "Erreur lors de l'ouverture du fichier \"" + filenameAndElem + "\"." << endl;
		exit(-1);
	}
	string line;
	getline(file, line);

	if (line.find(filenameAndElem + " Unlocked: ") == string::npos)
		displayFileError();

	line.erase(0, filenameAndElem.size() + 11);

	// On recrypte le fichier
	cryptOrDecryptFile(filenameAndElem);

	return stoi(line);
}

void fillLevel(Level* lvl)
{
	// On d�crypte le fichier
	cryptOrDecryptFile("Levels");

	// On ouvre le fichier et on va jusqu'au niveau correspondant
	ifstream lvls(assetsPath + "/Game Data/Levels");
	if (!lvls)
	{
		cout << "Erreur lors de l'ouverture du fichier \"Levels\"." << endl;
		exit(-1);
	}
	string line;
	string strfind = "Level " + to_string(lvl->numLvl) + ":";

	while (!lvls.eof())
	{
		getline(lvls, line);
		if (line.find(strfind) != string::npos)
			break;
	}
	if (lvls.eof())
		displayFileError();

	// On r�cup�re le mode du niveau
	getline(lvls, line);
	LvlMode mode = LvlMode::Asteroids;
	if (line == "Asteroids")
		mode = LvlMode::Asteroids;
	else if (line == "Ships")
		mode = LvlMode::Ships;
	else if (line == "Asteroids - Time")
		mode = LvlMode::AsteroidsTime;
	else if (line == "Ships - Time")
		mode = LvlMode::ShipsTime;
	else
		displayFileError();

	lvl->mode = mode;

	// On r�cup�re la fr�quence d'apparition des Asteroids
	getline(lvls, line);
	lvl->asteroidFrequency = stof(line);

	// On r�cup�re les vagues d'ennemis s'il y a en a dans ce mode
	if (lvl->mode == LvlMode::Ships || lvl->mode == LvlMode::ShipsTime)
	{
		vector<vector<int>> waves;
		bool rebreak = false;
		for (int i = 0; !lvls.eof(); i++)
		{
			getline(lvls, line);
			if (line.empty())
				break;
			streampos oldpos = lvls.tellg();
			waves.push_back(vector<int>());
			for (char c : line)
				if (c >= '1' && c <= '4')
					waves[i].push_back(c - '0');
				else if (c == ':' || (c != ' ' && (c < 1 || c > 4)))
				{
					waves.pop_back();
					lvls.seekg(oldpos);
					rebreak = true;
					break;
				}
				else if (c != ' ')
					displayFileError();
			if (rebreak)
				break;
		}

		lvl->waves = waves;
	}
	else
		lvl->waves.clear();

	// On r�cup�re le nombre d'Asteroids � d�truire s'il y en a dans ce mode
	if (lvl->mode == LvlMode::AsteroidsTime)
	{
		getline(lvls, line);
		lvl->nbrOfAsteroidsToDestroy = stoi(line);
	}

	// On r�cup�re la limite de temps s'il y en a dans ce mode
	if (lvl->mode != LvlMode::Ships)
	{
		while (!line.empty())
		{
			if (line.find(":") != string::npos)
				break;
			getline(lvls, line);
		}
		if (line.empty())
			displayFileError();

		string linecpy = line;
		line.erase(line.begin() + line.find(":"), line.end());									// On enl�ve les seconds + le ':' (ex : "1:30" devient "1")
		linecpy.erase(linecpy.begin(), linecpy.begin() + linecpy.find(":") + 1);				// On enl�ve les minutes + le ':' (ex : "1:30" devient "30")

		sf::Time time = sf::seconds(stoi(line) * 60 + stoi(linecpy));
		lvl->time = time;
	}

	// On recrypte le fichier
	cryptOrDecryptFile("Levels");
}

int getIndexNewShip(int numLvl)
{
	// On d�crypte le fichier
	cryptOrDecryptFile("Levels");

	// On ouvre le fichier et on va jusqu'au niveau correspondant
	ifstream lvls(assetsPath + "/Game Data/Levels");
	if (!lvls)
	{
		cout << "Erreur lors de l'ouverture du fichier \"Levels\"." << endl;
		exit(-1);
	}
	string line;
	string strfind = "Level " + to_string(numLvl) + ":";
	while (!lvls.eof())
	{
		getline(lvls, line);
		if (line.find(strfind) != string::npos)
			break;
	}
	if (lvls.eof())
		displayFileError();

	line.erase(0, strfind.size());

	// On recrypte le fichier
	cryptOrDecryptFile("Levels");

	return (!line.empty()) ? stoi(line) : 0;
}

void incrementUnlockedElem(string filenameAndElem)
{
	int unlockedElem = getNbrOfUnlockedElem(filenameAndElem) + 1;

	// On d�crypte le fichier
	cryptOrDecryptFile(filenameAndElem);

	// On ouvre le fichier et on va jusqu'au niveau correspondant
	fstream file(assetsPath + "/Game Data/" + filenameAndElem, ios::in | ios::out);
	if (!file)
	{
		cout << "Erreur lors de l'ouverture du fichier \"" << filenameAndElem << "\"." << endl;
		exit(-1);
	}

	file << filenameAndElem << " Unlocked: " << unlockedElem << endl;
	file.close();

	// On recrypte le fichier
	cryptOrDecryptFile(filenameAndElem);
}

void cryptOrDecryptFile(string filename)
{
	errno = 0;
	fstream lvls;
	lvls.open(assetsPath + "/Game Data/" + filename, ios::in);
	if (!lvls)
	{
		cout << "Erreur lors de l'ouverture en entr�e du fichier " << filename << "." << endl;
		exit(-1);
	}

	string filebuf;
	while (!lvls.eof())
	{
		char c;
		lvls.get(c);
		filebuf.push_back(c);
	}
	char newline = filebuf.back();
	for (int i = filebuf.size() - 1; i > 0 && filebuf[i - 1] == newline; i--)
		filebuf.erase(i, 1);
	for (char& c : filebuf)
		c ^= 202;
	lvls.close();

	lvls.open(assetsPath + "/Game Data/" + filename, ios::out | ios::trunc);
	if (!lvls)
		cout << "Erreur lors de l'ouverture en sortie du fichier " << filename << "." << endl;

	for (char& c : filebuf)
		lvls << c;
	lvls.close();
}

void displayFileError()
{
	MessageBox(NULL, L"Votre progression est perdue et votre jeu corrompu :( Cela peut �tre d� � un antivirus \
qui emp�che l'acc�s aux fichiers du dossier \"Game Data\" ou � une modification de l'un de ces derniers.",
		L"Erreur de lecture du fichier des donn�es du jeu.",
		MB_ICONERROR | MB_OK);
	ofstream lvls(assetsPath + "/Game Data/Levels"), ships(assetsPath + "/Game Data/Ships");
	lvls << "Fichier corrompu bzzt bzzzzt";
	ships << "Fichier corrompu bzzt bzzzzt";
	exit(-1);
}