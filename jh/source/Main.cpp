#include "SecondSolution.h"
using namespace cv;

int run(teeth& s) {
	VideoCapture cap;
	if (!cap.open(0))return 0;
	Mat frame;
	while (1) {
		cap >> frame;
		if (frame.empty())break;
		if (!s.run(frame))break;
	}
	return 0;
}

int main(int argc,char** argv)
{
	run(teeth(180));
	return 0;
}

