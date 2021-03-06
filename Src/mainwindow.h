#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define OPENCV
#include <QMainWindow>
#include <qmessagebox.h>
#include <qsettings.h>
#include <QCryptographicHash>
#include <qfile.h>
#include "QProcess"

#include "GenICam/System.h"
#include "GenICam/ParameterNode.h"
#include "Media/VideoRender.h"
#include "Media/ImageConvert.h"
#include "MessageQue.h"
#include <QElapsedTimer>

#include <opencv2/core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\dnn.hpp>

#include <ShellAPI.h>
#include <tchar.h>
#include <time.h>
#include <sstream>
#include <fstream>

#include <Src\parameterssetting.h>
#include <Src\takephoto.h>
#include <Src\Config.h>
#include <Src\alertwindow.h>
#include <Src\MySerialport.h>

#include "mythread.h"
#include <QThread>

#include "Src\yolo_v2_class.hpp"


using namespace cv;
using namespace dnn;
using namespace std;

#define WHITE_COLOR Vec3b(255, 255, 255)

class CFrameInfo : public Dahua::Memory::CBlock
{
public:
	CFrameInfo()
	{
		m_pImageBuf = NULL;
		m_nBufferSize = 0;
		m_nWidth = 0;
		m_nHeight = 0;
		m_PixelType = Dahua::GenICam::gvspPixelMono8;
		m_nPaddingX = 0;
		m_nPaddingY = 0;
		m_nTimeStamp = 0;
	}

	~CFrameInfo()
	{
	}

public:
	BYTE		*m_pImageBuf;
	int			m_nBufferSize;
	int			m_nWidth;
	int			m_nHeight;
	Dahua::GenICam::EPixelType	m_PixelType;
	int			m_nPaddingX;
	int			m_nPaddingY;
	uint64_t	m_nTimeStamp;
};

namespace Ui {
class MainWindow;
}

struct relative_location
{
	int x;
	int y;
	int width;
	int height;
	double k;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
	//图书总数
	int total_number = 0;
	//正确总数
	int sum_of_correct = 0;
	//错误总数
	int sum_of_wrong = 0;
	//设置拟合的直线的斜率、截距和相关系数
	double k, b, s;
	//黑色标志的数量ss
	int Num_of_blocks = 0;
	//图书的位置
	QRect Qrect_of_image;
	//Qt 与 OPenCV中的Rect不通用，转换一下
	Rect rect_of_image = Rect(Qrect_of_image.x(), Qrect_of_image.y(), Qrect_of_image.width(), Qrect_of_image.height());
	//用于保存各个中点相对位置关系
	vector<relative_location> re_locations;
	//保存各个黑块的位置信息
	vector<Rect> ab_locations;

	//用于计时
	clock_t startTime, startTime1, endTime;
	//bool revFlag = false;
	//拍照保存的照片
	Mat Pic_to_Save;
	//设置触发方式
	bool Mode_of_trig = false;
	//软触发拍照
	bool Mode_of_trig_soft = false;

	bool isGrabbingFlag = false;
	//枚举触发方式
	enum ETrigType
	{
		trigContinous = 0,	//连续拉流
		trigSoftware = 1,	//软件触发
		trigLine = 2,		//外部触发		
	};
	//检测像机数、序列号
	void CameraCheck(void);
	//回调函数
	void DahuaCallback(const Dahua::GenICam::CFrame & frame);
	//打开相机
	bool CameraOpen(void);
	//关闭相机
	bool CameraClose(void);
	//开始采集
	bool CameraStart(void);
	//停止采集
	bool CameraStop(void);
	//切换采集方式、触发方式 （连续采集、外部触发、软件触发）
	void CameraChangeTrig(ETrigType trigType = trigContinous);
	//执行一次软触发
	bool ExecuteSoftTrig(void);
	static bool setSoftTriggerConf(IAcquisitionControlPtr sptrAcquisitionCtl);

	void onGetFrame(const CFrame &pFrame);
	/// \brief 显示一帧图像
	/// \param [in] pRgbFrameBuf 要显示的图像数据
	/// \param [in] nWidth 图像的宽
	/// \param [in] nHeight 图像的高
	/// \param [in] pixelFormat 图像的格式
	/// \retval true 显示失败
	/// \retval false  显示成功 
	bool ShowImage(uint8_t* pRgbFrameBuf, int pRgbFrameBufSize, int nWidth, int nHeight, uint64_t pixelFormat);


signals:
	void sendData(double, double, double);
	void StartThread();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

	//保存图片
    void on_actionSavePic_triggered();
	//裁剪图片
    void on_actionCut_2_triggered();
	//打开标记软件
    void on_actionLabel_triggered();
	//训练模型
    void on_actionTrain_triggered();
	//打开裁剪窗口
    void on_actionOpenCutWindow_triggered();
	//打开参数设置窗口
    void on_actionPara_triggered();
	//从参数设置窗口接收新数据
	void recevieDataFromSubWin(double, double, double, int);
	//裁剪图片
    void on_actionCut_triggered();
	void on_actionGetParemeter_triggered();
	//设置新书拍照信号
	void set_Mode_of_trig_soft();

    //void on_actionGetParemeter_triggered();

    void on_actiontakephoto_triggered();

private:
    Ui::MainWindow *ui;
	ParametersSetting *para;
	TakePhoto *takephoto;
	AlertWindow *alertWindow;
	MyThread * m_MyThread;
	QThread * subthread;
	
	//串口类
	MyCSerialPort mycserialport;

	//检测类
	Detector *detector;
	//配置文件类
	Config *con;

	Dahua::Infra::TVector<Dahua::GenICam::ICameraPtr> m_vCameraPtrList;	// 发现的相机列表
	Dahua::GenICam::ICameraPtr m_pCamera;								// 当前相机，默认为列表中的第一个相机	
	Dahua::GenICam::IStreamSourcePtr m_pStreamSource;					// 流对象

	Dahua::Infra::CThreadLite           m_thdDisplayThread;   // 显示线程
	TMessageQue<CFrameInfo>				m_qDisplayFrameQueue;// 显示队列

	Dahua::Infra::CMutex				m_mxTime;
	int									m_nDisplayInterval;         // 显示间隔
	uintmax_t							m_nFirstFrameTime;          // 第一帧的时间戳
	uintmax_t							m_nLastFrameTime;           // 上一帧的时间戳
	QElapsedTimer						m_elapsedTimer;				//用来计时，控制显示帧率

	VR_HANDLE          m_handler;          /* 绘图句柄 */

	// 拉流线程
	void GrabFrameThreadProc(Dahua::Infra::CThreadLite& lite);
	// 显示线程
	void DisplayThreadProc(Dahua::Infra::CThreadLite& lite);
	// 设置显示频率，默认一秒钟显示30帧
	void setDisplayFPS(int nFPS);
	// 计算该帧是否显示
	bool isTimeToDisplay(uintmax_t nCurTime);

	float confThreshold = 0.5; // Confidence threshold
	float nmsThreshold = 0.4;  // Non-maximum suppression threshold
	int inpWidth = 416;  // Width of network's input image
	int inpHeight = 416; // Height of network's input image
	vector<string> classes;

	//图书检测
	bool bookdetection(Mat image_in);
	void testRun();
	/**
	* @brief 将 QImage 的类型图像转换为 cv::Mat 类型
	* @param image 待转换的图像，支持 Format_Indexed8/Format_Grayscale、24 位彩色、32 位彩色格式，
	* @param clone true 表示与 QImage 不共享内存，更改生成的 mat 不会影响原始图像，false 则会与 QImage 共享内存
	* @param rg_swap 只针对 RGB888 格式，如果 true 则会调换 R 与 B RGB->BGR，如果共享内存的话原始图像也会发生变化
	* @return 转换后的 cv::Mat 图像
	* https://blog.csdn.net/liyuanbhu/article/details/86307283
	*/
	Mat QImage2cvMat(QImage &image, bool clone = false, bool rb_swap = true);
	QImage cvMat2QImage(const cv::Mat& mat);

	/**
	* @name:
	* @test: test font
	* @msg: 线性拟合，根据传入参数点进行线性拟合判断相关系数跟斜率等
	* @param  const vector<Point> points 传入的点
	* @param slope 标准的斜率
	* @param intercept 标准的截距
	* @param r_square 标准的相关系数
	* @return: 0表示拟合的直线跟给定的直线不匹配，1表示拟合成功
	*/
	bool LinearFitting(const vector<Point> points, double slope, double intercept, double r_square);

	string get_datetime();

	vector<Rect> detect_image(Mat frame, string modelWeights, string modelConfiguration);
	vector<String> getOutputsNames(const Net& net);
	vector<Rect> postprocess_return(Mat& frame, const vector<Mat>& out);
	wstring string2tchar(const string& s);
	void run_exe(string path, string params);
	void run_database(string time, string result);
	void run_labelImg();
	void run_cut();
	void run_train();
	Mat read_mask();
	Rect mask_boundingRect(Mat mask);
	void draw_boxes(cv::Mat, std::vector<bbox_t>);

	void init_parameters();

	//最小二乘拟合相关函数定义----------------------
	int polyfit(vector<Point> points);
	double sum(vector<double> Vnum, int n);
	double MutilSum(vector<double> Vx, vector<double> Vy, int n);
	double RelatePow(vector<double> Vx, int n, int ex);
	double RelateMutiXY(vector<double> Vx, vector<double> Vy, int n, int ex);
	void EMatrix(vector<double> Vx, vector<double> Vy, int n, int ex, double coefficient[]);
	void CalEquation(int exp, double coefficient[]);
	double F(double c[], int l, int m);
	double Em[6][4];
	//最小二乘拟合相关函数定义-----------------------
	VR_HWND		m_hWnd;
};

#endif // MAINWINDOW_H
