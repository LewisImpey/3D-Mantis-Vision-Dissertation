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
	Mat image1_original, image2_original, diff, diffGray, dilatedImage;
	Mat image1, image2, threshold_image, contourImage;

	image1_original = imread("Resources/book_test1.jpg");
	image2_original = imread("Resources/book_test2.jpg");

	resize(image1_original, image1, Size(), 0.5, 0.5);
	resize(image2_original, image2, Size(), 0.5, 0.5);

	// getting the difference between the two images
	absdiff(image1, image2, diff);
	// converting to grayscale
	cvtColor(diff, diffGray, COLOR_BGR2GRAY);
	// applying a threshold to the difference image
	threshold(diffGray, threshold_image, 100, 255, THRESH_BINARY);

	Mat kernal = getStructuringElement(MORPH_ERODE, Size(100, 100));
	dilate(threshold_image, dilatedImage, kernal);

	imshow("threshold image", threshold_image);

	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	threshold(dilatedImage, threshold_output, 100, 255, THRESH_BINARY);
	findContours(threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());

	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}
	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++)
	{
		//drawContours(drawing, contours_poly, (int)i, 255, 1, 8, vector<Vec4i>(), 0, Point());
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), 255, 2, 8, 0);
	}
	namedWindow("Contours", WINDOW_AUTOSIZE);
	imshow("dilated", dilatedImage);
	imshow("Contours", drawing);

	waitKey(0);

}