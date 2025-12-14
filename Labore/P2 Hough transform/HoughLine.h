/*! Digital image processing using OpenCV.
*
* \category Lab 2 Code 
* \author Suman Kafle
*/

#pragma once
#ifndef IP_HOUGH_LINE_H
#define IP_HOUGH_LINE_H

/* Include files */
#include <opencv2/core/core.hpp>

namespace ip
{
	/* Prototypes */
	void houghTransform(const cv::Mat& edgeImage, cv::Mat& houghSpace, int height = 721, int width = 720);
	void houghSpaceToLine(cv::Size imgSize, cv::Size houghSize, int x, int y, double& r, double& theta);
	void drawLine(cv::Mat& image, double r, double theta);
	void drawHoughLineLabels(cv::Mat& houghSpace);
}

#endif /* IP_HOUGH_LINE_H */
