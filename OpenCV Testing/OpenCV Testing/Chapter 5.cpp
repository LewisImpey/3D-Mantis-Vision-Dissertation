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
///////////////////////  Warp Images   //////////////////////
//
//float w = 250, h = 250;
//Mat matrix, imageWarp;
//int main()
//{
//	string path = "Resources/cards.jpg";
//	Mat image = imread(path);
//
//	Point2f src[4] = { {529, 142}, {771, 190}, {405, 395}, {674, 457} };
//	Point2f dst[4] = { {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h} };
//
//	matrix = getPerspectiveTransform(src, dst);
//	warpPerspective(image, imageWarp, matrix, Point(w, h));
//
//	for (int i = 0; i < 4; i++)
//	{
//		circle(image, src[i], 10,  Scalar(0, 0, 255), FILLED);
//	}
//	imshow("Warp Image", image);
//	imshow("Warp Example", imageWarp);
//	waitKey(0);
//}
