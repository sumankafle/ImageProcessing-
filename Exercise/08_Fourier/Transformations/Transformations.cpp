/*! Digital image processing using OpenCV.
*
* \category Excercise Code
* \author Suman Kafle
*/


/* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()
#define _USE_MATH_DEFINES			// M_PI etc.

/* Include files */
#include <iostream>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Fourier.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")		// Read environment variable ImagingData
#define FILE_PATH "/images/Docks.jpg"				// Image file including relative path
#define DISPLAY_MAGNITUDE_AND_PHASE true
#define REORDER_POWER_SPECTRUM true
#define SAVE_IMAGES false

/* Namespaces */
using namespace std;

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

	// Fourier transform
	ip::fourier freqDomain;
	cv::Mat powerSpectrum, magnitude, phase;
	ip::fourierTransform(image, freqDomain);

	// Power spectrum
	ip::fourierLogPowerSpectrum(freqDomain, powerSpectrum, REORDER_POWER_SPECTRUM);

	// Magnitude
	double maxVal;
	cv::minMaxIdx(freqDomain.magnitude, NULL, &maxVal);
	freqDomain.magnitude.convertTo(magnitude, CV_8U, 255.0 / maxVal);
	ip::fourierReorderPowerSpectrum(magnitude);

	// Phase
	freqDomain.phase.convertTo(phase, CV_8U, 255.0 / (2.0 * M_PI));

	// Inverse Fourier transform
	cv::Mat restoredImage;
	ip::fourierInverse(freqDomain, restoredImage);

	// Display image(s)
	cv::imshow("Image", image);
#if DISPLAY_MAGNITUDE_AND_PHASE == true
	cv::imshow("Magnitude", magnitude);
	cv::imshow("Phase", phase);
#endif
	cv::imshow("Power spectrum", powerSpectrum);
	cv::imshow("Inverse transform", restoredImage);

#if SAVE_IMAGES == true
	// Save images to file
	cv::imwrite("D:/_gray.jpg", image);
	cv::imwrite("D:/_magnitude.jpg", magnitude);
	cv::imwrite("D:/_phase.jpg", phase);
	cv::imwrite("D:/_power.jpg", powerSpectrum);
	cv::imwrite("D:/_restored.jpg", restoredImage);
#endif

	// Wait for keypress and terminate
	cv::waitKey(0);
	return 0;
}
