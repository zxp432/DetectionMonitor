#pragma once

#include <math.h>

namespace UtilSpace {
	using namespace System::Collections::Generic;
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
	};

	//���ڴ洢�û������ļ������
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
		   //�ж����������Ƿ��غϣ������жϼ�����е������Ƿ����û������ļ�������غ�
		   static bool areTwoAreasOverlapped(List<Point ^> ^poly, Result ^rect)
		   {
			   float verticalDistance;    //��ֱ����  
			   float horizontalDistance;  //ˮƽ����  
			   verticalDistance = fabs((poly[0]->y + poly[1]->y) / 2 - (rect->y1 + rect->y2) / 2);//�����е����
			   horizontalDistance = fabs((poly[0]->x + poly[1]->x) / 2 - (rect->x1 + rect->x2) / 2);//�����е����

			   float verticalThreshold;   //�����η���Ĵ�ֱ�ٽ�ֵ  
			   float horizontalThreshold; //�����η����ˮƽ�ٽ�ֵ  
			   verticalThreshold = (poly[1]->y - poly[0]->y + rect->y2 - rect->y1) / 2;
			   horizontalThreshold = (poly[1]->x - poly[0]->x + rect->x2 - rect->x1) / 2;

			   if (verticalDistance > verticalThreshold || horizontalDistance > horizontalThreshold) //������ؽ���ľ��������ε����о��β��ཻ
			   {
				   return false;
			   }
			   ////������ؽ���ľ��������ε����о����ཻ,�Ұ�������ε����о��Σ��������ھ�����
			   if (poly[0]->x >= rect->x1 &&   poly[0]->y >= rect->y1 &&   poly[1]->x <= rect->x2 &&   poly[1]->y <= rect->y2)
				   return true;
			   for (int i = 3; i < poly->Count; i++)
			   {
				   //�ж϶���ε�ÿ�����Ƿ�������ཻ
				   if (lineAndRecIntersection(poly[i - 1]->x, poly[i - 1]->y, poly[i]->x, poly[i]->y, rect->x1, rect->y1, rect->x2, rect->y2))
					   return true;//����ཻ��������������ཻ
			   }

			   //�жϾ����е�һ���Ƿ��ڶ�����ڲ�
			   if (pointInPolygon(poly, rect->x1, rect->y1))
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

		   //�ж����������Ƿ��غϣ������жϼ�����е������Ƿ����û������ļ�������غ�
	public:static bool areTwoRectsOverlapped(Rectangle ^rect1, Result ^rect2)
	{
		float verticalDistance;    //��ֱ����  
		float horizontalDistance;  //ˮƽ����  
		verticalDistance = fabs((rect1->y1 + rect1->y2) / 2 - (rect2->y1 + rect2->y2) / 2);//�����е����
		horizontalDistance = fabs((rect1->x1 + rect1->x2) / 2 - (rect2->x1 + rect2->x2) / 2);//�����е����

		float verticalThreshold;   //�����η���Ĵ�ֱ�ٽ�ֵ  
		float horizontalThreshold; //�����η����ˮƽ�ٽ�ֵ  
		verticalThreshold = (rect1->y2 - rect1->y1 + rect2->y2 - rect2->y1) / 2;
		horizontalThreshold = (rect1->x2 - rect1->x1 + rect2->x2 - rect2->x1) / 2;

		if (verticalDistance > verticalThreshold || horizontalDistance > horizontalThreshold)
			return false;

		return true;
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