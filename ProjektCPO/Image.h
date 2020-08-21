#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>
#include <list>
#include <vector>
#include <cstddef>
#include <opencv2/world.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;
class Image
{
private:
	Mat myImg;
	int histogramTab[255];
	vector<Mat> cards;		//P2
	
public:

	bool isGrayScale;
	bool isBinary;
	bool isOneChannel;
	bool isThreeChannel;

public:
	Image(const cv::Mat myImg);
	Image();
	void grayScale();
	void show();
	void binary( double tresh);
	void opening(int parameter);
	void openingVer2(int parameter);
	void closing(int parameter);
	void closingVer2(int parameter);
	void histogram();
	void binaryBiModal(int parameter, int option);
	void adaptiveBinary(int maskSize,int c);
	void medianBlur(int maskSize);				//P2
	void sharpening();							//P2
	void isolateCards();						//P2
	void showCards();							//P2
	double calculateAverageValueFromEvenCards();//P2

private:
	void transformCards(Mat img, vector<Point> contour);				 //P2
	vector<Point2f > correctCornersCoordinates(vector<Point2f > corners);//P2
	vector<vector<Point>> findCardContours(Mat img, int threshType);	 //P2
	Mat dilatation(Mat& img);
	Mat erosion(Mat& img);
	void copy(Mat img, Mat& out);
	void replaceMat(const cv::Mat& input);
	Mat merge(const cv::Mat& picture1, const cv::Mat& picture2);
	void neighbourCheck(int i, int j, const cv::Mat& img, cv::Mat& tmp, int value);
	void findPeak(int tab[], int from, int to, int &peakIndex,int &peakValue);
	void findMin(int tab[], int from, int to,int &minIndex, int &minValue);
	int averageThresh(int tab[], int from, int to);

};

