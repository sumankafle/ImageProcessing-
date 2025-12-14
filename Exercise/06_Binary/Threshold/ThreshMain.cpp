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
#include "Histogram.h"
#include "Thresholding.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")				// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/Images/LandingPad.jpg"	// Image file including relative path

#define WINDOW_NAME_THRESHOLD "Threshold"
#define WINDOW_NAME_LOCAL "Locally adaptive threshold"
#define TRACKBAR_NAME_THRESHOLD "Threshold"
#define TRACKBAR_NAME_RADIUS "Radius"
#define TRACKBAR_NAME_MIN_CONTRAST "Contrast"
#define INITIAL_THRESHOLD 127
#define LOCAL_THRESH_MIN_CONTRAST 30
#define LOCAL_THRESH_RADIUS 15

/* Namespaces */
using namespace std;

/* Prototypes */
void onTrackbarThreshold(int thresh, void* notUsed);
void onTrackbarLocalAdaptive(int valueNotUsed, void* pointerNotUsed);

/* Global variables */
cv::Mat histogramImage;
cv::Mat image, binImageThresh, binImageGlobal, binImageLocal;

/* Main function */
int main()
{
	// Load image from file
	string inputImagePath = string(IMAGE_DATA_PATH).append(INPUT_IMAGE_RELATIVE_PATH);
	image = cv::imread(inputImagePath, cv::IMREAD_GRAYSCALE);

	if (image.empty()) {
		cout << "[ERROR] Cannot open image: " << inputImagePath << endl;
		return 0;
	}

	// Calculate histogram
	ip::createHistogramImage(image, histogramImage);

	// Apply global, adaptive global, and locally adaptive threshold
	uchar adaptiveThresh = 0;

	ip::threshold(image, binImageThresh, INITIAL_THRESHOLD);
	ip::isodataThreshold(image, binImageGlobal, &adaptiveThresh);
	ip::bernsenThreshold(image, binImageLocal, LOCAL_THRESH_RADIUS, LOCAL_THRESH_MIN_CONTRAST);

	// Draw adaptive global threshold in histogram image
	ip::addLineToHistogramImage(histogramImage, adaptiveThresh);

	// Display images
	cv::imshow("Image", image);
	cv::imshow("Histogram", histogramImage);
	cv::imshow(WINDOW_NAME_THRESHOLD, binImageThresh);
	cv::imshow(string("Global adaptive (t = ").append(to_string(adaptiveThresh)).append(")"), binImageGlobal);
	cv::imshow(WINDOW_NAME_LOCAL, binImageLocal);

	// Add window sliders ("trackbars")
	cv::createTrackbar(TRACKBAR_NAME_THRESHOLD, WINDOW_NAME_THRESHOLD, NULL, 255, onTrackbarThreshold);
	cv::setTrackbarPos(TRACKBAR_NAME_THRESHOLD, WINDOW_NAME_THRESHOLD, INITIAL_THRESHOLD);

	cv::createTrackbar(TRACKBAR_NAME_RADIUS, WINDOW_NAME_LOCAL, NULL, 51, onTrackbarLocalAdaptive);
	cv::createTrackbar(TRACKBAR_NAME_MIN_CONTRAST, WINDOW_NAME_LOCAL, NULL, 127, onTrackbarLocalAdaptive);
	cv::setTrackbarPos(TRACKBAR_NAME_RADIUS, WINDOW_NAME_LOCAL, LOCAL_THRESH_RADIUS);
	cv::setTrackbarPos(TRACKBAR_NAME_MIN_CONTRAST, WINDOW_NAME_LOCAL, LOCAL_THRESH_MIN_CONTRAST);

	// Wait for keypress and terminate
	cv::waitKey(0);
	return 0;
}

/*! Trackball callback for fixed global threshold.
*
* Applies threshold and updates the displayed binary image.
*
* \param thresh Threshold to apply (slider value)
* \param notUsed Parameter not used
*/
void onTrackbarThreshold(int thresh, void* notUsed) {
	ip::threshold(image, binImageThresh, thresh);
	cv::imshow(WINDOW_NAME_THRESHOLD, binImageThresh);
}

/*! Trackball callback for locally adaptive threshold.
*
* Reads the current slider values, processes, and updates the displayed binary image.
*
* \param valueNotUsed Parameter not used (slider value)
* \param pointerNotUsed Parameter not used
*/
void onTrackbarLocalAdaptive(int valueNotUsed, void* pointerNotUsed) {
	int kernelSize = cv::getTrackbarPos(TRACKBAR_NAME_RADIUS, WINDOW_NAME_LOCAL);
	int minContrast = cv::getTrackbarPos(TRACKBAR_NAME_MIN_CONTRAST, WINDOW_NAME_LOCAL);

	ip::bernsenThreshold(image, binImageLocal, kernelSize, minContrast);
	cv::imshow(WINDOW_NAME_LOCAL, binImageLocal);
}
