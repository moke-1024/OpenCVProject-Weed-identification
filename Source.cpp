#include <stdio.h>
#include <vector>
#include "cv.h"
#include "highgui.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>


int  main()
{
	cv::Mat image = cv::imread("Ta.jpg");

	cv::namedWindow("Myshow");
	cv::imshow("Myshow", image);
	cv::waitKey(5000);
	return 1;

}