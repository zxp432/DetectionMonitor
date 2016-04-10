#pragma once
using namespace System;
#include <math.h>

namespace UtilSpace {
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
	};
}