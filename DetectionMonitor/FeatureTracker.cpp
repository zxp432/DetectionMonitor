#pragma once
#include "opencv2/opencv.hpp"
#include "utilCLass.h"
using namespace cv;
//帧处理基类  
class FrameProcessor {
public:
	virtual vector<Point2f> process(Mat &input, Mat &, System::Collections::Generic::List<UtilSpace::Result ^> ^boxes) = 0;
};

//特征跟踪类，继承自帧处理基类  
class FeatureTracker : public FrameProcessor {
	Mat gray;  //当前灰度图  
	Mat gray_prev;  //之前的灰度图  
	vector<Point2f> points_prev;
	vector<Point2f> points;//前后两帧的特征点  
	vector<Point2f> initial;//初始特征点  
	vector<Point2f> features;//检测到的特征  
	int max_count; //要跟踪特征的最大数目  
	double qlevel; //特征检测的指标  
	double minDist;//特征点之间最小容忍距离  
	vector<uchar> status; //特征点被成功跟踪的标志  
	vector<float> err; //跟踪时的特征点小区域误差和  
public:
	FeatureTracker() :max_count(500), qlevel(0.01), minDist(10.) {
		points_prev.resize(1000);//如果没有事先分配一部分内存，points_prev不知道为什么会爆掉
	}
	vector<Point2f> process(Mat &frame, Mat &output, System::Collections::Generic::List<UtilSpace::Result ^> ^boxes) {
		//得到灰度图  
		cvtColor(frame, gray, CV_BGR2GRAY);
		frame.copyTo(output);
		//特征点太少了，重新检测特征点  
		if (addNewPoint()) {
			detectFeaturePoint();
			//插入检测到的特征点  
			int dddd = points_prev.size();
			int fff = features.size();
			//points[0].resize(points[0].size()+features.size());
			points_prev.insert(points_prev.end(), features.begin(), features.end());
			initial.insert(initial.end(), features.begin(), features.end());
		}
		//第一帧  
		if (gray_prev.empty()) {
			gray.copyTo(gray_prev);
		}
		//根据前后两帧灰度图估计前一帧特征点在当前帧的位置  
		//默认窗口是15*15  
		calcOpticalFlowPyrLK(
			gray_prev,//前一帧灰度图  
			gray,//当前帧灰度图  
			points_prev,//前一帧特征点位置  
			points,//当前帧特征点位置  
			status,//特征点被成功跟踪的标志  
			err);//前一帧特征点点小区域和当前特征点小区域间的差，根据差的大小可删除那些运动变化剧烈的点  
		int k = 0;
		//去除那些未移动的特征点
		vector<Point2f> offset(boxes->Count);//每个boxes的偏移量
		vector<int> offsetCount(boxes->Count);//每个boxes中特征点的数量
		for (int i = 0; i<points.size(); i++) {
			//计算每个box中特征的偏移量，确定最终整个box的偏移量
			/*for (int boxIndex = 0; boxIndex < boxes->Count; boxIndex++)
			{
				if (initial[i].x > boxes[boxIndex]->x1 && initial[i].x < boxes[boxIndex]->x2 && initial[i].y > boxes[boxIndex]->y1&& initial[i].y < boxes[boxIndex]->y2)
				{
					offset[boxIndex] += (points[i] - initial[i]);
					offsetCount[boxIndex]++;
				}
			}*/
			if (acceptTrackedPoint(i)) {
				initial[k] = initial[i];
				points[k++] = points[i];
			}
		}
		//归一化特征点偏移量
		/*for (int boxIndex = 0; boxIndex < boxes->Count; boxIndex++)
		{
			offset[boxIndex].x = offset[boxIndex].x / offsetCount[boxIndex];
			offset[boxIndex].y = offset[boxIndex].y / offsetCount[boxIndex];
		}*/
		points.erase(points.begin() + k, points.end());
		initial.resize(k);
		//标记被跟踪的特征点  
		handleTrackedPoint(frame, output);
		//为下一帧跟踪初始化特征点集和灰度图像  
		std::swap(points, points_prev);
		cv::swap(gray_prev, gray);
		return offset;
	}

	void detectFeaturePoint() {
		goodFeaturesToTrack(gray,//输入图片  
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
				abs(points_prev[i].y - points[i].y) >2);
	}

	//画特征点  
	void  handleTrackedPoint(Mat &frame, Mat &output) {
		for (int i = 0; i<points.size(); i++) {
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
};