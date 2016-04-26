#pragma once
#include "opencv2/opencv.hpp"
#include "utilCLass.h"
#include <iomanip>
#include <cstring>
using namespace cv;
//帧处理基类  
class FrameProcessor {
private://用于打开处理视频  
		//opencv 视频捕捉对象  
	VideoCapture capture;
	//是否调用回调函数的开关  
	bool callIt;
	//播放每帧之间的暂停间隔  
	int delay;
	//已处理的帧数  
	long fnumber;
	//停止在某一帧  
	long frameToStop;
	//停止处理的开关  
	bool stop;
	//存储获得的视频每帧  
	Mat image;
	//opencv用于视频输出类对象  
	VideoWriter writer;
	//输出文件名  
	string outputFile;
	//输出图片的当前索引值  
	int currentIndex;
	//输出图像的数字编号的位数  
	int digits;
	//扩展名  
	string extension;
	//保存图片文件名  
	vector<string> images;
	//遍历图片vector的迭代器  
	vector<string>::const_iterator itImg;
public:
	FrameProcessor(void) :
		callIt(true),
		delay(0),
		fnumber(0),
		stop(false),
		frameToStop(-1),
		currentIndex(0),
		digits(0)
	{
	}


	~FrameProcessor(void)
	{
	}

	//捕捉设备是否打开  
	bool isOpened() { return capture.isOpened() || !images.empty(); }
	//是否停止处理  
	bool isStopped() { return stop; }
	//停止处理  
	void stopIt() { stop = true; }
	//设置播放延时  
	void setDelay(int d) { delay = d; }
	//设置调用回调函数  
	void callProcess() { callIt = true; }
	//设置不调用回调函数  
	void dontCallProcess() { callIt = false; }
	//设置停留在某帧  
	void stopAtFrameNo(long frame) { frameToStop = frame; }
	//返回输入视频的帧频  
	long  getFrameRate() { return capture.get(CV_CAP_PROP_FPS); }

	//获得图片大小  
	cv::Size getFrameSize()
	{
		cv::Size S;
		return S = Size((int)capture.get(CV_CAP_PROP_FRAME_WIDTH),    //获取输入尺寸  
			(int)capture.get(CV_CAP_PROP_FRAME_HEIGHT));
	}

	bool setInput(string filename)
	{
		fnumber = 0;
		//万一该VideoCapture对象已经关联了一个资源  
		capture.release();
		//images.clear();  
		//打开视频文件  
		return capture.open(filename);

	}

	bool readNextFrame(Mat& frame)
	{
		if (images.size() == 0)
			return capture.read(frame);
		else {
			if (itImg != images.end())
			{
				frame = imread(*itImg);
				itImg++;
				return frame.data != 0;
			}
			else {
				return false;
			}
		}
	}

	//获取帧率
	long getFrameNumber(void)
	{
		long fnumber = static_cast<long>(capture.get(CV_CAP_PROP_POS_FRAMES));
		return fnumber;
	}

	//设置输出视频文件，默认使用于输入视频相同的参数
	bool setOutput(const string& filename, int codec = 0, double framerate = 0.0, bool isColor = true)
	{
		outputFile = filename;
		extension.clear();
		if (framerate == 0.0)
			framerate = getFrameRate();
		char c[4];
		//使用与输出视频相同的编码方式  
		if (codec == 0)
		{
			codec = getCodec(c);
		}

		writer.open(outputFile,
			codec,
			framerate,
			getFrameSize(),
			isColor);
		if (writer.isOpened())
			return true;
		else return false;
	}

	//输出帧
	void writeNextFrame(Mat& frame)
	{
		if (extension.length())
		{
			std::stringstream ss;
			//构成输出文件名  
			ss << outputFile << std::setfill('0')
				<< std::setw(digits)
				<< currentIndex++ << extension;
			imwrite(ss.str(), frame);
		}
		else//否则以视频格式写出  
		{
			writer.write(frame);
		}
	}

	//设置输出为独立的图像文件，扩展名必须是".jpg",".bmp"...
	bool setOutput(const string &filename,//文件前缀  
		const string& ext,//图片文件的扩展名  
		int numberOfDigits = 3,//数据位  
		int startIndex = 0) //开始索引值  
	{
		//数字必须为正  
		if (numberOfDigits < 0)
			return false;
		//文件名及其扩展名  
		outputFile = filename;
		extension = ext;
		//输出图片的编号  
		digits = numberOfDigits;
		//图片数字编号的起始值  
		currentIndex = startIndex;
		return true;
	}


	int getCodec(char codec[4])
	{
		//  
		if (images.size() != 0)
			return -1;
		union {//4-char编码的数据结构  
			int value;
			char code[4];
		}returned;
		//获得编码方式  
		returned.value = static_cast<int>(
			capture.get(CV_CAP_PROP_FOURCC));
		//获得四个字符  
		codec[0] = returned.code[0];
		codec[1] = returned.code[1];
		codec[2] = returned.code[2];
		codec[3] = returned.code[3];
		/*std::cout << "Codec: " << codec[0] << codec[1]
		<< codec[2] << codec[3] << std::endl;*/
		return returned.value;
	}


	bool setInput(const vector<string>& imgs)
	{
		fnumber = 0;
		capture.release();
		//将输入图片放入iamges中  
		images = imgs;
		itImg = images.begin();
		return true;
	}

public:
	virtual vector<Point2f> process(Mat &input, System::Collections::Generic::List<UtilSpace::Result ^> ^boxes) = 0;
};

//特征跟踪类，继承自帧处理基类  
class FeatureTracker : public FrameProcessor {
	Mat gray;  //当前灰度图  
	Mat gray_prev;  //之前的灰度图  
	vector<Point2f> points_prev;
	vector<Point2f> points;//前后两帧的特征点  
	vector<int> point_box;//point_prev中的每个特征点所对应的box框
	int points_prev_size;//当前points_prev中以存储的大小
	const int BOXMINSIZE = 50;//每个box中最少的特征点的数量
	vector<Point2f> boxes_prev[2];//用于存储上一个boxes
	vector<int> offsetCount;//每个boxes中特征点的数量
	vector<Point2f> initial;//初始特征点  
	vector<Point2f> features;//检测到的特征  
	int max_count; //要跟踪特征的最大数目  
	double qlevel; //特征检测的指标  
	double minDist;//特征点之间最小容忍距离  
	vector<uchar> status; //特征点被成功跟踪的标志  
	vector<float> err; //跟踪时的特征点小区域误差和  
public:
	FeatureTracker() :max_count(500), qlevel(0.01), minDist(10.) {
		points_prev.reserve(1000);//如果没有事先分配一部分内存，points_prev不知道为什么会爆掉
		points.reserve(1000);
		point_box.reserve(1000);
		points_prev_size = 0;
		offsetCount.reserve(50);
	}
	vector<Point2f> process1(Mat &frame, System::Collections::Generic::List<UtilSpace::Result ^> ^boxes) {
		//得到灰度图  
		cvtColor(frame, gray, CV_BGR2GRAY);
		//frame.copyTo(output);
		//第一帧  
		if (gray_prev.empty()) {
			gray.copyTo(gray_prev);
		}
		//特征点太少了，重新检测特征点  
		//if (addNewPoint()) {
		detectFeaturePoint();
		int debug = 0;
		//判断boxes是否改变，如果改变了，则删掉所有保存的特征点，重新检测
		if (boxes_prev[0].size() > 0)
		{
			if (boxes[0]->x1 != boxes_prev[0][0].x || boxes[0]->y1 != boxes_prev[0][0].y || boxes[0]->x2 != boxes_prev[1][0].x || boxes[0]->y2 != boxes_prev[1][0].y) {
				insertPrevBoxes(boxes);
			}
		}
		else {
			insertPrevBoxes(boxes);
		}


		//插入检测到的特征点 
		vector<Point2f> offset(boxes->Count);//每个boxes的偏移量
		//遍历检测出来的特征点，判断各个特征点归属的box，并放弃没有用的特征点
		debug = points_prev.size();
		if (boxes->Count == 0)
		{
			points_prev.clear();//如果boxes的数量为0，则要重新添加上一帧的特征点
			points_prev.insert(points_prev.end(), features.begin(), features.end());
		}
		else {
			for (int i = 0; i < features.size(); i++)
			{
				int selectBox = selectMinBox(features[i], boxes);
				if (selectBox != -1) {
					points_prev.insert(points_prev.end(), features[i]);
					//point_box.insert(point_box.end(), selectBox);
				}
			}
		}

		//根据前后两帧灰度图估计前一帧特征点在当前帧的位置  
		//默认窗口是15*15  
		if (points_prev.size() == 0)
			points_prev.insert(points_prev.end(), features.begin(), features.end());
		debug = points_prev.size();
		calcOpticalFlowPyrLK(
			gray_prev,//前一帧灰度图  
			gray,//当前帧灰度图  
			points_prev,//前一帧特征点位置  
			points,//当前帧特征点位置  
			status,//特征点被成功跟踪的标志  
			err);//前一帧特征点点小区域和当前特征点小区域间的差，根据差的大小可删除那些运动变化剧烈的点  
		int k = 0;
		//去除那些未移动的特征点
		//point_box.clear();
		int ppppp = points.size();
		int ppppprev = points_prev.size();
		if (debug != ppppprev)
			int mm = 1;
		resetOffsetCount(0);
		if (boxes->Count > 0)//加入这个boxes->Count的判断，避免无谓的循环
		{
			for (int i = 0; i < points.size(); i++) {
				//计算每个box中特征的偏移量，确定最终整个box的偏移量
				int selectBox = selectMinBox(points_prev[i], boxes);
				if (selectBox >= 0) {
					offset[selectBox] += points[i] - points_prev[i];
					Point2f debug = offset[selectBox];
					offsetCount[selectBox]++;
				}
			}
		}
		//归一化特征点偏移量
		for (int boxIndex = 0; boxIndex < boxes->Count; boxIndex++)
		{
			if (boxIndex >= offsetCount.size())
				int debuggg = 0;
			if (offsetCount[boxIndex] != 0) {
				offset[boxIndex].x = (int)(offset[boxIndex].x / offsetCount[boxIndex]);
				offset[boxIndex].y = (int)(offset[boxIndex].y / offsetCount[boxIndex]);
			}
			debug = offset[boxIndex].x;
			debug = offset[boxIndex].y;
			boxes_prev[0][boxIndex].x += offset[boxIndex].x;
			boxes_prev[1][boxIndex].x += offset[boxIndex].x;
			boxes_prev[0][boxIndex].y += offset[boxIndex].y;
			boxes_prev[1][boxIndex].y += offset[boxIndex].y;
		}
		//points.erase(points.begin() + k, points.end());
		//initial.resize(k);
		//标记被跟踪的特征点  
		//handleTrackedPoint(frame, output);
		//为下一帧跟踪初始化特征点集和灰度图像  
		std::swap(points, points_prev);
		cv::swap(gray_prev, gray);
		return offset;
	}

	vector<Point2f> process(Mat &frame, System::Collections::Generic::List<UtilSpace::Result ^> ^boxes) {
		vector<Point2f> offset(boxes->Count);//每个boxes的偏移量
		if (boxes->Count == 0)
			return offset;
		//得到灰度图  
		cvtColor(frame, gray, CV_BGR2GRAY);
		//frame.copyTo(output);
		//第一帧  
		if (gray_prev.empty()) {
			gray.copyTo(gray_prev);
		}
		//特征点太少了，重新检测特征点
		//判断boxes是否改变，如果改变了，则删掉所有保存的特征点，重新检测
		if (boxes->Count > 0) {
			if (boxes_prev[0].size() > 0)
			{
				if (boxes->Count != boxes_prev[0].size() || boxes[0]->x1 != boxes_prev[0][0].x || boxes[0]->y1 != boxes_prev[0][0].y || boxes[0]->x2 != boxes_prev[1][0].x || boxes[0]->y2 != boxes_prev[1][0].y) {
					detectFeaturePoint();
					points_prev.clear();
					points_prev.insert(points_prev.end(), features.begin(), features.end());
					insertPrevBoxes(boxes);
				}
			}
			else {
				detectFeaturePoint();
				points_prev.clear();
				points_prev.insert(points_prev.end(), features.begin(), features.end());
				insertPrevBoxes(boxes);
			}
		}

		//根据前后两帧灰度图估计前一帧特征点在当前帧的位置
		if (points_prev.size() == 0)//如果特征点都不在boxes里面，则直接返回
			return offset;
		calcOpticalFlowPyrLK(
			gray_prev,//前一帧灰度图  
			gray,//当前帧灰度图  
			points_prev,//前一帧特征点位置  
			points,//当前帧特征点位置  
			status,//特征点被成功跟踪的标志  
			err);//前一帧特征点点小区域和当前特征点小区域间的差，根据差的大小可删除那些运动变化剧烈的点  
		for (int i = 0; i < points.size(); i++) {
			//计算每个box中特征的偏移量，确定最终整个box的偏移量
			if (point_box[i] >= 0) {
				offset[point_box[i]] += points[i] - points_prev[i];
			}
		}
		//归一化特征点偏移量
		for (int boxIndex = 0; boxIndex < boxes->Count; boxIndex++)
		{
			if (offsetCount[boxIndex] != 0) {
				offset[boxIndex].x = (int)(offset[boxIndex].x / offsetCount[boxIndex]);
				offset[boxIndex].y = (int)(offset[boxIndex].y / offsetCount[boxIndex]);
			}
			boxes_prev[0][boxIndex].x += offset[boxIndex].x;
			boxes_prev[1][boxIndex].x += offset[boxIndex].x;
			boxes_prev[0][boxIndex].y += offset[boxIndex].y;
			boxes_prev[1][boxIndex].y += offset[boxIndex].y;
		}
		//为下一帧跟踪初始化特征点集和灰度图像  
		std::swap(points, points_prev);
		cv::swap(gray_prev, gray);
		return offset;
	}

	void detectFeaturePoint() {
		goodFeaturesToTrack(gray_prev,//输入图片  
			features,//输出特征点  
			max_count,//特征点最大数目  
			qlevel,//质量指标  
			minDist);//最小容忍距离  
	}
	bool addNewPoint() {
		//若特征点数目少于10，则决定添加特征点  
		return points_prev.size() <= 10;
	}

	//若特征点在前后两帧移动了，则认为该点是目标点，且可被跟踪  
	bool acceptTrackedPoint(int i) {
		return status[i] &&
			(abs(points_prev[i].x - points[i].x) +
				abs(points_prev[i].y - points[i].y) > 2);
	}

	//若该点在检测出来的方框内，则认为该点是目标点，且可被跟踪  
	bool acceptTrackedPoint(int i, System::Collections::Generic::List<UtilSpace::Result ^> ^boxes) {
		bool isAccept = false;
		for each (UtilSpace::Result ^result in boxes)
		{
			if (points_prev[i].x > result->x1 && points_prev[i].x < result->x2 && points_prev[i].y > result->y1 && points_prev[i].y < result->y2) {
				isAccept = true;
				break;
			}
		}
		return isAccept;
	}
	//画特征点  
	void  handleTrackedPoint(Mat &frame, Mat &output) {
		for (int i = 0; i < points.size(); i++) {
			//当前特征点到初始位置用直线表示  
			line(output, initial[i], points[i], Scalar::all(0));
			//当前位置用圈标出  
			circle(output, points[i], 3, Scalar::all(0), (-1));
		}
	}
	//重置所有成员
	void finilise() {
		gray.release();  //当前灰度图  
		gray_prev.release();  //之前的灰度图  
		points_prev.clear();//前后两帧的特征点  
		points.clear();
		initial.clear();//初始特征点  
		features.clear();//检测到的特征 
		status.clear(); //特征点被成功跟踪的标志  
		err.clear(); //跟踪时的特征点小区域误差和  
		point_box.clear();
		offsetCount.clear();
	}

	//将boxes保存起来
	void insertPrevBoxes(System::Collections::Generic::List<UtilSpace::Result ^> ^boxes) {
		boxes_prev[0].clear();
		boxes_prev[1].clear();
		offsetCount.clear();
		point_box.clear();
		for each (UtilSpace::Result ^ box in boxes)
		{
			Point2f temp[2];
			temp[0].x = box->x1;
			temp[0].y = box->y1;
			temp[1].x = box->x2;
			temp[1].y = box->y2;
			boxes_prev[0].insert(boxes_prev[0].end(), temp[0]);
			boxes_prev[1].insert(boxes_prev[1].end(), temp[1]);
			offsetCount.insert(offsetCount.end(), 0);
		}
		reflectPointBox(boxes);
	}

	//将上一帧的特征点与boxes对应
	void reflectPointBox(System::Collections::Generic::List<UtilSpace::Result ^> ^boxes) {

		//重新将上一帧检测到的特征点与box对应
		for (vector<Point2f>::iterator iter = points_prev.begin(); iter != points_prev.end(); )
		{
			int selectBox = selectMinBox(*iter, boxes);
			if (selectBox != -1) {
				point_box.insert(point_box.end(), selectBox);
				offsetCount[selectBox]++;
				iter++;
			}
			else {
				iter = points_prev.erase(iter);
			}
		}
	}

	//找出距离p点最近的box
	int selectMinBox(Point2f p, System::Collections::Generic::List<UtilSpace::Result ^> ^boxes) {
		float minDistance = float::MaxValue;//特征点与某个box的中心的最近距离
		int selectBox = -1;//该特征点归属的box的下标
		int debug = offsetCount.size();
		for (int boxIndex = 0; boxIndex < boxes->Count; boxIndex++) {
			if (offsetCount[boxIndex] < BOXMINSIZE && p.x > boxes[boxIndex]->x1 && p.x < boxes[boxIndex]->x2 && p.y > boxes[boxIndex]->y1 && p.y < boxes[boxIndex]->y2) {
				float centerX = (boxes[boxIndex]->x1 + boxes[boxIndex]->x2) / 2.0;
				float centerY = (boxes[boxIndex]->y1 + boxes[boxIndex]->y2) / 2.0;
				float distance = System::Math::Sqrt(System::Math::Pow(p.x - centerX, 2) + System::Math::Pow(p.y - centerY, 2));
				if (distance < minDistance) {
					minDistance = distance;
					selectBox = boxIndex;
				}
			}
		}
		return selectBox;
	}

	//重置offsetCount
	void resetOffsetCount(int value) {
		for (vector<int>::iterator iter = offsetCount.begin(); iter != offsetCount.end(); iter++)
			*iter = 0;
	}
};