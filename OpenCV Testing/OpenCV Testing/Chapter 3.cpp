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
///////////////////////  Resize and Crop   //////////////////////
//
//int main()
//{
//	string path = "Resources/doge.jpg";
//	Mat image = imread(path);
//	Mat imageResize;
//	Mat imageCrop;
//	cout << image.size() << endl;
//	resize(image, imageResize, Size(540,540));
//
//	Rect roi(80, 80, 400, 400);
//	imageCrop = imageResize(roi);
//
//	imshow("Doge", image);
//	imshow("Smol Doge", imageResize);
//	imshow("Cropped Doge", imageCrop);
//	waitKey(0);
//}
