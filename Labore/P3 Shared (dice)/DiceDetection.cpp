/*! Digital image processing using OpenCV.
*
* \category Lab 3 Code 
* \author Suman Kafle
*/

/* Include files */
#include <opencv2/imgproc/imgproc.hpp>
#include "DiceDetection.h"

/* Namespaces */
using namespace std;

namespace ip
{
	/*! Locate dices in image.
	* 
	* Convenience method. Please refer to documentation of other method locateDices().
	*/
	vector<ip::blob> locateDices(const cv::Mat& image, int threshPercent, int morphSize) {
		cv::Mat labeledImage;
		vector<ip::blob> dices;
		locateDices(image, labeledImage, dices, threshPercent, morphSize);
		return dices;
	}

	/*! Locate dices in image.
	* 
	* Approach:
	* 1. Apply threshold and remove small BLOBS by morphological OPEN
	* 2. Label white regions
	* 3. Remove regions with less than half size of largest object
	* 
	* Thresholding is applied relative to the dynamic range of the image:
	* tau = min + percentage/100 * (max - min)
	* 
	* \param image [in] Input image (type CV_U8)
	* \param labeledImage [out] Image containing BLOBs as labeled regions (label = 2, 3, 4, ...)
	* \param dices [out] List of resulting BLOBs supposed to contain dices
	* \param threshPercent [in] Threshold in percent 0 .. 100 of the dynamic range
	* \param morphSize [in] Kernel size for the morphological operation
	*/
	void locateDices(const cv::Mat& image, cv::Mat& labeledImage, vector<ip::blob>& dices, int threshPercent, int morphSize) {
		// Create clean binary image
		cv::Mat binImage;
		cv::threshold(image, binImage, getValueInDynamicRange(image, threshPercent), 255, cv::THRESH_BINARY);
		cv::morphologyEx(binImage, binImage, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(morphSize, morphSize)));

		// Identify largest binary regions
		labeledImage = binImage / 255;		// => Values in {0, 1}
		ip::labelRegions(labeledImage, dices);
		ip::removeSmallBlobs(labeledImage, dices, ip::maxBlobSize(dices) / 2);
	}

	/*! Locate dice pips in image.
	*
	* Convenience method. Please refer to documentation of other method locateDicePips().
	*/
	vector<ip::blob> locateDicePips(const cv::Mat& image, int threshPercent, int morphSize) {
		cv::Mat labeledImage;
		vector<ip::blob> pips;
		locateDicePips(image, labeledImage, pips, threshPercent, morphSize);
		return pips;
	}

	/*! Locate dice pips in image.
	* 
	* The input image should contain the dice, only, if possible.
	* For instance, use a subimage corresponding to the bounding box of the dice.
	* 
	* Approach:
	* 1. Apply threshold tau_1 and fill small holes by morphological OPEN
	* 2. Label dark regions
	* 3. Remove regions that do NOT ...
	*    a) contain a very dark pixel (threshold tau_2 < tau_1),
	*    b) have a bounding box which is approximately square.
	* 
	* Thresholding is applied relative to the dynamic range of the image:
	* tau = min + percentage/100 * (max - min)
	* 
	* Hysteresis thresholding:
	* Note that regions are identified using a kind of "threshold hysteresis": Regions <= tau_1
	* are kept only if they contain at least one dark pixel <= tau_2 < tau_1. In effect, pip regions
	* remain larger, but regions consisting of shades are excluded.
	*
	* \param image [in] Input image (type CV_U8)
	* \param labeledImage [out] Image containing BLOBs as labeled regions (label = 2, 3, 4, ...)
	* \param dices [out] List of resulting BLOBs supposed to contain pips
	* \param threshPercent [in] Threshold in percent 0 .. 100 of the dynamic range
	* \param morphSize [in] Kernel size for the morphological operation
	*/
	void locateDicePips(const cv::Mat& image, cv::Mat& labeledImage, vector<ip::blob>& pips, int threshPercent, int morphSize) {
		// Create clean binary image
		cv::Mat binImage;
		cv::threshold(image, binImage, getValueInDynamicRange(image, threshPercent), 255, cv::THRESH_BINARY);
		cv::morphologyEx(binImage, binImage, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(morphSize, morphSize)));

		// Label binary regions
		labeledImage = (255 - binImage) / 255;
		ip::labelRegions(labeledImage, pips);

		// Remove regions without very dark pixels and/or bounding box not (almost) squared
		for (int i = (int)pips.size() - 1; i >= 0; i--) {
			ip::blob blob = pips.at(i);
			cv::Rect2i box = blob.boundingBox;
			uchar pixelMin = ip::minBlobPixel(image, labeledImage, blob.label);
			uchar pixelLimit = getValueInDynamicRange(image, 5);
			double ratio = (box.width < box.height) ? (box.width / (double)box.height) : (box.height / (double)box.width);

			if ((pixelMin > pixelLimit) || (ratio < 0.75)) {
				ip::floodFill(labeledImage, blob.point.x, blob.point.y, 0, blob.label);
				pips.erase(pips.begin() + i);
			}
		}
	}

	/*! Get the gray value corresponding to p% of the dynamic range [min, max].
	* 
	* \param image [in] Input image (type CV_U8)
	* \param percentage [in] Percentage p in [0, 100]
	* \return min + p/100 * (max - min)
	*/
	uchar getValueInDynamicRange(const cv::Mat& image, int percentage) {
		double pixelMin, pixelMax;

		cv::minMaxLoc(image, &pixelMin, &pixelMax);
		return (uchar)(pixelMin + (percentage / 100.0) * (pixelMax - pixelMin));
	}
}