#pragma once
#include "Image.h"

class Interface
{
public:
	Image image;
	bool imageCreated = false;

	Interface(string fileName);
private:
	void menu();
	void menuOptions(char pick);
	Image createImage(string fileName);
	int getThreshValueFromUser();
	string getObjectColorValueFromUser();
	int getOptionForBiModal();
	
};

