#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <string.h>

using namespace cv;
using namespace std;

int main()
{
	Mat image1_original, image2_original, diff, diffGray;
	Mat image1, image2;

	image1_original = imread("Resources/test image1.jpg");
	image2_original = imread("Resources/test image2.jpg");

	resize(image1_original, image1, Size(), 0.5, 0.5);
	resize(image2_original, image2, Size(), 0.5, 0.5);


	absdiff(image1, image2, diff);
	cvtColor(diff, diffGray, COLOR_BGR2GRAY);

	imshow("normal diff", diff);
	imshow("difference gray", diffGray);
	waitKey(0);
}
