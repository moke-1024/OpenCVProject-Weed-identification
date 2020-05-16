#if !defined COLORDETECT
#define COLORDETECT

#include <opencv2/core/core.hpp>

class ColorDetector {

private:

	int minDist;

	cv::Vec3b target;

	cv::Mat result;

	cv::Mat converted;

	int getDistance(const cv::Vec3b& color) const {

		return abs(color[0] - target[0]) +
			abs(color[1] - target[1]) +
			abs(color[2] - target[2]);
	}

public:

	ColorDetector() : minDist(100) { target[0] = target[1] = target[2] = 0; }

	void setColorDistanceThreshold(int distance)
	{
		if (distance < 0)
			distance = 0;
		minDist = distance;
	}

	int getColorDistanceThreshold() const
	{

		return minDist;
	}

	void setTargetColor(unsigned char red,
		unsigned char green,
		unsigned char blue)
	{

		target[2] = red;
		target[1] = green;
		target[0] = blue;

	}

	void setTargetColor(cv::Vec3b color)
	{
		target = color;
	}

	cv::Vec3b getTargetColor() const
	{
		return target;
	}


	cv::Mat process(const cv::Mat &image)
	{
		result.create(image.rows, image.cols, CV_8U);

		cv::Mat_<cv::Vec3b>::const_iterator it = image.begin<cv::Vec3b>();
		cv::Mat_<cv::Vec3b>::const_iterator itend = image.end<cv::Vec3b>();
		cv::Mat_<uchar>::iterator itout = result.begin<uchar>();

		for (; it != itend; ++it, ++itout)
		{
			if (getDistance(*it) < minDist)
			{
				*itout = 255;
			}
			else
			{
				*itout = 0;
			}
		}
		return result;
	}
};

#endif