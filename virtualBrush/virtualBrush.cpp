#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;
//hmin,smin,vmin,hmax,smax,vmax
vector<vector<int>> colorValues = { {24,83,136,30,255,252},//yellow
									{101,71,123,115,170,255},//blue
									{77,52,111,88,238,255},// green
};

vector<Scalar> originalColorValues = { {255,255,0},{255,0,0},{0,255,0} };
Mat img;

vector<vector<int>> drawedPoints;

Point getContours(Mat imgDilate) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgDilate, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());
	Point mypoint;

	for (int i = 0; i < contours.size(); i++) {
		string objectType;
		int area = contourArea(contours[i]);
		cout << area << endl;
		if (area > 1500) {
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			drawContours(img, contours, i, Scalar(255, 0, 255), 2);
			boundRect[i] = boundingRect(conPoly[i]);
			mypoint.x = boundRect[i].x + boundRect[i].width/2;
			mypoint.y = boundRect[i].y;
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 3);

		}

	}
	return mypoint;
}

void drawOnCanvas() {
	for (int i = 0; i < drawedPoints.size(); i++) {
		circle(img, Point(drawedPoints[i][0], drawedPoints[i][1]),5,originalColorValues[drawedPoints[i][2]],FILLED);
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
		Point mypoint=getContours(mask);
		if (mypoint.x != 0 && mypoint.y != 0) {
			drawedPoints.push_back({ mypoint.x,mypoint.y,i });
		}

	}
}

int main() {
	VideoCapture cap(0);
	
	while (true) {
		cap.read(img);
		findColor(img);
		drawOnCanvas();
		imshow("cam", img);
		waitKey(1);
	}

}