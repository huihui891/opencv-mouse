#include<io.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include "CannyLine.h"

using namespace std;
using namespace cv;

void getFiles(string path, vector<string>& files)
{
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				//if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					//getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void process2(string fileCur)
{
	cv::Mat img = imread(fileCur, 0);

	CannyLine detector;
	std::vector<std::vector<float> > lines;
	detector.cannyLine(img, lines);

	// show
	cv::Mat imgShow(img.rows, img.cols, CV_8UC3, cv::Scalar(255, 255, 255));
	for (int m = 0; m < lines.size(); ++m)
	{
		cv::line(imgShow, cv::Point(lines[m][0], lines[m][1]), cv::Point(lines[m][2], lines[m][3]), cv::Scalar(0, 0, 0), 1, CV_AA);
	}
	imshow("", imgShow);
	imshow("ori", img);
	cv::waitKey(0);
}


int main3()
{

	char * filePath = "F:\\document\\毕业小论文\\障碍物检测\\pic\\3";
	vector<string> files;

	////获取该路径下的所有文件  
	getFiles(filePath, files);

	char str[30];
	int size = files.size();
	for (int i = 0; i < size; i++)
	{
		cout << files[i].c_str() << endl;

		process2(files[i].c_str());
	}

	return 1;

}

