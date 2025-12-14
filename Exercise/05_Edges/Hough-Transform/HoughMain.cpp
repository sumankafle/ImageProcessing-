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
#include "Sobel.h"
#include "HoughLine.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")			// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/Images/Docks.jpg"	// Image file including relative path
#define EDGE_IMAGE_THRESHOLD 25
#define SMOOTHING_KERNEL_SIZE 1
#define IS_WRITE_IMAGES false

/* Namespaces */
using namespace std;
using namespace ip;

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

	// Calculate edge image
	cv::Mat edgeImage;
	sobelFilter(image, edgeImage);
	cv::threshold(edgeImage, edgeImage, EDGE_IMAGE_THRESHOLD, 255, cv::THRESH_BINARY);

	// Calculate Hough transform
	cv::Mat houghSpace;
	houghTransform(edgeImage, houghSpace);

	// Find global maximum in Hough space ...
	cv::Point houghMaxLocation;
	cv::GaussianBlur(houghSpace, houghSpace, cv::Size(SMOOTHING_KERNEL_SIZE, SMOOTHING_KERNEL_SIZE), 0.0);
	cv::minMaxLoc(houghSpace, NULL, NULL, NULL, &houghMaxLocation);

	// ... and draw corresponding line in original image
	double r, theta;
	houghSpaceToLine(
		cv::Size(edgeImage.cols, edgeImage.rows),
		cv::Size(houghSpace.cols, houghSpace.rows),
		houghMaxLocation.x, houghMaxLocation.y, r, theta);
	drawLine(image, r, theta);

	// Prepare Hough space image for display
	houghSpace = 255 - houghSpace;											// Invert
	drawHoughLineLabels(houghSpace);										// Axes
	cv::circle(houghSpace, houghMaxLocation, 10, cv::Scalar(0, 0, 255), 2);	// Global maximum

	// Display image in named window
	cv::imshow("Image", image);
	cv::imshow("Edge image", edgeImage);
	cv::imshow("Hough transform", houghSpace);

	// Write images to file
	if (IS_WRITE_IMAGES) {
		imwrite("D:/_HoughLine.jpg", image);
		imwrite("D:/_HoughEdge.jpg", edgeImage);
		imwrite("D:/_HoughSpace.jpg", houghSpace);
	}

	// Wait for keypress and terminate
	cv::waitKey(0);
	return 0;
}
