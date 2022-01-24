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

	resize(ImageFrame1, ImageFrame1, Size(), 0.5, 0.5);
	resize(ImageFrame2, ImageFrame2, Size(), 0.5, 0.5);

	// converting to grayscale
	cvtColor(ImageFrame1, ImageFrame1, COLOR_BGR2GRAY);
	cvtColor(ImageFrame2, ImageFrame2, COLOR_BGR2GRAY);

	// getting the difference between the two images
	absdiff(ImageFrame1, ImageFrame2, diff);

	return diff;
}
Mat detectDilation(Mat diff)
{
	Mat dilatedImage;
	// applying a blur filter to the image to merge neighboring pixels together. This should allow the bounding boxes to pick up larger areas of pixels.
	GaussianBlur(diff, diff, Size(7, 7), 2, 2); // I think this should be put before the absdiff() function is run to get a more accurate bounding box (blur before makes it considerably slower)
	// applying a threshold to the difference image

	Mat kernal = getStructuringElement(MORPH_ERODE, Size(100, 100));
	dilate(diff, dilatedImage, kernal);

	return dilatedImage, diff;
}

void detectContours(Mat& image, Mat& dilatedImage, bool isLeftCamera)
{
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat threshold_output;
	threshold(dilatedImage, threshold_output, 10, 255, THRESH_BINARY);
	findContours(threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	Point2f leftCameraCornerCoordinates;
	Point2f rightCameraCornerCoordinates;

	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		if (isLeftCamera == true) { leftCameraCornerCoordinates.x = boundRect[i].x; leftCameraCornerCoordinates.y = boundRect[i].y; isLeftCamera = false; }
		else { rightCameraCornerCoordinates.x = boundRect[i].x; rightCameraCornerCoordinates.y = boundRect[i].y; isLeftCamera = true; }
	}
	//Mat image = Mat::zeros(threshold_output.size(), CV_8UC3);
	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);

	for (size_t i = 0; i < contours.size(); i++)
	{
		//drawContours(drawing, contours_poly, (int)i, 255, 1, 8, vector<Vec4i>(), 0, Point());
		rectangle(image, boundRect[i].tl(), boundRect[i].br(), 255, 2, 8, 0);
		putText(drawing, to_string(leftCameraCornerCoordinates.x) , Point(20, 20), FONT_HERSHEY_DUPLEX, 0.75, colour, 4);
		putText(drawing, to_string(leftCameraCornerCoordinates.y), Point(20, 40), FONT_HERSHEY_DUPLEX, 0.75, colour, 4);

		putText(drawing, to_string(rightCameraCornerCoordinates.x), Point(20, 60), FONT_HERSHEY_DUPLEX, 0.75, colour, 4);
		putText(drawing, to_string(rightCameraCornerCoordinates.y), Point(20, 80), FONT_HERSHEY_DUPLEX, 0.75, colour, 4);

		imshow("coords", drawing);
		waitKey(1);

	}

}

int main()
{
	Mat leftDiff, leftDiffGray, leftDilatedImage;
	Mat rightDiff, rightDiffGray, rightDilatedImage;

	/*
	// this if trying to use an image
	string leftImageSource = "Resources/book_test1.jpg"; // change for left frames
	string rightImageSource = "Resources/book_test2.jpg"; // same

	Mat leftImage = imread(leftImageSource);
	Mat rightImage = imread(rightImageSource);
	*/

	// this is taking image directly from cameras
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

		leftDilatedImage = detectDilation(leftDiff);
		rightDilatedImage = detectDilation(rightDiff);

		detectContours(leftDiff, leftDilatedImage, true);
		detectContours(rightDiff, rightDilatedImage, false);

		imshow("left dilated image", leftDiff);
		imshow("right dilated image", rightDiff);
		waitKey(1);
	}
}
