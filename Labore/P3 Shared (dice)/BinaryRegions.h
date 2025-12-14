/*! Digital image processing using OpenCV.
*
* \category Lab 3 Code 
* \author Suman Kafle
*/

#pragma once
#ifndef IP_BINARY_REGIONS_H
#define IP_BINARY_REGIONS_H

/* Include files */
#include <opencv2/core/core.hpp>

using namespace std;

namespace ip
{
	/* BLOB data type */
	typedef struct blob {
		int label = 0;
		cv::Point point;
		int size = 0;						// Number pixels
		cv::Point cog = cv::Point(0, 0);	// Center of gravity
		cv::Rect2i boundingBox;
	} blob;

	/* Region labeling */
	void labelRegions(cv::Mat& binImage, vector<blob>& blobs);
	blob floodFill(cv::Mat& binImage, int x, int y, uchar label, uchar oldLabel = 1);

	/* RGB display */
	void labels2RGB(const cv::Mat& labelImage, cv::Mat& rgbImage);
	void annotateBlobs(cv::Mat& rgbImage, vector<blob>& blobs);

	/* BLOBs */
	int maxBlobSize(vector<blob>& blobs);
	uchar minBlobPixel(const cv::Mat& grayImage, const cv::Mat& labeledImage, int label);
	void removeSmallBlobs(cv::Mat& labeledImage, vector<blob>& blobs, int minSize);
}

#endif /* IP_BINARY_REGIONS_H */
