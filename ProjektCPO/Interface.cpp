#include "Interface.h"
Interface::Interface(string fileName) {
	Mat img = imread(fileName);
	Image imageTmp(img);
	this->image = imageTmp;
	this->imageCreated = true;
	system("cls");
	menu();
}

void Interface::menu() {
	while (true) {
		
			int userValue;

			cout << "1. Stworz nowy obiekt Image" << endl;
			
			cout << "22. (PROJEKT 2) Policz srednia z kart o numerach parzystych" << endl;
			if (this->imageCreated&&!image.isOneChannel) {
				cout << "2. Do skali szarosci (w przypadku gdy obraz jest w skali szarosci funkcja spowoduje przetworzenie macierzy z 3 do 1 kanalowej" << endl;
				
			}

				if (image.isGrayScale) {
					
					cout << "3. Binaryzacja podstawowa" << endl;
					cout << "4. Binaryzacja z progiem obliczonym na podstawie bimodalnego histogramu" << endl;
					cout << "7. adaptive binary" << endl;
			
			}
				if (image.isBinary) {
					cout << "5. Operacja otwarcia" << endl;
					cout << "6. Operacja zamkniecia" << endl;
					
				}
			cout << "8. Wyswietl obraz" << endl;
			cout << "9. Zakoncz" << endl;
			cin >> userValue;
			if (userValue == 9)
				break;
			system("cls");
			menuOptions(userValue);
			system("cls");
			
	
}
}
void Interface::menuOptions(char pick) {
	switch (pick) {
	
	case 22: {
		
		image.medianBlur(7);
		image.sharpening();
		image.isolateCards();
		cout << "Srednia wynosi: " << image.calculateAverageValueFromEvenCards() << endl<<endl;
		cout<<"Wroc do menu (y/n)"<<endl;
		String userValue;
		
		while (true) {
			cin >> userValue;
			if (userValue._Equal("y"))
				break;
		}
		break;
	}
	case 1: {
		string fileName;
		cout << "Podaj nazwe pliku: ";
		cin >> fileName;
		cout << endl;
		this->image = createImage(fileName);
		break;
	}
	case 2: {
		image.grayScale();
		break;
	}
	case 3: {
		
		image.binary( getThreshValueFromUser());
		break;
	}
	case 4: {
		
		image.binaryBiModal( getThreshValueFromUser(), getOptionForBiModal());
		break;
	}
	case 5: {
		cout << "1. wersja 1: {powtarzanie- erozja,dylatacja,erozja,dylatacja}" << endl;
		cout << "2. wersja 2: {powtarzanie- erozja,erozja,dylatacja,dylatacja}" << endl;
		int version;
		cin >> version;
		cout << "Podaj ile razy ma powtorzyc sie operacja otwarcia" << endl;
		
		int thresh;
			cin >> thresh;	
			if (version == 1)
				image.opening(thresh);
			else
				image.openingVer2(thresh);
		break;
	}
	case 6: {
		cout << "1. wersja 1: {powtarzanie- erozja,dylatacja,erozja,dylatacja}" << endl;
		cout << "2. wersja 2: {powtarzanie- erozja,erozja,dylatacja,dylatacja}" << endl;
		int version;
		cin >> version;
		cout << "Podaj ile razy ma powtorzyc sie operacja zamkniecia" << endl;
		int thresh;
		cin >> thresh;
		if (version == 1)
			image.closing(thresh);
		else
			image.closingVer2(thresh);
		break;
	}
	case 7:
		int maskSize;
		cout << "podaj rozmiar maski: " << endl;
		cin >> maskSize;
		cout << "podaj wartosc c" << endl;
		int c;
		cin >> c;
		image.adaptiveBinary(maskSize,c);
		break;
	

	case 8:
		image.show();
		break;
	default:
		cout << "wybierz jeszcze raz" << endl;
	}
	
}
int Interface::getThreshValueFromUser() {
	cout << "Podaj wartosc progu (od 0 do 255)" << endl;
	int thresh;
	while (true) {
		cin >> thresh;
		if (thresh >= 0 && thresh <= 255)
			break;
		else
			cout << "Podaj wartosc jeszcze raz " << endl;
	}
	return thresh;
}
string Interface::getObjectColorValueFromUser() {
	string userValue;
	string objectColor;
	while (true) {
		cout << "Obiekt ma by czarny czy bialy, wpisz:  'c' lub 'b'" << endl;

		cin >> userValue;
		if (userValue._Equal("c")) {
			objectColor = "black";
			break;
		}
		else if (userValue._Equal("b")) {
			objectColor = "white";
			break;
		}
		else {
			cout << "Podaj jeszcze raz" << endl;
		}

	}
	return objectColor;
}
int Interface::getOptionForBiModal() {
	cout << "Globalny prog obliczany na podstawie dwumodalnego histogramu moze byc obliczany na dwa sposoby(do obliczen nalezy podac initial thresh value):  " << endl;
	cout << "1. Prog to minimum miedzy dwoma pikami" << endl;
	cout << "2. Prog jest obliczany na podstawie sredniej ilosci pikseli obiektu oraz sredniej ilosci pikseli tla " << endl;
	int userValue;
	while (true) {
		cin >> userValue;
		if (userValue == 1 || userValue == 2)
			break;
		else
			cout << "Podaj opcje jeszcze raz" << endl;
	}
	return userValue;
}
Image Interface::createImage(string fileName) {
	Mat img = imread(fileName);
	Image image(img);
	return image;
}
