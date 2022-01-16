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
	RNG rng(12345);
	Mat image1_original, image2_original, diff, dilatedImage, image1_grey, image2_grey;
	Mat image1, image2, threshold_image, contourImage;

	image1_original = imread("Resources/book_test2.jpg");
	image2_original = imread("Resources/book_test3.jpg");

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

	// vector for contours
	vector<vector<Point> > contours_poly(contours.size());
	// vector to store rectangles for bounding boxes
	vector<RotatedRect> minRect(contours.size());

	// create bounding boxes for contours found
	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		minRect[i] = minAreaRect(Mat(contours_poly[i]));
	}
	// creates a blank image that the drawings will be drawn on
	Mat drawing = Mat::zeros(dilatedImage.size(), CV_8UC3);

	// draws the shapes (rectangles and centre points) onto the newly created image
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar colour = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		drawContours(drawing, contours_poly, (int)i, 255, 1, 8, vector<Vec4i>(), 0, Point());
		Point2f rect_points[4];
		minRect[i].points(rect_points);
		for (int j = 0; j < 4; j++)
		{
			line(drawing, rect_points[j], rect_points[j + 1 % 4], colour);
		}

	}

	// code to show images in windows
	namedWindow("Bounding Boxes", WINDOW_AUTOSIZE);
	imshow("threshold image", threshold_image);
	imshow("dilated", dilatedImage);
	imshow("Bounding Boxes", drawing);
	waitKey(0);

}
