#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;


void CallBackFunc2(int event, int x, int y, int flags, void* userdata)
{
	if (flags == (EVENT_FLAG_CTRLKEY + EVENT_FLAG_LBUTTON))
	{
		cout << "Left mouse button is clicked while pressing CTRL key - position (" << x << ", " << y << ")" << endl;
	}
	else if (flags == (EVENT_FLAG_RBUTTON + EVENT_FLAG_SHIFTKEY))
	{
		cout << "Right mouse button is clicked while pressing SHIFT key - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == EVENT_MOUSEMOVE && flags == EVENT_FLAG_ALTKEY)
	{
		cout << "Mouse is moved over the window while pressing ALT key - position (" << x << ", " << y << ")" << endl;
	}
}

void CallBackFunc(int evt, int x, int y, int flags, void* param)
{
	Mat* rgb = (Mat*)param;
	if (evt == CV_EVENT_LBUTTONDOWN)
	{
		printf("%d %d: %d, %d, %d\n",
			x, y,
			(int)(*rgb).at<Vec3b>(y, x)[0],
			(int)(*rgb).at<Vec3b>(y, x)[1],
			(int)(*rgb).at<Vec3b>(y, x)[2]);
	}
}

int main()
{
	// Read image from file 
	Mat img = imread("./img/1.jpg");

	//if fail to read the image
	if (img.empty())
	{
		cout << "Error loading the image" << endl;
		return -1;
	}

	//Create a window
	namedWindow("ImageDisplay", 1);

	//set the callback function for any mouse event
	setMouseCallback("ImageDisplay", CallBackFunc, &img);

	//show the image
	imshow("ImageDisplay", img);

	// Wait until user press some key
	waitKey(0);

	return 0;

}
