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

	//resize(ImageFrame1, ImageFrame1, Size(), 0.5, 0.5);
	//resize(ImageFrame2, ImageFrame2, Size(), 0.5, 0.5);

	// converting to grayscale
	cvtColor(ImageFrame1, ImageFrame1, COLOR_BGR2GRAY);
	cvtColor(ImageFrame2, ImageFrame2, COLOR_BGR2GRAY);

	// getting the difference between the two images
	absdiff(ImageFrame1, ImageFrame2, diff);

	return diff;
}

Mat applyDilation(Mat diff)
{
	Mat dilatedImage;
	// applying a blur filter to the image to merge neighboring pixels together. This should allow the bounding boxes to pick up larger areas of pixels.
	GaussianBlur(diff, diff, Size(15, 15), 2, 2); // the blur seems to have a significant affect on the outcome of the image

	Mat kernal = getStructuringElement(MORPH_ERODE, Size(100, 100));
	dilate(diff, dilatedImage, kernal);

	return dilatedImage, diff;
}

void detectContours(Mat& image, Mat& dilatedImage)
{
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat threshold_output;
	threshold(dilatedImage, threshold_output, 50, 255, THRESH_BINARY);
	findContours(threshold_output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());

	int leftmostBox_x = 0;
	int leftmostBox_y = 0;
	int rightmostBox_x = 0;
	int rightmostBox_y = 0;

	for (int i = 0; i < contours.size(); i++)
	{
		Scalar colour = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true); // can change epsilon accuracy to change polygonal precision
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

	for (int i = 0; i < contours.size(); i++)
	{
		drawContours(image, contours_poly, (int)i, 255, 1, 8, vector<Vec4i>(), 0, Point());
		//rectangle(image, boundRect[i].tl(), boundRect[i].br(), 255, 2, 8, 0);
	}

	putText(image, "Left x", Point(boundRect[leftmostBox_x].tl()), FONT_HERSHEY_DUPLEX, 0.50, colour, 1);
	putText(image, "Left y", Point(boundRect[leftmostBox_y].tl()), FONT_HERSHEY_DUPLEX, 0.50, colour, 1);
	putText(image, "Right x", Point(boundRect[rightmostBox_x].br()), FONT_HERSHEY_DUPLEX, 0.50, colour, 1);
	putText(image, "Right y", Point(boundRect[rightmostBox_y].br()), FONT_HERSHEY_DUPLEX, 0.50, colour, 1);

	putText(image, "Lx: " + to_string(boundRect[leftmostBox_x].tl().x), Point(10, 20), FONT_HERSHEY_DUPLEX, 0.50, colour, 1);
	putText(image, "Ly: " + to_string(boundRect[leftmostBox_y].tl().y), Point(10, 40), FONT_HERSHEY_DUPLEX, 0.50, colour, 1);
	putText(image, "Rx: " + to_string(boundRect[rightmostBox_x].br().x), Point(10, 80), FONT_HERSHEY_DUPLEX, 0.50, colour, 1, true);
	putText(image, "Ry: " + to_string(boundRect[rightmostBox_y].br().y), Point(10, 100), FONT_HERSHEY_DUPLEX, 0.50, colour, 1, true);

	rectangle(image,Point(boundRect[leftmostBox_x].tl().x, boundRect[leftmostBox_y].tl().y), Point(boundRect[rightmostBox_x].br().x, boundRect[rightmostBox_y].br().y), colour, 2, 8, 0);
	circle(image, Point(boundRect[leftmostBox_x].tl().x, boundRect[leftmostBox_x].tl().y), 7, Scalar(255, 255, 255), FILLED);
	circle(image, Point(boundRect[leftmostBox_y].tl().x, boundRect[leftmostBox_y].tl().y), 7, Scalar(255, 255, 255), FILLED);
	circle(image, Point(boundRect[rightmostBox_x].br().x, boundRect[rightmostBox_x].br().y), 7, Scalar(255, 255, 255), FILLED);
	circle(image, Point(boundRect[rightmostBox_y].br().x, boundRect[rightmostBox_y].br().y), 7, Scalar(255, 255, 255), FILLED);
}

int main()
{
	Mat leftDiff, leftDilatedImage;
	Mat rightDiff, rightDilatedImage;

	// this if trying to use an image
	////////////////////////////////////////////////////////////////////////////////////
	string leftImageSource1 = "leftFrame1.jpg";
	string rightImageSource1 = "rightFrame1.jpg";

	string leftImageSource2 = "leftFrame2.jpg";
	string rightImageSource2 = "rightFrame2.jpg";

	Mat leftImage1 = imread(leftImageSource1);
	Mat rightImage1 = imread(rightImageSource1);

	Mat leftImage2 = imread(leftImageSource2);
	Mat rightImage2 = imread(rightImageSource2);

	leftDiff = detectDifference(leftImage1, leftImage2);
	rightDiff = detectDifference(rightImage1, rightImage2);

	leftDilatedImage = applyDilation(leftDiff);
	rightDilatedImage = applyDilation(rightDiff);

	detectContours(leftDiff, leftDilatedImage);
	detectContours(rightDiff, rightDilatedImage);

	imshow("Left Image", leftDiff);
	imshow("Right Image", rightDiff);
	waitKey(0);
	/////////////////////////////////////////////////////////////////////////////////////
	// this is taking image directly from cameras

	/*
	VideoCapture leftImageSource(0), rightImageSource(1);
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
		leftDilatedImage = applyDilation(leftDiff);
		rightDilatedImage = applyDilation(rightDiff);
		detectContours(leftDiff, leftDilatedImage, true);
		detectContours(rightDiff, rightDilatedImage, false);
		imshow("left dilated image", leftDiff);
		imshow("right dilated image", rightDiff);
		waitKey(1);
	}
	*/
}
