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
	GaussianBlur(ImageFrame1, ImageFrame1, Size(15, 15), 0, 0);
	GaussianBlur(ImageFrame2, ImageFrame2, Size(15, 15), 0, 0);

	// getting the difference between the two images
	absdiff(ImageFrame1, ImageFrame2, diff);

	return diff;
}

int main()
{
	Mat leftDiff, rightDiff;
	VideoCapture leftImageSource(0), rightImageSource(1);

 ////////////////////////////////////////////// This section is to create disparity maps from static images ////////////////////////////////////////////////

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

	// applies a threshold to the merged image, small changes that fall below this threshold will be forgotten about and not tracked
	threshold(leftDiff, leftDiff, 30, 255, THRESH_BINARY);
	threshold(rightDiff, rightDiff, 30, 255, THRESH_BINARY);

	/* a kernel is created and the dilate function is called, which dilates the image making all of the
	   things still detected after the threshold larger. This can help to merge the contours together */
	Mat kernel = getStructuringElement(MORPH_ERODE, Size(30, 30));
	dilate(leftDiff, leftDiff, kernel);
	dilate(rightDiff, rightDiff, kernel);

	imshow("left image", leftDiff);
	imshow("right image", rightDiff);
	
	// parameters for stereo block matching are set
	int minDisparity = 0;
	int numDisparities = 12;
	int blockSize = 3;
	int disp12MaxDiff = 1;
	int uniquenessRatio = 10;
	int speckleWindowSize = 200;
	int speckleRange = 1;

	Ptr<StereoSGBM> stereo = StereoSGBM::create(minDisparity, numDisparities, blockSize, disp12MaxDiff, uniquenessRatio, speckleWindowSize, speckleRange);

	Mat disparity;
	stereo->compute(leftDiff, rightDiff, disparity);

	normalize(disparity, disparity, 0, 255, NORM_MINMAX, CV_8UC1);

	imshow("disparity", disparity);
	waitKey(0);
	 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 

////////////////////////////////////////////////////// This section is used to create disparity maps on live video (is very slow) ////////////////////////////////////////////////////////////////
	/*while (true)
	{
		Mat leftImageFrame1, leftImageFrame2, rightImageFrame1, rightImageFrame2;
		leftImageSource >> leftImageFrame1;
		rightImageSource >> rightImageFrame1;
		this_thread::sleep_for(chrono::milliseconds(1));
		leftImageSource >> leftImageFrame2;
		rightImageSource >> rightImageFrame2;

		leftDiff = detectDifference(leftImageFrame1, leftImageFrame2);
		rightDiff = detectDifference(rightImageFrame1, rightImageFrame2);

		threshold(leftDiff, leftDiff, 50, 255, THRESH_BINARY);
		threshold(rightDiff, rightDiff, 50, 255, THRESH_BINARY);

		Mat kernel = getStructuringElement(MORPH_ERODE, Size(30, 30));
		dilate(leftDiff, leftDiff, kernel);
		dilate(rightDiff, rightDiff, kernel);


		imshow("left dilated image", leftDiff);
		imshow("right dilated image", rightDiff);

		int minDisparity = 0;
		int numDisparities = 12;
		int blockSize = 3;
		int disp12MaxDiff = 1;
		int uniquenessRatio = 10;
		int speckleWindowSize = 200;
		int speckleRange = 1;

		Ptr<StereoSGBM> stereo = StereoSGBM::create(minDisparity, numDisparities, blockSize, disp12MaxDiff, uniquenessRatio, speckleWindowSize, speckleRange);

		Mat disparity;
		stereo->compute(leftDiff, rightDiff, disparity);

		normalize(disparity, disparity, 0, 255, NORM_MINMAX, CV_8UC1);

		imshow("disparity", disparity);
		waitKey(1);
	}*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return 0;
}
