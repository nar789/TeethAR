#include <jni.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
using namespace cv;
using namespace std;

extern "C" {
    JNIEXPORT void JNICALL
    Java_hume_app_teeth_MainActivity_ConvertRGBtoGray(JNIEnv *env, jobject instance, jlong matAddrInput,
                                                      jlong matAddrResult,jint k, jlong matProcess, jstring data) {
        Mat &b = *(Mat *) matAddrInput;
        Mat &matResult = *(Mat *) matAddrResult;
        Mat &process = *(Mat *)matProcess;

        int h=b.rows;
        int w=b.cols;
        while(h>1000){ h/=2;w/=2; }
        resize(b, process, Size(w,h), 0, 0, CV_INTER_LINEAR);

        matResult=b.clone();
        Size size = b.size();
        char str[255];
        sprintf(str, "K=%d", k);
        putText(matResult, str, Point(10, 40), 2, 2.0, Vec3b(255, 0, 0));

        for (int i = 0; i < size.height; i++)
        {
            for (int j = 0; j < size.width; j++) {
                Vec3b& p = b.at<Vec3b>(i, j);
                if ((p[0]>k && p[1]>k && p[2]>k)) { p[0] = 255; p[1] = 255; p[2] = 255; }
                else { p[0] = 0; p[1] = 0; p[2] = 0; }
            }
        }

        Mat gray;
        vector<vector<Point> > contours;
        vector<Point2f> approx;
        cvtColor(b, gray, COLOR_BGR2GRAY);
        threshold(gray, gray, k, 255, THRESH_BINARY_INV | THRESH_OTSU);
        findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);


        for (size_t i = 0; i < contours.size(); i++)
        {
            approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.002, true);
            double a = fabs(contourArea(Mat(approx)));
            if (a>1000 && a<1000000)
            {
                int size = approx.size();
                if (size % 2 == 0) {
                    line(matResult, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 3);

                    for (int k = 0; k < size - 1; k++)
                        line(matResult, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);
                }
                else {
                    line(matResult, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 3);

                    for (int k = 0; k < size - 1; k++)
                        line(matResult, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);
                }
            }
        }
    }
}