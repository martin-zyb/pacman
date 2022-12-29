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
			if (s1 == "fix" && s2 == "true")		
			{
				st_fix();				// Start cheating
			}
			if (s1 == "Show_Console" && s2 == "true")
			{
				Show_Console();			// Keep Command Line Window
			}
			if (s1 == "language")
			{
				Set_language(s2);			// Set Language
			}
		 }
	}
}