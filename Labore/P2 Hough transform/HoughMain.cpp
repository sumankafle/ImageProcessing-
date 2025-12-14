/*! Digital image processing using OpenCV.
*
* \category Lab 2 Code 
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
#define INPUT_IMAGE_RELATIVE_PATH "/Images/Cologne.jpg"	// Image file including relative path
#define WINDOW_NAME_IMAGE "Image"
#define WINDOW_NAME_EDGE_IMAGE "Edges"
#define WINDOW_NAME_HOUGH "Hough transform"
#define TRACKBAR_NAME_THRESHOLD "Threshold"
#define EDGE_IMAGE_THRESHOLD 25

/* Namespaces */
using namespace std;
using namespace ip;

/* Prototypes */
void displayImages();
void createHoughImage();
void onTrackbarThreshold(int thresh, void* notUsed);
void onMouseHoughSpace(int event, int x, int y, int flags, void* notUsed);

/* Global variables */
cv::Mat image, imageClone, sobelImage, edgeImage, houghSpace;

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
	imageClone = image.clone();		// Keep copy for reset (remove drawn lines)

	// Calculate edge image
	sobelFilter(image, sobelImage);
	cv::threshold(sobelImage, edgeImage, EDGE_IMAGE_THRESHOLD, 255, cv::THRESH_BINARY);

	// Calculate Hough transform and prepare Hough image for display
	createHoughImage();

	// Display images in named windows
	displayImages();

	// Add window slider ("trackbar") for edge image threshold
	cv::createTrackbar(TRACKBAR_NAME_THRESHOLD, WINDOW_NAME_EDGE_IMAGE, NULL, 255, onTrackbarThreshold);
	cv::setTrackbarPos(TRACKBAR_NAME_THRESHOLD, WINDOW_NAME_EDGE_IMAGE, EDGE_IMAGE_THRESHOLD);

	// Add window mouse callback
	cv::setMouseCallback(WINDOW_NAME_HOUGH, onMouseHoughSpace);

	// Wait for keypress and terminate
	cv::waitKey(0);
	return 0;
}

/*! Display all images in named windows.
* 
* Will update displays when called more than once.
*/
void displayImages() {
	cv::imshow(WINDOW_NAME_EDGE_IMAGE, edgeImage);
	cv::imshow(WINDOW_NAME_IMAGE, image);
	cv::imshow(WINDOW_NAME_HOUGH, houghSpace);
}

/*! Apply Hough transform and prepare Hough image for display.
* 
* Maximizes contrast, inverts the Hough image, and draws coordinate system lines and label.
*/
void createHoughImage() {
	// Calculate Hough space image
	houghTransform(edgeImage, houghSpace);

	// Convert to maximized 8-bit grayscale
	double maxValue;
	cv::minMaxLoc(houghSpace, NULL, &maxValue);
	houghSpace.convertTo(houghSpace, CV_8U, 255.0 / maxValue);

	// Invert and add axes
	houghSpace = 255 - houghSpace;
	drawHoughLineLabels(houghSpace);
}

/*! Trackball callback for edge image threshold.
*
* \param alphaPercent Parameter alpha in percent (slider value)
* \param imagePtr Source image to process
*/
void onTrackbarThreshold(int thresh, void* notUsed) {
	cv::threshold(sobelImage, edgeImage, thresh, 255, cv::THRESH_BINARY);
	createHoughImage();
	imageClone.copyTo(image);	// Remove lines drawn

	// Update display
	displayImages();
}

/*! Mouse callback to select line parameters in Hough space image.
*
* \param event Mouse event
* \param x Mouse position x coordinate
* \param y Mouse position y coordinate
* \param flags Mouse or key press flags
* \param notUsed Not used
*/
void onMouseHoughSpace(int event, int x, int y, int flags, void* notUsed) {
	if (event == cv::EVENT_LBUTTONDOWN) {
		// Click position to line parameters
		double r, theta;
		houghSpaceToLine(
			cv::Size(image.cols, image.rows),
			cv::Size(houghSpace.cols, houghSpace.rows),
			x, y, r, theta);

		// Mark click position and draw line in image
		cv::circle(houghSpace, cv::Point(x, y), 10, cv::Scalar(0, 0, 255), 2);
		drawLine(image, r, theta);

		// Update display
		displayImages();
	}
}
