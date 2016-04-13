#pragma once

#include <math.h>

namespace UtilSpace {
	using namespace System::Collections::Generic;
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
	};

	//用于存储用户画出的监控区域
	ref class Rectangle {
	public:int x1;
	public:int y1;
	public:int x2;
	public:int y2;


	public:Rectangle(int x1, int y1, int x2, int y2) {
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
	}
		   //判断两个矩形是否重合，用于判断检测结果中的区域是否与用户画出的检测区域重合
		   static bool areTwoAreasOverlapped(List<Point ^> ^poly, Result ^rect)
		   {
			   float verticalDistance;    //垂直距离  
			   float horizontalDistance;  //水平距离  
			   verticalDistance = fabs((poly[0]->y + poly[1]->y) / 2 - (rect->y1 + rect->y2) / 2);//纵轴中点距离
			   horizontalDistance = fabs((poly[0]->x + poly[1]->x) / 2 - (rect->x1 + rect->x2) / 2);//横轴中点距离

			   float verticalThreshold;   //两矩形分离的垂直临界值  
			   float horizontalThreshold; //两矩形分离的水平临界值  
			   verticalThreshold = (poly[1]->y - poly[0]->y + rect->y2 - rect->y1) / 2;
			   horizontalThreshold = (poly[1]->x - poly[0]->x + rect->x2 - rect->x1) / 2;

			   if (verticalDistance > verticalThreshold || horizontalDistance > horizontalThreshold) //如果返回结果的矩形与多边形的外切矩形不相交
			   {
				   return false;
			   }
			   ////如果返回结果的矩形与多边形的外切矩形相交,且包含多边形的外切矩形，则多边形在矩形内
			   if (poly[0]->x >= rect->x1 &&   poly[0]->y >= rect->y1 &&   poly[1]->x <= rect->x2 &&   poly[1]->y <= rect->y2)
				   return true;
			   for (int i = 3; i < poly->Count; i++)
			   {
				   //判断多边形的每条边是否与矩形相交
				   if (lineAndRecIntersection(poly[i - 1]->x, poly[i - 1]->y, poly[i]->x, poly[i]->y, rect->x1, rect->y1, rect->x2, rect->y2))
					   return true;//如果相交，则矩形与多边形相交
			   }

			   //判断矩形中的一点是否在多边形内部
			   if (pointInPolygon(poly, rect->x1, rect->y1))
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
			   for (i = 2; i<polySides; i++) {
				   if (points[i]->y < y && points[j]->y >= y
					   || points[j]->y<y&& points[i]->y >= y) {
					   if (points[i]->x + (y - points[i]->y) / (points[j]->y - points[i]->y)*(points[j]->y - points[i]->x)<x) {
						   oddNodes = !oddNodes;
					   }
				   }
				   j = i;
			   }

			   return oddNodes;
		   }

		   //判断两个矩形是否重合，用于判断检测结果中的区域是否与用户画出的检测区域重合
	public:static bool areTwoRectsOverlapped(Rectangle ^rect1, Result ^rect2)
	{
		float verticalDistance;    //垂直距离  
		float horizontalDistance;  //水平距离  
		verticalDistance = fabs((rect1->y1 + rect1->y2) / 2 - (rect2->y1 + rect2->y2) / 2);//纵轴中点距离
		horizontalDistance = fabs((rect1->x1 + rect1->x2) / 2 - (rect2->x1 + rect2->x2) / 2);//横轴中点距离

		float verticalThreshold;   //两矩形分离的垂直临界值  
		float horizontalThreshold; //两矩形分离的水平临界值  
		verticalThreshold = (rect1->y2 - rect1->y1 + rect2->y2 - rect2->y1) / 2;
		horizontalThreshold = (rect1->x2 - rect1->x1 + rect2->x2 - rect2->x1) / 2;

		if (verticalDistance > verticalThreshold || horizontalDistance > horizontalThreshold)
			return false;

		return true;
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
		if (namenda>1 || namenda<0)
		{
			return false;
		}
		double miu = determinant(bb->x - aa->x, cc->x - aa->x, bb->y - aa->y, cc->y - aa->y) / delta;
		if (miu>1 || miu<0)
		{
			return false;
		}
		return true;
	}
	};


}