#include<iostream>

#include "OpenFile.h"
#include "Teeth.h"

using namespace std;
using namespace cv;

int main(int argc,char** argv)
{
	cout << "TeethAR [K;value of color] [D;reconginition dist] [DebugMode;1or0]" << endl;
	cout << CV_VERSION << endl;
	Mat img;
	int cnt = 0;
	int K = 200;
	int dist = 30;
	bool debug = true;

	if (argc > 1)
	{
		K = atoi(argv[1]);
		cout << "K=" << K << endl;
	}
	if (argc > 2)
	{
		dist = atoi(argv[2]);
		cout << "Dist=" << dist << endl;
	}

	VideoCapture cap;
	if (!cap.open(0))return 0;
	while (1) {
		cap >> img;
		if (img.empty())break;
		Teeth t(img, K, dist);
		t.DrawRect(debug);
		imshow("teeth", img);
		if (waitKey(10)==27)break;
	}
	waitKey(0);
	return 0;
}