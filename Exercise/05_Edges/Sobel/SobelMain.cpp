/*! Digital image processing using OpenCV.
*
* \category Lecture sample code
* \author Marc Hensel (http://www.haw-hamburg.de/marc-hensel)
*/

/* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")			// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/Images/Docks.jpg"	// Image file including relative path
#define IS_WRITE_IMAGES false

/* Namespaces */
using namespace std;

/* Prototypes */
void sobel(const cv::Mat& image, cv::Mat& sobelAbs, cv::Mat& sobelX, cv::Mat& sobelY);

/* Main function */
int main()
{
	// Load image from file
	string inputImagePath = string(IMAGE_DATA_PATH).append(INPUT_IMAGE_RELATIVE_PATH);
	cv::Mat image = cv::imread(inputImagePath, cv::IMREAD_GRAYSCALE);

	if (image.empty()) {
		cout << "[ERROR] Cannot open image: " << inputImagePath << endl;
		return 0;
	}

	// Calculate Sobel edge images
	cv::Mat sobelAbs, sobelX, sobelY, sobelXAbs, sobelYAbs;
	sobel(image, sobelAbs, sobelX, sobelY);

	// Sobel edge images in x and y:
	// Signed (CV_8S) to absolute (CV_8U, [-127, 127] -> [0,127])
	sobelXAbs = cv::abs(sobelX);
	sobelYAbs = cv::abs(sobelY);
	sobelXAbs.convertTo(sobelXAbs, CV_8U);
	sobelYAbs.convertTo(sobelYAbs, CV_8U);

	// Sobel edge images in x and y:
	// Shift signed Sobel = 0 -> 127 ([-127, 127] -> [0,254])
	sobelX.convertTo(sobelX, CV_8U, 1, 127);
	sobelY.convertTo(sobelY, CV_8U, 1, 127);

	// Scale to use range
	sobelXAbs *= 2;								// [0, 127] -> [0, 254]
	sobelYAbs *= 2;
	sobelAbs *= 255.5 / (int)(sqrt(2) * 127);	// sqrt(2) * [0, 127] = [0, 179] -> [0, 255]

	// Display images
	cv::imshow("Image", image);
	cv::imshow("Sobel x", sobelX);
	cv::imshow("Sobel y", sobelY);
	cv::imshow("Absolute Sobel x", sobelXAbs);
	cv::imshow("Absolute Sobel y", sobelYAbs);
	cv::imshow("Absolute Sobel", sobelAbs);

	// Write images to file
	if (IS_WRITE_IMAGES) {
		imwrite("D:/Gray.jpg", image);
		imwrite("D:/_SobelX.jpg", sobelX);
		imwrite("D:/_SobelY.jpg", sobelY);
		imwrite("D:/_SobelXAbs.jpg", sobelXAbs);
		imwrite("D:/_SobelYAbs.jpg", sobelYAbs);
		imwrite("D:/_Sobel.jpg", sobelAbs);
	}

	// Wait for keypress and terminate
	cv::waitKey(0);
	return 0;
}

/*! Calculate Sobel edge image(s).
*
* \param image Source image to calculate Sobel edge images for
* \param sobelAbs Absolute Sobel image sqrt(Sobel(x)^2 + Sobel(y)^2) in [0, sqrt(2) * 127]
* \param sobelX Signed Sobel edge image in x direction in [-127, 127]
* \param SobelY Signed Sobel edge image in y direction [-127, 127]
*/
void sobel(const cv::Mat& image, cv::Mat& sobelAbs, cv::Mat& sobelX, cv::Mat& sobelY) {
	// Filter kernels
	cv::Mat kernelGradient = (cv::Mat_<double>(1, 3) << -1, 0, 1) / 2.0;
	cv::Mat kernelBinomial = (cv::Mat_<double>(1, 3) << 1, 2, 1) / 4.0;

	// Signed sobel edge images in x and y
	cv::Mat image16S;
	image.convertTo(image16S, CV_16S, 128);	// sepFilter2D does not support CV_8U -> CV_16S
	cv::sepFilter2D(image16S, sobelX, CV_16S, kernelGradient, kernelBinomial);
	cv::sepFilter2D(image16S, sobelY, CV_16S, kernelBinomial, kernelGradient);

	// Calculate absolute Sobel edge image
	sobelAbs = image.clone();
	for (int y = 0; y < image.rows; y++) {
		short* rowX = sobelX.ptr<short>(y);
		short* rowY = sobelY.ptr<short>(y);
		uchar* rowAbs = sobelAbs.ptr<uchar>(y);

		for (int x = 0; x < image.cols; x++) {
			int gx = (int)rowX[x], gy = (int)rowY[x];
			rowAbs[x] = (uchar)(cv::sqrt(gx * gx + gy * gy) / 128.0);	// CV_16S -> CV_8U
		}
	}

	// Convert CV_16S -> CV_8S
	sobelX.convertTo(sobelX, CV_8S, 1.0 / 128.0);
	sobelY.convertTo(sobelY, CV_8S, 1.0 / 128.0);
}
