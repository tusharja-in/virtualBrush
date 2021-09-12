#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;
//hmin,smin,vmin,hmax,smax,vmax
vector<vector<int>> colorValues = { {24,83,136,30,255,252},//yellow
									{101,69,123,115,170,255},//blue
									{39,65,111,73,180,255},// green
									{0,101,138,14,210,255}//red 
};
vector<Scalar> originalColorValues = { {255,255,0},{255,0,0},{0,255,0},{0,0,255} };


void findColor(Mat img) {
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	for (int i = 0; i < colorValues.size(); i++) {
		Scalar lower(colorValues[i][0], colorValues[i][1], colorValues[i][2]);
		Scalar upper(colorValues[i][3], colorValues[i][4], colorValues[i][5]);
		Mat mask;
		inRange(imgHSV, lower, upper, mask);
		imshow(to_string(i), mask);
	}
}

int main() {
	VideoCapture cap(0);
	Mat img;
	while (true) {
		cap.read(img);
		findColor(img);
		imshow("cam", img);
		waitKey(1);
	}

}