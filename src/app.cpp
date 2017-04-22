/*
 * =====================================================================================
 *
 *       Filename:  app.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/22/2017 20:46:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ada Kaminkure (AdaCode), ada@adacode.io
 *        Company:  ADACODE.IO
 *
 * =====================================================================================
 */

#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

struct mouseParams{
	Mat img;
	string windowName;
	vector<Point2f> pst;
};

void myMouseCallback(int event, int x, int y, int flags, void *userdata)
{
	if(event == EVENT_LBUTTONDOWN)
	{
		mouseParams* mp = (mouseParams*)userdata;
		Mat img(mp->img);
		Point2f center(x,y); 
		mp->pst.push_back(center);
		int rad = 10;
		Scalar red(0, 0, 255);
		Scalar blue(255, 0, 0);
		int thickness = 2;
		string strPoint = "( " + to_string(center.x) + ", " + to_string(center.y) + ")";
		circle(img, center, rad, red, thickness);
		//putText(img, strPoint, Point(center.x, center.y-3), FONT_HERSHEY_SIMPLEX, 1, blue, 4);
		cout << "Click on " + mp->windowName + " : " << center << endl;
		imshow(mp->windowName, img);
	}
}

int main()
{
	int input = 0;
	mouseParams srcParams, dstParams;
	Mat src = imread("public/brain1.jpg");	
	Mat dst = imread("public/brain2.jpg");
	resize(src, src, Size(500,500), INTER_AREA);
	resize(dst, dst, Size(500,500), INTER_AREA);
	namedWindow("Src", 0);
	namedWindow("Dst", 0);
	
	srcParams.img = src.clone();
	srcParams.windowName = "Src";
	dstParams.img = dst.clone();
	dstParams.windowName = "Dst";

	setMouseCallback("Src", myMouseCallback, (void*)&srcParams);
	setMouseCallback("Dst", myMouseCallback, (void*)&dstParams);
	imshow("Src", src);
	imshow("Dst", dst);
	bool loop = true;
	int ctrlPts = 8;
	while(loop)
	{
		if(srcParams.pst.size() == ctrlPts && dstParams.pst.size() == ctrlPts )
		{
			loop = false;
			Mat h = findHomography(srcParams.pst, dstParams.pst);
			Mat output;
			double alpha, beta;
			alpha = 0.5;
			beta = 1 - alpha;
			warpPerspective(src, output, h, dst.size());

			namedWindow("Output", 0);
			imshow("Output", output);

			addWeighted(dst, alpha,	output, beta, 10.0, output);
			namedWindow("Merged", 0);
			imshow("Merged", output );
		}
		waitKey(1000);
	}
	waitKey(0);
	return 0;
}

