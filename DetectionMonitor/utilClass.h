#pragma once
using namespace System;
#include <math.h>

namespace UtilSpace {
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
	};
}