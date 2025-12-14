/*! Digital image processing using OpenCV.
*
* \category Excercise Code
* \author Suman Kafle
*/


#pragma once
#ifndef IP_HISTOGRAM_H
#define IP_HISTOGRAM_H

/* Include files */
#include <opencv2/core/core.hpp>

namespace ip
{
	/* Prototypes */
	unsigned max(unsigned values[], int size);
	void calcHistogram(const cv::Mat& image, unsigned histogram[256], unsigned cumulative[256] = NULL);
	void createHistogramImage(const cv::Mat& image, cv::Mat& histogram, bool isCumulative = true);
	void addLineToHistogramImage(cv::Mat& histogram, uchar level, cv::Scalar color = cv::Scalar(255, 0, 0));
}

#endif /* IP_HISTOGRAM_H */
