#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;
//hmin,smin,vmin,hmax,smax,vmax
vector<vector<int>> colorValues = { {24,83,136,30,255,252},//yellow
									{101,69,123,115,170,255},//blue
									{77,52,111,88,238,255},// green
};
vector<Scalar> originalColorValues = { {255,255,0},{255,0,0},{0,255,0} };
Mat img;

void getContours(Mat imgDilate) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgDilate, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());


	for (int i = 0; i < contours.size(); i++) {
		string objectType;
		int area = contourArea(contours[i]);
		cout << area << endl;
		if (area > 1000) {
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			drawContours(img, contours, i, Scalar(255, 0, 255), 2);
			boundRect[i] = boundingRect(conPoly[i]);
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 3);

		}

	}
}


void findColor(Mat img) {
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	for (int i = 0; i < colorValues.size(); i++) {
		Scalar lower(colorValues[i][0], colorValues[i][1], colorValues[i][2]);
		Scalar upper(colorValues[i][3], colorValues[i][4], colorValues[i][5]);
		Mat mask;
		inRange(imgHSV, lower, upper, mask);
		//imshow(to_string(i), mask);
		getContours(mask);
	}
}

int main() {
	VideoCapture cap(0);
	
	while (true) {
		cap.read(img);
		findColor(img);
		imshow("cam", img);
		waitKey(1);
	}

}