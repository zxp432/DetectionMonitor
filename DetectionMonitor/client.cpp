#pragma once
#include <cstring>
#include "opencv2/opencv.hpp"
namespace SocketCommu {
	using namespace System;
	using namespace System::IO;
	using namespace System::Text;
	using namespace System::Net::Sockets;
	using namespace System::Net;
	using namespace System::Drawing;
	public ref class Client
	{
	private: String^ serverIp = "172.16.13.123";//服务端Ip
	private: int serverPort = 10086;//服务端端口号
	private:Socket ^sendsocket;

	public: Client(String^ ip, int port) {
		this->serverIp = ip;
		this->serverPort = port;
	}
	public:Client() {
	}
		   /// 连接到服务端
	public:void connect() {
		sendsocket = gcnew Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
		IPEndPoint ^ipendpiont = gcnew IPEndPoint(IPAddress::Parse(serverIp), 10086);
		sendsocket->Connect(ipendpiont);
	}
		   /// 发送图片
	public:void sendImg(IplImage *image) {
		//char* imageInBytes = image->imageData;// new Byte[(safeLongToInt(frame.total())) * frame.channels()];
		//array<unsigned char> ^buff = gcnew array<unsigned char>(image->imageSize);// = image.imageData;
		/*Bitmap ^imgBit = gcnew Bitmap("C:\\visualStudioProject\\OPCVProject\\OPCVProject\\1.jpg");
		MemoryStream ^stream = gcnew MemoryStream();
		imgBit->Save(stream, System::Drawing::Imaging::ImageFormat::Jpeg);
		array<unsigned char> ^buff = gcnew array<unsigned char>(stream->Length);
		stream->Seek(0, SeekOrigin::Begin);
		stream->Read(buff, 0, Convert::ToInt32(stream->Length));*/
		/*for (int n = 0; n < image->imageSize; n++) {
			buff[n] = image->imageData[n];
		}*/
		Bitmap ^imgBit = gcnew Bitmap(image->width, image->height, image->widthStep, System::Drawing::Imaging::PixelFormat::Format24bppRgb, (System::IntPtr) image->imageData);
		//Bitmap ^imgBit = ConvertMatToBitmap(cv::Mat(image));
		MemoryStream ^stream = gcnew MemoryStream();
		imgBit->Save(stream, System::Drawing::Imaging::ImageFormat::Jpeg);
		array<unsigned char> ^buff = gcnew array<unsigned char>(stream->Length);
		stream->Seek(0, SeekOrigin::Begin);
		stream->Read(buff, 0, Convert::ToInt32(stream->Length));
		sendsocket->Send(buff); 
		sendsocket->Shutdown(System::Net::Sockets::SocketShutdown::Send);
	}

	public:String ^receive() {
		array<unsigned char> ^buff = gcnew array<unsigned char>(1024);
		sendsocket->Receive(buff);
		std::string temp = "";
		for (size_t i = 0; i < buff->GetLength(0); i++) {
			temp = temp + (char)buff[i];
		}
		String^ result = gcnew String(temp.c_str());
		return result;
	}
	public: void closeSocket() {
		try
		{
			sendsocket->Close();
		}
		catch (Exception ^e) {
		}
	}
	};

}