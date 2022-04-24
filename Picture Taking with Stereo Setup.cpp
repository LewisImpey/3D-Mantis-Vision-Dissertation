//#include <iostream>
//#include <opencv2/opencv.hpp>
//#include "opencv2/core/core.hpp"
//#include "opencv2/calib3d/calib3d.hpp"
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <stdio.h>
//#include <string.h>
//#include <vector>
//#include <string>
//#include <chrono>
//#include <thread>x	
//
//using namespace cv;
//using namespace std;
//
//int main()
//{
//	VideoCapture leftCam(0), rightCam(1);
//	Mat leftFrame, rightFrame;
//	while (true)
//	{
//		this_thread::sleep_for(chrono::milliseconds(4000));
//		leftCam >> leftFrame;
//		rightCam >> rightFrame;
//		imwrite("leftFrame1.jpg", leftFrame);
//		imwrite("rightFrame1.jpg", rightFrame);
//		this_thread::sleep_for(chrono::milliseconds(1000));
//		leftCam >> leftFrame;
//		rightCam >> rightFrame;
//		imwrite("leftFrame2.jpg", leftFrame);
//		imwrite("rightFrame2.jpg", rightFrame);
//		this_thread::sleep_for(chrono::milliseconds(4000));
//		leftCam >> leftFrame;
//		rightCam >> rightFrame;
//		imwrite("leftFrame3.jpg", leftFrame);
//		imwrite("rightFrame3.jpg", rightFrame);
//		break;
//	}
//}