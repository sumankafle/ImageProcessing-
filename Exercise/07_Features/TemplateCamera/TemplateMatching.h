/*! Digital image processing using OpenCV.
*
* \category Excercise Code
* \author Suman Kafle
*/

#pragma once
#ifndef IP_TEMPLATE_MATCHING_H
#define IP_TEMPLATE_MATCHING_H

/* Include files */
#include <opencv2/core/core.hpp>

namespace ip
{
	void templateMatching(const cv::Mat& image, const cv::Mat& object, cv::Mat& ssdImage, cv::Point& bestMatch);
}

#endif /* IP_TEMPLATE_MATCHING_H */
