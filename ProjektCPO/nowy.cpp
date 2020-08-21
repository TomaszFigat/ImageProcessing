#include<opencv2/opencv.hpp>
#include<iostream>
#include "Image.h"
#include "Interface.h"
#include <opencv2/world.hpp>
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <functional>
#include<array>
using namespace std;
using namespace cv;




int main()
{
	cout << "wprowadz nazwe pliku:" << endl;
	string fileName;
	cin >> fileName;
	Interface interface(fileName);

	waitKey(0);
	return 0;
}
