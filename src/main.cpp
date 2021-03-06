#include <opencv2/opencv.hpp>
#include <iostream>
#include <ctime>
#include <chrono>
#include <memory>
#include "yolov5.h"
#include "hikcam.h"

std::unique_ptr<yolov5> yolo;
std::map<int, std::string> labels;
std::unique_ptr<JsonDeal> jsonDeal;

int main(int argc, char **argv)
{
	jsonDeal = std::make_unique<JsonDeal>();


	std::shared_ptr<HikCam> phikcam;
	try
	{
		phikcam = std::make_shared<HikCam>(HikCam("192.168.1.64", 8000, "admin", "hfut8888"));
	}
	catch (const char *&e)
	{
		std::cout << e << std::endl;
		return -1;
	}

	// 第二个参数为是否启用 cuda 详细用法可以参考 yolov5.h 文件
	std::cout << torch::cuda::is_available() << std::endl;
	yolo = std::make_unique<yolov5>("/home/xac/rhs/Hikcam-using-yolov5-cpp/models/yolov5s.cuda.pt", torch::cuda::is_available());
	yolo->prediction(torch::rand({1, 3, 640, 640}));
	// 读取分类标签（我们用的官方的所以这里是 coco 中的分类）
	std::ifstream f("../models/coco.txt");
	std::string name = "";
	int i = 0;
	while (std::getline(f, name))
	{
		labels.insert(std::pair<int, std::string>(i, name));
		i++;
		//std::cout << name << std::endl;
	}

	
	
	// 用 OpenCV 打开摄像头读取文件
	cv::VideoCapture cap = cv::VideoCapture(0);
	// 设置宽高 无所谓多宽多高后面都会通过一个算法转换为固定宽高的
	// 固定宽高值应该是你通过YoloV5训练得到的模型所需要的
	// 传入方式是构造 yolov5 对象时传入 width 默认值为 640，height 默认值为 640
	cap.set(cv::CAP_PROP_FRAME_WIDTH, 1000);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 800);
	cv::Mat frame;

	while (cap.isOpened())
	{
		// 读取一帧
		cap.read(frame);
		if (frame.empty())
		{
			std::cout << "Read frame failed!" << std::endl;
			break;
		}

		// 预测
		clock_t start = clock();
		std::vector<torch::Tensor> r = yolo->prediction(frame);
		clock_t ends = clock();
		std::cout << "Running Time : " << (double)(ends - start) / CLOCKS_PER_SEC << std::endl;

		// 画框根据你自己的项目调用相应的方法，也可以不画框自己处理
		frame = yolo->drawRectangle(frame, r[0], labels);
		// show 图片
		cv::imshow("", frame);
		if (cv::waitKey(1) == 27)
			break;
	}
	
	phikcam->startStream(0, 1, 0, 1, 1);
	

	return 0;
}
