/*! Digital image processing using OpenCV.
*
* \category Lab 2 Code 
* \author Suman Kafle
*/

#pragma once
#ifndef IP_SOBEL_H
#define IP_SOBEL_H

/* Include files */
#include <opencv2/core/core.hpp>

namespace ip
{
	/* Prototypes */
	void sobelFilter(const cv::Mat& image, cv::Mat& sobel);
}

#endif /* IP_SOBEL_H */
