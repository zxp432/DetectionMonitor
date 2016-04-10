#pragma once
#include "opencv2/opencv.hpp"
char *StringToCharArray(System::String ^origin) {
	char *result = new char[origin->Length + 1];
	int i = 0;
	for (i = 0; i < origin->Length; i++) {
		result[i] = origin[i];
	}
	result[i] = '\0';
	return result;
}
System::Drawing::Bitmap^ ConvertMatToBitmap(cv::Mat& cvImg)
{
	System::Drawing::Bitmap^ bmpImg;

	//¼ì²éÍ¼ÏñÎ»Éî  
	if (cvImg.depth() != CV_8U)
	{
		bmpImg = gcnew System::Drawing::Bitmap(1, 1, System::Drawing::Imaging::PixelFormat::Format8bppIndexed);
		return (bmpImg);
	}

	//²ÊÉ«Í¼Ïñ  
	if (cvImg.channels() == 3)
	{
		bmpImg = gcnew System::Drawing::Bitmap(
			cvImg.cols,
			cvImg.rows,
			cvImg.step,
			System::Drawing::Imaging::PixelFormat::Format24bppRgb,
			(System::IntPtr)cvImg.data);
	}
	//»Ò¶ÈÍ¼Ïñ  
	else if (cvImg.channels() == 1)
	{
		bmpImg = gcnew System::Drawing::Bitmap(
			cvImg.cols,
			cvImg.rows,
			cvImg.step,
			System::Drawing::Imaging::PixelFormat::Format8bppIndexed,
			(System::IntPtr)cvImg.data);
	}

	return (bmpImg);
}