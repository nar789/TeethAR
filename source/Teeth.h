#pragma once

#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<queue>
#include<vector>
using namespace cv;
using namespace std;

class Teeth
{

private:
	struct P {
		P(int a, int b, int c) :y(a), x(b), g(c) {};
		int x, y, g;
	};
	vector<P> teeth;
	Mat origin;
	Mat img;
	Mat teethimg;
	int dist = 100;
	int K=200;
	vector<vector<int>> visit;
	int W=0, H=0;
	
public:
	Teeth(Mat Image) :origin(Image) { img = origin.clone(); W = img.size().width; H = img.size().height; };
	Teeth(Mat Image, int _K, int Dist) :K(_K),dist(Dist), origin(Image) { img = origin.clone(); W = img.size().width; H = img.size().height;};

	void PreProcess(void) {
		visit.clear();
		for (int i = 0; i < H; i++) {
			visit.push_back({ 0 });
			for (int j = 0; j < W; j++)
			{
				visit[i].push_back(0);
				auto& p = img.at<Vec3b>(i, j);
				p = { (p[0] > K&&p[1]>K&&p[2] > K) ? (uchar)0 : (uchar)255,255,255 };
			}
		}
	}

	void TeethDetect(void) {
		PreProcess();
		queue<P> qc;
		int gc = 0;
		const int d[][2] = { { 0,1 },{ 1,0 },{ -1,0 },{ 0,-1 } };

		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++)
			{
				auto& p = img.at<Vec3b>(i, j);
				if (p[0] == 255 && p[1] == 255 && p[2] == 255)continue;
				if (p[0] == 0 && visit[i][j] == 0) {
					visit[i][j] = 1;
					p[2] = ++gc * 100 % 255; p[0] = gc * 50 % 255; p[1] = gc * 70 % 255;
					qc.push(P(i, j, gc));
					vector<P> v;
					v.push_back(P(i, j, gc));
					while (!qc.empty()) {
						auto cur = qc.front(); qc.pop();
						for (int k = 0; k < 4; k++) {
							for (int r = 1; r < dist; r++) {
								int dy = cur.y + (d[k][0] * r); int dx = cur.x + (d[k][1] * r);
								if (dy < 0 || dy >= H || dx < 0 || dx >= W)continue;
								auto& ar = img.at<Vec3b>(dy, dx);
								if (ar[0] == 0 && visit[dy][dx] == 0)
								{
									qc.push(P(dy, dx, cur.g));
									v.push_back(P(dy, dx, cur.g));
									visit[dy][dx] = 1;
									ar = p;
								}
							}
						}
					}
					if (teeth.size() < v.size())
						teeth = v;
				}
			}
		}
	}
	void DrawRect(bool Debug)
	{
		if (Debug) {
			DrawRect();
			imshow("debug1", img);
			imshow("debug2", teethimg);
		}
		else
			DrawRect();
	}
	void DrawRect(void) {
		if (!teeth.size())
			TeethDetect();
		int top, left, bottom, right;
		top = left = 0x7fffffff;
		bottom = right = 0;
		teethimg = origin.clone();
		for (auto i : teeth) {
			if (top > i.y)top = i.y;
			if (bottom < i.y)bottom = i.y;
			if (left > i.x)left = i.x;
			if (right < i.x)right = i.x;
			teethimg.at<Vec3b>(i.y, i.x) = { (uchar)(i.g * 100 % 255), (uchar)(i.g * 50 % 255),(uchar)(i.g * 70 % 255) };
		}
		rectangle(origin, Rect(left, top, right - left, bottom - top), Scalar(0, 255, 0), 3);
	}
};