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
/////////////////////  Basic Functions   //////////////////////
//
//int main()
//{
//	string path = "Resources/doge.jpg";
//	Mat img = imread(path);
//	Mat image;
//	resize(img, image, Size(), 0.5, 0.5);
//	Mat imageGray, imageBlur, imageCanny, imageDilate, imageErode;
//
//	cvtColor(image, imageGray, COLOR_BGR2GRAY);
//	GaussianBlur(image, imageBlur, Size(3,3),3,5);
//	Canny(imageBlur, imageCanny, 50, 150);
//
//	Mat kernal = getStructuringElement(MORPH_ERODE, Size(5, 5));
//	dilate(imageCanny, imageDilate, kernal);
//	erode(imageDilate, imageErode, kernal);
//
//	imshow("Doge", image);
//	imshow("Doge Gray", imageGray);
//	imshow("Doge Blur", imageBlur);
//	imshow("Doge Canny", imageCanny);
//	imshow("Doge Dilate", imageDilate);
//	imshow("Doge Erode", imageErode);
//	waitKey(0);
//}
