// This document mainly defines the setting class

#include <fstream>

#define Filename "config\\pacman.config"		// Profile Path

bool isFileExists_ifstream(std::string name)		// Judge whether there is a file
{
	std::ifstream f(name.c_str());
	return f.good();
}

void get_setting()			// Get Settings
{
	Set_language();			// Set Default Language
	if (isFileExists_ifstream(Filename))
	{
		string s1, s2;
		ifstream fin(Filename);
		while (fin >> s1)
		 {
			fin >> s2;
			fin >> s2;
			if (s1 == "Cheat" && s2 == "true")		
			{
				st_cheat();				// Start cheating
			}
			if (s1 == "Show_Console" && s2 == "true")
			{
				Show_Console();			// Keep Command Line Window
			}
			if (s1 == "Language")
			{
				Set_language(s2);			// Set Language
			}
			if (s1 == "Path_DeBug" && s2 == "true")
			{
				Show_Path();					// Set Path Show in DeBug mode
			}
		 }
	}
}