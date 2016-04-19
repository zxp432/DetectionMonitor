#pragma once
using namespace System;
char *StringToCharArray(System::String ^origin);
System::Drawing::Bitmap^ ConvertMatToBitmap(cv::Mat& cvImg);