/*! Digital image processing using OpenCV.
*
* \category Excercise Code
* \author Suman Kafle
*/


/* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Fourier.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")		// Read environment variable ImagingData
#define FILE_PATH "/images/Docks.jpg"				// Image file including relative path
#define SAVE_IMAGES true

/* Namespaces */
using namespace std;

/* Prototypes */
void kernelSeparatedTo2D(const cv::Mat& kernelX, const cv::Mat& kernelY, cv::Mat& kernel2D, bool isNormalize = true);

/* Main function */
int main()
{
	// Load image from file
	string filePath = string(IMAGE_DATA_PATH).append(FILE_PATH);
	cv::Mat image = cv::imread(filePath, cv::IMREAD_GRAYSCALE);

	if (image.empty()) {
		cout << "[ERROR] Cannot open image: " << filePath << endl;
		return 0;
	}
	cv::imshow("Image", image);

	// Define filter kernel
	//cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
	//	1, 0, -1,
	//	2, 0, -2,
	//	1, 0, -1) / 4.0;

	cv::Mat kernel;
	cv::Mat kernelX = cv::getGaussianKernel(7, -1, CV_32F);
	printf("sigma = %f\n", 0.3 * ((kernelX.rows - 1.0) * 0.5) + 0.8);
	kernelSeparatedTo2D(kernelX, kernelX, kernel);

	// Linear filtering by convolution
	cv::Mat filtered;
	cv::filter2D(image, filtered, image.depth(), kernel);
	cv::imshow("Filtered (space domain)", filtered);

	// Image and kernel in frequency domain
	ip::fourier freqImage, freqKernel;
	cv::Mat powerSpectrum;
	ip::fourierTransform(image, freqImage);
	ip::fourierTransformFilterKernel(kernel, freqImage.magnitude.size(), freqKernel);
	ip::fourierLogPowerSpectrum(freqKernel, powerSpectrum, true);
	cv::imshow("Power spectrum (filter kernel)", powerSpectrum);

	// Filter image in frequency domain
	ip::fourier freqFiltered;
	ip::fourierMultiply(freqImage, freqKernel, freqFiltered);

	// Inverse transformation of filtered image
	cv::Mat restored;
	ip::fourierInverse(freqFiltered, restored);
	cv::imshow("Filtered (frequency domain)", restored);

#if SAVE_IMAGES == true
	// Save images to file
	cv::imwrite("D:/_gray.jpg", image);
	cv::imwrite("D:/_power_kernel.jpg", powerSpectrum);
	cv::imwrite("D:/_filtered_space.jpg", filtered);
	cv::imwrite("D:/_filtered_frequency.jpg", restored);
#endif

	// Wait for keypress and terminate
	cv::waitKey(0);
	return 0;
}

/*! Generate 2D linear kernel from separated k1D kernels in x- and y-direction.
* 
* \param kernelX [in] 1D kernel in x-direction
* \param kernelY [in] 1D kernel in y-direction
* \param kernel2D [out] 2D kernel aquivalent to kernelX and kernelY
* \param isNormalize [in] Normize sum of coefficients to 1 (if true)
*/
void kernelSeparatedTo2D(const cv::Mat& kernelX, const cv::Mat& kernelY, cv::Mat& kernel2D, bool isNormalize) {
	int sizeX = max(kernelX.cols, kernelX.rows);
	int sizeY = max(kernelY.cols, kernelY.rows);
	float sum = 0.0;

	// Generate 2D kernel from 1D kernels
	kernel2D = cv::Mat(cv::Size(sizeX, sizeY), CV_32F);
	for (int y = 0; y < sizeY; y++) {
		float* row = (float*)kernel2D.ptr<float>(y);
		float yVal = kernelY.at<float>(y);

		for (int x = 0; x < sizeX; x++) {
			float hXY = yVal * kernelX.at<float>(x);
			row[x] = hXY;
			sum += hXY;
		}
	}

	// Normalize sum of coefficients to 1
	if (isNormalize)
		kernel2D /= sum;
}
