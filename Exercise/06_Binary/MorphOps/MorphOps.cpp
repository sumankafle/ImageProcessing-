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
#define INPUT_IMAGE_RELATIVE_PATH "/Images/Resistors.jpg"	// Image file including relative path

#define TRACKBAR_NAME_THRESHOLD "Threshold"
#define TRACKBAR_NAME_MORPH_SIZE "Morph size"
#define IS_INVERT_BINARY_IMAGE true
#define INITIAL_MORPH_SIZE 3
#define IS_SAVE_IMAGE_FILES false

/* Namespaces */
using namespace std;

/* Prototypes */
void onTrackbars(int valueNotUsed, void* pointerNotUsed);
void processAndDisplay(uchar thresh, int morphSize);

/* Global variables */
string windowNameThreshold;
cv::Mat histogramImage;
cv::Mat image, binImageThresh, binEroded, binDilated, binClosed, binOpened, binClosedOpened, binOpenedClosed;

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

	// Initialize with adaptive global thresholding (Isodata algorithm)
	uchar thresh = 0;
	ip::isodataThreshold(image, binImageThresh, &thresh);

	thresh = 132;
	
	// Display images
	windowNameThreshold = string("Threshold (Isodata: t = ").append(to_string(thresh)).append(")");
	cv::imshow("Image", image);
	cv::imshow("Histogram", histogramImage);
	cv::imshow(windowNameThreshold, binImageThresh);

	// Add window sliders ("trackbars")
	int initialMorphSize = INITIAL_MORPH_SIZE;
	cv::createTrackbar(TRACKBAR_NAME_THRESHOLD, windowNameThreshold, NULL, 255, onTrackbars);
	cv::createTrackbar(TRACKBAR_NAME_MORPH_SIZE, windowNameThreshold, &initialMorphSize, 15, onTrackbars);
	cv::setTrackbarPos(TRACKBAR_NAME_THRESHOLD, windowNameThreshold, thresh);

	// Save images to file
#if IS_SAVE_IMAGE_FILES == true
	int morphSize = cv::getTrackbarPos(TRACKBAR_NAME_MORPH_SIZE, windowNameThreshold);
	thresh = cv::getTrackbarPos(TRACKBAR_NAME_THRESHOLD, windowNameThreshold);
	string suffix = string("_t").append(to_string(thresh)).append("_k").append(to_string(morphSize)).append(".jpg");

	cv::imwrite(string("D:/CloseOpen").append(suffix), binClosedOpened);
#endif

	// Wait for keypress and terminate
	cv::waitKey(0);
	return 0;
}

/*! Trackball callbacks.
*
* Gets values of trackbars, processes images, and updates image displays.
*
* \param valueNotUsed Parameter not used
* \param pointerNotUsed Parameter not used
*/
void onTrackbars(int valueNotUsed, void* pointerNotUsed) {
	int thresh = cv::getTrackbarPos(TRACKBAR_NAME_THRESHOLD, windowNameThreshold);
	int morphSize = cv::getTrackbarPos(TRACKBAR_NAME_MORPH_SIZE, windowNameThreshold);

	processAndDisplay(thresh, morphSize);
}

/*! Process images and update image displays.
* 
* \param thresh Threshold to apply to original gray-values image
* \param morphSize Size of rectangular structure elements for morphological operations
*/
void processAndDisplay(uchar thresh, int morphSize) {
	static int lastThresh = -1;
	static int lastMorphSize = -1;

	// Threshold gray-values input image and update image display
	if (thresh != lastThresh) {
		ip::threshold(image, binImageThresh, thresh, IS_INVERT_BINARY_IMAGE);
		cv::imshow(windowNameThreshold, binImageThresh);
	}

	// Apply morphological operations (dilate, erode, close, open) and update image displays
	if ((thresh != lastThresh) || (morphSize != lastMorphSize)) {
		// Apply operations
		if (morphSize >= 1) {
			cv::Mat structure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(morphSize, morphSize));

			cv::dilate(binImageThresh, binDilated, structure);
			cv::erode(binImageThresh, binEroded, structure);
			cv::morphologyEx(binImageThresh, binClosed, cv::MORPH_CLOSE, structure);
			cv::morphologyEx(binImageThresh, binOpened, cv::MORPH_OPEN, structure);
			cv::morphologyEx(binClosed, binClosedOpened, cv::MORPH_OPEN, structure);
			cv::morphologyEx(binOpened, binOpenedClosed, cv::MORPH_CLOSE, structure);
		}
		else {
			binDilated = binImageThresh.clone();
			binEroded = binImageThresh.clone();
			binClosed = binImageThresh.clone();
			binOpened = binImageThresh.clone();
			binClosedOpened = binImageThresh.clone();
			binOpenedClosed = binImageThresh.clone();
		}

		// Update image displays
		cv::imshow("Dilated", binDilated);
		cv::imshow("Eroded", binEroded);
		cv::imshow("Closed", binClosed);
		cv::imshow("Opened", binOpened);
		cv::imshow("Closed / opened", binClosedOpened);
		cv::imshow("Opened / closed", binOpenedClosed);
		cv::imshow("Binary - eroded", binImageThresh - binEroded);
	}

	// Remember last values to detect parameter changes
	lastThresh = thresh;
	lastMorphSize = morphSize;
}
