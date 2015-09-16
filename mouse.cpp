#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>
#include <math.h>

#define WINDOW_FLAGS CV_WINDOW_KEEPRATIO | CV_WINDOW_NORMAL | CV_GUI_EXPANDED
#define ORIG_NAME "Original"
#define FG_NAME "Foreground"

#define LINE_THICKNESS 2

class ROI {
	private:
		cv::Point p1;
		cv::Point p2;
		
		bool holding;
		bool keycrtl;
		cv::Mat orig;	// original image
		cv::Mat drawn;	// original that will hold the displayed image
		cv::Mat mask;
		
		// rectangle colour (BGR)
		cv::Scalar colour;
	
	public:
		ROI(cv::Mat image){
			holding = false;
			keycrtl = false; // TODO: usar isto para desenhar o bg mask
			// fg mask n vale a pena pq as tatuagens podem ser mt finas
			orig = image;
			mask = cv::Mat(image.size(), CV_8UC1, cv::GC_PR_FGD);
			// set colour of rectangle
			colour = cv::Scalar(0,0,255);
		}
		
		void click(cv::Point p){
			hover(p);
			holding = !holding;
			if (holding){
				p1 = p;
			} else {
				p2 = p;
			}
		}
		
		void draw(cv::Point p){
			hover(p);
			keycrtl = !keycrtl;
		}
		
		void hover(cv::Point p){
			if (holding){
				drawn = orig.clone();
				cv::rectangle(drawn, p1, p, colour, LINE_THICKNESS);
				cv::imshow(ORIG_NAME, drawn);
			} else if (keycrtl) {
				cv::circle(drawn,
					p,
					3,
					cv::Scalar( 255, 0, 0 ),
					-1,
					8);
				cv::circle(mask,
					p,
					3,
					cv::GC_BGD,
					-1,
					8);
				cv::imshow(ORIG_NAME, drawn);
			}
		}
		
		cv::Rect get_rect(){
			return cv::Rect(p1,p2);
		}
		
		cv::Mat get_mask(){
			return mask;
		}
};

void mouseHandler(int event, int x, int y, int flags, void* param)
{
	//circle(Mat& img, Point center, int radius, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
	ROI * rect = (ROI *) param;
	
	switch(event){
		case CV_EVENT_LBUTTONDOWN:
			if(flags & CV_EVENT_FLAG_CTRLKEY){
				rect->draw(cv::Point(x,y));
			} else {
				rect->click(cv::Point(x,y));
			}
			break;
		case CV_EVENT_LBUTTONUP:
			if(flags & CV_EVENT_FLAG_CTRLKEY) {
				rect->draw(cv::Point(x,y));
			} else {
				rect->click(cv::Point(x,y));
			}
			break;
		default:
			rect->hover(cv::Point(x,y));
	}
}

void trackHandler(int pos, void* param){
	cv::VideoCapture * capture = (cv::VideoCapture *) param;
	capture->set(CV_CAP_PROP_POS_FRAMES, pos);
}

int main(int argc, char ** argv){
	
  	std::string inputFileName;
  	if (argc>1)
  		inputFileName = argv[1];
  	else
  		return 0;
  	
  	cv::Mat image = cv::imread(inputFileName);
  	cv::Mat imageHSV;
  	
  	if(image.empty()){ 
  		std::cout << "Couldn't open " << inputFileName << std::endl; 
  		return -1;
  	}
  	cv::cvtColor(image, imageHSV, CV_BGR2HSV);
  	
  	// create window and UI
  	cv::namedWindow(ORIG_NAME, WINDOW_FLAGS);
  	cv::namedWindow(FG_NAME, WINDOW_FLAGS);
  	
  	ROI rect = ROI(image);
  	
  	// event handlers
  	//int mouseParam = CV_EVENT_FLAG_LBUTTON;
  	cv::setMouseCallback(ORIG_NAME,mouseHandler, &rect);
  	
  	cv::imshow(ORIG_NAME, image);
	}
