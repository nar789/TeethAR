#pragma once
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;
class teeth {
private:
	int k=180;
public:
	teeth(int _k) {
		k = _k;
	};
	int run(Mat& frame) {
		Mat b = frame.clone();
		auto size = b.size();
		char str[255];
		sprintf(str, "K=%d", k);
		putText(frame, str, Point(10, 40), 2, 1.3, Vec3b(255, 0, 0));
		for (int i = 0; i < size.height; i++)
		{
			for (int j = 0; j < size.width; j++) {
				auto& p = b.at<Vec3b>(i, j);
				float f = (p[0] + p[1] + p[2]) / 3;
				if (f > k && (p[0]>k && p[1]>k && p[2]>k)) { p[0] = 255; p[1] = 255; p[2] = 255; }
				else { p[0] = 0; p[1] = 0; p[2] = 0; }
			}
		}
		teetdetect(k, b, frame);
		int key = waitKey(30);
		if (key == 27)return 0;
		else if (key == 119)k += 10;
		else if (key == 115)k -= 10;
		return 1;
	};
	void teetdetect(int k, Mat& frame, Mat& org) {

		Mat gray;
		vector<vector<Point> > contours;
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		threshold(gray, gray, k, 255, THRESH_BINARY_INV | THRESH_OTSU);
		//imshow("edge", gray);
		findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

		//contour를 근사화한다.
		vector<Point2f> approx;
		Mat img_result = frame.clone();

		for (size_t i = 0; i < contours.size(); i++)
		{
			approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.0002, true);
			double a = fabs(contourArea(Mat(approx)));
			if (a>1000 && a<10000)
			{
				int size = approx.size();
				vector<int> angle;
				int fi = -1;
				vector<Point2f> ca;
				for (int k = 0; k < size; k++) {
					int ang = GetAngleABC(approx[k], approx[(k + 1) % size], approx[(k + 2) % size]);
					//if (ang >180)break;
					ca.push_back(approx[k]);
					angle.push_back(ang);
				}
				if (ca.size() != 0)approx = ca;
				size = ca.size();
				sort(angle.begin(), angle.end());

				int minAngle = angle.front();
				int maxAngle = angle.back();
				int threshold = 8;

				//도형을 판정한다.
				/*
				if (size <= 3) {
				//continue;
				setLabel(org, "triangle", contours[i]);
				}
				else if (size == 4 && minAngle >= 90 - threshold && maxAngle <= 90 + threshold)
				setLabel(org, "rectangle", contours[i]);
				else if (size == 5 && minAngle >= 108 - threshold && maxAngle <= 108 + threshold)
				setLabel(org, "pentagon", contours[i]);
				else if (size == 6 && minAngle >= 120 - threshold && maxAngle <= 120 + threshold)
				setLabel(org, "hexagon", contours[i]);
				else {
				setLabel(org, to_string(approx.size()), contours[i]);//알수 없는 경우에는 찾아낸 꼭지점 갯수를 표
				}*/
				//cout << "===" << size << " " << a << endl;
				//Contour를 근사화한 직선을 그린다.
				if (size % 2 == 0) {
					line(org, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 3);

					for (int k = 0; k < size - 1; k++)
						line(org, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);
					/*
					for (int k = 0; k < size; k++)
					circle(org, approx[k], 3, Scalar(0, 0, 255));*/
				}
				else {
					line(org, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 3);

					for (int k = 0; k < size - 1; k++)
						line(org, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);
					/*
					for (int k = 0; k < size; k++)
					circle(org, approx[k], 3, Scalar(0, 0, 255));*/

				}
			}
		}
		imshow("frame", org);
	};
	void setLabel(Mat& image, string str, vector<Point> contour)
	{
		int fontface = FONT_HERSHEY_SIMPLEX;
		double scale = 0.5;
		int thickness = 1;
		int baseline = 0;

		Size text = getTextSize(str, fontface, scale, thickness, &baseline);
		Rect r = boundingRect(contour);

		Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
		rectangle(image, pt + Point(0, baseline), pt + Point(text.width, -text.height), CV_RGB(200, 200, 200), CV_FILLED);
		putText(image, str, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
	};
	int GetAngleABC(Point a, Point b, Point c)
	{
		Point ab = { b.x - a.x, b.y - a.y };
		Point cb = { b.x - c.x, b.y - c.y };

		float dot = (ab.x * cb.x + ab.y * cb.y); // dot product
		float cross = (ab.x * cb.y - ab.y * cb.x); // cross product

		float alpha = atan2(cross, dot);

		return (int)floor(alpha * 180.0 / CV_PI + 0.5);
	};

};