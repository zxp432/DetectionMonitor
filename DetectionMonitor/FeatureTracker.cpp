#pragma once
#include "opencv2/opencv.hpp"
#include "utilCLass.h"
using namespace cv;
//֡�������  
class FrameProcessor {
public:
	virtual vector<Point2f> process(Mat &input, System::Collections::Generic::List<UtilSpace::Result ^> ^boxes) = 0;
};

//���������࣬�̳���֡�������  
class FeatureTracker : public FrameProcessor {
	Mat gray;  //��ǰ�Ҷ�ͼ  
	Mat gray_prev;  //֮ǰ�ĻҶ�ͼ  
	vector<Point2f> points_prev;
	vector<Point2f> points;//ǰ����֡��������  
	vector<int> point_box;//point_prev�е�ÿ������������Ӧ��box��
	int points_prev_size;//��ǰpoints_prev���Դ洢�Ĵ�С
	const int BOXMINSIZE = 50;//ÿ��box�����ٵ������������
	vector<Point2f> boxes_prev[2];//���ڴ洢��һ��boxes
	vector<int> offsetCount;//ÿ��boxes�������������
	vector<Point2f> initial;//��ʼ������  
	vector<Point2f> features;//��⵽������  
	int max_count; //Ҫ���������������Ŀ  
	double qlevel; //��������ָ��  
	double minDist;//������֮����С���̾���  
	vector<uchar> status; //�����㱻�ɹ����ٵı�־  
	vector<float> err; //����ʱ��������С��������  
public:
	FeatureTracker() :max_count(500), qlevel(0.01), minDist(10.) {
		points_prev.reserve(1000);//���û�����ȷ���һ�����ڴ棬points_prev��֪��Ϊʲô�ᱬ��
		points.reserve(1000);
		point_box.reserve(1000);
		points_prev_size = 0;
		offsetCount.reserve(50);
	}
	vector<Point2f> process1(Mat &frame, System::Collections::Generic::List<UtilSpace::Result ^> ^boxes) {
		//�õ��Ҷ�ͼ  
		cvtColor(frame, gray, CV_BGR2GRAY);
		//frame.copyTo(output);
		//��һ֡  
		if (gray_prev.empty()) {
			gray.copyTo(gray_prev);
		}
		//������̫���ˣ����¼��������  
		//if (addNewPoint()) {
		detectFeaturePoint();
		int debug = 0;
		//�ж�boxes�Ƿ�ı䣬����ı��ˣ���ɾ�����б���������㣬���¼��
		if (boxes_prev[0].size() > 0)
		{
			if (boxes[0]->x1 != boxes_prev[0][0].x || boxes[0]->y1 != boxes_prev[0][0].y || boxes[0]->x2 != boxes_prev[1][0].x || boxes[0]->y2 != boxes_prev[1][0].y) {
				insertPrevBoxes(boxes);
			}
		}
		else {
			insertPrevBoxes(boxes);
		}


		//�����⵽�������� 
		vector<Point2f> offset(boxes->Count);//ÿ��boxes��ƫ����
		//�����������������㣬�жϸ��������������box��������û���õ�������
		debug = points_prev.size();
		if (boxes->Count == 0)
		{
			points_prev.clear();//���boxes������Ϊ0����Ҫ���������һ֡��������
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

		//����ǰ����֡�Ҷ�ͼ����ǰһ֡�������ڵ�ǰ֡��λ��  
		//Ĭ�ϴ�����15*15  
		if (points_prev.size() == 0)
			points_prev.insert(points_prev.end(), features.begin(), features.end());
		debug = points_prev.size();
		calcOpticalFlowPyrLK(
			gray_prev,//ǰһ֡�Ҷ�ͼ  
			gray,//��ǰ֡�Ҷ�ͼ  
			points_prev,//ǰһ֡������λ��  
			points,//��ǰ֡������λ��  
			status,//�����㱻�ɹ����ٵı�־  
			err);//ǰһ֡�������С����͵�ǰ������С�����Ĳ���ݲ�Ĵ�С��ɾ����Щ�˶��仯���ҵĵ�  
		int k = 0;
		//ȥ����Щδ�ƶ���������
		//point_box.clear();
		int ppppp = points.size();
		int ppppprev = points_prev.size();
		if (debug != ppppprev)
			int mm = 1;
		resetOffsetCount(0);
		if (boxes->Count > 0)//�������boxes->Count���жϣ�������ν��ѭ��
		{
			for (int i = 0; i < points.size(); i++) {
				//����ÿ��box��������ƫ������ȷ����������box��ƫ����
				int selectBox = selectMinBox(points_prev[i], boxes);
				if (selectBox >= 0) {
					offset[selectBox] += points[i] - points_prev[i];
					Point2f debug = offset[selectBox];
					offsetCount[selectBox]++;
				}
			}
		}
		//��һ��������ƫ����
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
		//��Ǳ����ٵ�������  
		//handleTrackedPoint(frame, output);
		//Ϊ��һ֡���ٳ�ʼ�������㼯�ͻҶ�ͼ��  
		std::swap(points, points_prev);
		cv::swap(gray_prev, gray);
		return offset;
	}

	vector<Point2f> process(Mat &frame, System::Collections::Generic::List<UtilSpace::Result ^> ^boxes) {
		vector<Point2f> offset(boxes->Count);//ÿ��boxes��ƫ����
		if (boxes->Count == 0)
			return offset;
		int mmm = offset.size();
		int debug = points_prev.size();
		int debug2 = boxes_prev[0].size();
		int debug3 = boxes_prev[1].size();
		int debug4 = point_box.size();
		int debug5 = offsetCount.size();
		//�õ��Ҷ�ͼ  
		cvtColor(frame, gray, CV_BGR2GRAY);
		//frame.copyTo(output);
		//��һ֡  
		if (gray_prev.empty()) {
			gray.copyTo(gray_prev);
		}
		//������̫���ˣ����¼��������
		//�ж�boxes�Ƿ�ı䣬����ı��ˣ���ɾ�����б���������㣬���¼��
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

		//����ǰ����֡�Ҷ�ͼ����ǰһ֡�������ڵ�ǰ֡��λ��
		if (points_prev.size() == 0)//��������㶼����boxes���棬��ֱ�ӷ���
			return offset;
		int mmmm_prev= points_prev.size();
		calcOpticalFlowPyrLK(
			gray_prev,//ǰһ֡�Ҷ�ͼ  
			gray,//��ǰ֡�Ҷ�ͼ  
			points_prev,//ǰһ֡������λ��  
			points,//��ǰ֡������λ��  
			status,//�����㱻�ɹ����ٵı�־  
			err);//ǰһ֡�������С����͵�ǰ������С�����Ĳ���ݲ�Ĵ�С��ɾ����Щ�˶��仯���ҵĵ�  
		int mmmm = points_prev.size();
		if (mmmm_prev != mmmm)
			int ddddddddd = 0;
		for (int i = 0; i < points.size(); i++) {
			//����ÿ��box��������ƫ������ȷ����������box��ƫ����
			if (i >= point_box.size())
				int mmmmm = point_box.size();
			if (point_box[i] >= offset.size())
			{
				int mmmm = point_box[i];
				int mm = offset.size();
				for (int kk = 0; kk < point_box.size(); kk++)
				{
					int ddddd = point_box[kk];
					if (i > 0)
					{
						int dd = point_box[kk - 1];
						if (dd != ddddd) {
							int mmmm = point_box.size();
						}
					}
				}
			}
			if (point_box[i] >= 0) {
				offset[point_box[i]] += points[i] - points_prev[i];
			}
		}
		//��һ��������ƫ����
		for (int boxIndex = 0; boxIndex < boxes->Count; boxIndex++)
		{
			if (boxIndex >= offsetCount.size())
				int mmmm = offsetCount.size();
			if (boxIndex >= offset.size())
				int mmmm = offset.size();
			if (boxIndex >= boxes_prev[0].size())
				int mmmm = boxes_prev[0].size();
			if (offsetCount[boxIndex] != 0) {
				offset[boxIndex].x = (int)(offset[boxIndex].x / offsetCount[boxIndex]);
				offset[boxIndex].y = (int)(offset[boxIndex].y / offsetCount[boxIndex]);
			}
			boxes_prev[0][boxIndex].x += offset[boxIndex].x;
			boxes_prev[1][boxIndex].x += offset[boxIndex].x;
			boxes_prev[0][boxIndex].y += offset[boxIndex].y;
			boxes_prev[1][boxIndex].y += offset[boxIndex].y;
		}
		//Ϊ��һ֡���ٳ�ʼ�������㼯�ͻҶ�ͼ��  
		std::swap(points, points_prev);
		cv::swap(gray_prev, gray);
		return offset;
	}

	void detectFeaturePoint() {
		goodFeaturesToTrack(gray_prev,//����ͼƬ  
			features,//���������  
			max_count,//�����������Ŀ  
			qlevel,//����ָ��  
			minDist);//��С���̾���  
	}
	bool addNewPoint() {
		//����������Ŀ����10����������������  
		return points_prev.size() <= 10;
	}

	//����������ǰ����֡�ƶ��ˣ�����Ϊ�õ���Ŀ��㣬�ҿɱ�����  
	bool acceptTrackedPoint(int i) {
		return status[i] &&
			(abs(points_prev[i].x - points[i].x) +
				abs(points_prev[i].y - points[i].y) > 2);
	}

	//���õ��ڼ������ķ����ڣ�����Ϊ�õ���Ŀ��㣬�ҿɱ�����  
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
	//��������  
	void  handleTrackedPoint(Mat &frame, Mat &output) {
		for (int i = 0; i < points.size(); i++) {
			//��ǰ�����㵽��ʼλ����ֱ�߱�ʾ  
			line(output, initial[i], points[i], Scalar::all(0));
			//��ǰλ����Ȧ���  
			circle(output, points[i], 3, Scalar::all(0), (-1));
		}
	}
	//�������г�Ա
	void finilise() {
		gray.release();  //��ǰ�Ҷ�ͼ  
		gray_prev.release();  //֮ǰ�ĻҶ�ͼ  
		points_prev.clear();//ǰ����֡��������  
		points.clear();
		initial.clear();//��ʼ������  
		features.clear();//��⵽������ 
		status.clear(); //�����㱻�ɹ����ٵı�־  
		err.clear(); //����ʱ��������С��������  
		point_box.clear();
		offsetCount.clear();
	}

	//��boxes��������
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

	//����һ֡����������boxes��Ӧ
	void reflectPointBox(System::Collections::Generic::List<UtilSpace::Result ^> ^boxes) {

		//���½���һ֡��⵽����������box��Ӧ
		for (vector<Point2f>::iterator iter = points_prev.begin(); iter != points_prev.end(); )
		{
			int selectBox = selectMinBox(*iter, boxes);
			if (selectBox != -1) {
				if (selectBox >= boxes->Count)
					int mm = boxes->Count;
				point_box.insert(point_box.end(), selectBox);
				offsetCount[selectBox]++;
				iter++;
			}
			else {
				iter = points_prev.erase(iter);
			}
		}
	}

	//�ҳ�����p�������box
	int selectMinBox(Point2f p, System::Collections::Generic::List<UtilSpace::Result ^> ^boxes) {
		float minDistance = float::MaxValue;//��������ĳ��box�����ĵ��������
		int selectBox = -1;//�������������box���±�
		int debug = offsetCount.size();
		for (int boxIndex = 0; boxIndex < boxes->Count; boxIndex++) {
			if (boxIndex >= offsetCount.size())
				int mmmm = offsetCount.size();
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
		if (selectBox >= boxes->Count)
			int mm = boxes->Count;
		return selectBox;
	}

	//����offsetCount
	void resetOffsetCount(int value) {
		for (vector<int>::iterator iter = offsetCount.begin(); iter != offsetCount.end(); iter++)
			*iter = 0;
	}
};