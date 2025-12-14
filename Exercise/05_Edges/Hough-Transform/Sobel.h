/*! Digital image processing using OpenCV.
*
* \category Excercise Code
* \author Suman Kafle
*/


#pragma once
#ifndef IP_SOBEL_H
#define IP_SOBEL_H

/* Include files */
#include <opencv2/core/core.hpp>

namespace ip
{
	void sobelFilter(const cv::Mat& image, cv::Mat& sobel);
}

#endif /* IP_SOBEL_H */
