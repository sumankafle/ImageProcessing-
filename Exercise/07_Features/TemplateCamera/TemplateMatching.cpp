/*! Digital image processing using OpenCV.
*
* \category Excercise Code
* \author Suman Kafle
*/


/* Include files */
#include <cstdint>						// Declare int32_t
#include <opencv2/imgproc/imgproc.hpp>
#include "TemplateMatching.h"

/* Namespaces */
using namespace std;

namespace ip {

	/*! Template matching using sum of squared differences (SSD).
	* 
	* \param image [in] Source image
	* \param object [in] Template to search for in source image
	* \param ssdImage [out] Calculated sums of squared differences (SSD)
	* \param bestMatch [out] Location with minimum SSD
	*/
	void templateMatching(const cv::Mat& image, const cv::Mat& object, cv::Mat& ssdImage, cv::Point& bestMatch) {
		int ssdCols = image.cols - object.cols + 1;
		int ssdRows = image.rows - object.rows + 1;
		int32_t minSSD = INT32_MAX, maxSSD = -1;

		// Error: Template image not smaller than source image
		if ((ssdCols < 1) || (ssdRows < 1)) {
			ssdImage = cv::Mat();
			return;
		}

		// Allocate resulting image (sum of squared differences)
		ssdImage = cv::Mat(cv::Size(ssdCols, ssdRows), CV_32S);

		// Run through upper left corners (x, y) in source image
		for (int y = 0; y < ssdRows; y++) {
			for (int x = 0; x < ssdCols; x++) {
				int32_t* ssdRow = (int32_t*)ssdImage.ptr<int32_t>(y);
				int32_t ssd = 0;

				// Calculate SSD for template and source image at (x, y)
				for (int n = 0; n < object.rows; n++) {
					const uchar* imageRow = (uchar*)image.ptr<uchar>(y + n);
					const uchar* objectRow = (uchar*)object.ptr<uchar>(n);

					for (int m = 0; m < object.cols; m++) {
						int32_t difference = (int32_t)imageRow[x + m] - (int32_t)objectRow[m];
						ssd += difference * difference;
					}
				}
				ssdRow[x] = ssd;

				// Determine minimum SSD (= best match)
				if (ssd < minSSD) {
					minSSD = ssd;
					bestMatch.x = x;
					bestMatch.y = y;
				}

				// Determine maximum SSD (to scale contrast)
				if (ssd > maxSSD)
					maxSSD = ssd;
			}
		}

		// Convert SSD image to 8-bit grayscale in [min, 255]
		ssdImage.convertTo(ssdImage, CV_8U, 255.0 / maxSSD);
	}
}
