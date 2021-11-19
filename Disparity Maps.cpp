#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <string.h>

using namespace cv;

int main()
{

	Mat imgL, imgR, image_left, image_right;
	double newSize = 0.5;
	imgL = imread("Resources/im0.png");
	imgR = imread("Resources/im1.png");

	resize(imgL, image_left, Size(), newSize, newSize);	
	resize(imgR, image_right, Size(), newSize, newSize);

	int minDisparity = 0;
	int numDisparities = 128;
	int blockSize = 8;
	int disp12MaxDiff = 1;
	int uniquenessRatio = 10;
	int speckleWindowSize = 20;
	int speckleRange = 8;

	Ptr<StereoSGBM> stereo = StereoSGBM::create(minDisparity, numDisparities, blockSize, disp12MaxDiff, uniquenessRatio, speckleWindowSize, speckleRange);

	Mat disp;
	stereo->compute(image_left, image_right, disp);
	//stereo->compute(imgL, imgR, disp);

	normalize(disp, disp, 0, 255, NORM_MINMAX, CV_8UC1);

	imshow("disparity", disp);
	waitKey(0);

	return 0;
}
