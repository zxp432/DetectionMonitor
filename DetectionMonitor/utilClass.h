#pragma once
#include <math.h>

namespace UtilSpace {
	using namespace System::Collections::Generic;
	using namespace System::Drawing;
	using namespace System;
	//���ڴ洢���������صļ����
	ref struct Result
	{
		int x1;
		int y1;
		int x2;
		int y2;
		System::String ^score;
		System::String ^cls;  /*'aeroplane', 'bicycle', 'bird', 'boat',
							  'bottle', 'bus', 'car', 'cat', 'chair',
							  'cow', 'diningtable', 'dog', 'horse',
							  'motorbike', 'person', 'pottedplant',
							  'sheep', 'sofa', 'train', 'tvmonitor'*/
	};

	ref struct Point
	{
		int x;
		int y;
	public:Point() {
		this->x = 0;
		this->y = 0;
	}
	public:Point(int x, int y) {
		this->x = x;
		this->y = y;
	}

		   static double determinant(double v1, double v2, double v3, double v4)  // ����ʽ  
		   {
			   return (v1*v4 - v2*v3);
		   }

	public:static bool isIntersect(Point ^aa, Point ^bb, Point ^cc, Point ^dd)
	{
		double delta = determinant(bb->x - aa->x, cc->x - dd->x, bb->y - aa->y, cc->y - dd->y);
		if (delta <= (1e-6) && delta >= -(1e-6))  // delta=0����ʾ���߶��غϻ�ƽ��  
		{
			return false;
		}
		double namenda = determinant(cc->x - aa->x, cc->x - dd->x, cc->y - aa->y, cc->y - dd->y) / delta;
		if (namenda > 1 || namenda < 0)
		{
			return false;
		}
		double miu = determinant(bb->x - aa->x, cc->x - aa->x, bb->y - aa->y, cc->y - aa->y) / delta;
		if (miu > 1 || miu < 0)
		{
			return false;
		}
		return true;
	}
	};

	//���ڴ洢�û������ļ����������ͼ�εĸ���
	ref class Region {
	public:bool drawing;
	public:bool newDraw;
	public:Region() {
		this->drawing = true;
		this->newDraw = true;
	}
	public:virtual int mouseDown(int x, int y) { return 1; }
	public:virtual int mouseUp(int x, int y) { return 1; }
	public:virtual int mouseMove(System::Windows::Forms::PictureBox^ frameShowBox, System::Windows::Forms::MouseEventArgs^  e, int frameWidth, int frameHeight) { return 1; }
	public:virtual void draw(IplImage *frameToShow, int r, int g, int b) {}
	public:virtual bool areTwoAreasOverlapped(Result ^rect) { return false; }
	public:virtual void drawIncomplete(IplImage *frameToShow) {}
	public:virtual float timeToAlarm(Result ^result, cv::Point2f speed) { return 0; }//����result���뱨�������ʱ��
	public: float timeToAlarm(Result ^result, cv::Point2f speed, int x1, int y1, int x2, int y2) {
		int time = 0;//���뱨�������ʱ��
		float xCenterDistance = (result->x1 + result->x2) / 2.0 - (x1 + x2) / 2.0;//x���������ĵ�ľ���
		float yCenterDistance = (result->y1 + result->y2) / 2.0 - (y1 + y2) / 2.0;//y���������ĵ�ľ���
		if (xCenterDistance * speed.x >= 0 || yCenterDistance * speed.y >= 0)
			return time;//���result���˶������뱨��������˶������෴���򲻻��ཻ
		int totalWidth = abs(result->x1 - result->x2) + abs(x1 - x2);//���������ο��
		int totalLength = abs(result->y1 - result->y2) + abs(y1 - y2);//���������γ���
		float xDistance = abs(xCenterDistance) - totalWidth / 2.0;
		float yDistance = abs(yCenterDistance) - totalLength / 2.0;
		if (xDistance <= 0 && yDistance <= 0)//����������ĵ�ľ���С�ڿ�ͳ���һ�룬��ض��ཻ
			return 0;
		float xRate = abs(speed.x);//x���������
		float yRate = abs(speed.y);//x���������
		float xTime = xDistance / xRate;//��x������뱨�������ʱ��
		float yTime = yDistance / yRate;//���з�����뱨�������ʱ��
		if (xTime < yTime)
		{
			if ((xTime + totalWidth / xRate) > yTime) {//��x����ӽ��뱨�������뿪��ʱ���ڣ����y������뱨���������������뱨������
				time = yTime;
			}
		}
		else if (yTime < xTime) {
			if ((yTime + totalLength / yRate) > xTime)//��y����ӽ��뱨�������뿪��ʱ���ڣ����x������뱨���������������뱨������
				time = xTime;
		}
		return time;
	}
	};

	//��ؾ�������
	ref class Rectangle :Region {
	public:int x1;
	public:int y1;
	public:int x2;
	public:int y2;
	public:System::Drawing::Point ^startPoint = nullptr, ^endPoint = nullptr;

	public:Rectangle(int x1, int y1, int x2, int y2) :Region() {
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;

	}
	public:Rectangle() :Region() {
		this->x1 = 0;
		this->y1 = 0;
		this->x2 = 0;
		this->y2 = 0;

	}

	public:int mouseDown(int x, int y) override {
		this->startPoint = gcnew System::Drawing::Point(x, y);
		this->endPoint = nullptr;
		this->drawing = true;
		return 0;
	}

	public:int mouseUp(int x, int y) override {
		this->drawing = false;
		this->endPoint = gcnew System::Drawing::Point(x, y);
		this->newDraw = false;
		//���㵱ǰpicturebox�ϵĵ��λ��
		this->x1 = startPoint->X < endPoint->X ? startPoint->X : endPoint->X;
		this->y1 = startPoint->Y < endPoint->Y ? startPoint->Y : endPoint->Y;
		this->x2 = startPoint->X > endPoint->X ? startPoint->X : endPoint->X;
		this->y2 = startPoint->Y > endPoint->Y ? startPoint->Y : endPoint->Y;
		return 1;
	}

	public:int mouseMove(System::Windows::Forms::PictureBox^ frameShowBox, System::Windows::Forms::MouseEventArgs^  e, int frameWidth, int frameHeight) override {
		if (e->Button == System::Windows::Forms::MouseButtons::Left) {
			Graphics ^g = frameShowBox->CreateGraphics();
			g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;//�������  
			frameShowBox->Refresh();
			int x = frameWidth * e->X / frameShowBox->Width;
			int y = frameHeight * e->Y / frameShowBox->Height;
			//�ҳ����ε������Ͻ�
			int leftTopX = this->startPoint->X < x ? this->startPoint->X : x;
			int leftTopY = this->startPoint->Y < y ? this->startPoint->Y : y;

			g->DrawRectangle(gcnew Pen(Color::Blue, 2), leftTopX, leftTopY, Math::Abs(x - this->startPoint->X), Math::Abs(y - this->startPoint->Y));
		}
		return 0;
	}

		   //������
	public:void draw(IplImage *frameToShow, int r, int g, int b) override {
		cvRectangle(frameToShow, cvPoint(this->x1, this->y1), cvPoint(this->x2, this->y2), CV_RGB(r, g, b), 1);
	}

	public:void drawIncomplete(IplImage *frameToShow) override {}

		   //�жϾ��μ�������Ƿ��������غ�
	public:bool areTwoAreasOverlapped(Result ^rect) override {
		float verticalDistance;    //��ֱ����  
		float horizontalDistance;  //ˮƽ����  
		verticalDistance = fabs((this->y1 + this->y2) / 2 - (rect->y1 + rect->y2) / 2);//�����е����
		horizontalDistance = fabs((this->x1 + this->x2) / 2 - (rect->x1 + rect->x2) / 2);//�����е����

		float verticalThreshold;   //�����η���Ĵ�ֱ�ٽ�ֵ  
		float horizontalThreshold; //�����η����ˮƽ�ٽ�ֵ  
		verticalThreshold = (this->y2 - this->y1 + rect->y2 - rect->y1) / 2;
		horizontalThreshold = (this->x2 - this->x1 + rect->x2 - rect->x1) / 2;

		if (verticalDistance > verticalThreshold || horizontalDistance > horizontalThreshold)
			return false;

		return true;
	}
	public: float timeToAlarm(Result ^result, cv::Point2f speed) override {
		return timeToAlarm(result, speed, this->x1, this->y1, this->x2, this->y2);
	}
	};

	//��ؾ�������
	ref class Polygon :Region {
	public:List<UtilSpace::Point ^> ^points;
	public:Polygon() :Region() {
		points = gcnew List<UtilSpace::Point ^>();
	}

		   //���ַ���״̬ -1�ཻ���½��� 0���ڻ� 1����
	public:int mouseDown(int x, int y) override {
		this->points->Add(gcnew UtilSpace::Point(x, y));

		if (this->newDraw) {
			//�ȼ����������¼�ö���ε����о��ε�x1,y1,x2,y2
			this->points->Add(gcnew UtilSpace::Point(x, y));
			this->points->Add(gcnew UtilSpace::Point(x, y));
			this->newDraw = false;
			return 0;
		}
		else {
			//���ж������ӵ��߶���û�к�֮ǰ���߶��ཻ
			bool isIntersect = false;
			for (int i = 3; i < this->points->Count - 2; i++)//���ж����±ߺ�������һ�����Ƿ��غ�
				if (UtilSpace::Point::isIntersect(this->points[i], this->points[i - 1], this->points[this->points->Count - 2], this->points[this->points->Count - 1]))
					isIntersect = true;

			if (isIntersect) {
				this->drawing = false;
				return -1;
			}
			else {
				if (abs(x - this->points[2]->x <= 10 && abs(y - this->points[2]->y <= 10))) {
					drawing = false;
					this->points[this->points->Count - 1]->x = this->points[2]->x;
					this->points[this->points->Count - 1]->y = this->points[2]->y;
					return 1;
				}
				else {
					//�������о��εĲ���
					if (this->points[0]->x > x)
						this->points[0]->x = x;
					if (this->points[0]->y > y)
						this->points[0]->y = y;
					if (this->points[1]->x < x)
						this->points[1]->x = x;
					if (this->points[1]->y < y)
						this->points[1]->y = y;
					return 0;
				}
			}
		}
	}

	public:int mouseUp(int x, int y) override {
		return 0;
	}

	public:int mouseMove(System::Windows::Forms::PictureBox^ frameShowBox, System::Windows::Forms::MouseEventArgs^  e, int frameWidth, int frameHeight) override {
		return 0;
	}

		   //�������
	public:void draw(IplImage *frameToShow, int r, int g, int b) override {
		int i = 3;
		for (; i < this->points->Count; i++)//��֮ǰ���߶�
			cvLine(frameToShow, cvPoint(this->points[i - 1]->x, this->points[i - 1]->y), cvPoint(this->points[i]->x, this->points[i]->y), CV_RGB(r, g, b), 1, CV_AA, 0);
	}

	public:void drawIncomplete(IplImage *frameToShow) override {
		cvCircle(frameToShow, cvPoint(this->points[2]->x, this->points[2]->y), 10, CV_RGB(255, 255, 255), 1, CV_AA, 0);
		int i = 3;
		for (; i < this->points->Count; i++)//��֮ǰ���߶�
			cvLine(frameToShow, cvPoint(this->points[i - 1]->x, this->points[i - 1]->y), cvPoint(this->points[i]->x, this->points[i]->y), CV_RGB(255, 255, 255), 1, CV_AA, 0);
	}

		   //�ж϶���κ;����Ƿ��غϣ������жϼ�����е������Ƿ����û������ļ�������غ�
		   bool areTwoAreasOverlapped(Result ^rect) override
		   {
			   float verticalDistance;    //��ֱ����  
			   float horizontalDistance;  //ˮƽ����  
			   verticalDistance = fabs((this->points[0]->y + this->points[1]->y) / 2 - (rect->y1 + rect->y2) / 2);//�����е����
			   horizontalDistance = fabs((this->points[0]->x + this->points[1]->x) / 2 - (rect->x1 + rect->x2) / 2);//�����е����

			   float verticalThreshold;   //�����η���Ĵ�ֱ�ٽ�ֵ  
			   float horizontalThreshold; //�����η����ˮƽ�ٽ�ֵ  
			   verticalThreshold = (this->points[1]->y - this->points[0]->y + rect->y2 - rect->y1) / 2;
			   horizontalThreshold = (this->points[1]->x - this->points[0]->x + rect->x2 - rect->x1) / 2;

			   if (verticalDistance > verticalThreshold || horizontalDistance > horizontalThreshold) //������ؽ���ľ��������ε����о��β��ཻ
			   {
				   return false;
			   }
			   ////������ؽ���ľ��������ε����о����ཻ,�Ұ�������ε����о��Σ��������ھ�����
			   if (this->points[0]->x >= rect->x1 &&   this->points[0]->y >= rect->y1 &&   this->points[1]->x <= rect->x2 &&   this->points[1]->y <= rect->y2)
				   return true;
			   for (int i = 3; i < this->points->Count; i++)
			   {
				   //�ж϶���ε�ÿ�����Ƿ�������ཻ
				   if (lineAndRecIntersection(this->points[i - 1]->x, this->points[i - 1]->y, this->points[i]->x, this->points[i]->y, rect->x1, rect->y1, rect->x2, rect->y2))
					   return true;//����ཻ��������������ཻ
			   }

			   //�жϾ����е�һ���Ƿ��ڶ�����ڲ�
			   if (pointInPolygon(this->points, rect->x1, rect->y1))
				   return true;
			   return false;

		   }

		   static bool lineAndRecIntersection(int x1, int y1, int x2, int y2, int left, int top, int right, int bottom)
		   {
			   //�ȿ��߶�����ֱ���Ƿ�������ཻ��������ཻ���Ϊ ���ơ���

			   //����ཻ�����߶ε��������Ƿ��ھ��ε�ͬһ�ߣ�������� x��y)  ���궼�Ⱦ��ε�С x(y) ����С�����ߴ�,

			   //����ͬһ����Ϊ���ơ�����������ཻ�������
			   int a = y1 - y2;
			   int b = x2 - x1;
			   int c = x1*y2 - x2*y1;
			   if ((a* left + b*top + c >= 0 && a* right + b* bottom + c <= 0) ||
				   (a* left + b*top + c <= 0 && a* right + b* bottom + c >= 0) ||
				   (a* left + b*bottom + c >= 0 && a* right + b* top + c <= 0) ||
				   (a* left + b*bottom + c >= 0 && a* right + b* top + c <= 0))
			   {
				   if ((x1 < left && x2 < left) || (x1 > right && x2> right) || (y1< top && y2 < top) || (y1> bottom && y2 > bottom))  //�ж��߶��Ƿ��ھ���һ��
				   {
					   return false;
				   }
				   else
				   {
					   return true;
				   }
			   }
			   else
			   {
				   return false;
			   }

		   }
		   //
		   // int    polySides  =  how many cornersthe polygon has
		   // float  polyX[]    =  horizontalcoordinates of corners
		   // float  polyY[]    =  verticalcoordinates of corners
		   // float  x,y       =  point to be tested
		   //
		   // (Globals are used in this example for purposes of speed.  Change as
		   // desired.)
		   //
		   //  The function will return true if the point x,y is inside the polygon, or
		   //  false if it is not.  If the point is exactly on the edge of the polygon,
		   // then the function may return true or false.
		   //
		   // Note that division by zero is avoided because the division is protected
		   //  bythe "if" clause which surrounds it.

		   static bool pointInPolygon(List<Point ^> ^points, int x, int y) {
			   int polySides = points->Count;
			   int   i, j = polySides - 2;
			   bool  oddNodes = false;
			   for (i = 2; i < polySides; i++) {
				   if (points[i]->y < y && points[j]->y >= y
					   || points[j]->y < y&& points[i]->y >= y) {
					   if (points[i]->x + (y - points[i]->y) / (points[j]->y - points[i]->y)*(points[j]->y - points[i]->x) < x) {
						   oddNodes = !oddNodes;
					   }
				   }
				   j = i;
			   }

			   return oddNodes;
		   }
	public: float timeToAlarm(Result ^result, cv::Point2f speed) override {
		int leftTopx = int::MaxValue;
		int leftTopy = int::MaxValue;
		int rightBottomx = -1;
		int rightBottomy = -1;
		//�ҳ��������ε���ӳ����ε��󶥵�����½�
		for each (Point ^point in this->points)
		{
			if (point->x < leftTopx)
				leftTopx = point->x;
			if (point->y < leftTopy)
				leftTopy = point->y;
			if (point->x > rightBottomx)
				rightBottomx = point->x;
			if (point->y > rightBottomy)
				rightBottomy = point->y;
		}
		return timeToAlarm(result, speed, leftTopx, leftTopy, rightBottomx, rightBottomy);
	}
	};

	ref class Circle :Region {
	public:Point ^center;
	public:int r;
	public:System::Drawing::Point ^startPoint = nullptr, ^endPoint = nullptr;
	public: Circle() :Region()
	{
		this->center = gcnew Point(0, 0);
		this->r = 0;
	}
	public:Circle(Point ^center, int r) :Region()
	{
		this->center = center;
		this->r = r;
	}
	public:int mouseDown(int x, int y) override {
		this->startPoint = gcnew System::Drawing::Point(x, y);
		this->endPoint = nullptr;
		this->drawing = true;
		return 0;
	}

	public:int mouseUp(int x, int y) override {
		this->drawing = false;
		int w = Math::Abs(x - this->startPoint->X);
		int h = Math::Abs(y - this->startPoint->Y);
		//��Բ���ѳ�����Ϊ��������С��ֵ
		w = h = Math::Min(w, h);
		this->endPoint = gcnew System::Drawing::Point();
		this->endPoint->X = this->startPoint->X < x ? this->startPoint->X + w : this->startPoint->X - w;
		this->endPoint->Y = this->startPoint->Y < y ? this->startPoint->Y + h : this->startPoint->Y - h;

		this->newDraw = false;
		//���㵱ǰpicturebox�ϵĵ��λ��
		this->r = abs(this->endPoint->X - this->startPoint->X) / 2;
		this->center->x = this->startPoint->X < this->endPoint->X ? this->endPoint->X - this->r : this->startPoint->X - this->r;
		this->center->y = this->startPoint->Y < this->endPoint->Y ? this->endPoint->Y - this->r : this->startPoint->Y - this->r;
		return 1;
	}

	public:int mouseMove(System::Windows::Forms::PictureBox^ frameShowBox, System::Windows::Forms::MouseEventArgs^  e, int frameWidth, int frameHeight) override {
		if (e->Button == System::Windows::Forms::MouseButtons::Left) {
			Graphics ^g = frameShowBox->CreateGraphics();
			g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;//�������  
			frameShowBox->Refresh();
			int x = frameWidth * e->X / frameShowBox->Width;
			int y = frameHeight * e->Y / frameShowBox->Height;
			int h = Math::Abs(y - this->startPoint->Y);
			int w = Math::Abs(x - this->startPoint->X);
			//��Բ���ѳ�����Ϊ��������С��ֵ
			h = w = Math::Min(h, w);
			//�ҳ����ε������Ͻ�
			int leftTopX = this->startPoint->X < x ? this->startPoint->X : this->startPoint->X - w;
			int leftTopY = this->startPoint->Y < y ? this->startPoint->Y : this->startPoint->Y - h;
			g->DrawEllipse(gcnew Pen(Color::Blue, 2), Drawing::Rectangle(leftTopX, leftTopY, w, h));
		}
		return 0;
	}

		   //��Բ��
	public:void draw(IplImage *frameToShow, int r, int g, int b) override {
		cvCircle(frameToShow, cvPoint(this->center->x, this->center->y), this->r, CV_RGB(r, g, b), 1);
	}

	public:void drawIncomplete(IplImage *frameToShow) override {}

	private:int distance(int x1, int y1, int x2, int y2)
	{
		return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
	}
	private:int distancePointAndLine(int x, int y, int x1, int y1, int x2, int y2)
	{
		int result = abs(((x - x1)*(y2 - y1) - (x2 - x1)*(y - y1))) / sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
		return result;
	}
			//�жϾ��μ�������Ƿ��������غ�
	public:bool areTwoAreasOverlapped(Result ^rect) override {
		//������������
		float rectx = (rect->x1 + rect->x2) / 2;
		float recty = (rect->y1 + rect->y2) / 2;
		//����������Բ�����ľ���
		float disCirRect = distance(this->center->x, this->center->y, rectx, recty);
		//���ζԽ���һ��ĳ���
		float disRect = distance(rect->x1, rect->y1, rectx, recty);
		//���γ�
		float widthX = abs(rect->x2 - rect->x1);
		float widthY = abs(rect->y2 - rect->y1);
		float width = widthY;
		if (widthX >= widthY)
			width = widthX;
		//��Բ�뾶���Ͼ��γ���һ����Բ���������������԰�ڣ�������Բ���ཻ��
		if (width / 2 + this->r > disCirRect)
			return true;
		//��Բ�뾶���Ͼ��ζԽ���һ����Բ�����Բ�Ĳ���԰�ڣ�������Բ�β��ཻ��
		if (disRect + this->r < disCirRect)
			return false;

		//��������һ����Բ���������Բ���ཻ
		if (distancePointAndLine(this->center->x, this->center->y, rect->x1, rect->y1, rect->x1, rect->y2) < this->r
			|| distancePointAndLine(this->center->x, this->center->y, rect->x1, rect->y1, rect->x2, rect->y1) < this->r
			|| distancePointAndLine(this->center->x, this->center->y, rect->x2, rect->y1, rect->x2, rect->y2) < this->r
			|| distancePointAndLine(this->center->x, this->center->y, rect->x2, rect->y2, rect->x1, rect->y2) < this->r)
			return true;
		return false;
	}
	public: float timeToAlarm(Result ^result, cv::Point2f speed) override {
		return timeToAlarm(result, speed, center->x - this->r, center->y - this->r, center->x + this->r, center->y + this->r);
	}
	};
}