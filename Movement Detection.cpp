#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include <chrono>
#include <thread>

using namespace cv;
using namespace std;

RNG rng(12345);
Scalar colour = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));

Mat detectDifference(Mat ImageFrame1, Mat ImageFrame2)
{
	Mat diff;
	Mat dilatedImage;

	// resize size of images for visual clarity
	//resize(ImageFrame1, ImageFrame1, Size(), 0.5, 0.5);
	//resize(ImageFrame2, ImageFrame2, Size(), 0.5, 0.5);

	// converting to grayscale
	cvtColor(ImageFrame1, ImageFrame1, COLOR_BGR2GRAY);
	cvtColor(ImageFrame2, ImageFrame2, COLOR_BGR2GRAY);

	// applying a blur filter to the image to merge neighboring pixels together. This should allow the bounding boxes to pick up larger areas of pixels.
	GaussianBlur(ImageFrame1, ImageFrame1, Size(7, 7), 0, 0);
	GaussianBlur(ImageFrame2, ImageFrame2, Size(7, 7), 0, 0);
	Mat kernel = getStructuringElement(MORPH_ERODE, Size(100, 100));
	
	//erode(ImageFrame1, ImageFrame1, kernel);
	//dilate(ImageFrame1, ImageFrame2, kernel);


	// getting the difference between the two images
	absdiff(ImageFrame1, ImageFrame2, diff);

	return diff;
}

void detectContours(Mat& image)
{
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat threshold_output;
	int largestContourArea = 0;
	int largestContour = 0;
	threshold(image, threshold_output, 25, 255, THRESH_BINARY);
	Canny(threshold_output, threshold_output, 100, 200);
	findContours(threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());

	int leftmostBox_x = 0;
	int leftmostBox_y = 0;
	int rightmostBox_x = 0;
	int rightmostBox_y = 0;

	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 50, true); // can change epsilon accuracy to change polygonal precision
		boundRect[i] = boundingRect(Mat(contours_poly[i]));

		if (i == 0) { leftmostBox_x = 0; }
		else if (boundRect[i].tl().x < boundRect[i - 1].tl().x) leftmostBox_x = i;

		if (i == 0) { leftmostBox_y = 0; }
		else if (boundRect[i].tl().y < boundRect[i - 1].tl().y) leftmostBox_y = i;

		if (i == 0) { rightmostBox_x = 0; }
		else if (boundRect[i].br().x > boundRect[i - 1].br().x) rightmostBox_x = i;

		if (i == 0) { rightmostBox_y = 0; }
		else if (boundRect[i].br().y > boundRect[i - 1].br().y) rightmostBox_y = i;
	}

	for (size_t i = 0; i < contours.size(); i++)
	{
		if (i == 0) { largestContourArea = boundRect[0].area(); }
		else if (boundRect[i].area() > largestContourArea) {largestContourArea = boundRect[i].area(); largestContour = i;}
	}

	for (size_t i = 0; i < contours.size(); i++)
	{
		drawContours(image, contours_poly, (int)i, 255, 1, 8, vector<Vec4i>(), 0, Point());
		//rectangle(image, boundRect[i].tl(), boundRect[i].br(), 255, 2, 8, 0);
		rectangle(image, boundRect[largestContour].tl(), boundRect[largestContour].br(), 255, 2, 8, 0);
		//rectangle(image, Point(boundRect[leftmostBox_x].tl().x, boundRect[leftmostBox_y].tl().y), Point(boundRect[rightmostBox_x].br().x, boundRect[rightmostBox_y].br().y), colour, 2, 8, 0);
	}
	
	//rectangle(image, boundRect[largestContour].tl(), boundRect[largestContour].br(), 255, 2, 8, 0);
	waitKey(1);
}

int main()
{
	Mat leftDiff, leftDilatedImage;
	Mat rightDiff, rightDilatedImage;
	/*
	// this if trying to use an image
	string leftImageSource1 = "Resources/left_image_1.jpg";
	string rightImageSource1 = "Resources/right_image_1.jpg";

	string leftImageSource2 = "Resources/left_image_2.jpg";
	string rightImageSource2 = "Resources/right_image_2.jpg";

	Mat leftImage1 = imread(leftImageSource1);
	Mat rightImage1 = imread(rightImageSource1);

	Mat leftImage2 = imread(leftImageSource2);
	Mat rightImage2 = imread(rightImageSource2);

	leftDiff = detectDifference(leftImage1, leftImage2);
	rightDiff = detectDifference(rightImage1, rightImage2);

	//leftDilatedImage = applyDilation(leftDiff);
	//rightDilatedImage = applyDilation(rightDiff);

	detectContours(leftDiff);
	detectContours(rightDiff);

	imshow("left image", leftDiff);
	imshow("right image", rightDiff);
	waitKey(0);
	*/
	// this is taking image directly from cameras

	

	VideoCapture leftImageSource(1), rightImageSource(2);

	while (true)
	{
		Mat leftImageFrame1, leftImageFrame2, rightImageFrame1, rightImageFrame2;
		leftImageSource >> leftImageFrame1;
		rightImageSource >> rightImageFrame1;
		this_thread::sleep_for(chrono::milliseconds(1));
		leftImageSource >> leftImageFrame2;
		rightImageSource >> rightImageFrame2;

		leftDiff = detectDifference(leftImageFrame1, leftImageFrame2);
		rightDiff = detectDifference(rightImageFrame1, rightImageFrame2);

		//leftDilatedImage = applyDilation(leftDiff);
		//rightDilatedImage = applyDilation(rightDiff);

		detectContours(leftDiff);
		detectContours(rightDiff);

		imshow("left dilated image", leftDiff);
		imshow("right dilated image", rightDiff);
		waitKey(1);
	}
	
}
