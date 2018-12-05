// This is a demo introduces you to reading a video and camera 
#include <iostream>
#include <string>
#include <sstream>
#include <conio.h>
using namespace std;

// OpenCV includes
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp> // for camera
using namespace cv;


/*--------------------------------定义鼠标事件--画直线--------------------------*/
bool got_line = false;
//全局变量
Point beginPoint = Point(0, 0);//--注意这个有一个初始化的(0,0)
bool got_beigin_point = false;
Point endPoint = Point(0, 0);//--注意这个有一个自己默认的初始化(0,0)
void mouseLineHandler(int event, int x, int y, int flags, void *param)
{
	switch (event)
	{
		case CV_EVENT_LBUTTONDOWN:
			beginPoint = Point(x, y);
			endPoint = beginPoint;
			got_beigin_point = true;
			break;
		case CV_EVENT_MOUSEMOVE:
			if (got_beigin_point)
			{
				endPoint = Point(x, y);
			}
			break;
		case CV_EVENT_LBUTTONUP:
			got_line = true;
			endPoint = Point(x, y);
			got_beigin_point = false;
			break;
		default:
			break;
	}
}
// OpenCV command line parser functions
// Keys accepted by command line parser
const char* keys = 
{
    "{help h usage ? | | print this message}"
    "{@video |   | Video file, if not defined try to use webcamera}"
	"{camera | 0 | the saved video by opencv}"
	"{sourceFile | 0 | the source video by opencv}"
	"{dstFile | save.avi | the saved video by opencv}"
};

int main(int argc, const char** argv)
{
    CommandLineParser parser(argc, argv, keys);
    parser.about("Reading a video and camera v1.0.0");

    // If requires help show
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }
	String videoFile = parser.get<String>("sourceFile");
	string fileName = parser.get<string>("dstFile");
	char flag;
	int ch;
	int num = 0;
    // Check if params are correctly parsed in his variables
    if (!parser.check())
    {
        parser.printErrors();
        return 0;
    }

    VideoCapture cap; 
	VideoWriter writer;
	writer.open(fileName, CV_FOURCC('P', 'I', 'M', '1'), 30.0, Size(640, 480),true);
    if (videoFile != "0")
    {
        cap.open(videoFile);// read a video file
    }
	else 
	{
        cap.open(0);// read the default camera
    }
    if (!cap.isOpened())// check if we succeeded
    {
        return -1;
    }
	cout << "请摁下空格键，开始录制视频,摁ESC退出录制" << endl; 
	namedWindow("Video", 1);
	setMouseCallback("Video", mouseLineHandler, NULL);//定义鼠标回调事件	
	while (true)
	{
		if (_kbhit() && _getch() == 32)//如果有键盘摁下，该函数返回真
		{
			++num;
		}
		else if ((_kbhit() && _getch() == 27))//摁下ESC、
		{
			cv::destroyWindow("Video");
			cap.release();//录像结束
			cout << "视频录制结束，摁r键并回车进行回放" << endl;
			break;
		}
		Mat frame;
		switch(num%2)
		{
	    	case 0:
		        break;			
			case 1:
							
				cap >> frame; // get a new frame from camera
				//画直线
				if (got_beigin_point || got_line)
				{
					line(frame, beginPoint, endPoint, Scalar(0, 0, 255), 2);
				}
				
				imshow("Video", frame);
				writer << frame;//write a new frame in an output file
				imshow("Video", frame);
				waitKey(1000 / 30);
				break;
			
			default:
				break;
			
		}
		
		
	}
	cin >> flag;
	if (flag == 'r')
	{
		cap.open(".//"+fileName);
		while (1)
		{
			Mat frame;
			cap >> frame;
			if (!frame.data)
			{
				break;
			}
			imshow("reShowVideo", frame);
			waitKey(1000 / 30);
		}
		cout << "视频回放结束！" << endl;
		cv::destroyWindow("reShowVideo");
	} 
	else
	{
		cout << "不需要回放，直接退出" << endl;
	}
	system("pause");
    // Release the camera or video file
	writer.release();
    cap.release();
    return 0;
	
}