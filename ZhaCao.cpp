#include <iostream>
using namespace std;

#include <stdio.h>
#include <vector>
#include "cv.h"
#include "highgui.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include "Fenge.h"

int  main()
{
	//阈值分割
	ColorDetector cdetect;

	cv::Mat image = cv::imread("Ta.jpg");

	cv::namedWindow("show");
	cv::imshow("show", image);

	cdetect.setTargetColor(-149, 218, -73);

	cv::namedWindow("fenge result");
	cv::imshow("fenge result", cdetect.process(image));
	cv::imwrite("fenge.jpg", cdetect.process(image));

	//中值滤波
	cv::Mat image2 = cdetect.process(image);;

	cv::Mat lbresult;
	cv::medianBlur(image2, lbresult, 7);

	cv::namedWindow("Median Image");
	cv::imshow("Median Image", lbresult);
	cv::imwrite("Median.jpg", lbresult);

	//形态学处理
	cv::Mat image3 = lbresult;

	cv::Mat element3(3, 3, CV_8U, cv::Scalar(1));
	cv::Mat opened;
	cv::morphologyEx(image3, opened, cv::MORPH_OPEN, element3);

	cv::namedWindow("Opened Image");
	cv::imshow("Opened Image", opened);
	cv::imwrite("open.jpg", opened);


	//边缘检测与提取
	cv::Mat image4 = opened;

	cv::Mat bianyuan;
	cv::Canny(image4, bianyuan, 10, 255);

	cv::namedWindow("canny Image");
	cv::imshow("canny Image", bianyuan);
	cv::imwrite("bianyuan.jpg", bianyuan);


	//矩形与质心
	cv::Mat image5 = cv::imread("bianyuan.jpg", 0);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(image5,
		contours,
		CV_RETR_EXTERNAL,
		CV_CHAIN_APPROX_NONE);

	std::cout << "Contours: " << contours.size() << std::endl;
	std::vector<std::vector<cv::Point>>::const_iterator itContours = contours.begin();
	for (; itContours != contours.end(); ++itContours) {

		std::cout << "Size: " << itContours->size() << std::endl;
	}

	int cmin = 250;
	int number = 0;
	std::vector<std::vector<cv::Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end()) {

		if (itc->size() < cmin)
		{
			itc = contours.erase(itc);
		}
		else
		{
			++itc, number++;
		}
	}
	std::cout << "contours number: " << number << std::endl;

	cv::Mat result = bianyuan;
	result.setTo(cv::Scalar(0));
	cv::drawContours(result, contours,
		-1,
		cv::Scalar(255),
		1);

	cv::namedWindow("descriptors");
	cv::imshow("descriptors", result);

	int i = 0;
	for (; i < number; i++)
	{
		cv::Rect ri = cv::boundingRect(cv::Mat(contours[i]));
		cv::rectangle(result, ri, cv::Scalar(255), 1);
	}

	double a[2][100];

	int j = 0;
	itc = contours.begin();
	while (itc != contours.end()) {
		j++;
		cv::Moments mom = cv::moments(cv::Mat(*itc++));

		cv::circle(result,
			cv::Point(mom.m10 / mom.m00, mom.m01 / mom.m00),
			2, cv::Scalar(255), 2);
		std::cout << "Image Point" << j << " U: " <<
			(mom.m10 / mom.m00) << "  V: " << (mom.m01 / mom.m00) << std::endl;
		a[0][j] = mom.m10 / mom.m00; a[1][j] = mom.m01 / mom.m00;
	}

	cv::namedWindow("Some Shape descriptors");
	cv::imshow("Some Shape descriptors", result);
	cv::imwrite("weizhi.jpg", result);


	//坐标转换
	std::cout << "Space Point : " << std::endl;
	for (j = 0; j < number; j++)
	{

		double W = 640, H = 480, px = 100, py = 80;
		double X, Y, U, V, x, y;
		double d = 80, f = 0.5, Uo = W / 2, Vo = H / 2;

		U = a[0][j + 1];
		V = a[1][j + 1];

		x = (U - Uo);
		y = (V - Vo);

		X = 100 * U / W - px / 2;
		Y = 80 * V / H - py / 2;

		std::cout << "x  Point" << j + 1 << " x: " << x << "  y: " << y << std::endl;
		std::cout << "WPoint " << j + 1 << " X: " << X << std::endl;
		std::cout << "        " << " Y: " << Y << std::endl;

	}

	cv::waitKey();
	return 0;
}