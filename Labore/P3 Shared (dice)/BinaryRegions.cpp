/*! Digital image processing using OpenCV.
*
* \category Lab 3 Code 
* \author Suman Kafle
*/

/* Include files */
#include <iostream>
#include <queue>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "BinaryRegions.h"

/* Namespaces */
using namespace std;

namespace ip
{
	/*! Label regions in binary image.
	* 
	* Resulting image will have following pixel values:
	* - 0: Background ("0 remains 0")
	* - 2 to 255: Binary regions (no further regions when 255 reached)
	* 
	* \param binImage [in/out] Binary image with values in {0, 1} to label
	* \param blobs [out] List of labeled regions
	*/
	void labelRegions(cv::Mat& binImage, vector<blob>& blobs) {
		uchar nextLabel = 2;

		// Empty list of blobs
		blobs.clear();

		// Run through binary image pixels
		for (int y = 0; y < binImage.rows; y++) {
			uchar* row = binImage.ptr<uchar>(y);

			for (int x = 0; x < binImage.cols; x++) {
				// Unlabeled pixel found => Label region
				if (row[x] == 1) {
					blobs.push_back(floodFill(binImage, x, y, nextLabel));

					if (nextLabel < 255)
						nextLabel++;
					else {
						cout << "WARNING (labelRegions): Maximum number of regions reached." << endl;
						return;
					}
				}
			}
		}
	}

	/*! Fills binary object using flood fill.
	* 
	* - The implemtation uses a breadth-first approach with N4 neighborhood.
	* - Unlabeled pixels are supposed to have the value oldLabel.
	* - Pass label = 0 and appropriate oldLabel to remove a BLOB from a binary image
	* 
	* \param binImage [in/out] Binary image to label region in
	* \param x [in] Location (x,y) of a pixel of the region to label
	* \param y [in] Location (x,y) of a pixel of the region to label
	* \param label [in] Value to assign to the binary region
	* \param oldLabel [in] Value of unlabeled pixels
	*/
	blob floodFill(cv::Mat& binImage, int x, int y, uchar label, uchar oldLabel) {
		queue<cv::Point> q;

		// Init blob
		blob blob;
		int minX = x, maxX = x;
		int minY = y, maxY = y;
		blob.label = label;
		blob.point = cv::Point(x, y);

		// Init queue with first pixel location of BLOB
		q.push(cv::Point(x, y));

		// Process queue
		while (!q.empty()) {
			// Get and remove next pixel location
			x = q.front().x;
			y = q.front().y;
			q.pop();

			// Is pixel new BLOB pixel?
			if ((x >= 0) && (x < binImage.cols) && (y >= 0) && (y < binImage.rows) && binImage.at<uchar>(y, x) == oldLabel) {
				// Mark as BLOB pixel
				binImage.at<uchar>(y, x) = label;

				// Update blob information
				blob.size++;
				blob.cog.x += x;
				blob.cog.y += y;
				minX = min(x, minX);
				maxX = max(x, maxX);
				minY = min(y, minY);
				maxY = max(y, maxY);

				// Add 4-neighbors to queue
				q.push(cv::Point(x + 1, y));
				q.push(cv::Point(x, y + 1));
				q.push(cv::Point(x, y - 1));
				q.push(cv::Point(x - 1, y));
			}
		}

		// Calculate center of gravity
		blob.cog.x = (int)((blob.cog.x + 0.5) / blob.size);
		blob.cog.y = (int)((blob.cog.y + 0.5) / blob.size);

		// Set bounding box
		blob.boundingBox.x = minX;
		blob.boundingBox.y = minY;
		blob.boundingBox.width = maxX - minX + 1;
		blob.boundingBox.height = maxY - minY + 1;

		return blob;
	}

	/*! Created RGB image with regions displayed in different colors.
	*
	* Regions are defined by the numeric value in the input image.
	* The background (region value = 0) is set to white.
	*
	* \param labelImage [in] Input image containing regions (i. e., connected sets with same value > 0)
	* \param rgbImage [out] Image with regions having different colors
	*/
	void labels2RGB(const cv::Mat& labelImage, cv::Mat& rgbImage) {
		const uchar deltaHue = 30;
		const uchar deltaValue = -50;
		cv::Mat colors(cv::Size(256, 1), CV_8UC3);
		cv::Vec3b* colorsPtr = colors.ptr<cv::Vec3b>(0);

		// Init HSV colors and convert to RGB
		uchar hue = 0, saturation = 255, value = 200;

		colorsPtr[0] = cv::Vec3b(0, 0, 255);	// White
		for (int i = 1; i < 256; i++) {
			colorsPtr[i] = cv::Vec3b(hue, saturation, value);
			hue += deltaHue;
			if (i % (255 / deltaHue) == 0)
				value -= deltaValue;
		}

		// Convert HSV colors to RGB
		cv::cvtColor(colors, colors, cv::COLOR_HSV2BGR);

		// Create colored copy
		rgbImage = cv::Mat(cv::Size(labelImage.cols, labelImage.rows), CV_8UC3);

		for (int y = 0; y < labelImage.rows; y++) {
			const uchar* srcRow = labelImage.ptr<uchar>(y);
			cv::Vec3b* dstRow = rgbImage.ptr<cv::Vec3b>(y);

			for (int x = 0; x < labelImage.cols; x++) {
				dstRow[x] = colors.at<cv::Vec3b>(srcRow[x]);
			}
		}
	}

	/*! Draw blob information on RGB image.
	*
	* \param rgbImage [in/out] Image to draw on (typically contains regions corresponding to parameter blobs)
	* \param blobs [in] Information to draw on image
	*/
	void annotateBlobs(cv::Mat& rgbImage, vector<blob>& blobs) {
		cv::Scalar BLACK = cv::Scalar(0, 0, 0);
		cv::Scalar RED = cv::Scalar(0, 0, 255);

		// Run through labels (BLOBs)
		for (int i = 0; i < blobs.size(); i++) {
			blob blob = blobs.at(i);
			cv::Rect box = blob.boundingBox;

			// Draw center of gravity, bounding box, and size
			cv::circle(rgbImage, blob.cog, 1, BLACK, 2);
			cv::rectangle(rgbImage, box, RED, 1);
			cv::putText(
				rgbImage,
				to_string(blob.size),
				cv::Point(box.x + box.width, box.y),
				cv::FONT_HERSHEY_PLAIN, 1.0, RED, 1);
		}
	}

	/*! Get size of largest BLOB in a list.
	* 
	* \param blobs [in] List of BLOBs
	* \return size of largest BLOB in pixel
	*/
	int maxBlobSize(vector<blob>& blobs) {
		int max = 0;

		for (int i = 0; i < blobs.size(); i++) {
			ip::blob blob = blobs.at(i);

			if (blob.size > max)
				max = blob.size;
		}
		return max;
	}

	/*! Get minimum gray value of pixels in a area defined by a BLOB.
	* 
	* \param grayImage [in] Image containing the original gray values
	* \param labeledImage [in] Image containing BLOBs as labeled regions (label = 2, 3, 4, ...)
	* \param label [in] Label of BLOB for which to return the minimum gray value
	*/
	uchar minBlobPixel(const cv::Mat& grayImage, const cv::Mat& labeledImage, int label) {
		uchar minPixel = 255;

		for (int y = 0; y < labeledImage.rows; y++) {
			const uchar* labeledRow = labeledImage.ptr<uchar>(y);
			const uchar* grayRow = grayImage.ptr<uchar>(y);

			for (int x = 0; x < labeledImage.cols; x++) {
				if ((labeledRow[x] == label) && (grayRow[x] < minPixel)) {
					minPixel = grayRow[x];
				}
			}
		}

		return minPixel;
	}

	/*! Remove all BLOBs with a size smaller than the argument minSize.
	* 
	* BLOBs are removed in the image and in the list of BLOBs.
	* 
	* \param
	*/
	void removeSmallBlobs(cv::Mat& labeledImage, vector<blob>& blobs, int minSize) {
		for (int i = (int)blobs.size() - 1; i >= 0; i--) {
			ip::blob blob = blobs.at(i);

			if (blob.size < minSize) {
				ip::floodFill(labeledImage, blob.point.x, blob.point.y, 0, blob.label);
				blobs.erase(blobs.begin() + i);
			}
		}
	}
}
