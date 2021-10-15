#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/objdetect/objdetect.hpp>
using namespace std;
using namespace cv;

///////////////////  Face Detection   //////////////////////

int main()
{
	string path = "Resources/2person_test.jpg"; //lewis_test.jpg
	Mat image = imread(path);

	CascadeClassifier faceCascade;
	faceCascade.load("Resources/haarcascade_frontalface_default.xml");

	if (faceCascade.empty()) (cout << "XML File not loaded" << endl);

	vector<Rect> faces;
	faceCascade.detectMultiScale(image, faces, 1.1, 10);

	for (int i = 0; i < faces.size(); i++)
	{
		rectangle(image, faces[i].tl(), faces[i].br(), Scalar(255, 0, 255), 3);
	}
	imshow("Face Detection", image);
	waitKey(0);
}