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
	Mat image1, image2, diff, diffGray;
	image1 = imread("Resources/image1.png");
	image2 = imread("Resources/image2.png");
	
	absdiff(image1, image2, diff);
	cvtColor(diff, diffGray, COLOR_BGR2GRAY);

	imshow("normal diff", diff);
	imshow("difference gray", diffGray);
	waitKey(0);
}
