#include "Image.h"

Image::Image(const cv::Mat img) {
	this->isThreeChannel = true;
	this->copy(img, myImg);
	this->isBinary = false;
	this->isGrayScale = false;
	this->isOneChannel = false;
	

}
Image::Image() {

}
void Image::show() {
	Mat tmp = this->myImg;  // ten mat jest tutaj tylko wylacznie po to by moc ogladac obraz w Image Watch
		imshow("ads",tmp);

}

void Image::histogram() {

	if(this->isGrayScale)
	for (int i = 0; i < myImg.cols; i++) {
		for (int j = 0; j < myImg.rows; j++) {
			histogramTab[myImg.at<uchar>(j	, i)]++;
		}
	}
}
void Image::binaryBiModal(int initialThresh, int option) {
	histogram();
	if(this->isGrayScale)
	switch (option) {
	case 1:{
		int peakObjectIndex = 0;
		int peakBackgroundIndex = 0;
		int minBetweenPeaksIndex = 0;
		int peakObjectValue = 0;
		int peakBackgroundValue = 0;
		int minBetweenPeaksValue = 0;

		findPeak(this->histogramTab, 0, initialThresh, peakObjectIndex, peakObjectValue);
		findPeak(this->histogramTab, initialThresh, 255, peakBackgroundIndex, peakBackgroundValue);
		findMin(this->histogramTab, peakObjectIndex, peakBackgroundIndex, minBetweenPeaksIndex, minBetweenPeaksValue);
		cout << minBetweenPeaksIndex << "  to je minbetwe" << endl;
		binary( minBetweenPeaksIndex);
		break;
	}
	case 2: {
		int globalThresh = (averageThresh(this->histogramTab, 0, initialThresh) +
			averageThresh(this->histogramTab, initialThresh, 255)) / 2;
		cout << globalThresh << "  to je minbetwe" << endl;
		binary( globalThresh);
		break;
	}
	}
}
int Image::averageThresh(int tab[], int from, int to) {
	int summedValue=0;
	int sum=0;
	for (int i = from; i < to; i++) {
		summedValue += tab[i] * i;
		sum += tab[i];
	}
	if (sum == 0) {
		summedValue = 0;
		sum = 1;
	}
	return summedValue / sum;
}
void Image::findPeak(int tab[], int from, int to, int& peakIndex, int& peakValue) {

	for (int i = from; i < to; i++)
		if (tab[i] > peakValue) {
			peakIndex = i;
			peakValue = tab[i];
		}
	
}
void Image::findMin(int tab[],int from, int to,int &minIndex, int &minValue) {
	minValue = tab[from];
	for (int i = from; i < to; i++)
		if (tab[i] < minValue&&tab[i]!=0) {
			minIndex = i;
			minValue = tab[i];
		}
	
}
void Image::copy(Mat img, Mat& out) {
	
	Mat tmp(img.rows, img.cols, isOneChannel ? CV_8UC1 : CV_8UC3);
	int numberOfChannels = isOneChannel ? 1 : 3;
	for (int i = 0; i < tmp.cols; i++) {
		for (int j = 0; j < tmp.rows; j++) {
			if (numberOfChannels == 3)
				tmp.at<cv::Vec3b>(j, i) = img.at<cv::Vec3b>(j, i);
			if (numberOfChannels == 1)
				tmp.at<uchar>(j, i) = img.at<uchar>(j, i);
		}
	}
	out = tmp;
}
void Image::grayScale() {
	if (isThreeChannel) {
		Mat tmp(myImg.rows, myImg.cols, CV_8UC1);
		for (int i = 0; i < myImg.cols; i++) {
			for (int j = 0; j < myImg.rows; j++) {

				unsigned char r, g, b, sum;

				r = (unsigned char)(myImg.data[j * myImg.step + i * myImg.elemSize() + 0]);
				g = (unsigned char)(myImg.data[j * myImg.step + i * myImg.elemSize() + 1]);
				b = (unsigned char)(myImg.data[j * myImg.step + i * myImg.elemSize() + 2]);
				sum = (r + g + b) / 3;

				
				tmp.at<uchar>(j, i) = { sum };
			}
		}
	
		this->isOneChannel = true;
		this->isThreeChannel = false;
		this->isGrayScale = true;
		myImg = tmp;
	}
	else
		cout << "Obraz nie jest 3 kanalowy, nie mozna przetworzyc do skali szarosci" << endl;

}

void Image::replaceMat(const cv::Mat& input) {
	myImg = input;
}
void Image::binary( double thresh) {
	if (isGrayScale){
	int object, background;

		object = 255;
		background = 0;
	


	for (int i = 0; i < myImg.cols; i++) {
		for (int j = 0; j < myImg.rows; j++) {
			if (myImg.at<uchar>(j, i) >= thresh)
				myImg.at<uchar>(j, i) = background;
			else
				myImg.at<uchar>(j, i) = object;
		}
	}
	this->isBinary = true;
	this->isGrayScale = false;
	}
else
cout << "Obraz nie jest w skali szarosci, nie mozna przetworzyc obrazu" << endl;
}


Mat Image::merge(const cv::Mat& picture1, const cv::Mat& picture2) {
	Mat tmp(picture1.rows, picture1.cols, CV_8UC1, Scalar(255));
	for (int i = 0; i < picture1.cols; i++) {
		for (int j = 0; j < picture1.rows; j++) {
			if (picture1.at<uchar>(j, i) == 0 || picture2.at<uchar>(j, i) == 0)
				tmp.at<uchar>(j, i) = 0;

		}
	}

	return tmp;
}
Mat Image::dilatation(Mat& img) {
	Mat tmp(img.rows, img.cols, CV_8UC1, Scalar(255));


	for (int i = 0; i < img.cols; i++) {

		for (int j = 0; j < img.rows; j++) {
			if (img.at<uchar>(j, i) == 255) {
				neighbourCheck(i, j, img, tmp, 0);
			}
		}
	}
	Mat out;
	out = merge(img, tmp);
	
	return out;
}
void Image::neighbourCheck(int i, int j, const cv::Mat& img, cv::Mat& tmp, int value) {
	bool status = false;
	for (int x = i - 1; x < i + 2; x++) {

		for (int z = j - 1; z < j + 2; z++) {
			
			if ((z >= 0 && x >= 0) && (z < img.rows && x < img.cols)) {
				
				if (img.at<uchar>(z, x) == value) {
					tmp.at<uchar>(j, i) = value;
					status = true;
				}
				if (status)
					break;
			}
		}
		if (status)
			break;
	}
}
Mat Image::erosion(Mat& img) {
	Mat tmp;
	this->copy(img, tmp);

	for (int i = 0; i < img.cols; i++) {

		for (int j = 0; j < img.rows; j++) {
			if (img.at<uchar>(j, i) == 0) {
				neighbourCheck(i, j, img, tmp,255);
			}
		}
	}
	
	return tmp;
}
void Image::opening(int parameter) {
	
	for (int i = 0; i < parameter; i++) {
		this->myImg = erosion(myImg);
		this->myImg = dilatation(myImg);
	}

	

}
void Image::closing(int parameter) {
	
	for (int i = 0; i < parameter; i++) {
		this->myImg = dilatation(myImg);
		this->myImg = erosion(myImg);
	}

}
void Image::openingVer2(int parameter) {

	for (int i = 0; i < parameter; i++) {
		this->myImg = erosion(myImg);
	}
		for (int i = 0; i < parameter; i++) {
		this->myImg = dilatation(myImg);
	}
}
void Image::closingVer2(int parameter) {
	
	for (int i = 0; i < parameter; i++) {
		this->myImg = dilatation(myImg);
	}
		for (int i = 0; i < parameter; i++) {
		this->myImg = erosion(myImg);
	}

}

void Image::adaptiveBinary(int maskSize, int c) {
	Mat tmp(myImg.rows, myImg.cols, CV_8UC1);
	Mat piekneslowo = myImg;
	for (int i = 0; i < myImg.cols; i++) {
		for (int j = 0; j < myImg.rows; j++) {
			
			std::vector < uchar > myVec;
			for (int x = i - (maskSize - 1)/2; x <= i+(maskSize - 1) / 2; x++) {
				for (int z = j - (maskSize - 1) / 2; z <= j + (maskSize - 1) / 2; z++) {
					if (x >= 0 && z >= 0 && x < myImg.cols && z < myImg.rows) {
						myVec.push_back(myImg.at<uchar>(z, x));
					}
				}
			}
			
			sort(myVec.begin(), myVec.end());
			//for (int p = 0; p < myVec.size(); p++) {
			//	cout << (int)myVec[p] << endl;

			//}
			int mediana;
			if (myVec.size() % 2 == 0) {
				mediana =(int) ((myVec[myVec.size() / 2]) + myVec[((myVec.size() / 2) - 1)])/2;
			}
			else {
				mediana=(int)(myVec[(myVec.size() - 1) / 2]);
			}
			mediana = mediana - c;
			
			if (mediana > (int)myImg.at<uchar>(j, i))
				tmp.at<uchar>(j, i) = 255;
			else
				tmp.at<uchar>(j, i) = 0;
		}

	}
	myImg = tmp;
}
// Poni¿ej funkcje dotycz¹ce projektu 2
void Image::showCards() {		// funkcja pokazuj¹ca macierze poszczególnych kart s¹ zrobione 4 macierze by móc ³atwo korzystaæ z Image Watch
	Mat card1 = this->cards[0];
	Mat card2 = this->cards[1];
	Mat card3 = this->cards[2];
	Mat card4 = this->cards[3];
}
void Image::medianBlur(int maskSize) {		// Filtr medianowy

	cv::medianBlur(this->myImg, this->myImg, maskSize);
}

void Image::sharpening() {			// wyostrzanie obrazu
	
	cv::detailEnhance(this->myImg, this->myImg);

	
	
}

void Image::isolateCards() {		// Funckja która izoluje odpowiednie kontury i przesy³a je dalej do funkcji tworzacej nowe macierze z tych konturów

	vector<vector<Point>> contours = findCardContours(this->myImg,0);

	for (int i = 0; i < contours.size(); i++) {		
		if (contours[i].size() > 500) {
			transformCards(this->myImg, contours[i]);
			
		}
	}
}
void Image::transformCards(Mat img,vector<Point> contour) {		// Funckcja na podstawie konturów tworzy now¹ macierz z pojedyncz¹ kart¹
	Mat singleCard;
	vector<Point2f> correspondingPoints;
	correspondingPoints.push_back(Point2f(0, 0));
	correspondingPoints.push_back(Point2f(0, 600));
	correspondingPoints.push_back(Point2f(300, 600));
	correspondingPoints.push_back(Point2f(300, 0));
	vector<Point2f > corners;
	approxPolyDP(contour, corners, arcLength(contour, true) * 0.02, true);
	corners = correctCornersCoordinates(corners);
	Mat transformedMatrix = cv::getPerspectiveTransform(corners, correspondingPoints);
	
	warpPerspective(img, singleCard, transformedMatrix, Size(300, 600));
	
	singleCard = cv::Mat(singleCard, cv::Rect(20, 20, 275, 575)).clone();
	this->cards.push_back(singleCard);
}
vector<Point2f > Image::correctCornersCoordinates(vector<Point2f > corners) {	// Funkkcja która ma na celu poprawne ustawienie karty (d³u¿szy bok lewo-prawo, krótszy bok góra-dó³)
	double distance1;
	double distance2;

	distance1 = sqrt(pow((corners[1].x - corners[0].x), 2) + pow((corners[1].y - corners[0].y), 2));
	distance2 = sqrt(pow((corners[1].x - corners[2].x), 2) + pow((corners[1].y - corners[2].y), 2));

	if (distance1 < distance2) {
		Point2f tmp = corners[1];
		corners[1] = corners[3];
		corners[3] = tmp;
	}
	return corners;
}
double Image::calculateAverageValueFromEvenCards() {		//Funkcja zwracaj¹ca œredni¹ wartoœæ z wszystkich kart parzystych
	vector<Mat> cards = this->cards;
	showCards();
	int numberOfCards=0;
	
	double sum=0;
	
	for (int i = 0; i < cards.size(); i++) {			//Ka¿da iteracja zwraca wartoœæ poszczególnej karty
		int numberOfSigns = 0;
		
		vector < vector < Point >> contours= findCardContours(cards[i],1);
		for (int j = 0; j < contours.size(); j++) {
			if (contours[j].size() > 35) {		//Sprawdzenie czy nie ma jakiegoœ ma³ego nic nie znacz¹cego kszta³tu (jakieœ pojedyczne piksele 
												//które mog¹ zostaæ uznane za kontur). W przypadku gdy s¹ nie wliczaj¹ sie do liczby znaków na karcie 
				numberOfSigns++;
			}
		}
		if (numberOfSigns %2==0) {			// Jeœli karta jest parzysta to dodaje sie jej wartoœæ do sumy(licznik) oraz do liczby kart(mianownik)
			numberOfCards++;
			sum =sum + (numberOfSigns - 4);
		}

	}
	return sum / numberOfCards;

}

vector<vector<Point>> Image::findCardContours(Mat img, int threshType) {	//Funkcja szuka konturów, Argument threshType==0 - ca³y obrazek

	Mat binary;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

	if (threshType == 1) {
	cv::threshold(img, binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
	dilate(binary, binary, Mat(), Point(-1, -1), 2);
	}
	else
		cv::threshold(img, binary, 130, 255, THRESH_BINARY);

	
	cv::findContours(binary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	
	//------------------------------------------------------------Kod poni¿ej wy³¹cznie do wizualizacji konturów----------------
	Mat dst = Mat::zeros(img.rows, img.cols, CV_8UC3);
	int idx = 0;
	Mat output = Mat::zeros(img.rows, img.cols, CV_8UC3);
	for (; idx >= 0; idx = hierarchy[idx][0])
	{
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		drawContours(dst, contours, idx, color, FILLED, 8, hierarchy);
	}
	//---------------------------------------------------------------------------------------------------------------------------
	return contours;
	

}

