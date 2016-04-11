#pragma once
#pragma comment (lib, "opencv_calib3d2411d.lib")
#pragma comment (lib, "opencv_contrib2411d.lib")
#pragma comment (lib, "opencv_core2411d.lib")
#pragma comment (lib, "opencv_features2d2411d.lib")
#pragma comment (lib, "opencv_flann2411d.lib")
#pragma comment (lib, "opencv_gpu2411d.lib")
#pragma comment (lib, "opencv_highgui2411d.lib")
#pragma comment (lib, "opencv_imgproc2411d.lib")
#pragma comment (lib, "opencv_legacy2411d.lib")
#pragma comment (lib, "opencv_ml2411d.lib")
#pragma comment (lib, "opencv_nonfree2411d.lib")
#pragma comment (lib, "opencv_objdetect2411d.lib")
#pragma comment (lib, "opencv_ocl2411d.lib")
#pragma comment (lib, "opencv_photo2411d.lib")
#pragma comment (lib, "opencv_stitching2411d.lib")
#pragma comment (lib, "opencv_superres2411d.lib")
#pragma comment (lib, "opencv_ts2411d.lib")
#pragma comment (lib, "opencv_video2411d.lib")
#pragma comment (lib, "opencv_videostab2411d.lib")

#pragma comment (lib, "opencv_calib3d2411.lib")
#pragma comment (lib, "opencv_contrib2411.lib")
#pragma comment (lib, "opencv_core2411.lib")
#pragma comment (lib, "opencv_features2d2411.lib")
#pragma comment (lib, "opencv_flann2411.lib")
#pragma comment (lib, "opencv_gpu2411.lib")
#pragma comment (lib, "opencv_highgui2411.lib")
#pragma comment (lib, "opencv_imgproc2411.lib")
#pragma comment (lib, "opencv_legacy2411.lib")
#pragma comment (lib, "opencv_ml2411.lib")
#pragma comment (lib, "opencv_nonfree2411.lib")
#pragma comment (lib, "opencv_objdetect2411.lib")
#pragma comment (lib, "opencv_ocl2411.lib")
#pragma comment (lib, "opencv_photo2411.lib")
#pragma comment (lib, "opencv_stitching2411.lib")
#pragma comment (lib, "opencv_superres2411.lib")
#pragma comment (lib, "opencv_ts2411.lib")
#pragma comment (lib, "opencv_video2411.lib")
#pragma comment (lib, "opencv_videostab2411.lib")
#include <math.h>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "client.cpp"
#include <windows.h>
#include <queue>
#include "utils.h"
#include "utilCLass.h"
#include "FeatureTracker.cpp"
CvCapture* capture;
IplImage *frame;
IplImage *frameSend;//用于发送的帧
int timeCount;
double fps;
int fpsCount;
int now_frame_no = 0;
int fame_continue = 6;
std::queue<IplImage> imageQueue;
FeatureTracker tracker;// = new FeatureTracker();
struct MyRGB
{
	int R;
	int G;
	int B;
};
std::map<std::string, MyRGB> colorMap;
std::map<std::string, MyRGB>::iterator it;
namespace DetectionMonitor {

	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;
	using namespace System::Threading;
	using namespace SocketCommu;
	using namespace System::Collections::Generic;
	using namespace System::Media;

	/// <summary>
	/// MonitorForm 摘要
	/// </summary>
	public ref class MonitorForm : public System::Windows::Forms::Form
	{
	private:Client ^client = gcnew Client();
			List<UtilSpace::Result ^> ^results = gcnew List<UtilSpace::Result ^>();
			List<Thread ^> ^threadList = gcnew List<Thread ^>();//将所有线程放入线程池中
			System::String ^result = "";// "12-12-15-115-0.98-person,12-12-15-115-0.98-person,";

			List<UtilSpace::Rectangle ^> ^regions;
			HANDLE frameMutex = CreateMutex(NULL, FALSE, NULL);//用于frame多线程读写时的互斥变量
			HANDLE resultMutex = CreateMutex(NULL, FALSE, NULL);
			HANDLE frameTimerHandle = CreateMutex(NULL, FALSE, NULL);
			HANDLE socketHandle = CreateMutex(NULL, FALSE, NULL);
			int frameWidth = 640;
			int frameHeight = 480;
			//警告区域相关的变量
	private:System::Drawing::Point ^startPoint = nullptr, ^endPoint = nullptr;//鼠标下落点和离开店
			bool drawing = false;
			bool newDraw = false;
	private: System::Windows::Forms::Timer^  frameTimer;
	private: System::Windows::Forms::Timer^  beepTimer;
	private: System::Windows::Forms::Timer^  calculateTImer;
	private: System::Windows::Forms::OpenFileDialog^  videoFileDialog;

			 System::Windows::Forms::Timer^  beepTime;

	public:
		MonitorForm(void)
		{
			//定义颜色map
			colorMap["person"] = { 247, 186, 64 };
			colorMap["tvmonitor"] = { 65, 105, 225 };
			colorMap["chair"] = { 135, 38, 87 };
			colorMap["aeroplane"] = { 160, 82, 45 };
			colorMap["bottle"] = { 227, 23, 13 };
			colorMap["bicycle"] = { 218, 112, 214 };
			colorMap["bird"] = { 25, 25, 112 };
			colorMap["boat"] = { 255, 69, 0 };
			colorMap["bus"] = { 41, 36, 33 };
			colorMap["car"] = { 237, 145, 33 };
			colorMap["cat"] = { 127, 255, 0 };
			colorMap["cow"] = { 64, 224, 208 };
			colorMap["diningtable"] = { 128,42,42 };
			colorMap["dog"] = { 255, 125, 64 };
			colorMap["horse"] = { 255, 69, 0 };
			colorMap["motorbike"] = { 51, 161, 201 };
			colorMap["pottedplant"] = { 163, 148, 128 };
			colorMap["sheep"] = { 250, 235, 215 };
			colorMap["sofa"] = { 138, 43, 226 };
			colorMap["train"] = { 48, 128, 20 };
			colorMap[""] = { 0, 0, 0 };
			regions = gcnew List<UtilSpace::Rectangle^>();

			InitializeComponent();
			//
			//TODO:  在此处添加构造函数代码
			//
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~MonitorForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::GroupBox^  detectionSourceGroup;
	protected:

	protected:
	private: System::Windows::Forms::ComboBox^  detectInterval;
	private: System::Windows::Forms::Button^  startBtn;

	private: System::Windows::Forms::ComboBox^  detectionSrcCombo;
	private: System::Windows::Forms::GroupBox^  videoGroup;
	private: System::Windows::Forms::TrackBar^  videoBar;

	private: System::Windows::Forms::PictureBox^  frameShowBox;
	private: System::Windows::Forms::Label^  labelWarning;
	private: System::Windows::Forms::Button^  buttonClean;
	private: System::Windows::Forms::Button^  buttonPaint;
	private: System::Windows::Forms::Label^  receiveRateLabel;

	private: System::Windows::Forms::Label^  frameRateLabel;
	private: System::ComponentModel::IContainer^  components;





	protected:

	private:
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要修改
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->detectionSourceGroup = (gcnew System::Windows::Forms::GroupBox());
			this->detectInterval = (gcnew System::Windows::Forms::ComboBox());
			this->startBtn = (gcnew System::Windows::Forms::Button());
			this->detectionSrcCombo = (gcnew System::Windows::Forms::ComboBox());
			this->videoGroup = (gcnew System::Windows::Forms::GroupBox());
			this->videoBar = (gcnew System::Windows::Forms::TrackBar());
			this->frameShowBox = (gcnew System::Windows::Forms::PictureBox());
			this->labelWarning = (gcnew System::Windows::Forms::Label());
			this->buttonClean = (gcnew System::Windows::Forms::Button());
			this->buttonPaint = (gcnew System::Windows::Forms::Button());
			this->receiveRateLabel = (gcnew System::Windows::Forms::Label());
			this->frameRateLabel = (gcnew System::Windows::Forms::Label());
			this->frameTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->beepTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->calculateTImer = (gcnew System::Windows::Forms::Timer(this->components));
			this->videoFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->detectionSourceGroup->SuspendLayout();
			this->videoGroup->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->videoBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->frameShowBox))->BeginInit();
			this->SuspendLayout();
			// 
			// detectionSourceGroup
			// 
			this->detectionSourceGroup->Controls->Add(this->detectInterval);
			this->detectionSourceGroup->Controls->Add(this->startBtn);
			this->detectionSourceGroup->Controls->Add(this->detectionSrcCombo);
			this->detectionSourceGroup->Font = (gcnew System::Drawing::Font(L"Calibri", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->detectionSourceGroup->Location = System::Drawing::Point(3, 3);
			this->detectionSourceGroup->Name = L"detectionSourceGroup";
			this->detectionSourceGroup->Size = System::Drawing::Size(666, 62);
			this->detectionSourceGroup->TabIndex = 11;
			this->detectionSourceGroup->TabStop = false;
			this->detectionSourceGroup->Text = L"检测源";
			// 
			// detectInterval
			// 
			this->detectInterval->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->detectInterval->FormattingEnabled = true;
			this->detectInterval->Items->AddRange(gcnew cli::array< System::Object^  >(31) {
				L"1", L"2", L"3", L"4", L"5", L"6", L"7",
					L"8", L"9", L"10", L"11", L"12", L"13", L"14", L"15", L"16", L"17", L"18", L"19", L"20", L"21", L"22", L"23", L"24", L"25", L"26",
					L"27", L"28", L"29", L"30", L"检测帧间隔"
			});
			this->detectInterval->Location = System::Drawing::Point(336, 23);
			this->detectInterval->Name = L"detectInterval";
			this->detectInterval->Size = System::Drawing::Size(93, 27);
			this->detectInterval->TabIndex = 2;
			// 
			// startBtn
			// 
			this->startBtn->Location = System::Drawing::Point(435, 23);
			this->startBtn->Name = L"startBtn";
			this->startBtn->Size = System::Drawing::Size(79, 27);
			this->startBtn->TabIndex = 1;
			this->startBtn->Text = L"开始";
			this->startBtn->UseVisualStyleBackColor = true;
			this->startBtn->Click += gcnew System::EventHandler(this, &MonitorForm::startBtn_Click);
			// 
			// detectionSrcCombo
			// 
			this->detectionSrcCombo->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->detectionSrcCombo->FormattingEnabled = true;
			this->detectionSrcCombo->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"打开摄像头进行检测", L"选择视频进行检测", L"请选择检测种类" });
			this->detectionSrcCombo->Location = System::Drawing::Point(13, 23);
			this->detectionSrcCombo->Name = L"detectionSrcCombo";
			this->detectionSrcCombo->Size = System::Drawing::Size(317, 27);
			this->detectionSrcCombo->TabIndex = 0;
			// 
			// videoGroup
			// 
			this->videoGroup->Controls->Add(this->videoBar);
			this->videoGroup->Controls->Add(this->frameShowBox);
			this->videoGroup->Font = (gcnew System::Drawing::Font(L"Calibri", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->videoGroup->Location = System::Drawing::Point(5, 61);
			this->videoGroup->Margin = System::Windows::Forms::Padding(5);
			this->videoGroup->Name = L"videoGroup";
			this->videoGroup->Padding = System::Windows::Forms::Padding(5);
			this->videoGroup->Size = System::Drawing::Size(664, 559);
			this->videoGroup->TabIndex = 12;
			this->videoGroup->TabStop = false;
			this->videoGroup->Text = L"视频";
			// 
			// videoBar
			// 
			this->videoBar->AutoSize = false;
			this->videoBar->Location = System::Drawing::Point(13, 534);
			this->videoBar->Name = L"videoBar";
			this->videoBar->Size = System::Drawing::Size(639, 26);
			this->videoBar->TabIndex = 1;
			this->videoBar->Visible = false;
			this->videoBar->Scroll += gcnew System::EventHandler(this, &MonitorForm::videoBar_Scroll);
			// 
			// frameShowBox
			// 
			this->frameShowBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->frameShowBox->Location = System::Drawing::Point(12, 38);
			this->frameShowBox->Name = L"frameShowBox";
			this->frameShowBox->Size = System::Drawing::Size(640, 480);
			this->frameShowBox->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->frameShowBox->TabIndex = 0;
			this->frameShowBox->TabStop = false;
			this->frameShowBox->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MonitorForm::frameShowBox_MouseDown);
			this->frameShowBox->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MonitorForm::frameShowBox_MouseMove);
			this->frameShowBox->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MonitorForm::frameShowBox_MouseUp);
			// 
			// labelWarning
			// 
			this->labelWarning->AutoSize = true;
			this->labelWarning->Font = (gcnew System::Drawing::Font(L"Calibri", 14.25F));
			this->labelWarning->ForeColor = System::Drawing::Color::Red;
			this->labelWarning->Location = System::Drawing::Point(280, 638);
			this->labelWarning->Name = L"labelWarning";
			this->labelWarning->Size = System::Drawing::Size(0, 23);
			this->labelWarning->TabIndex = 15;
			// 
			// buttonClean
			// 
			this->buttonClean->Enabled = false;
			this->buttonClean->Font = (gcnew System::Drawing::Font(L"Calibri", 14.25F));
			this->buttonClean->Location = System::Drawing::Point(126, 633);
			this->buttonClean->Name = L"buttonClean";
			this->buttonClean->Size = System::Drawing::Size(102, 33);
			this->buttonClean->TabIndex = 14;
			this->buttonClean->Text = L"清除区域";
			this->buttonClean->UseVisualStyleBackColor = true;
			this->buttonClean->Click += gcnew System::EventHandler(this, &MonitorForm::buttonClean_Click);
			// 
			// buttonPaint
			// 
			this->buttonPaint->Enabled = false;
			this->buttonPaint->Font = (gcnew System::Drawing::Font(L"Calibri", 14.25F));
			this->buttonPaint->Location = System::Drawing::Point(18, 633);
			this->buttonPaint->Name = L"buttonPaint";
			this->buttonPaint->Size = System::Drawing::Size(102, 33);
			this->buttonPaint->TabIndex = 13;
			this->buttonPaint->Text = L"新建区域";
			this->buttonPaint->UseVisualStyleBackColor = true;
			this->buttonPaint->Click += gcnew System::EventHandler(this, &MonitorForm::buttonPaint_Click);
			// 
			// receiveRateLabel
			// 
			this->receiveRateLabel->AutoSize = true;
			this->receiveRateLabel->Font = (gcnew System::Drawing::Font(L"Calibri", 14.25F));
			this->receiveRateLabel->Location = System::Drawing::Point(280, 688);
			this->receiveRateLabel->Name = L"receiveRateLabel";
			this->receiveRateLabel->Size = System::Drawing::Size(135, 23);
			this->receiveRateLabel->TabIndex = 12;
			this->receiveRateLabel->Text = L"平均接收帧率:";
			// 
			// frameRateLabel
			// 
			this->frameRateLabel->AutoSize = true;
			this->frameRateLabel->Font = (gcnew System::Drawing::Font(L"Calibri", 14.25F));
			this->frameRateLabel->Location = System::Drawing::Point(25, 688);
			this->frameRateLabel->Name = L"frameRateLabel";
			this->frameRateLabel->Size = System::Drawing::Size(95, 23);
			this->frameRateLabel->TabIndex = 11;
			this->frameRateLabel->Text = L"视频帧率:";
			// 
			// frameTimer
			// 
			this->frameTimer->Interval = 30;
			this->frameTimer->Tick += gcnew System::EventHandler(this, &MonitorForm::frameTimer_Tick);
			// 
			// beepTimer
			// 
			this->beepTimer->Interval = 1000;
			this->beepTimer->Tick += gcnew System::EventHandler(this, &MonitorForm::beepTimer_Tick);
			// 
			// calculateTImer
			// 
			this->calculateTImer->Interval = 1000;
			this->calculateTImer->Tick += gcnew System::EventHandler(this, &MonitorForm::calculateTImer_Tick);
			// 
			// videoFileDialog
			// 
			this->videoFileDialog->FileName = L"videoFileDialog";
			// 
			// MonitorForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(682, 736);
			this->Controls->Add(this->labelWarning);
			this->Controls->Add(this->buttonClean);
			this->Controls->Add(this->buttonPaint);
			this->Controls->Add(this->receiveRateLabel);
			this->Controls->Add(this->frameRateLabel);
			this->Controls->Add(this->videoGroup);
			this->Controls->Add(this->detectionSourceGroup);
			this->Name = L"MonitorForm";
			this->Text = L"MonitorForm";
			this->detectionSourceGroup->ResumeLayout(false);
			this->videoGroup->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->videoBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->frameShowBox))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void startBtn_Click(System::Object^  sender, System::EventArgs^  e) {
		if (detectionSrcCombo->Text == "请选择检测种类" || detectInterval->Text == "检测帧率")
		{
			MessageBox::Show(this, "请选择检测种类或帧率", "错误!!!");
		}
		if (startBtn->Text == "开始")
		{
			if (detectionSrcCombo->Text == "打开摄像头进行检测")
			{
				capture = cvCaptureFromCAM(0);
				//fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS); //视频帧率
				fame_continue = int::Parse(detectInterval->Text);
				videoBar->Visible = false;
				videoBar->Minimum = 0;
				videoBar->Maximum = 0;
				startBtn->Text = "停止";
				timeCount = 0;
				fpsCount = 0;
				frameTimer->Start();
				calculateTImer->Start();
				frameShowBox->Location = System::Drawing::Point(12, 38);
				frameRateLabel->Text = "视频帧率:";
				frameWidth = 640;
				frameHeight = 480;
				buttonPaint->Enabled = true;
				buttonClean->Enabled = true;
			}
			else if (detectionSrcCombo->Text == "选择视频进行检测")
			{
				videoFileDialog->Filter = "AVI files (*.avi)|*.txt|All files (*.*)|*.*";
				videoFileDialog->FilterIndex = 2;
				videoFileDialog->RestoreDirectory = true;
				videoFileDialog->FileName = "";
				videoBar->Visible = true;
				videoBar->Value = 0;
				frameShowBox->Location = System::Drawing::Point(12, 28);
				if (videoFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				{
					char *fileName = (char*)Marshal::StringToHGlobalAnsi(videoFileDialog->FileName).ToPointer();
					capture = cvCaptureFromFile(fileName);
					fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS); //视频帧率
					fame_continue = int::Parse(detectInterval->Text);
					videoBar->Minimum = 0;
					timeCount = 0;
					fpsCount = 0;
					calculateTImer->Start();
					//frameRateLabel->Text = "视频帧率: " + double(int(fps * 100)) / 100 + "fps";
					videoBar->Maximum = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
					startBtn->Text = "停止";
					frameTimer->Start();
					buttonPaint->Enabled = false;
					buttonClean->Enabled = false;
				}
			}
		}
		else if (startBtn->Text == "停止")
		{
			frameTimer->Stop();
			calculateTImer->Stop();
			for each(Thread ^t in threadList)
			{
				t->Join();
			}
			cvReleaseCapture(&capture);
			startBtn->Text = "开始";
			now_frame_no = 0;
			startPoint = nullptr;
			endPoint = nullptr;
			//frameShowBox->Enabled = false;
			drawing = false;
			newDraw = false;
			results->Clear();
			regions->Clear();
			ReleaseMutex(frameMutex);
			ReleaseMutex(resultMutex);
			ReleaseMutex(frameTimerHandle);
			ReleaseMutex(socketHandle);
			tracker.finilise();
			int frameWidth = 640;
			int frameHeight = 480;
			/*while (!imageQueue.empty()) {
			imageQueue.pop();
			}*/
		}
	}
			 //视频滚动条的函数
	private: System::Void videoBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
		cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, videoBar->Value);
	}

			 //写字的
	private: System::Void cvText(IplImage* img, const char* cls, int x, int y, const char* score)
	{
		CvFont font;

		double hscale = 1.0;
		double vscale = 1.0;
		int linewidth = 2;
		cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hscale, vscale, 0, linewidth);
		it = colorMap.find(cls);
		CvScalar textColor = CV_RGB((*it).second.R, (*it).second.G, (*it).second.B);
		CvPoint textPos = cvPoint(x, y);
		char temp[60];
		strcpy(temp, cls);
		strcat(temp, ":");
		strcat(temp, score);
		cvPutText(img, temp, textPos, &font, textColor);
	}
			 //画框的
	private: System::Void cvFrame(IplImage* img, int x1, int y1, int x2, int y2, const char* cls)
	{
		it = colorMap.find(cls);
		cvRectangle(img, cvPoint(x1, y1), cvPoint(x2, y2), CV_RGB((*it).second.R, (*it).second.G, (*it).second.B), 2);
	}

			 //循环画帧
	private: System::Void frameTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
		try
		{
			frame = cvQueryFrame(capture);
			if (frame != NULL)
			{
				/*cv::Mat output;
				vector<cv::Point2f> offset(tracker.process(Mat(frame), output, results));
				frameShowBox->Image = client->ConvertMatToBitmap(output);
				frameShowBox->Refresh();
				IplImage *temp = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);
				cvCopy(frame, temp);*/
				//imageQueue.push(*frame);
				if (now_frame_no % fame_continue == 0)//发送帧，改变检测结果.
				{
					WaitForSingleObject(frameMutex, INFINITE);
					cvReleaseImage(&frameSend);//先释放内存，不然随着程序的运行内存会不断增加
					frameSend = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);
					cvCopy(frame, frameSend);
					ReleaseMutex(frameMutex);
					Thread ^thread = gcnew Thread(gcnew ThreadStart(this, &MonitorForm::SendFrame));
					thread->IsBackground = true;
					if (threadList->Count > 0)
						threadList->RemoveAt(0);
					threadList->Add(thread);
					thread->Start();
				}
				labelWarning->Text = "";//初始化警告框
				IplImage frameToShow = *frame;

				WaitForSingleObject(resultMutex, INFINITE);
				/*frameRateLabel->Text = "results: " + results->Count;
				label2->Text = "offset: " + offset.size();*/
				//画出检测结果
				//vector<cv::Point2f> offset(tracker.process(Mat(frame), results));
				for (int i = 0; i < results->Count; i++)
				{
					/*if (i < offset.size()) {
						results[i]->x1 = results[i]->x1 + offset[i].x;
						results[i]->x2 = results[i]->x2 + offset[i].x;
						results[i]->y1 = results[i]->y1 + offset[i].y;
						results[i]->y2 = results[i]->y2 + offset[i].y;
					}*/
					char *cls = StringToCharArray(results[i]->cls);
					char *score = StringToCharArray(results[i]->score);
					//写字
					cvText(&frameToShow, cls, results[i]->x1, results[i]->y1, score);
					//画框子
					cvFrame(&frameToShow, results[i]->x1, results[i]->y1, results[i]->x2, results[i]->y2, cls);
				}
				//for each (UtilSpace::Result ^result in results)
				//{
				//	char *cls = StringToCharArray(result->cls);
				//	char *score = StringToCharArray(result->score);
				//	//写字
				//	cvText(&frameToShow, cls, result->x1, result->y1, score);
				//	//画框子
				//	cvFrame(&frameToShow, result->x1, result->y1, result->x2, result->y2, cls);
				//}
				//画出警告区域
				for each(UtilSpace::Rectangle ^region in regions) {

					bool call_110 = false;
					//检测有没有是人的框子踏入了警告区域
					for each (UtilSpace::Result ^result in results)
					{
						if (result->cls->Equals("person") && UtilSpace::Rectangle::areTwoRectsOverlapped(region, result))//如果有人的区域与警告区域重叠，跳出循环，警告
						{
							call_110 = true;
							break;
						}
					}
					if (!call_110)
						cvRectangle(&frameToShow, cvPoint(region->x1, region->y1), cvPoint(region->x2, region->y2), CV_RGB(0, 0, 255), 2);
					else
					{
						cvRectangle(&frameToShow, cvPoint(region->x1, region->y1), cvPoint(region->x2, region->y2), CV_RGB(255, 0, 0), CV_FILLED);
						labelWarning->Text = "警告！！！";
						beepTime->Start();
					}
				}
				ReleaseMutex(resultMutex);
				frameShowBox->Image = gcnew System::Drawing::Bitmap(frameToShow.width, frameToShow.height, frameToShow.widthStep, System::Drawing::Imaging::PixelFormat::Format24bppRgb, (System::IntPtr) frameToShow.imageData);
				WaitForSingleObject(frameTimerHandle, INFINITE);
				now_frame_no++;
				ReleaseMutex(frameTimerHandle);
			}
			else
			{
				ReleaseMutex(frameMutex);
			}
		}
		catch (System::Exception ^g) {
		}
	}
			 //int save = 1;
			 //发送需要被检测的帧到服务器并返回结果
	private:void SendFrame() {
		WaitForSingleObject(frameMutex, INFINITE);
		client->connect();
		//测试所用
		//Bitmap ^temp = gcnew System::Drawing::Bitmap(frameSend->width, frameSend->height, frameSend->widthStep, System::Drawing::Imaging::PixelFormat::Format24bppRgb, (System::IntPtr) frameSend->imageData);
		//temp->Save(save + ".jpg", System::Drawing::Imaging::ImageFormat::Jpeg);
		//save++;
		//if (save == 30)
		//	save = 1;
		//测试end
		client->sendImg(frameSend);
		result = client->receive();
		client->closeSocket();
		ReleaseMutex(frameMutex);
		List<UtilSpace::Result ^> ^resultsTemp = gcnew List<UtilSpace::Result ^>();
		if (!result->Equals(""))
		{
			array<System::String ^> ^resultString = result->Split(',');
			for (int i = 0; i < resultString->Length - 1; i++)
			{
				UtilSpace::Result ^temp = gcnew UtilSpace::Result();
				array<System::String ^> ^result = resultString[i]->Split('-');
				temp->x1 = int::Parse(result[0]);
				temp->y1 = int::Parse(result[1]);
				temp->x2 = int::Parse(result[2]);
				temp->y2 = int::Parse(result[3]);
				temp->score = result[4];
				temp->cls = result[5];
				resultsTemp->Add(temp);
			}
		}
		WaitForSingleObject(resultMutex, INFINITE);
		results = resultsTemp;
		fpsCount++;
		ReleaseMutex(resultMutex);
	}
	private: System::Void calculateTImer_Tick(System::Object^  sender, System::EventArgs^  e) {
		timeCount++;
		frameRateLabel->Text = "视频帧率: " + (double)((int)(((double)now_frame_no / timeCount) * 100)) / 100 + "fps";
		receiveRateLabel->Text = "平均接收帧率: " + (double)((int)(((double)fpsCount / timeCount) * 100)) / 100 + "fps";
	}
	private: System::Void frameShowBox_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		if (newDraw) {
			startPoint = gcnew System::Drawing::Point(e->X, e->Y);
			endPoint = nullptr;
			drawing = true;
		}
	}
	private: System::Void frameShowBox_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		if (newDraw) {
			drawing = false;
			endPoint = gcnew System::Drawing::Point(e->X, e->Y);
			newDraw = false;
			//计算当前picturebox上的点的位置
			int topleftX = startPoint->X < endPoint->X ? startPoint->X : endPoint->X;
			int topleftY = startPoint->Y < endPoint->Y ? startPoint->Y : endPoint->Y;
			int bootomRightX = startPoint->X > endPoint->X ? startPoint->X : endPoint->X;
			int bootomRightY = startPoint->Y > endPoint->Y ? startPoint->Y : endPoint->Y;
			//映射回frame
			int x1 = frameWidth * topleftX / frameShowBox->Width;
			int y1 = frameHeight * topleftY / frameShowBox->Height;
			int x2 = frameWidth * bootomRightX / frameShowBox->Width;
			int y2 = frameHeight * bootomRightY / frameShowBox->Height;

			regions->Add(gcnew UtilSpace::Rectangle(x1, y1, x2, y2));
		}
	}
	private: System::Void frameShowBox_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		Graphics ^g = frameShowBox->CreateGraphics();
		if (e->Button == System::Windows::Forms::MouseButtons::Left) {
			if (drawing)
			{
				g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;//消除锯齿  
				frameShowBox->Refresh();
				//找出矩形的最左上角
				int leftTopX = startPoint->X < e->X ? startPoint->X : e->X;
				int leftTopY = startPoint->Y < e->Y ? startPoint->Y : e->Y;

				g->DrawRectangle(gcnew Pen(Color::Blue, 2), leftTopX, leftTopY, Math::Abs(e->X - startPoint->X), Math::Abs(e->Y - startPoint->Y));
			}
		}
	}
	private: System::Void buttonPaint_Click(System::Object^  sender, System::EventArgs^  e) {
		newDraw = true;
	}
	private: System::Void buttonClean_Click(System::Object^  sender, System::EventArgs^  e) {
		regions->Clear();
	}
			 //警告区域相关的函数结束
	private: System::Void beepTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
		System::Media::SoundPlayer ^sp = gcnew SoundPlayer();
		sp->SoundLocation = "BLEEP1_S.WAV";
		sp->Play();
		beepTime->Stop();
	}
	};
}
