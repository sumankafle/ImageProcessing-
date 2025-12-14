/*! Digital image processing using OpenCV.
*
* \category Lab 2 Code  
* \author Suman Kafle
*/

/* Include files */
#include "Sobel.h"
#include <opencv2/imgproc/imgproc.hpp>

namespace ip {
	/*! Calculate Sobel edge image(s).
	*
	* \param image Source image to calculate Sobel edge images for
	* \param sobel Absolute Sobel image sqrt(Sobel(x)^2 + Sobel(y)^2) in [0, sqrt(2) * 127]
	*/
	void sobelFilter(const cv::Mat& image, cv::Mat& sobel) {
		// Filter kernels
		cv::Mat kernelGradient = (cv::Mat_<double>(1, 3) << -1, 0, 1) / 2.0;
		cv::Mat kernelBinomial = (cv::Mat_<double>(1, 3) << 1, 2, 1) / 4.0;

		// Signed sobel edge images in x and y
		cv::Mat image16S, sobelX, sobelY;
		image.convertTo(image16S, CV_16S, 128);	// sepFilter2D does not support CV_8U -> CV_16S
		cv::sepFilter2D(image16S, sobelX, CV_16S, kernelGradient, kernelBinomial);
		cv::sepFilter2D(image16S, sobelY, CV_16S, kernelBinomial, kernelGradient);

		// Calculate absolute Sobel edge image
		sobel = image.clone();
		for (int y = 0; y < image.rows; y++) {
			short* rowX = sobelX.ptr<short>(y);
			short* rowY = sobelY.ptr<short>(y);
			uchar* rowAbs = sobel.ptr<uchar>(y);

			for (int x = 0; x < image.cols; x++) {
				int gx = (int)rowX[x];
				int gy = (int)rowY[x];

				rowAbs[x] = (uchar)(cv::sqrt(gx * gx + gy * gy) / 128.0);	// CV_16S -> CV_8U
			}
		}
	}
}
