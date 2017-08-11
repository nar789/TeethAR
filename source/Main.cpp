#include<iostream>

#include "OpenFile.h"
#include "Teeth.h"

using namespace std;
using namespace cv;

int main(int argc,char** argv)
{
	cout << "TeethAR [filename] [K;value of color] [D;reconginition dist] [DebugMode;1or0]" << endl;
	cout << CV_VERSION << endl;
	Mat img;
	int cnt = 0;
	int K = 200;
	int dist = 100;
	bool debug = false;
	if (argc > 1)
	{
		string s = argv[0];
		int x=s.find_last_of('\\');
		string filename = s.substr(0, x + 1) + argv[1];
		cout << filename << endl;
		img = imread(filename);
		if (!img.size().width) { cout << "This file is not exist." << endl; exit(1); }
	}
	if (argc > 2)
	{
		K = atoi(argv[2]);
		cout << "K=" << K << endl;
	}
	if (argc > 3)
	{
		dist = atoi(argv[3]);
		cout << "Dist=" << dist << endl;
	}
	if (argc > 4) {
		if (atoi(argv[4]) > 0)
			debug = true;
		else
			debug = false;
		cout << "DebugMOde=" << debug << endl;
	}
	if(argc<2) {
		do {
			string filename = OpenFileDialogA();
			cout << filename << endl;
			if (filename != "") { img = imread(filename); break; }
			else { cout << "arUkiddingMe?" << endl; if (++cnt > 2)exit(1); }
		} while (1);
	}

	Teeth t(img,K,dist);
	t.DrawRect(debug);
	imshow("teeth",img);
	waitKey(0);
	return 0;
}