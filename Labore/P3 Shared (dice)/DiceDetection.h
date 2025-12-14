/*! Digital image processing using OpenCV.
*
* \category Lab 3 Code 
* \author Suman Kafle
*/

#ifndef IP_DICE_DETECTION_H
#define IP_DICE_DETECTION_H

/* Include files */
#include <opencv2/core/core.hpp>
#include "BinaryRegions.h"

using namespace std;

namespace ip
{
	vector<ip::blob> locateDices(const cv::Mat& image, int threshPercent = 70, int morphSize = 3);
	void locateDices(const cv::Mat& image, cv::Mat& labeledImage, vector<ip::blob>& dices, int threshPercent = 70, int morphSize = 3);

	vector<ip::blob> locateDicePips(const cv::Mat& image, int threshPercent = 40, int morphSize = 5);
	void locateDicePips(const cv::Mat& image, cv::Mat& labeledImage, vector<ip::blob>& pips, int threshPercent = 40, int morphSize = 5);

	uchar getValueInDynamicRange(const cv::Mat& image, int percentage);
}

#endif /* IP_DICE_DETECTION_H */
