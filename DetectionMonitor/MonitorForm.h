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

//#pragma comment (lib, "opencv_calib3d2411.lib")
//#pragma comment (lib, "opencv_contrib2411.lib")
//#pragma comment (lib, "opencv_core2411.lib")
//#pragma comment (lib, "opencv_features2d2411.lib")
//#pragma comment (lib, "opencv_flann2411.lib")
//#pragma comment (lib, "opencv_gpu2411.lib")
//#pragma comment (lib, "opencv_highgui2411.lib")
//#pragma comment (lib, "opencv_imgproc2411.lib")
//#pragma comment (lib, "opencv_legacy2411.lib")
//#pragma comment (lib, "opencv_ml2411.lib")
//#pragma comment (lib, "opencv_nonfree2411.lib")
//#pragma comment (lib, "opencv_objdetect2411.lib")
//#pragma comment (lib, "opencv_ocl2411.lib")
//#pragma comment (lib, "opencv_photo2411.lib")
//#pragma comment (lib, "opencv_stitching2411.lib")
//#pragma comment (lib, "opencv_superres2411.lib")
//#pragma comment (lib, "opencv_ts2411.lib")
//#pragma comment (lib, "opencv_video2411.lib")
//#pragma comment (lib, "opencv_videostab2411.lib")

#include <list>   
#include <numeric> 
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
#include <opencv2/legacy/compat.hpp>  
#define LENGTHSHORTQUEUE 20
#define LENGTHLONGQUEUE 20
#define STUDYLENGTH 100
CvCapture* capture;
IplImage *frame;
//以下三个设置成全局变量的原因是要释放内存，否则随着程序的运行内存会溢出
IplImage *frameSend;//用于发送的帧
IplImage *prevFrame;//用于复制前一帧并存进队列
IplImage *frameToShow;//用于显示帧
IplImage *oneFrame;//用于灰度图

int timeCount;
double fps;
int fpsCount;
int now_frame_no = 0;
int fame_continue = 6;
std::queue<IplImage *> imageQueue;
std::queue<int> shortQueue;
std::queue<int> longQueue;
vector<float> shortVector;
vector<float> longVector;
int wraningShe = 0;
int dBetween;
int dLong = -1;
int wraning;
int isStudying = 0;
float studyTemp[STUDYLENGTH];
int nowStudyLength = 0;
int finshNum;
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
			FileStream ^fs;
			List<UtilSpace::Region ^> ^regions = gcnew List<UtilSpace::Region ^>();
			System::String ^strNum = "LONG\t\t\tBTWEEN\r\n";
			HANDLE frameMutex = CreateMutex(NULL, FALSE, NULL);//用于frame多线程读写时的互斥变量
			HANDLE resultMutex = CreateMutex(NULL, FALSE, NULL);
			HANDLE frameTimerHandle = CreateMutex(NULL, FALSE, NULL);
			HANDLE socketHandle = CreateMutex(NULL, FALSE, NULL);
			int frameWidth = 640;
			int frameHeight = 480;
			int isShel = 0;//用于开关异物遮挡报警
						   //警告区域相关的变量
	private: System::Windows::Forms::Timer^  frameTimer;
	private: System::Windows::Forms::Timer^  beepTimer;
	private: System::Windows::Forms::Timer^  calculateTImer;
	private: System::Windows::Forms::OpenFileDialog^  videoFileDialog;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  mainTabPage;
	private: System::Windows::Forms::TabPage^  deTabPage;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem2;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem3;
	private: System::Windows::Forms::Button^  videoButton;
	private: System::Windows::Forms::Button^  captureButton;
	private: System::Windows::Forms::GroupBox^  derterNumGroupBox;
	private: System::Windows::Forms::Button^  stopButton;
	private: System::Windows::Forms::Button^  shelterButton;
	private: System::Windows::Forms::Label^  cleanLabel;
	private: System::Windows::Forms::Label^  paintLabel;
	private: System::Windows::Forms::Label^  shelterLabel;
	private: System::Windows::Forms::Label^  stopLabel;
	private: System::Windows::Forms::Button^  startDeButton;
	private: System::Windows::Forms::Button^  setButton;
	private: System::Windows::Forms::Button^  checkButton;
	private: System::Windows::Forms::Label^  setLabel;
	private: System::Windows::Forms::Label^  checkLabel;
	private: System::Windows::Forms::Label^  startDeLabel;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Button^  mainButton;
	private: System::Windows::Forms::Button^  deButton;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Label^  finshLabel;
	private: System::Windows::Forms::Label^  cameraLabel;
	private: System::Windows::Forms::Label^  videoLable;
	private: System::Windows::Forms::Button^  startStudyButton;

	private: System::Windows::Forms::Button^  button1;

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
	private: System::Windows::Forms::TrackBar^  videoBar;
	private: System::Windows::Forms::PictureBox^  frameShowBox;
	private: System::Windows::Forms::Label^  labelWarning;
	private: System::Windows::Forms::Button^  buttonClean;
	private: System::Windows::Forms::Button^  buttonPaint;
	private: System::Windows::Forms::Label^  receiveRateLabel;
	private: System::Windows::Forms::Label^  frameRateLabel;
	private: System::ComponentModel::IContainer^  components;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MonitorForm::typeid));
			this->detectionSourceGroup = (gcnew System::Windows::Forms::GroupBox());
			this->videoLable = (gcnew System::Windows::Forms::Label());
			this->cameraLabel = (gcnew System::Windows::Forms::Label());
			this->startStudyButton = (gcnew System::Windows::Forms::Button());
			this->videoButton = (gcnew System::Windows::Forms::Button());
			this->captureButton = (gcnew System::Windows::Forms::Button());
			this->buttonClean = (gcnew System::Windows::Forms::Button());
			this->buttonPaint = (gcnew System::Windows::Forms::Button());
			this->videoBar = (gcnew System::Windows::Forms::TrackBar());
			this->frameShowBox = (gcnew System::Windows::Forms::PictureBox());
			this->labelWarning = (gcnew System::Windows::Forms::Label());
			this->receiveRateLabel = (gcnew System::Windows::Forms::Label());
			this->frameRateLabel = (gcnew System::Windows::Forms::Label());
			this->frameTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->beepTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->calculateTImer = (gcnew System::Windows::Forms::Timer(this->components));
			this->videoFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->deTabPage = (gcnew System::Windows::Forms::TabPage());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->derterNumGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->finshLabel = (gcnew System::Windows::Forms::Label());
			this->stopButton = (gcnew System::Windows::Forms::Button());
			this->paintLabel = (gcnew System::Windows::Forms::Label());
			this->cleanLabel = (gcnew System::Windows::Forms::Label());
			this->shelterButton = (gcnew System::Windows::Forms::Button());
			this->stopLabel = (gcnew System::Windows::Forms::Label());
			this->shelterLabel = (gcnew System::Windows::Forms::Label());
			this->mainTabPage = (gcnew System::Windows::Forms::TabPage());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->startDeButton = (gcnew System::Windows::Forms::Button());
			this->setLabel = (gcnew System::Windows::Forms::Label());
			this->checkButton = (gcnew System::Windows::Forms::Button());
			this->checkLabel = (gcnew System::Windows::Forms::Label());
			this->setButton = (gcnew System::Windows::Forms::Button());
			this->startDeLabel = (gcnew System::Windows::Forms::Label());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem3 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mainButton = (gcnew System::Windows::Forms::Button());
			this->deButton = (gcnew System::Windows::Forms::Button());
			this->detectionSourceGroup->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->videoBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->frameShowBox))->BeginInit();
			this->tabControl1->SuspendLayout();
			this->deTabPage->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->derterNumGroupBox->SuspendLayout();
			this->mainTabPage->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->contextMenuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// detectionSourceGroup
			// 
			this->detectionSourceGroup->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(28)),
				static_cast<System::Int32>(static_cast<System::Byte>(28)), static_cast<System::Int32>(static_cast<System::Byte>(28)));
			this->detectionSourceGroup->Controls->Add(this->startStudyButton);
			this->detectionSourceGroup->Controls->Add(this->videoLable);
			this->detectionSourceGroup->Controls->Add(this->cameraLabel);
			this->detectionSourceGroup->Controls->Add(this->videoButton);
			this->detectionSourceGroup->Controls->Add(this->captureButton);
			this->detectionSourceGroup->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->detectionSourceGroup->ForeColor = System::Drawing::Color::White;
			this->detectionSourceGroup->Location = System::Drawing::Point(6, 17);
			this->detectionSourceGroup->Name = L"detectionSourceGroup";
			this->detectionSourceGroup->Size = System::Drawing::Size(168, 318);
			this->detectionSourceGroup->TabIndex = 11;
			this->detectionSourceGroup->TabStop = false;
			this->detectionSourceGroup->Text = L"检测源";
			this->detectionSourceGroup->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MonitorForm::detectionSourceGroup_Paint);
			// 
			// videoLable
			// 
			this->videoLable->AutoSize = true;
			this->videoLable->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 10));
			this->videoLable->ForeColor = System::Drawing::Color::DarkGray;
			this->videoLable->Location = System::Drawing::Point(48, 64);
			this->videoLable->Name = L"videoLable";
			this->videoLable->Size = System::Drawing::Size(64, 18);
			this->videoLable->TabIndex = 17;
			this->videoLable->Text = L"视频检测";
			this->videoLable->Visible = false;
			// 
			// cameraLabel
			// 
			this->cameraLabel->AutoSize = true;
			this->cameraLabel->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 10));
			this->cameraLabel->ForeColor = System::Drawing::Color::DarkGray;
			this->cameraLabel->Location = System::Drawing::Point(39, 31);
			this->cameraLabel->Name = L"cameraLabel";
			this->cameraLabel->Size = System::Drawing::Size(78, 18);
			this->cameraLabel->TabIndex = 5;
			this->cameraLabel->Text = L"摄像头检测";
			this->cameraLabel->Visible = false;
			// 
			// startStudyButton
			// 
			this->startStudyButton->Location = System::Drawing::Point(41, 92);
			this->startStudyButton->Name = L"startStudyButton";
			this->startStudyButton->Size = System::Drawing::Size(75, 30);
			this->startStudyButton->TabIndex = 17;
			this->startStudyButton->Text = L"button4";
			this->startStudyButton->UseVisualStyleBackColor = true;
			this->startStudyButton->Click += gcnew System::EventHandler(this, &MonitorForm::startStudyButton_Click);
			// 
			// videoButton
			// 
			this->videoButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(28)), static_cast<System::Int32>(static_cast<System::Byte>(28)),
				static_cast<System::Int32>(static_cast<System::Byte>(28)));
			this->videoButton->FlatAppearance->BorderSize = 0;
			this->videoButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->videoButton->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 10));
			this->videoButton->Location = System::Drawing::Point(3, 59);
			this->videoButton->Name = L"videoButton";
			this->videoButton->Size = System::Drawing::Size(159, 27);
			this->videoButton->TabIndex = 16;
			this->videoButton->Text = L"视频检测";
			this->videoButton->UseVisualStyleBackColor = false;
			this->videoButton->Click += gcnew System::EventHandler(this, &MonitorForm::videoButton_Click);
			this->videoButton->MouseLeave += gcnew System::EventHandler(this, &MonitorForm::videoButton_MouseLeave);
			this->videoButton->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MonitorForm::videoButton_MouseMove);
			// 
			// captureButton
			// 
			this->captureButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(28)), static_cast<System::Int32>(static_cast<System::Byte>(28)),
				static_cast<System::Int32>(static_cast<System::Byte>(28)));
			this->captureButton->FlatAppearance->BorderSize = 0;
			this->captureButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->captureButton->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 10));
			this->captureButton->ForeColor = System::Drawing::Color::White;
			this->captureButton->Location = System::Drawing::Point(3, 26);
			this->captureButton->Name = L"captureButton";
			this->captureButton->Size = System::Drawing::Size(159, 27);
			this->captureButton->TabIndex = 15;
			this->captureButton->Text = L"摄像头检测";
			this->captureButton->UseVisualStyleBackColor = false;
			this->captureButton->Click += gcnew System::EventHandler(this, &MonitorForm::captureButton_Click);
			this->captureButton->MouseLeave += gcnew System::EventHandler(this, &MonitorForm::captureButton_MouseLeave);
			this->captureButton->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MonitorForm::captureButton_MouseMove);
			// 
			// buttonClean
			// 
			this->buttonClean->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"buttonClean.BackgroundImage")));
			this->buttonClean->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->buttonClean->Enabled = false;
			this->buttonClean->Font = (gcnew System::Drawing::Font(L"Calibri", 14.25F));
			this->buttonClean->Location = System::Drawing::Point(303, 509);
			this->buttonClean->Name = L"buttonClean";
			this->buttonClean->Size = System::Drawing::Size(35, 35);
			this->buttonClean->TabIndex = 14;
			this->buttonClean->UseVisualStyleBackColor = true;
			this->buttonClean->Click += gcnew System::EventHandler(this, &MonitorForm::buttonClean_Click);
			this->buttonClean->MouseLeave += gcnew System::EventHandler(this, &MonitorForm::buttonClean_MouseLeave);
			this->buttonClean->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MonitorForm::buttonClean_MouseMove);
			// 
			// buttonPaint
			// 
			this->buttonPaint->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"buttonPaint.BackgroundImage")));
			this->buttonPaint->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->buttonPaint->Enabled = false;
			this->buttonPaint->Font = (gcnew System::Drawing::Font(L"Calibri", 14.25F));
			this->buttonPaint->Location = System::Drawing::Point(262, 510);
			this->buttonPaint->Name = L"buttonPaint";
			this->buttonPaint->Size = System::Drawing::Size(35, 35);
			this->buttonPaint->TabIndex = 13;
			this->buttonPaint->UseVisualStyleBackColor = true;
			this->buttonPaint->Click += gcnew System::EventHandler(this, &MonitorForm::buttonPaint_Click);
			this->buttonPaint->MouseLeave += gcnew System::EventHandler(this, &MonitorForm::buttonPaint_MouseLeave);
			this->buttonPaint->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MonitorForm::buttonPaint_MouseMove);
			// 
			// videoBar
			// 
			this->videoBar->AutoSize = false;
			this->videoBar->Location = System::Drawing::Point(344, 519);
			this->videoBar->Name = L"videoBar";
			this->videoBar->Size = System::Drawing::Size(476, 26);
			this->videoBar->TabIndex = 1;
			this->videoBar->Visible = false;
			this->videoBar->Scroll += gcnew System::EventHandler(this, &MonitorForm::videoBar_Scroll);
			// 
			// frameShowBox
			// 
			this->frameShowBox->BackColor = System::Drawing::Color::Black;
			this->frameShowBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->frameShowBox->Location = System::Drawing::Point(180, 28);
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
			this->labelWarning->Location = System::Drawing::Point(6, 132);
			this->labelWarning->Name = L"labelWarning";
			this->labelWarning->Size = System::Drawing::Size(0, 23);
			this->labelWarning->TabIndex = 15;
			// 
			// receiveRateLabel
			// 
			this->receiveRateLabel->AutoSize = true;
			this->receiveRateLabel->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 10));
			this->receiveRateLabel->ForeColor = System::Drawing::Color::White;
			this->receiveRateLabel->Location = System::Drawing::Point(6, 39);
			this->receiveRateLabel->Name = L"receiveRateLabel";
			this->receiveRateLabel->Size = System::Drawing::Size(95, 18);
			this->receiveRateLabel->TabIndex = 12;
			this->receiveRateLabel->Text = L"平均接收帧率:";
			// 
			// frameRateLabel
			// 
			this->frameRateLabel->AutoSize = true;
			this->frameRateLabel->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 10));
			this->frameRateLabel->ForeColor = System::Drawing::Color::White;
			this->frameRateLabel->Location = System::Drawing::Point(6, 67);
			this->frameRateLabel->Name = L"frameRateLabel";
			this->frameRateLabel->Size = System::Drawing::Size(67, 18);
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
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->deTabPage);
			this->tabControl1->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(134)));
			this->tabControl1->Location = System::Drawing::Point(-3, -30);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(859, 810);
			this->tabControl1->TabIndex = 3;
			this->tabControl1->DoubleClick += gcnew System::EventHandler(this, &MonitorForm::tabControl1_DoubleClick);
			// 
			// deTabPage
			// 
			this->deTabPage->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(28)), static_cast<System::Int32>(static_cast<System::Byte>(28)),
				static_cast<System::Int32>(static_cast<System::Byte>(28)));
			this->deTabPage->Controls->Add(this->groupBox1);
			this->deTabPage->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 12));
			this->deTabPage->Location = System::Drawing::Point(4, 29);
			this->deTabPage->Name = L"deTabPage";
			this->deTabPage->Padding = System::Windows::Forms::Padding(3);
			this->deTabPage->Size = System::Drawing::Size(851, 777);
			this->deTabPage->TabIndex = 1;
			this->deTabPage->Text = L"检测页面";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->button3);
			this->groupBox1->Controls->Add(this->button2);
			this->groupBox1->Controls->Add(this->button1);
			this->groupBox1->Controls->Add(this->buttonClean);
			this->groupBox1->Controls->Add(this->videoBar);
			this->groupBox1->Controls->Add(this->buttonPaint);
			this->groupBox1->Controls->Add(this->derterNumGroupBox);
			this->groupBox1->Controls->Add(this->stopButton);
			this->groupBox1->Controls->Add(this->paintLabel);
			this->groupBox1->Controls->Add(this->cleanLabel);
			this->groupBox1->Controls->Add(this->detectionSourceGroup);
			this->groupBox1->Controls->Add(this->shelterButton);
			this->groupBox1->Controls->Add(this->stopLabel);
			this->groupBox1->Controls->Add(this->shelterLabel);
			this->groupBox1->Controls->Add(this->frameShowBox);
			this->groupBox1->Location = System::Drawing::Point(-4, 19);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(906, 631);
			this->groupBox1->TabIndex = 20;
			this->groupBox1->TabStop = false;
			this->groupBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MonitorForm::groupBox1_Paint);
			// 
			// button3
			// 
			this->button3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button3->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 10));
			this->button3->ForeColor = System::Drawing::Color::White;
			this->button3->Location = System::Drawing::Point(424, 490);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 20;
			this->button3->Text = L"多边形";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Visible = false;
			this->button3->Click += gcnew System::EventHandler(this, &MonitorForm::button3_Click_1);
			// 
			// button2
			// 
			this->button2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button2->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 10));
			this->button2->ForeColor = System::Drawing::Color::White;
			this->button2->Location = System::Drawing::Point(343, 488);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 20;
			this->button2->Text = L"圆形";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Visible = false;
			this->button2->Click += gcnew System::EventHandler(this, &MonitorForm::button2_Click);
			// 
			// button1
			// 
			this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button1->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 10));
			this->button1->ForeColor = System::Drawing::Color::White;
			this->button1->Location = System::Drawing::Point(262, 487);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 20;
			this->button1->Text = L"矩形";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Visible = false;
			this->button1->Click += gcnew System::EventHandler(this, &MonitorForm::button1_Click);
			// 
			// derterNumGroupBox
			// 
			this->derterNumGroupBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(28)),
				static_cast<System::Int32>(static_cast<System::Byte>(28)), static_cast<System::Int32>(static_cast<System::Byte>(28)));
			this->derterNumGroupBox->Controls->Add(this->finshLabel);
			this->derterNumGroupBox->Controls->Add(this->labelWarning);
			this->derterNumGroupBox->Controls->Add(this->receiveRateLabel);
			this->derterNumGroupBox->Controls->Add(this->frameRateLabel);
			this->derterNumGroupBox->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(134)));
			this->derterNumGroupBox->ForeColor = System::Drawing::Color::White;
			this->derterNumGroupBox->Location = System::Drawing::Point(6, 341);
			this->derterNumGroupBox->Name = L"derterNumGroupBox";
			this->derterNumGroupBox->Size = System::Drawing::Size(168, 203);
			this->derterNumGroupBox->TabIndex = 16;
			this->derterNumGroupBox->TabStop = false;
			this->derterNumGroupBox->Text = L"检测参数";
			this->derterNumGroupBox->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MonitorForm::derterNumGroupBox_Paint);
			// 
			// finshLabel
			// 
			this->finshLabel->AutoSize = true;
			this->finshLabel->Location = System::Drawing::Point(10, 100);
			this->finshLabel->Name = L"finshLabel";
			this->finshLabel->Size = System::Drawing::Size(109, 20);
			this->finshLabel->TabIndex = 16;
			this->finshLabel->Text = L"学习遮挡进度:";
			this->finshLabel->Visible = false;
			// 
			// stopButton
			// 
			this->stopButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"stopButton.BackgroundImage")));
			this->stopButton->Location = System::Drawing::Point(180, 511);
			this->stopButton->Name = L"stopButton";
			this->stopButton->Size = System::Drawing::Size(35, 35);
			this->stopButton->TabIndex = 17;
			this->stopButton->UseVisualStyleBackColor = true;
			this->stopButton->Click += gcnew System::EventHandler(this, &MonitorForm::stopButton_Click);
			this->stopButton->MouseLeave += gcnew System::EventHandler(this, &MonitorForm::stopButton_MouseLeave);
			this->stopButton->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MonitorForm::stopButton_MouseMove);
			// 
			// paintLabel
			// 
			this->paintLabel->AutoSize = true;
			this->paintLabel->BackColor = System::Drawing::Color::White;
			this->paintLabel->Location = System::Drawing::Point(264, 488);
			this->paintLabel->Name = L"paintLabel";
			this->paintLabel->Size = System::Drawing::Size(73, 20);
			this->paintLabel->TabIndex = 19;
			this->paintLabel->Text = L"新建区域";
			this->paintLabel->Visible = false;
			// 
			// cleanLabel
			// 
			this->cleanLabel->AutoSize = true;
			this->cleanLabel->BackColor = System::Drawing::Color::White;
			this->cleanLabel->Location = System::Drawing::Point(299, 486);
			this->cleanLabel->Name = L"cleanLabel";
			this->cleanLabel->Size = System::Drawing::Size(73, 20);
			this->cleanLabel->TabIndex = 19;
			this->cleanLabel->Text = L"删除区域";
			this->cleanLabel->Visible = false;
			// 
			// shelterButton
			// 
			this->shelterButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"shelterButton.BackgroundImage")));
			this->shelterButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->shelterButton->Location = System::Drawing::Point(221, 511);
			this->shelterButton->Name = L"shelterButton";
			this->shelterButton->Size = System::Drawing::Size(35, 35);
			this->shelterButton->TabIndex = 18;
			this->shelterButton->UseVisualStyleBackColor = true;
			this->shelterButton->Click += gcnew System::EventHandler(this, &MonitorForm::shelterButton_Click);
			this->shelterButton->MouseLeave += gcnew System::EventHandler(this, &MonitorForm::shelterButton_MouseLeave);
			this->shelterButton->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MonitorForm::shelterButton_MouseMove);
			// 
			// stopLabel
			// 
			this->stopLabel->AutoSize = true;
			this->stopLabel->BackColor = System::Drawing::Color::White;
			this->stopLabel->Location = System::Drawing::Point(180, 488);
			this->stopLabel->Name = L"stopLabel";
			this->stopLabel->Size = System::Drawing::Size(41, 20);
			this->stopLabel->TabIndex = 19;
			this->stopLabel->Text = L"停止";
			this->stopLabel->Visible = false;
			// 
			// shelterLabel
			// 
			this->shelterLabel->AutoSize = true;
			this->shelterLabel->BackColor = System::Drawing::Color::White;
			this->shelterLabel->Location = System::Drawing::Point(217, 488);
			this->shelterLabel->Name = L"shelterLabel";
			this->shelterLabel->Size = System::Drawing::Size(144, 20);
			this->shelterLabel->TabIndex = 19;
			this->shelterLabel->Text = L"开启/关闭异物遮挡";
			this->shelterLabel->Visible = false;
			// 
			// mainTabPage
			// 
			this->mainTabPage->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(28)), static_cast<System::Int32>(static_cast<System::Byte>(28)),
				static_cast<System::Int32>(static_cast<System::Byte>(28)));
			this->mainTabPage->Controls->Add(this->groupBox2);
			this->mainTabPage->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 12));
			this->mainTabPage->ForeColor = System::Drawing::SystemColors::ControlText;
			this->mainTabPage->Location = System::Drawing::Point(4, 29);
			this->mainTabPage->Name = L"mainTabPage";
			this->mainTabPage->Padding = System::Windows::Forms::Padding(3);
			this->mainTabPage->Size = System::Drawing::Size(851, 777);
			this->mainTabPage->TabIndex = 0;
			this->mainTabPage->Text = L"主页";
			// 
			// groupBox2
			// 
			this->groupBox2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(28)), static_cast<System::Int32>(static_cast<System::Byte>(28)),
				static_cast<System::Int32>(static_cast<System::Byte>(28)));
			this->groupBox2->Controls->Add(this->startDeButton);
			this->groupBox2->Controls->Add(this->setLabel);
			this->groupBox2->Controls->Add(this->checkButton);
			this->groupBox2->Controls->Add(this->checkLabel);
			this->groupBox2->Controls->Add(this->setButton);
			this->groupBox2->Controls->Add(this->startDeLabel);
			this->groupBox2->Location = System::Drawing::Point(-4, 19);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(893, 608);
			this->groupBox2->TabIndex = 4;
			this->groupBox2->TabStop = false;
			this->groupBox2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MonitorForm::groupBox1_Paint);
			// 
			// startDeButton
			// 
			this->startDeButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"startDeButton.BackgroundImage")));
			this->startDeButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->startDeButton->Location = System::Drawing::Point(37, 28);
			this->startDeButton->Name = L"startDeButton";
			this->startDeButton->Size = System::Drawing::Size(75, 75);
			this->startDeButton->TabIndex = 0;
			this->startDeButton->UseVisualStyleBackColor = true;
			this->startDeButton->Click += gcnew System::EventHandler(this, &MonitorForm::startDeButton_Click);
			// 
			// setLabel
			// 
			this->setLabel->AutoSize = true;
			this->setLabel->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 10));
			this->setLabel->ForeColor = System::Drawing::Color::White;
			this->setLabel->Location = System::Drawing::Point(317, 106);
			this->setLabel->Name = L"setLabel";
			this->setLabel->Size = System::Drawing::Size(36, 18);
			this->setLabel->TabIndex = 3;
			this->setLabel->Text = L"设置";
			// 
			// checkButton
			// 
			this->checkButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"checkButton.BackgroundImage")));
			this->checkButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->checkButton->Location = System::Drawing::Point(167, 28);
			this->checkButton->Name = L"checkButton";
			this->checkButton->Size = System::Drawing::Size(75, 75);
			this->checkButton->TabIndex = 1;
			this->checkButton->UseVisualStyleBackColor = true;
			// 
			// checkLabel
			// 
			this->checkLabel->AutoSize = true;
			this->checkLabel->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 10));
			this->checkLabel->ForeColor = System::Drawing::Color::White;
			this->checkLabel->Location = System::Drawing::Point(178, 106);
			this->checkLabel->Name = L"checkLabel";
			this->checkLabel->Size = System::Drawing::Size(64, 18);
			this->checkLabel->TabIndex = 3;
			this->checkLabel->Text = L"查看证据";
			// 
			// setButton
			// 
			this->setButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"setButton.BackgroundImage")));
			this->setButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->setButton->Location = System::Drawing::Point(297, 28);
			this->setButton->Name = L"setButton";
			this->setButton->Size = System::Drawing::Size(75, 75);
			this->setButton->TabIndex = 2;
			this->setButton->UseVisualStyleBackColor = true;
			// 
			// startDeLabel
			// 
			this->startDeLabel->AutoSize = true;
			this->startDeLabel->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 10));
			this->startDeLabel->ForeColor = System::Drawing::Color::White;
			this->startDeLabel->Location = System::Drawing::Point(57, 106);
			this->startDeLabel->Name = L"startDeLabel";
			this->startDeLabel->Size = System::Drawing::Size(36, 18);
			this->startDeLabel->TabIndex = 3;
			this->startDeLabel->Text = L"开始";
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->toolStripMenuItem2,
					this->toolStripMenuItem3
			});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(84, 48);
			// 
			// toolStripMenuItem2
			// 
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			this->toolStripMenuItem2->Size = System::Drawing::Size(83, 22);
			this->toolStripMenuItem2->Text = L"1";
			// 
			// toolStripMenuItem3
			// 
			this->toolStripMenuItem3->Name = L"toolStripMenuItem3";
			this->toolStripMenuItem3->Size = System::Drawing::Size(83, 22);
			this->toolStripMenuItem3->Text = L"2";
			// 
			// mainButton
			// 
			this->mainButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(122)),
				static_cast<System::Int32>(static_cast<System::Byte>(204)));
			this->mainButton->FlatAppearance->BorderSize = 0;
			this->mainButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->mainButton->Font = (gcnew System::Drawing::Font(L"宋体", 10));
			this->mainButton->ForeColor = System::Drawing::Color::White;
			this->mainButton->Location = System::Drawing::Point(1, 4);
			this->mainButton->Name = L"mainButton";
			this->mainButton->Size = System::Drawing::Size(75, 22);
			this->mainButton->TabIndex = 4;
			this->mainButton->Text = L"主页";
			this->mainButton->UseVisualStyleBackColor = false;
			this->mainButton->Click += gcnew System::EventHandler(this, &MonitorForm::mainButton_Click);
			this->mainButton->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MonitorForm::button1_Paint);
			// 
			// deButton
			// 
			this->deButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(100)), static_cast<System::Int32>(static_cast<System::Byte>(100)),
				static_cast<System::Int32>(static_cast<System::Byte>(100)));
			this->deButton->FlatAppearance->BorderSize = 0;
			this->deButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->deButton->Font = (gcnew System::Drawing::Font(L"宋体", 10));
			this->deButton->ForeColor = System::Drawing::Color::White;
			this->deButton->Location = System::Drawing::Point(72, 4);
			this->deButton->Name = L"deButton";
			this->deButton->Size = System::Drawing::Size(75, 22);
			this->deButton->TabIndex = 4;
			this->deButton->Text = L"检测";
			this->deButton->UseVisualStyleBackColor = false;
			this->deButton->Visible = false;
			this->deButton->DoubleClick += gcnew System::EventHandler(this, &MonitorForm::tabControl1_DoubleClick);
			this->deButton->Click += gcnew System::EventHandler(this, &MonitorForm::deButton_Click);
			this->deButton->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MonitorForm::button1_Paint);
			this->deButton->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MonitorForm::deButton_MouseDown);
			// 
			// MonitorForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(28)), static_cast<System::Int32>(static_cast<System::Byte>(28)),
				static_cast<System::Int32>(static_cast<System::Byte>(28)));
			this->ClientSize = System::Drawing::Size(833, 584);
			this->Controls->Add(this->deButton);
			this->Controls->Add(this->mainButton);
			this->Controls->Add(this->tabControl1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MonitorForm";
			this->Text = L"视频流监控";
			this->detectionSourceGroup->ResumeLayout(false);
			this->detectionSourceGroup->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->videoBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->frameShowBox))->EndInit();
			this->tabControl1->ResumeLayout(false);
			this->deTabPage->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->derterNumGroupBox->ResumeLayout(false);
			this->derterNumGroupBox->PerformLayout();
			this->mainTabPage->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->contextMenuStrip1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion

		//private: System::Void startBtn_Click(System::Object^  sender, System::EventArgs^  e) {
		//		frameTimer->Stop();
		//		calculateTImer->Stop();
		//		for each(Thread ^t in threadList)
		//		{
		//			t->Join();
		//		}
		//		cvReleaseCapture(&capture);
		//		now_frame_no = 0;
		//		startPoint = nullptr;
		//		endPoint = nullptr;
		//		//frameShowBox->Enabled = false;
		//		drawing = false;
		//		newDraw = false;
		//		results->Clear();
		//		regions->Clear();
		//		ReleaseMutex(frameMutex);
		//		ReleaseMutex(resultMutex);
		//		ReleaseMutex(frameTimerHandle);
		//		ReleaseMutex(socketHandle);
		//		tracker.finilise();
		//		int frameWidth = 640;
		//		int frameHeight = 480;
		//		/*while (!imageQueue.empty()) {
		//		imageQueue.pop();
		//		}*/
		//}
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
				frameShowBox->Refresh();*/
				//cvReleaseImage(&prevFrame);
				prevFrame = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);
				cvCopy(frame, prevFrame);
				imageQueue.push(prevFrame);
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
				if (imageQueue.size() == fame_continue) {
					labelWarning->Text = "";//初始化警告框
					cvReleaseImage(&frameToShow);
					frameToShow = imageQueue.front();
					if (isShel == 1 && now_frame_no % 3 == 0)
					{
						if (wraningShe == 1)
						{
							labelWarning->Text = "警告！！！";
							beepTimer->Start();
						}
						if (dLong > 0)
						{
							finshLabel->Text = "正在进行遮挡检测";
						}
						Thread ^thread = gcnew Thread(gcnew ThreadStart(this, &MonitorForm::shelFrame));
						thread->Start();
					}
					if (isStudying == 1 && now_frame_no % 3 == 0)
					{
						Thread ^thread = gcnew Thread(gcnew ThreadStart(this, &MonitorForm::shelStudyFrame));
						thread->Start();

					}
					if (finshNum>0)
						finshLabel->Text = "学习遮挡进度:" + finshNum + "%";
					imageQueue.pop();
					WaitForSingleObject(resultMutex, INFINITE);
					vector<cv::Point2f> offset(tracker.process(Mat(frameToShow), results));

					//画出检测结果
					/*frameShowBox->Image = ConvertMatToBitmap(output);
					frameShowBox->Refresh();*/
					for (int i = 0; i < results->Count; i++)
					{
						if (i < offset.size()) {//offset有可能为空
							int debug = offset[i].x;
							results[i]->x1 = results[i]->x1 + offset[i].x;
							results[i]->x2 = results[i]->x2 + offset[i].x;
							results[i]->y1 = results[i]->y1 + offset[i].y;
							results[i]->y2 = results[i]->y2 + offset[i].y;
						}
						char *cls = StringToCharArray(results[i]->cls);
						char *score = StringToCharArray(results[i]->score);
						//写字
						cvText(frameToShow, cls, results[i]->x1, results[i]->y1, score);
						//画框子
						cvFrame(frameToShow, results[i]->x1, results[i]->y1, results[i]->x2, results[i]->y2, cls);
					}
					//画出检测结果
					//for each (UtilSpace::Result ^result in results)
					//{
					//	char *cls = StringToCharArray(result->cls);
					//	char *score = StringToCharArray(result->score);
					//	//写字
					//	cvText(frameToShow, cls, result->x1, result->y1, score);
					//	//画框子
					//	cvFrame(frameToShow, result->x1, result->y1, result->x2, result->y2, cls);
					//}
					//画出警告区域(多边形)
					for each(UtilSpace::Region  ^region in regions) {

						bool call_110 = false;
						//if(count>2 && region[count]->x == region[2]->x && region[count]->y == region[2]->y){
						//检测有没有是人的框子踏入了警告区域
						int i = 0;
						float time = 0;
						for each (UtilSpace::Result ^result in results)
						{
							if (!call_110 && result->cls->Equals("person") && region->areTwoAreasOverlapped(result))//如果有人的区域与警告区域重叠，跳出循环，警告
							{
								call_110 = true;
							}
							/*if (i <= offset.size() && result->cls->Equals("person"))
							{
							float tempTime = region->timeToAlarm(result, offset[i]);
							if ((time == 0 && tempTime != 0) || tempTime < time)
							time = tempTime;
							}*/
							i++;
						}
						//labelWarning->Text = "警告！！！" + time;
						if (!call_110) {
							region->draw(frameToShow, 255, 255, 255);
						}
						else
						{
							region->draw(frameToShow, 255, 0, 0);
							labelWarning->Text = "警告！！！" + time;
							beepTimer->Start();
						}

					}

					if (region != nullptr && region->drawing && !region->newDraw)//当前是绘画状态而且起码画了一个点，则把画完的线显示出来
					{
						region->drawIncomplete(frameToShow);
					}
					ReleaseMutex(resultMutex);
					frameShowBox->Image = gcnew System::Drawing::Bitmap(frameToShow->width, frameToShow->height, frameToShow->widthStep, System::Drawing::Imaging::PixelFormat::Format24bppRgb, (System::IntPtr) frameToShow->imageData);
					WaitForSingleObject(frameTimerHandle, INFINITE);
					ReleaseMutex(frameTimerHandle);
				}
			}
			else
			{
				ReleaseMutex(frameMutex);
			}
			now_frame_no++;
		}
		catch (System::Exception ^g) {
		}
	}

			 //发送需要被检测的帧到服务器并返回结果
	private:void SendFrame() {
		WaitForSingleObject(frameMutex, INFINITE);
		client->connect();
		client->sendImg(frameSend);
		result = client->receive();
		client->closeSocket();
		ReleaseMutex(frameMutex);
		WaitForSingleObject(resultMutex, INFINITE);
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
		results = resultsTemp;
		fpsCount++;
		ReleaseMutex(resultMutex);
	}
			//异物遮挡报警
	private:void shelFrame()
	{
		int nHistSize = 256;
		cvReleaseImage(&oneFrame);//先释放内存，不然随着程序的运行内存会不断增加
		oneFrame = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
		cvCvtColor(frameToShow, oneFrame, CV_BGR2GRAY);
		float fRange[] = { 0, 255 };  //灰度级的范围    
		float *pfRanges[] = { fRange };
		CvHistogram *pcvHistogram = cvCreateHist(1, &nHistSize, CV_HIST_ARRAY, pfRanges);
		cvCalcHist(&oneFrame, pcvHistogram);
		float fMaxHistValue = 0;
		cvGetMinMaxHistValue(pcvHistogram, NULL, &fMaxHistValue, NULL, NULL);
		//分别将每个直方块的值绘制到图中  
		float width = oneFrame->width;
		float count = 0;
		for (int i = 0; i <256; i++)
		{
			float fHistValue = cvQueryHistValue_1D(pcvHistogram, i); //像素为i的直方块大小  	
																	 //count += Math::Log(fHistValue+1, (i+1)*266);
																	 //count += fHistValue*(i + 1);
			count += Math::Log(i + 2, fHistValue + 2);
		}
		//strNum += "" + count + "\r\n";
		//初始化
		if (shortVector.size() <= LENGTHSHORTQUEUE)
		{
			shortVector.push_back(count);
		}
		else if (longVector.size() <= LENGTHLONGQUEUE)
		{
			longVector.push_back(count);

		}//初始化结束
		else if (dLong < 0)//第一次计算
		{
			float temp[LENGTHLONGQUEUE];
			for (int i = 0; i < LENGTHLONGQUEUE - 1; i++)
			{
				temp[i] = Math::Abs(longVector[i] - longVector[i + 1]);
			}
			quickSort(temp, LENGTHLONGQUEUE, (int)LENGTHLONGQUEUE / 2);
			dLong = temp[(int)LENGTHLONGQUEUE / 2];

		}
		else if (now_frame_no % 5 == 0)
		{
			longVector.erase(longVector.begin());
			longVector.push_back(count);
			float temp[LENGTHLONGQUEUE];
			for (int i = 0; i < LENGTHLONGQUEUE - 1; i++)
			{
				temp[i] = Math::Abs(longVector[i] - longVector[i + 1]);
			}
			quickSort(temp, LENGTHLONGQUEUE - 1, (int)LENGTHLONGQUEUE / 2);
			dLong = temp[(int)LENGTHLONGQUEUE / 2];
		}
		else
		{
			shortVector.erase(shortVector.begin());
			shortVector.push_back(count);
			float temp[LENGTHLONGQUEUE];
			for (int i = 0; i < LENGTHLONGQUEUE; i++)
			{
				temp[i] = Math::Abs(shortVector[i] - longVector[i]);
			}
			quickSort(temp, LENGTHLONGQUEUE, (int)LENGTHLONGQUEUE / 2);
			dBetween = temp[(int)LENGTHLONGQUEUE / 2];
			DateTime dtime = DateTime::Now;
			wraning = Math::Abs(dBetween - dLong);
			if (wraning > 6)
			{
				wraningShe = 1;
			}
		}
		//	else if
		//	{
		//		;
		//	}

	}

	private:void shelStudyFrame()
	{
		int nHistSize = 256;
		cvReleaseImage(&oneFrame);//先释放内存，不然随着程序的运行内存会不断增加
		oneFrame = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
		cvCvtColor(frameToShow, oneFrame, CV_BGR2GRAY);
		float fRange[] = { 0, 255 };  //灰度级的范围    
		float *pfRanges[] = { fRange };
		CvHistogram *pcvHistogram = cvCreateHist(1, &nHistSize, CV_HIST_ARRAY, pfRanges);
		cvCalcHist(&oneFrame, pcvHistogram);
		float fMaxHistValue = 0;
		cvGetMinMaxHistValue(pcvHistogram, NULL, &fMaxHistValue, NULL, NULL);
		//分别将每个直方块的值绘制到图中  
		float width = oneFrame->width;
		float count = 0;
		for (int i = 0; i < 256; i++)
		{
			float fHistValue = cvQueryHistValue_1D(pcvHistogram, i); //像素为i的直方块大小  	
																	 //count += Math::Log(fHistValue+1, (i+1)*266);
																	 //count += fHistValue*(i + 1);
			count += Math::Log(i + 2, fHistValue + 2);
		}
		//strNum += "" + count + "\r\n";
		//初始化
		if (shortVector.size() <= LENGTHSHORTQUEUE)
		{
			shortVector.push_back(count);
		}
		else if (longVector.size() <= LENGTHLONGQUEUE)
		{
			longVector.push_back(count);

		}//初始化结束
		else if (dLong < 0)//第一次计算
		{
			float temp[LENGTHLONGQUEUE];
			for (int i = 0; i < LENGTHLONGQUEUE - 1; i++)
			{
				temp[i] = Math::Abs(longVector[i] - longVector[i + 1]);
			}
			quickSort(temp, LENGTHLONGQUEUE, (int)LENGTHLONGQUEUE / 2);
			dLong = temp[(int)LENGTHLONGQUEUE / 2];

		}
		else if (now_frame_no % 5 == 0)
		{
			longVector.erase(longVector.begin());
			longVector.push_back(count);
			float temp[LENGTHLONGQUEUE];
			for (int i = 0; i < LENGTHLONGQUEUE - 1; i++)
			{
				temp[i] = Math::Abs(longVector[i] - longVector[i + 1]);
			}
			quickSort(temp, LENGTHLONGQUEUE - 1, (int)LENGTHLONGQUEUE / 2);
			dLong = temp[(int)LENGTHLONGQUEUE / 2];
		}
		else
		{
			shortVector.erase(shortVector.begin());
			shortVector.push_back(count);
			float temp[LENGTHLONGQUEUE];
			for (int i = 0; i < LENGTHLONGQUEUE; i++)
			{
				temp[i] = Math::Abs(shortVector[i] - longVector[i]);
			}
			quickSort(temp, LENGTHLONGQUEUE, (int)LENGTHLONGQUEUE / 2);
			dBetween = temp[(int)LENGTHLONGQUEUE / 2];
			DateTime dtime = DateTime::Now;
			studyTemp[nowStudyLength] = Math::Abs(dBetween - dLong);
			nowStudyLength++;
			finshNum = (int)100 * nowStudyLength / STUDYLENGTH;
			if (nowStudyLength == STUDYLENGTH)
			{
				isStudying = 0;
				quickSort(studyTemp, STUDYLENGTH - 1, (int)STUDYLENGTH / 2);
				//File::Create("D:\\TestFilem.txt");
				File::WriteAllText("thresold.txt", "" + studyTemp[STUDYLENGTH - 1]);
			}
		}
	}



			void quickSort(float r[], int n, int k) {
				qsort_improve(r, 0, n, k);//先调用改进算法Qsort使之基本有序  

										  //再用插入排序对基本有序序列排序  
				for (int i = 1; i <= n; i++) {
					int tmp = r[i];
					int j = i - 1;
					while (tmp < r[j]) {
						r[j + 1] = r[j]; j = j - 1;
					}
					r[j + 1] = tmp;
				}
			}
			void qsort_improve(float r[], int low, int high, int k) {
				if (high - low > k) { //长度大于k时递归, k为指定的数  
					int pivot = partition(r, low, high); // 调用的Partition算法保持不变  
					qsort_improve(r, low, pivot - 1, k);
					qsort_improve(r, pivot + 1, high, k);
				}
			}
			int partition(float a[], int low, int high)
			{
				int privotKey = a[low];                 //基准元素  
				while (low < high) {                   //从表的两端交替地向中间扫描  
					while (low < high  && a[high] >= privotKey) --high; //从high 所指位置向前搜索，至多到low+1 位置。将比基准元素小的交换到低端  
					swap(&a[low], &a[high]);
					while (low < high  && a[low] <= privotKey) ++low;
					swap(&a[low], &a[high]);
				}
				return low;
			}
			void swap(float *a, float *b)
			{
				int tmp = *a;
				*a = *b;
				*b = tmp;
			}

			// 创建灰度图像的直方图  
			CvHistogram* CreateGrayImageHist(IplImage **ppImage)
			{
				int nHistSize = 256;
				float fRange[] = { 0, 255 };  //灰度级的范围    
				float *pfRanges[] = { fRange };
				CvHistogram *pcvHistogram = cvCreateHist(1, &nHistSize, CV_HIST_ARRAY, pfRanges);
				cvCalcHist(ppImage, pcvHistogram);
				return pcvHistogram;
			}
			// 根据直方图创建直方图图像  
			IplImage* CreateHisogramImage(int nImageWidth, int nScale, int nImageHeight, CvHistogram *pcvHistogram)
			{
				IplImage *pHistImage = cvCreateImage(cvSize(nImageWidth * nScale, nImageHeight), IPL_DEPTH_8U, 1);

				//统计直方图中的最大直方块  
				float fMaxHistValue = 0;
				cvGetMinMaxHistValue(pcvHistogram, NULL, &fMaxHistValue, NULL, NULL);

				//分别将每个直方块的值绘制到图中  
				int i;
				for (i = 0; i < nImageWidth; i++)
				{
					float fHistValue = cvQueryHistValue_1D(pcvHistogram, i); //像素为i的直方块大小  
					int nRealHeight = cvRound((fHistValue / fMaxHistValue) * nImageHeight);  //要绘制的高度  
					cvRectangle(pHistImage,
						cvPoint(i * nScale, nImageHeight - 1),
						cvPoint((i + 1) * nScale - 1, nImageHeight - nRealHeight),
						cvScalar(i, 0, 0, 0),
						CV_FILLED
						);
				}
				return pHistImage;
			}

	private: System::Void calculateTImer_Tick(System::Object^  sender, System::EventArgs^  e) {
		timeCount++;
		frameRateLabel->Text = "视频帧率: " + (double)((int)(((double)now_frame_no / timeCount) * 100)) / 100 + "fps";
		receiveRateLabel->Text = "平均接收帧率: " + (double)((int)(((double)fpsCount / timeCount) * 100)) / 100 + "fps";
	}

	private: System::Void frameShowBox_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		if (region != nullptr) {
			int x = frameWidth * e->X / frameShowBox->Width;
			int y = frameHeight * e->Y / frameShowBox->Height;
			int flag = region->mouseDown(x, y);//-1相交导致结束 0正在画 1画好
			if (flag == 1) {
				regions->Add(region);
				region = nullptr;
			}
			else if (flag == -1) {
				region = nullptr;
			}
		}
	}

	private: System::Void frameShowBox_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		if (region != nullptr) {
			int x = frameWidth * e->X / frameShowBox->Width;
			int y = frameHeight * e->Y / frameShowBox->Height;
			int flag = region->mouseUp(x, y);//-1相交导致结束 0正在画 1画好
			if (flag == 1) {
				regions->Add(region);
				region = nullptr;
			}
			else if (flag == -1) {
				region = nullptr;
			}
		}
	}

	private: System::Void frameShowBox_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		if (region != nullptr) {
			int x = frameWidth * e->X / frameShowBox->Width;
			int y = frameHeight * e->Y / frameShowBox->Height;
			int flag = region->mouseMove(frameShowBox, e, frameWidth, frameHeight);//-1相交导致结束 0正在画 1画好
		}
	}

	private: UtilSpace::Region ^region;

	private: System::Void buttonPaint_Click(System::Object^  sender, System::EventArgs^  e) {
		button1->Visible = true;
		button2->Visible = true;
		button3->Visible = true;
	}

	private: System::Void buttonClean_Click(System::Object^  sender, System::EventArgs^  e) {
		regions->Clear();
		region = nullptr;
	}

			 //警告区域相关的函数结束
	private: System::Void beepTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
		System::Media::SoundPlayer ^sp = gcnew SoundPlayer();
		sp->SoundLocation = "BLEEP1_S.WAV";
		sp->Play();
		beepTimer->Stop();
	}

	private: System::Void captureButton_Click(System::Object^  sender, System::EventArgs^  e) {
		captureButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(122)), static_cast<System::Int32>(static_cast<System::Byte>(204)));
		captureButton->Enabled = false;
		videoButton->Enabled = false;
		cameraLabel->Visible = true;
		videoLable->Visible = true;
		capture = cvCaptureFromCAM(0);
		//fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS); //视频帧率
		fame_continue = 6;
		videoBar->Visible = false;
		videoBar->Minimum = 0;
		videoBar->Maximum = 0;
		timeCount = 0;
		fpsCount = 0;
		frameTimer->Start();
		calculateTImer->Start();
		frameRateLabel->Text = "视频帧率:";
		frameWidth = 640;
		frameHeight = 480;
		buttonPaint->Enabled = true;
		buttonClean->Enabled = true;
	}

	private: System::Void videoButton_Click(System::Object^  sender, System::EventArgs^  e) {
		videoFileDialog->Filter = "AVI files (*.avi)|*.txt|All files (*.*)|*.*";
		videoFileDialog->FilterIndex = 2;
		videoButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(122)), static_cast<System::Int32>(static_cast<System::Byte>(204)));
		captureButton->Enabled = false;
		videoButton->Enabled = false;
		cameraLabel->Visible = true;
		videoLable->Visible = true;
		videoFileDialog->RestoreDirectory = true;
		videoFileDialog->FileName = "";
		videoBar->Visible = true;
		videoBar->Value = 0;
		if (videoFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			char *fileName = (char*)Marshal::StringToHGlobalAnsi(videoFileDialog->FileName).ToPointer();
			capture = cvCaptureFromFile(fileName);
			fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS); //视频帧率
			fame_continue = 6;
			videoBar->Minimum = 0;
			timeCount = 0;
			fpsCount = 0;
			calculateTImer->Start();
			//frameRateLabel->Text = "视频帧率: " + double(int(fps * 100)) / 100 + "fps";
			videoBar->Maximum = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
			frameTimer->Start();
			buttonPaint->Enabled = false;
			buttonClean->Enabled = false;
		}
	}

	private: System::Void stopButton_Click(System::Object^  sender, System::EventArgs^  e) {
		frameTimer->Stop();
		calculateTImer->Stop();
		captureButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(28)), static_cast<System::Int32>(static_cast<System::Byte>(28)), static_cast<System::Int32>(static_cast<System::Byte>(28)));
		videoButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(28)), static_cast<System::Int32>(static_cast<System::Byte>(28)), static_cast<System::Int32>(static_cast<System::Byte>(28)));
		for each(Thread ^t in threadList)
		{
			t->Join();
		}
		cvReleaseCapture(&capture);
		now_frame_no = 0;
		//frameShowBox->Enabled = false;
		region = nullptr;
		results->Clear();
		regions->Clear();
		ReleaseMutex(frameMutex);
		ReleaseMutex(resultMutex);
		ReleaseMutex(frameTimerHandle);
		ReleaseMutex(socketHandle);
		tracker.finilise();
		int frameWidth = 640;
		int frameHeight = 480;
		captureButton->Enabled = true;
		videoButton->Enabled = true;
		cameraLabel->Visible = false;
		videoLable->Visible = false;
		/*while (!imageQueue.empty()) {
		imageQueue.pop();
		}*/
	}

	private: System::Void stopButton_MouseLeave(System::Object^  sender, System::EventArgs^  e) {
		stopLabel->Visible = false;
	}
	private: System::Void stopButton_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		stopLabel->Visible = true;
	}
	private: System::Void shelterButton_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		shelterLabel->Visible = true;
	}
	private: System::Void shelterButton_MouseLeave(System::Object^  sender, System::EventArgs^  e) {
		shelterLabel->Visible = false;
	}
	private: System::Void buttonPaint_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		paintLabel->Visible = true;
	}
	private: System::Void buttonPaint_MouseLeave(System::Object^  sender, System::EventArgs^  e) {
		paintLabel->Visible = false;
	}
	private: System::Void buttonClean_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		cleanLabel->Visible = true;
	}
	private: System::Void buttonClean_MouseLeave(System::Object^  sender, System::EventArgs^  e) {
		cleanLabel->Visible = false;
	}
	private: System::Void startDeButton_Click(System::Object^  sender, System::EventArgs^  e) {
		tabControl1->SelectedIndex = 1;
		deButton->Visible = true;
		mainButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(100)), static_cast<System::Int32>(static_cast<System::Byte>(100)), static_cast<System::Int32>(static_cast<System::Byte>(100)));
		deButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(122)), static_cast<System::Int32>(static_cast<System::Byte>(204)));
	}
	private: System::Void tabControl1_DoubleClick(System::Object^  sender, System::EventArgs^  e) {
		tabControl1->SelectedIndex = 0;
		deButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(100)), static_cast<System::Int32>(static_cast<System::Byte>(100)), static_cast<System::Int32>(static_cast<System::Byte>(100)));
		mainButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(122)), static_cast<System::Int32>(static_cast<System::Byte>(204)));
		deButton->Visible = false;
	}
	private: System::Void detectionSourceGroup_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
		e->Graphics->Clear(detectionSourceGroup->BackColor);
		e->Graphics->DrawString(detectionSourceGroup->Text, detectionSourceGroup->Font, Brushes::White, 10, 1);
		e->Graphics->DrawLine(Pens::Gray, 1, 7, 8, 7);
		e->Graphics->DrawLine(Pens::Gray, (int)e->Graphics->MeasureString(detectionSourceGroup->Text, detectionSourceGroup->Font).Width + 8, 7, detectionSourceGroup->Width - 2, 7);
		e->Graphics->DrawLine(Pens::Gray, 1, 7, 1, detectionSourceGroup->Height - 2);
		e->Graphics->DrawLine(Pens::Gray, 1, detectionSourceGroup->Height - 2, detectionSourceGroup->Width - 2, detectionSourceGroup->Height - 2);
		e->Graphics->DrawLine(Pens::Gray, detectionSourceGroup->Width - 2, 7, detectionSourceGroup->Width - 2, detectionSourceGroup->Height - 2);
	}
	private: System::Void derterNumGroupBox_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
		e->Graphics->Clear(derterNumGroupBox->BackColor);
		e->Graphics->DrawString(derterNumGroupBox->Text, derterNumGroupBox->Font, Brushes::White, 10, 1);
		e->Graphics->DrawLine(Pens::Gray, 1, 7, 8, 7);
		e->Graphics->DrawLine(Pens::Gray, (int)e->Graphics->MeasureString(derterNumGroupBox->Text, derterNumGroupBox->Font).Width + 8, 7, derterNumGroupBox->Width - 2, 7);
		e->Graphics->DrawLine(Pens::Gray, 1, 7, 1, derterNumGroupBox->Height - 2);
		e->Graphics->DrawLine(Pens::Gray, 1, derterNumGroupBox->Height - 2, derterNumGroupBox->Width - 2, derterNumGroupBox->Height - 2);
		e->Graphics->DrawLine(Pens::Gray, derterNumGroupBox->Width - 2, 7, derterNumGroupBox->Width - 2, derterNumGroupBox->Height - 2);
	}
	private: System::Void captureButton_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		captureButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(122)), static_cast<System::Int32>(static_cast<System::Byte>(204)));
	}
	private: System::Void captureButton_MouseLeave(System::Object^  sender, System::EventArgs^  e) {
		captureButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(28)), static_cast<System::Int32>(static_cast<System::Byte>(28)), static_cast<System::Int32>(static_cast<System::Byte>(28)));
	}
	private: System::Void videoButton_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		videoButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(122)), static_cast<System::Int32>(static_cast<System::Byte>(204)));
	}
	private: System::Void videoButton_MouseLeave(System::Object^  sender, System::EventArgs^  e) {
		videoButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(28)), static_cast<System::Int32>(static_cast<System::Byte>(28)), static_cast<System::Int32>(static_cast<System::Byte>(28)));
	}
	private: System::Void button3_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {

	}
	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
		tabControl1->SelectedIndex = 0;
	}
	private: System::Void groupBox1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
		e->Graphics->Clear(groupBox1->BackColor);
		e->Graphics->DrawString(groupBox1->Text, groupBox1->Font, Brushes::White, 10, 1);
		e->Graphics->DrawLine(gcnew Pen(Color::FromArgb(0, 122, 204), 2), 1, 7, 8, 7);
		e->Graphics->DrawLine(gcnew Pen(Color::FromArgb(0, 122, 204), 2), (int)e->Graphics->MeasureString(groupBox1->Text, groupBox1->Font).Width + 8, 7, groupBox1->Width - 2, 7);
		//e->Graphics->DrawLine(gcnew Pen(Color::FromArgb(0, 122, 204), 2), 0, 7, 1, groupBox1->Height - 2);
		//e->Graphics->DrawLine(gcnew Pen(Color::FromArgb(0, 122, 204), 2), 0, groupBox1->Height - 2, groupBox1->Width - 2, groupBox1->Height - 2);
		//e->Graphics->DrawLine(gcnew Pen(Color::FromArgb(0, 122, 204), 2), groupBox1->Width, 7, groupBox1->Width , groupBox1->Height );
	}
	private: System::Void button1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
	}
	private: System::Void mainButton_Click(System::Object^  sender, System::EventArgs^  e) {
		tabControl1->SelectedIndex = 0;
		mainButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(122)), static_cast<System::Int32>(static_cast<System::Byte>(204)));
		deButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(100)), static_cast<System::Int32>(static_cast<System::Byte>(100)), static_cast<System::Int32>(static_cast<System::Byte>(100)));
	}
	private: System::Void deButton_Click(System::Object^  sender, System::EventArgs^  e) {
		deButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(122)), static_cast<System::Int32>(static_cast<System::Byte>(204)));
		mainButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(100)), static_cast<System::Int32>(static_cast<System::Byte>(100)), static_cast<System::Int32>(static_cast<System::Byte>(100)));
		tabControl1->SelectedIndex = 1;
	}

	private: System::Void deButton_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		if (e->Button == System::Windows::Forms::MouseButtons::Right)
		{
			tabControl1->SelectedIndex = 0;
			deButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(100)), static_cast<System::Int32>(static_cast<System::Byte>(100)), static_cast<System::Int32>(static_cast<System::Byte>(100)));
			mainButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(122)), static_cast<System::Int32>(static_cast<System::Byte>(204)));
			deButton->Visible = false;
		}
	}
			 //矩形
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		//button1的单击事件
		button1->Visible = false;
		button2->Visible = false;
		button3->Visible = false;
		region = gcnew UtilSpace::Rectangle();
	}
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
		//button2的单击事件
		button1->Visible = false;
		button2->Visible = false;
		button3->Visible = false;
		region = gcnew UtilSpace::Circle();
	}
			 //多边形
	private: System::Void button3_Click_1(System::Object^  sender, System::EventArgs^  e) {
		//button3的单击事件
		button1->Visible = false;
		button2->Visible = false;
		button3->Visible = false;
		region = gcnew UtilSpace::Polygon();
	}
	private: System::Void shelterButton_Click(System::Object^  sender, System::EventArgs^  e) {
		if (isShel == 0)
		{
			if (File::Exists("thresold.txt"))
			{
				System::String ^num = File::ReadAllText("thresold.txt");
				wraning = INT16::Parse(num);
				isShel = 1;
				dLong = -1;
				dBetween = -1;
				finshLabel->Text = "正在准备遮挡检测";
				finshLabel->Visible = true;
				while (shortQueue.size() != 0)
				{
					shortQueue.pop();
				}
				while (longQueue.size() != 0)
				{
					longQueue.pop();
				}
				while (shortVector.size() != 0)
				{
					shortVector.clear();
				}
				while (longVector.size() != 0)
				{
					longVector.clear();
				}
				shortVector.reserve(LENGTHSHORTQUEUE);
				longVector.reserve(LENGTHLONGQUEUE);

			}
			else
			{
				finshLabel->Text = "请开始遮挡学习";
				finshLabel->Visible = true;
			}
		}
		else
		{
			isShel = 0;
		}
	}
	private: System::Void startStudyButton_Click(System::Object^  sender, System::EventArgs^  e) {
		if (isStudying == 0)
		{
			startStudyButton->Text = "结束学习";
			shortVector.reserve(LENGTHSHORTQUEUE);
			longVector.reserve(LENGTHLONGQUEUE);
			isStudying = 1;
			nowStudyLength = 0;
			finshLabel->Visible = true;
			finshLabel->Text = "正在准备开始学习";
		}
		else if (isStudying == 1)
		{
			startStudyButton->Text = "开始学习";
			isStudying = 0;
		}
	}
	};
}
