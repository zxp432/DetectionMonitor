#pragma once
#include "opencv2/opencv.hpp"
#include "utilCLass.h"
#include <iomanip>
#include <cstring>
using namespace cv;
//֡�������  
class FrameProcessor {
private://���ڴ򿪴�����Ƶ  
		//opencv ��Ƶ��׽����  
	VideoCapture capture;
	//�Ƿ���ûص������Ŀ���  
	bool callIt;
	//����ÿ֮֡�����ͣ���  
	int delay;
	//�Ѵ����֡��  
	long fnumber;
	//ֹͣ��ĳһ֡  
	long frameToStop;
	//ֹͣ����Ŀ���  
	bool stop;
	//�洢��õ���Ƶÿ֡  
	Mat image;
	//opencv������Ƶ��������  
	VideoWriter writer;
	//����ļ���  
	string outputFile;
	//���ͼƬ�ĵ�ǰ����ֵ  
	int currentIndex;
	//���ͼ������ֱ�ŵ�λ��  
	int digits;
	//��չ��  
	string extension;
	//����ͼƬ�ļ���  
	vector<string> images;
	//����ͼƬvector�ĵ�����  
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

	//��׽�豸�Ƿ��  
	bool isOpened() { return capture.isOpened() || !images.empty(); }
	//�Ƿ�ֹͣ����  
	bool isStopped() { return stop; }
	//ֹͣ����  
	void stopIt() { stop = true; }
	//���ò�����ʱ  
	void setDelay(int d) { delay = d; }
	//���õ��ûص�����  
	void callProcess() { callIt = true; }
	//���ò����ûص�����  
	void dontCallProcess() { callIt = false; }
	//����ͣ����ĳ֡  
	void stopAtFrameNo(long frame) { frameToStop = frame; }
	//����������Ƶ��֡Ƶ  
	long  getFrameRate() { return capture.get(CV_CAP_PROP_FPS); }

	//���ͼƬ��С  
	cv::Size getFrameSize()
	{
		cv::Size S;
		return S = Size((int)capture.get(CV_CAP_PROP_FRAME_WIDTH),    //��ȡ����ߴ�  
			(int)capture.get(CV_CAP_PROP_FRAME_HEIGHT));
	}

	bool setInput(string filename)
	{
		fnumber = 0;
		//��һ��VideoCapture�����Ѿ�������һ����Դ  
		capture.release();
		//images.clear();  
		//����Ƶ�ļ�  
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

	//��ȡ֡��
	long getFrameNumber(void)
	{
		long fnumber = static_cast<long>(capture.get(CV_CAP_PROP_POS_FRAMES));
		return fnumber;
	}

	//���������Ƶ�ļ���Ĭ��ʹ����������Ƶ��ͬ�Ĳ���
	bool setOutput(const string& filename, int codec = 0, double framerate = 0.0, bool isColor = true)
	{
		outputFile = filename;
		extension.clear();
		if (framerate == 0.0)
			framerate = getFrameRate();
		char c[4];
		//ʹ���������Ƶ��ͬ�ı��뷽ʽ  
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

	//���֡
	void writeNextFrame(Mat& frame)
	{
		if (extension.length())
		{
			std::stringstream ss;
			//��������ļ���  
			ss << outputFile << std::setfill('0')
				<< std::setw(digits)
				<< currentIndex++ << extension;
			imwrite(ss.str(), frame);
		}
		else//��������Ƶ��ʽд��  
		{
			writer.write(frame);
		}
	}

	//�������Ϊ������ͼ���ļ�����չ��������".jpg",".bmp"...
	bool setOutput(const string &filename,//�ļ�ǰ׺  
		const string& ext,//ͼƬ�ļ�����չ��  
		int numberOfDigits = 3,//����λ  
		int startIndex = 0) //��ʼ����ֵ  
	{
		//���ֱ���Ϊ��  
		if (numberOfDigits < 0)
			return false;
		//�ļ���������չ��  
		outputFile = filename;
		extension = ext;
		//���ͼƬ�ı��  
		digits = numberOfDigits;
		//ͼƬ���ֱ�ŵ���ʼֵ  
		currentIndex = startIndex;
		return true;
	}


	int getCodec(char codec[4])
	{
		//  
		if (images.size() != 0)
			return -1;
		union {//4-char��������ݽṹ  
			int value;
			char code[4];
		}returned;
		//��ñ��뷽ʽ  
		returned.value = static_cast<int>(
			capture.get(CV_CAP_PROP_FOURCC));
		//����ĸ��ַ�  
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
		//������ͼƬ����iamges��  
		images = imgs;
		itImg = images.begin();
		return true;
	}

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
		calcOpticalFlowPyrLK(
			gray_prev,//ǰһ֡�Ҷ�ͼ  
			gray,//��ǰ֡�Ҷ�ͼ  
			points_prev,//ǰһ֡������λ��  
			points,//��ǰ֡������λ��  
			status,//�����㱻�ɹ����ٵı�־  
			err);//ǰһ֡�������С����͵�ǰ������С�����Ĳ���ݲ�Ĵ�С��ɾ����Щ�˶��仯���ҵĵ�  
		for (int i = 0; i < points.size(); i++) {
			//����ÿ��box��������ƫ������ȷ����������box��ƫ����
			if (point_box[i] >= 0) {
				offset[point_box[i]] += points[i] - points_prev[i];
			}
		}
		//��һ��������ƫ����
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

	//����offsetCount
	void resetOffsetCount(int value) {
		for (vector<int>::iterator iter = offsetCount.begin(); iter != offsetCount.end(); iter++)
			*iter = 0;
	}
};