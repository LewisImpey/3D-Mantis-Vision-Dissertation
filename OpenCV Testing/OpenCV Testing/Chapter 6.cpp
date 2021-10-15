//#include <iostream>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgcodecs/imgcodecs.hpp>
//
//using namespace std;
//using namespace cv;
//
//
///////////////////////  Colour Detection   //////////////////////
//
//int hmin = 0, smin = 0, vmin = 0;
//int hmax = 179, smax = 255, vmax = 255;
//
//int main()
//{
//	string path = "Resources/shapes.png";
//	Mat image = imread(path);
//	Mat imageHSV, mask;
//
//	cvtColor(image, imageHSV, COLOR_BGR2HSV);
//
//	namedWindow("Trackbars", (640, 200));
//	createTrackbar("Hue Min", "Trackbars", &hmin, 179);
//	createTrackbar("Hue Max", "Trackbars", &hmax, 179);
//	createTrackbar("Sat Min", "Trackbars", &smin, 255);
//	createTrackbar("Sat Max", "Trackbars", &smax, 255);
//	createTrackbar("Val Min", "Trackbars", &vmin, 255);
//	createTrackbar("Val Max", "Trackbars", &vmax, 255);
//
//	while (true)
//	{
//		Scalar lower(hmin, smin, vmin);
//		Scalar upper(hmax, smax, vmax);
//		inRange(imageHSV, lower, upper, mask);
//
//		imshow("Colour Detection Example", image);
//		imshow("Colour Detection", imageHSV);
//		imshow("Mask", mask);
//		waitKey(1);
//	}
//}
