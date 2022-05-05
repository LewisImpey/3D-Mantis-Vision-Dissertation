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

using namespace cv;
using namespace std;

RNG rng(12345);

float left_x_coord = 0;
float right_x_coord = 0;
float left_y_coord = 0;
float right_y_coord = 0;
float temp_x_coord = 0;
float temp_y_coord = 0;

Mat detectDifference(Mat ImageFrame1, Mat ImageFrame2)
{
	Mat diff;
	Mat dilatedImage;

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

void detectContours(Mat& image, Mat& originalFrame)
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
	Mat kernel = getStructuringElement(MORPH_ERODE, Size(30, 30));
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
		else if (boundRect[i].area() > largestContourArea) { largestContourArea = boundRect[i].area(); largestContour = i; }
	}

	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar colour = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));

		// every contour detected is then drawn onto the image
		drawContours(image, contours_poly, (int)i, colour, 2, 8, vector<Vec4i>(), 0, Point());

		// the largest bounding box is then drawn onto the image using the rectangle function
		rectangle(image, boundRect[largestContour].tl(), boundRect[largestContour].br(), 255, 2, 8, 0);
		rectangle(originalFrame, boundRect[largestContour].tl(), boundRect[largestContour].br(), 255, 2, 8, 0);
		// the centroid of the largest box is also drawn using the circle function
		circle(image, mc[largestContour], 5, Scalar(255, 255, 255), FILLED);

		// the coordinates of this largest contour is then saved
		temp_x_coord = mc[largestContour].x;
		temp_y_coord = mc[largestContour].y;
	}
}

int calculateDepth(float left_x_coord, float right_y_coord, int cameraFocalLength, int baseline)
{
	// depth formula is used to calculation the distance of the object from the cameras
	int depth = (cameraFocalLength * baseline) / (right_x_coord - left_x_coord);
	return depth;
}

int main()
{
	Mat leftDiff;
	Mat rightDiff;

	int focalLength = 60;
	int baseline = 160;
	int depth = 0;


	///////////////////////////////////////////////////////////////// This section of code is used to perform the detection on static images /////////////////////////////////////////////////////////////
		//string leftImageFrame1 = "Resources/leftFrame1.jpg";
		//string rightImageFrame1 = "Resources/rightFrame1.jpg";

		//string leftImageFrame2 = "Resources/leftFrame2.jpg";
		//string rightImageFrame2 = "Resources/rightFrame2.jpg";

		//Mat leftImage1 = imread(leftImageFrame1);
		//Mat rightImage1 = imread(rightImageFrame1);

		//Mat leftImage2 = imread(leftImageFrame2);
		//Mat rightImage2 = imread(rightImageFrame2);

		//leftDiff = detectDifference(leftImage1, leftImage2);
		//rightDiff = detectDifference(rightImage1, rightImage2);

		//detectContours(leftDiff, leftImage2);
		//left_x_coord = temp_x_coord;
		//left_y_coord = temp_y_coord;

		//detectContours(rightDiff, rightImage2);
		//right_x_coord = temp_x_coord;
		//right_y_coord = temp_y_coord;
		//if (right_y_coord > left_y_coord - 20 && right_y_coord < left_y_coord + 20)
		//{
		//	depth = calculateDepth(left_x_coord, right_x_coord, focalLength, baseline);
		//	if (depth > 0)
		//	{
		//		putText(leftDiff, "distance to object: " + to_string(depth) + "cm", Point(200, 20), FONT_HERSHEY_DUPLEX, 0.50, Scalar(255, 255, 255), 1);
		//		putText(rightDiff, "distance to object: " + to_string(depth) + "cm", Point(200, 20), FONT_HERSHEY_DUPLEX, 0.50, Scalar(255, 255, 255), 1);
		// 
		// 		putText(leftImage2, "distance to object: " + to_string(depth) + "cm", Point(175, 20), FONT_HERSHEY_DUPLEX, 0.75, Scalar(255, 255, 255), 1);
		//		putText(rightImage2, "distance to object: " + to_string(depth) + "cm", Point(175, 20), FONT_HERSHEY_DUPLEX, 0.75, Scalar(255, 255, 255), 1);
		//	}
		//}

		//imshow("left image", leftDiff);
		//imshow("right image", rightDiff);
		//imshow("Normal Left Camera Frame", leftImage2);
		//imshow("Normal Right Camera Frame", rightImage2);

		//waitKey(0);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////// This section of code is used to perform the detection on live video ///////////////////////////////////////////////////////////////

	VideoCapture leftImageSource(0), rightImageSource(1);

	while (true)
	{
		Mat leftImageFrame1, leftImageFrame2, rightImageFrame1, rightImageFrame2;

		// frames from each camera are saved which will be used as the previous frames
		leftImageSource >> leftImageFrame1;
		rightImageSource >> rightImageFrame1;
		// a small wait is then performed and the current frames are then collected again
		this_thread::sleep_for(chrono::milliseconds(1));
		leftImageSource >> leftImageFrame2;
		rightImageSource >> rightImageFrame2;

		leftDiff = detectDifference(leftImageFrame1, leftImageFrame2);
		rightDiff = detectDifference(rightImageFrame1, rightImageFrame2);

		detectContours(leftDiff, leftImageFrame2);
		left_x_coord = temp_x_coord;
		left_y_coord = temp_y_coord;

		detectContours(rightDiff, rightImageFrame2);
		right_x_coord = temp_x_coord;
		right_y_coord = temp_y_coord;

		// in the event that the cameras are ever so slightly not level some leeway is allowed
		if (right_y_coord > left_y_coord - 20 && right_y_coord < left_y_coord + 20)
		{
			depth = calculateDepth(left_x_coord, right_x_coord, focalLength, baseline);
			if (depth > 0)
			{
				putText(leftDiff, "distance to object: " + to_string(depth) + "cm", Point(175, 20), FONT_HERSHEY_DUPLEX, 0.75, Scalar(255, 255, 255), 1);
				putText(rightDiff, "distance to object: " + to_string(depth) + "cm", Point(175, 20), FONT_HERSHEY_DUPLEX, 0.75, Scalar(255, 255, 255), 1);

				putText(leftImageFrame2, "distance to object: " + to_string(depth) + "cm", Point(175, 20), FONT_HERSHEY_DUPLEX, 0.75, Scalar(255, 255, 255), 1);
				putText(rightImageFrame2, "distance to object: " + to_string(depth) + "cm", Point(175, 20), FONT_HERSHEY_DUPLEX, 0.75, Scalar(255, 255, 255), 1);
			}
		}

		imshow("Left Camera Image", leftDiff);
		imshow("Right Camera Image", rightDiff);

		imshow("Normal Left Camera Frame", leftImageFrame2);
		imshow("Normal Right Camera Frame", rightImageFrame2);

		waitKey(1);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
