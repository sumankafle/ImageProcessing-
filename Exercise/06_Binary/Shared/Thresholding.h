/*! Digital image processing using OpenCV.
*
* \category Excercise Code
* \author Suman Kafle
*/


#pragma once
#ifndef IP_THRESHOLDING_H
#define IP_THRESHOLDING_H

/* Include files */
#include <opencv2/core/core.hpp>

namespace ip
{
	void threshold(const cv::Mat& image, cv::Mat& binImage, uchar thresh, bool isInvert = false);
	void isodataThreshold(const cv::Mat& image, cv::Mat& binImage, uchar* calculatedThresh = NULL);
	void bernsenThreshold(const cv::Mat& image, cv::Mat& binImage, int radius, uchar minContrast, uchar background = 0);
}

#endif /* IP_THRESHOLDING_H */
