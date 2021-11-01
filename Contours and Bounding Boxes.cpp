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
	Mat image1_original, image2_original, diff, dilatedImage, image1_grey, image2_grey;
	Mat image1, image2, threshold_image, contourImage;

	image1_original = imread("Resources/circle_test_1.png");
	image2_original = imread("Resources/circle_test_2.png");

	// resizes images if needed
	//resize(image1_original, image1, Size(), 0.5, 0.5);
	//resize(image2_original, image2, Size(), 0.5, 0.5);

	cvtColor(image1_original, image1_grey, COLOR_BGR2GRAY);
	cvtColor(image2_original, image2_grey, COLOR_BGR2GRAY);

	// getting the difference between the two images
	absdiff(image1_grey, image2_grey, diff);

	// applying a threshold to the difference image
	threshold(diff, threshold_image, 100, 255, THRESH_BINARY);

	// dilates the image
	Mat kernal = getStructuringElement(MORPH_ERODE, Size(1, 1));
	dilate(threshold_image, dilatedImage, kernal);

	// Creates bounding boxes around all things detected to have moved in the image.
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	// find contours in image
	findContours(dilatedImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	// get the moments
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{ 
		mu[i] = moments(contours[i], false); 
	}

	// get the centroid of figures.
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{ 
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00); 
	}

	// vector for contours
	vector<vector<Point> > contours_poly(contours.size());
	// vector to store rectangles for bounding boxes
	vector<Rect> boundRect(contours.size());

	// create bounding boxes for contours found
	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}
	// creates a blank image that the drawings will be drawn on
	Mat drawing = Mat::zeros(dilatedImage.size(), CV_8UC3);

	// draws the shapes (rectangles and centre points) onto the newly created image
	for (size_t i = 0; i < contours.size(); i++)
	{
		drawContours(drawing, contours_poly, (int)i, 255, 1, 8, vector<Vec4i>(), 0, Point());
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), 255, 2, 8, 0);
		circle(drawing, mc[i], 3, Scalar(255, 255, 255), FILLED);
	}

	// code to show images in windows
	namedWindow("Bounding Boxes", WINDOW_AUTOSIZE);
	imshow("threshold image", threshold_image);
	imshow("dilated", dilatedImage);
	imshow("Bounding Boxes", drawing);
	waitKey(0);

}
