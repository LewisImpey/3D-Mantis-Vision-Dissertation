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
	Mat camera1_frame, camera2_frame, diff, diffGray, threshold_image;
	VideoCapture camera1(0);
	VideoCapture camera2(1);

	camera1.read(camera1_frame);
	camera2.read(camera2_frame);
	absdiff(camera1_frame, camera2_frame, diff);
	cvtColor(diff, diffGray, COLOR_BGR2GRAY);

	threshold(diffGray, threshold_image, 150, 255, THRESH_BINARY);

	imshow("webcam 1", camera1_frame);
	imshow("webcam 2", camera2_frame);
	imshow("normal diff", diff);
	imshow("difference gray", diffGray);
	imshow("threshold image", threshold_image);
	waitKey(0);

	//while (true)
	//{
	//	camera1.read(camera1_frame);
	//	camera2.read(camera2_frame);
	//	absdiff(camera1_frame, camera2_frame, diff);
	//	cvtColor(diff, diffGray, COLOR_BGR2GRAY);
	//	threshold(diffGray, threshold_image, 100, 255, THRESH_BINARY);

	//	imshow("difference webcam", diff);
	//	imshow("difference webcam gray", diffGray);
	//	imshow("webcam 1", camera1_frame);
	//	imshow("webcam 2", camera2_frame);
	//	imshow("threshold webcam", threshold_image);
	//	waitKey(1);
	//}
}
