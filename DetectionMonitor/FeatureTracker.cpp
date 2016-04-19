#pragma once
#include "opencv2/opencv.hpp"
#include "utilCLass.h"
using namespace cv;
//帧处理基类  
class FrameProcessor {
public:
	virtual vector<Point2f> process(Mat &input, System::Collections::Generic::List<UtilSpace::Result ^> ^boxes) = 0;
};

//特征跟踪类，继承自帧处理基类  
class FeatureTracker : public FrameProcessor {
	Mat gray;  //当前灰度图  
	Mat gray_prev;  //之前的灰度图  
	vector<Point2f> points_prev;
	vector<Point2f> points;//前后两帧的特征点  
	//vector<int> point_box;//point_prev中的每个特征点所对应的box框
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
		//point_box.resize(1000);
		points_prev_size = 0;
	}
	vector<Point2f> process(Mat &frame, System::Collections::Generic::List<UtilSpace::Result ^> ^boxes) {
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
		if (boxes->Count > 0) {
			if (boxes_prev[0].size() > 0)
			{
				if (boxes[0]->x1 != boxes_prev[0][0].x || boxes[0]->y1 != boxes_prev[0][0].y || boxes[0]->x2 != boxes_prev[1][0].x || boxes[0]->y2 != boxes_prev[1][0].y) {
					insertPrevBoxes(boxes);
				}
			}
			else {
				insertPrevBoxes(boxes);
			}
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
		if(points_prev.size() == 0)
			points_prev.insert(points_prev.end(), features.begin(), features.end());
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
		resetOffsetCount(0);
		for (int i = 0; i < points.size(); i++) {
			//计算每个box中特征的偏移量，确定最终整个box的偏移量
			/*for (int boxIndex = 0; boxIndex < boxes->Count; boxIndex++)
			{
			if (initial[i].x > boxes[boxIndex]->x1 || initial[i].x < boxes[boxIndex]->x2 || initial[i].y > boxes[boxIndex]->y1|| initial[i].y < boxes[boxIndex]->y2)
			{
			offset[boxIndex] += (points[i] - initial[i]);
			offsetCount[boxIndex]++;
			}
			}*/
			int selectBox = selectMinBox(points_prev[i], boxes);
			if (selectBox >= 0) {
				offset[selectBox] += points[i] - points_prev[i];
				Point2f debug = offset[selectBox];
				offsetCount[selectBox]++;
			}
			/*if (acceptTrackedPoint(i)) {
				initial[k] = initial[i];
				points[k++] = points[i];
			}*/
		}
		//归一化特征点偏移量
		for (int boxIndex = 0; boxIndex < boxes->Count; boxIndex++)
		{
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
	}

	//将boxes保存起来
	void insertPrevBoxes(System::Collections::Generic::List<UtilSpace::Result ^> ^boxes) {
		boxes_prev[0].clear();
		boxes_prev[1].clear();
		offsetCount.clear();
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
		int debug = points_prev.size();
		for (vector<Point2f>::iterator iter = points_prev.begin(); iter != points_prev.end(); )
		{
			int selectBox = selectMinBox(*iter, boxes);
			if (selectBox != -1) {
				//point_box.insert(point_box.end(), selectBox);
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