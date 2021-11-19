#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <string.h>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
	Mat imgL, imgR, imageLeft, imageRight, disp;
	imgL = imread("Resources/test_image1.jpg");
	imgR = imread("Resources/test_image2.jpg");

	// Setting parameters for StereoSGBM algorithm
	int minDisparity = 0;
	int numDisparities = 64;
	int blockSize = 8;
	int disp12MaxDiff = 1;
	int uniquenessRatio = 10;
	int speckleWindowSize = 10;
	int speckleRange = 8;

	// Creating an object of StereoSGBM algorithm
	Ptr<StereoSGBM> stereo = StereoSGBM::create(minDisparity, numDisparities, blockSize, disp12MaxDiff, uniquenessRatio, speckleWindowSize, speckleRange);

	// Calculating disparith using the StereoSGBM algorithm
	stereo->compute(imageLeft, imageRight, disp);

	// Normalizing the disparity map for better visualisation 
	normalize(disp, disp, 0, 255, NORM_MINMAX, CV_8UC1);

	// Displaying the disparity map
	imshow("disparity", disp);
	waitKey(0);
}
