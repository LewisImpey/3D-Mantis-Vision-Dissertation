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

	// applies a threshold to the merged image, small changes that fall below this threshold will be forgotten about and not tracked
	threshold(image, threshold_output, 30, 255, THRESH_BINARY);

	/* a kernel is created and the dilate function is called, which dilates the image making all of the 
	   things still detected after the threshold larger. This can help to merge the contours together */
	Mat kernel = getStructuringElement(MORPH_ERODE, Size(20, 20));
	dilate(threshold_output, threshold_output, kernel);

	// the findContours function is called, which finds and stores all of the found contours for the image
	findContours(threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Moments> mu(contours.size());
	vector<Point2f> mc(contours.size());

	// the moments for each contour are collected using the moments function
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}

	// this is then used to get the centre of each contour found
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}

	for (size_t i = 0; i < contours.size(); i++)
	{
		// the approxPolyDP function allow for the precision and accuracy of the contours to be changed. a lower accuracy can make the contours more polygonal
		approxPolyDP(Mat(contours[i]), contours_poly[i], 1, true);
		// A bounding box is then created for each of the contours found in the image
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}

	for (size_t i = 0; i < contours.size(); i++)
	{
		// the bounding boxes made are then looped through to find out which is the largest, as we only want to show one box, that being the biggest
		if (i == 0) { largestContourArea = boundRect[0].area(); }
		else if (boundRect[i].area() > largestContourArea) {largestContourArea = boundRect[i].area(); largestContour = i;}
	}

	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar colour = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));

		drawContours(image, contours_poly, (int)i, colour, 2, 8, vector<Vec4i>(), 0, Point());

		// the largest bounding box is then drawn onto the image using the rectangle function
		rectangle(image, boundRect[largestContour].tl(), boundRect[largestContour].br(), 255, 2, 8, 0);
		// the centroid of the largest box is also drawn using the circle function
		circle(image, mc[largestContour], 5, Scalar(255,255,255), FILLED);
		putText(image, "y: " + to_string(mc[largestContour].y), Point(10, 20), FONT_HERSHEY_DUPLEX, 0.50, colour, 1);
	}
	// get the centre of the left and right boxes
	// if they have the same y coordinate
	// do the mathematical equation to find disparity 
}

//int calculateDepth(int leftCoordinate, int rightCoordinate)
//{
//
//}
int main()
{
	Mat leftDiff;
	Mat rightDiff;
	/*
	// this if trying to use an image
	string leftImageSource1 = "leftFrame2.jpg";
	string rightImageSource1 = "rightFrame2.jpg";

	string leftImageSource2 = "leftFrame3.jpg";
	string rightImageSource2 = "rightFrame3.jpg";

	Mat leftImage1 = imread(leftImageSource1);
	Mat rightImage1 = imread(rightImageSource1);

	Mat leftImage2 = imread(leftImageSource2);
	Mat rightImage2 = imread(rightImageSource2);

	leftDiff = detectDifference(leftImage1, leftImage2);
	rightDiff = detectDifference(rightImage1, rightImage2);

	detectContours(leftDiff);
	detectContours(rightDiff);

	imshow("left image", leftDiff);
	imshow("right image", rightDiff);
	waitKey(0);
	// this is taking image directly from cameras
	*/
	

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

		detectContours(leftDiff);
		detectContours(rightDiff);

		imshow("Left Camera Image", leftDiff);
		imshow("Right Camera Image", rightDiff);

		imshow("normal cam left", leftImageFrame1);
		imshow("normal cam right", rightImageFrame1);
		waitKey(1);
	}
	
}
