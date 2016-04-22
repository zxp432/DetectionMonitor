#pragma once
#include <math.h>

namespace UtilSpace {
	using namespace System::Collections::Generic;
	using namespace System::Drawing;
	using namespace System;
	//用于存储服务器返回的检测结果
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

		   static double determinant(double v1, double v2, double v3, double v4)  // 行列式  
		   {
			   return (v1*v4 - v2*v3);
		   }

	public:static bool isIntersect(Point ^aa, Point ^bb, Point ^cc, Point ^dd)
	{
		double delta = determinant(bb->x - aa->x, cc->x - dd->x, bb->y - aa->y, cc->y - dd->y);
		if (delta <= (1e-6) && delta >= -(1e-6))  // delta=0，表示两线段重合或平行  
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

	//用于存储用户画出的监控区域三种图形的父类
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
	public:virtual float timeToAlarm(Result ^result, cv::Point2f speed) { return 0; }//计算result进入报警区域的时间
	public: float timeToAlarm(Result ^result, cv::Point2f speed, int x1, int y1, int x2, int y2) {
		int time = 0;//进入报警区域的时间
		float xCenterDistance = (result->x1 + result->x2) / 2.0 - (x1 + x2) / 2.0;//x方向上中心点的距离
		float yCenterDistance = (result->y1 + result->y2) / 2.0 - (y1 + y2) / 2.0;//y方向上中心点的距离
		if (xCenterDistance * speed.x >= 0 || yCenterDistance * speed.y >= 0)
			return time;//如果result的运动方向与报警区域的运动方向相反，则不会相交
		int totalWidth = abs(result->x1 - result->x2) + abs(x1 - x2);//两个长方形宽和
		int totalLength = abs(result->y1 - result->y2) + abs(y1 - y2);//两个长方形长和
		float xDistance = abs(xCenterDistance) - totalWidth / 2.0;
		float yDistance = abs(yCenterDistance) - totalLength / 2.0;
		if (xDistance <= 0 && yDistance <= 0)//如果两个中心点的距离小于宽和长的一半，则必定相交
			return 0;
		float xRate = abs(speed.x);//x方向的速率
		float yRate = abs(speed.y);//x方向的速率
		float xTime = xDistance / xRate;//在x方向进入报警区域的时间
		float yTime = yDistance / yRate;//在有方向进入报警区域的时间
		if (xTime < yTime)
		{
			if ((xTime + totalWidth / xRate) > yTime) {//在x方向从进入报警区域到离开的时间内，如果y方向进入报警区域才真正会进入报警区域
				time = yTime;
			}
		}
		else if (yTime < xTime) {
			if ((yTime + totalLength / yRate) > xTime)//在y方向从进入报警区域到离开的时间内，如果x方向进入报警区域才真正会进入报警区域
				time = xTime;
		}
		return time;
	}
	};

	//监控矩形区域
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
		//计算当前picturebox上的点的位置
		this->x1 = startPoint->X < endPoint->X ? startPoint->X : endPoint->X;
		this->y1 = startPoint->Y < endPoint->Y ? startPoint->Y : endPoint->Y;
		this->x2 = startPoint->X > endPoint->X ? startPoint->X : endPoint->X;
		this->y2 = startPoint->Y > endPoint->Y ? startPoint->Y : endPoint->Y;
		return 1;
	}

	public:int mouseMove(System::Windows::Forms::PictureBox^ frameShowBox, System::Windows::Forms::MouseEventArgs^  e, int frameWidth, int frameHeight) override {
		if (e->Button == System::Windows::Forms::MouseButtons::Left) {
			Graphics ^g = frameShowBox->CreateGraphics();
			g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;//消除锯齿  
			frameShowBox->Refresh();
			int x = frameWidth * e->X / frameShowBox->Width;
			int y = frameHeight * e->Y / frameShowBox->Height;
			//找出矩形的最左上角
			int leftTopX = this->startPoint->X < x ? this->startPoint->X : x;
			int leftTopY = this->startPoint->Y < y ? this->startPoint->Y : y;

			g->DrawRectangle(gcnew Pen(Color::Blue, 2), leftTopX, leftTopY, Math::Abs(x - this->startPoint->X), Math::Abs(y - this->startPoint->Y));
		}
		return 0;
	}

		   //画矩形
	public:void draw(IplImage *frameToShow, int r, int g, int b) override {
		cvRectangle(frameToShow, cvPoint(this->x1, this->y1), cvPoint(this->x2, this->y2), CV_RGB(r, g, b), 1);
	}

	public:void drawIncomplete(IplImage *frameToShow) override {}

		   //判断矩形监控区域是否与检测结果重合
	public:bool areTwoAreasOverlapped(Result ^rect) override {
		float verticalDistance;    //垂直距离  
		float horizontalDistance;  //水平距离  
		verticalDistance = fabs((this->y1 + this->y2) / 2 - (rect->y1 + rect->y2) / 2);//纵轴中点距离
		horizontalDistance = fabs((this->x1 + this->x2) / 2 - (rect->x1 + rect->x2) / 2);//横轴中点距离

		float verticalThreshold;   //两矩形分离的垂直临界值  
		float horizontalThreshold; //两矩形分离的水平临界值  
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

	//监控矩形区域
	ref class Polygon :Region {
	public:List<UtilSpace::Point ^> ^points;
	public:Polygon() :Region() {
		points = gcnew List<UtilSpace::Point ^>();
	}

		   //三种返回状态 -1相交导致结束 0正在画 1画好
	public:int mouseDown(int x, int y) override {
		this->points->Add(gcnew UtilSpace::Point(x, y));

		if (this->newDraw) {
			//先加入两个点记录该多边形的外切矩形的x1,y1,x2,y2
			this->points->Add(gcnew UtilSpace::Point(x, y));
			this->points->Add(gcnew UtilSpace::Point(x, y));
			this->newDraw = false;
			return 0;
		}
		else {
			//先判断新增加的线段有没有和之前的线段相交
			bool isIntersect = false;
			for (int i = 3; i < this->points->Count - 2; i++)//不判断最新边和它的上一条边是否重合
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
					//更改外切矩形的参数
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

		   //画多边形
	public:void draw(IplImage *frameToShow, int r, int g, int b) override {
		int i = 3;
		for (; i < this->points->Count; i++)//画之前的线段
			cvLine(frameToShow, cvPoint(this->points[i - 1]->x, this->points[i - 1]->y), cvPoint(this->points[i]->x, this->points[i]->y), CV_RGB(r, g, b), 1, CV_AA, 0);
	}

	public:void drawIncomplete(IplImage *frameToShow) override {
		cvCircle(frameToShow, cvPoint(this->points[2]->x, this->points[2]->y), 10, CV_RGB(255, 255, 255), 1, CV_AA, 0);
		int i = 3;
		for (; i < this->points->Count; i++)//画之前的线段
			cvLine(frameToShow, cvPoint(this->points[i - 1]->x, this->points[i - 1]->y), cvPoint(this->points[i]->x, this->points[i]->y), CV_RGB(255, 255, 255), 1, CV_AA, 0);
	}

		   //判断多边形和矩形是否重合，用于判断检测结果中的区域是否与用户画出的检测区域重合
		   bool areTwoAreasOverlapped(Result ^rect) override
		   {
			   float verticalDistance;    //垂直距离  
			   float horizontalDistance;  //水平距离  
			   verticalDistance = fabs((this->points[0]->y + this->points[1]->y) / 2 - (rect->y1 + rect->y2) / 2);//纵轴中点距离
			   horizontalDistance = fabs((this->points[0]->x + this->points[1]->x) / 2 - (rect->x1 + rect->x2) / 2);//横轴中点距离

			   float verticalThreshold;   //两矩形分离的垂直临界值  
			   float horizontalThreshold; //两矩形分离的水平临界值  
			   verticalThreshold = (this->points[1]->y - this->points[0]->y + rect->y2 - rect->y1) / 2;
			   horizontalThreshold = (this->points[1]->x - this->points[0]->x + rect->x2 - rect->x1) / 2;

			   if (verticalDistance > verticalThreshold || horizontalDistance > horizontalThreshold) //如果返回结果的矩形与多边形的外切矩形不相交
			   {
				   return false;
			   }
			   ////如果返回结果的矩形与多边形的外切矩形相交,且包含多边形的外切矩形，则多边形在矩形内
			   if (this->points[0]->x >= rect->x1 &&   this->points[0]->y >= rect->y1 &&   this->points[1]->x <= rect->x2 &&   this->points[1]->y <= rect->y2)
				   return true;
			   for (int i = 3; i < this->points->Count; i++)
			   {
				   //判断多边形的每条边是否与矩形相交
				   if (lineAndRecIntersection(this->points[i - 1]->x, this->points[i - 1]->y, this->points[i]->x, this->points[i]->y, rect->x1, rect->y1, rect->x2, rect->y2))
					   return true;//如果相交，则矩形与多边形相交
			   }

			   //判断矩形中的一点是否在多边形内部
			   if (pointInPolygon(this->points, rect->x1, rect->y1))
				   return true;
			   return false;

		   }

		   static bool lineAndRecIntersection(int x1, int y1, int x2, int y2, int left, int top, int right, int bottom)
		   {
			   //先看线段所在直线是否与矩形相交，如果不相交则必为 “Ｆ”，

			   //如果相交，则看线段的两个点是否在矩形的同一边（即两点的 x（y)  坐标都比矩形的小 x(y) 坐标小，或者大）,

			   //若在同一边则为“Ｆ”，否则就是相交的情况。
			   int a = y1 - y2;
			   int b = x2 - x1;
			   int c = x1*y2 - x2*y1;
			   if ((a* left + b*top + c >= 0 && a* right + b* bottom + c <= 0) ||
				   (a* left + b*top + c <= 0 && a* right + b* bottom + c >= 0) ||
				   (a* left + b*bottom + c >= 0 && a* right + b* top + c <= 0) ||
				   (a* left + b*bottom + c >= 0 && a* right + b* top + c <= 0))
			   {
				   if ((x1 < left && x2 < left) || (x1 > right && x2> right) || (y1< top && y2 < top) || (y1> bottom && y2 > bottom))  //判断线段是否在矩形一侧
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
		//找出这个多边形的外接长方形的左顶点和右下角
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
		//画圆，把长宽都设为长宽中最小的值
		w = h = Math::Min(w, h);
		this->endPoint = gcnew System::Drawing::Point();
		this->endPoint->X = this->startPoint->X < x ? this->startPoint->X + w : this->startPoint->X - w;
		this->endPoint->Y = this->startPoint->Y < y ? this->startPoint->Y + h : this->startPoint->Y - h;

		this->newDraw = false;
		//计算当前picturebox上的点的位置
		this->r = abs(this->endPoint->X - this->startPoint->X) / 2;
		this->center->x = this->startPoint->X < this->endPoint->X ? this->endPoint->X - this->r : this->startPoint->X - this->r;
		this->center->y = this->startPoint->Y < this->endPoint->Y ? this->endPoint->Y - this->r : this->startPoint->Y - this->r;
		return 1;
	}

	public:int mouseMove(System::Windows::Forms::PictureBox^ frameShowBox, System::Windows::Forms::MouseEventArgs^  e, int frameWidth, int frameHeight) override {
		if (e->Button == System::Windows::Forms::MouseButtons::Left) {
			Graphics ^g = frameShowBox->CreateGraphics();
			g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;//消除锯齿  
			frameShowBox->Refresh();
			int x = frameWidth * e->X / frameShowBox->Width;
			int y = frameHeight * e->Y / frameShowBox->Height;
			int h = Math::Abs(y - this->startPoint->Y);
			int w = Math::Abs(x - this->startPoint->X);
			//画圆，把长宽都设为长宽中最小的值
			h = w = Math::Min(h, w);
			//找出矩形的最左上角
			int leftTopX = this->startPoint->X < x ? this->startPoint->X : this->startPoint->X - w;
			int leftTopY = this->startPoint->Y < y ? this->startPoint->Y : this->startPoint->Y - h;
			g->DrawEllipse(gcnew Pen(Color::Blue, 2), Drawing::Rectangle(leftTopX, leftTopY, w, h));
		}
		return 0;
	}

		   //画圆形
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
			//判断矩形监控区域是否与检测结果重合
	public:bool areTwoAreasOverlapped(Result ^rect) override {
		//矩形中心坐标
		float rectx = (rect->x1 + rect->x2) / 2;
		float recty = (rect->y1 + rect->y2) / 2;
		//矩形中心与圆型中心距离
		float disCirRect = distance(this->center->x, this->center->y, rectx, recty);
		//矩形对角线一半的长度
		float disRect = distance(rect->x1, rect->y1, rectx, recty);
		//矩形长
		float widthX = abs(rect->x2 - rect->x1);
		float widthY = abs(rect->y2 - rect->y1);
		float width = widthY;
		if (widthX >= widthY)
			width = widthX;
		//以圆半径加上矩形长的一半做圆，如果矩形中心在园内，矩形与圆形相交；
		if (width / 2 + this->r > disCirRect)
			return true;
		//以圆半径加上矩形对角线一半做圆，如果圆心不在园内，矩形与圆形不相交；
		if (disRect + this->r < disCirRect)
			return false;

		//矩形任意一点在圆内则矩形与圆形相交
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