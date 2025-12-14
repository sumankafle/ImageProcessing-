/*! Digital image processing using OpenCV.
*
* \category Excercise Code
* \author Suman Kafle
*/


#pragma once
#ifndef IP_BINARY_REGIONS_H
#define IP_BINARY_REGIONS_H

/* Include files */
#include <opencv2/core/core.hpp>

namespace ip
{
	/* BLOB data type */
	typedef struct blob {
		unsigned size = 0;			// Number pixels
		cv::Point cog;				// Center of gravity
		cv::Rect2i boundingBox;
	} blob;

	/* Region labeling */
	void labelRegions(cv::Mat& binImage);
	void floodFill(cv::Mat& binImage, int x, int y, uchar label);

	/* BLOB processing */
	void labels2RGB(const cv::Mat& labelImage, cv::Mat& rgbImage);
	void labels2BlobFeatures(const cv::Mat& labelImage, blob blobs[256]);
	void annotateBlobs(cv::Mat& rgbImage, const blob blobs[256]);
}

#endif /* IP_BINARY_REGIONS_H */
