/*! Digital image processing using OpenCV.
*
* \category Lab 1 code
* \author Suman Kafle
*/

/* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")		// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/Images/PCB.jpg"	// Image file including relative path

#define WINDOW_NAME "Unsharp masking"
#define TRACKBAR_NAME_ALPHA "Alpha"
#define INITIAL_ALPHA_PERCENT 100

/* Namespaces */
using namespace std;

/* Prototypes */
void onTrackbarAlpha(int alphaPercent, void* imagePtr);
void onMouseSplitScreen(int event, int x, int y, int flags, void* imagePtr);
void processAndDisplay(cv::Mat image, int alphaPercent, int splitScreenX = -1);
void unsharpMasking(const cv::Mat& source, cv::Mat& processed, double alpha);

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

	// Apply processing and display image
	processAndDisplay(image, INITIAL_ALPHA_PERCENT, image.cols / 2);

	// Add window slider ("trackbar") for parameter alpha
	cv::createTrackbar(TRACKBAR_NAME_ALPHA, WINDOW_NAME, NULL, 100, onTrackbarAlpha, reinterpret_cast<void*>(&image));
	cv::setTrackbarPos(TRACKBAR_NAME_ALPHA, WINDOW_NAME, INITIAL_ALPHA_PERCENT);

	// Add window mouse callback
	cv::setMouseCallback(WINDOW_NAME, onMouseSplitScreen, reinterpret_cast<void*>(&image));

	// Wait for keypress and terminate
	cv::waitKey(0);
	return 0;
}

/*! Trackball callback for parameter alpha.
* 
* \param alphaPercent Parameter alpha in percent (slider value)
* \param imagePtr Source image to process
*/
void onTrackbarAlpha(int alphaPercent, void* imagePtr) {
	cv::Mat image = *(static_cast<cv::Mat*>(imagePtr));

	processAndDisplay(image, alphaPercent);
}

/*! Mouse callback to move split position.
* 
* The result is displayed in a slit image (one side processed, other side not processed).
* Dragging the mouse with the left button pressed will move the split position.
* 
* \param event Mouse event
* \param x Mouse position x coordinate
* \param y Mouse position y coordinate
* \param flags Mouse or key press flags
* \param imagePtr Source image to process
*/
void onMouseSplitScreen(int event, int x, int y, int flags, void* imagePtr) {
	bool isLeftButtonDragged = (event == cv::EVENT_MOUSEMOVE) && (flags == cv::EVENT_FLAG_LBUTTON);

	if (isLeftButtonDragged) {
		cv::Mat image = *(static_cast<cv::Mat*>(imagePtr));
		int alphaPercent = cv::getTrackbarPos(TRACKBAR_NAME_ALPHA, WINDOW_NAME);

		processAndDisplay(image, alphaPercent, x);
	}
}

/*! Apply unsharp masking and display result in split image.
* 
* The displayed image is displayed as a split image (left part: processed, right part: not processed).
* 
* \param image Source image to process
* \param alphaPercent Masking factor alpha in percent [0, 100]
* \param splitScreenX Image split x coordinate (will not change, when no argument passed)
*/
void processAndDisplay(cv::Mat image, int alphaPercent, int splitScreenX) {
	static int lastAlphaPercent = -1;
	static int splitX = 0;
	static cv::Mat sharpenedImage, splitImageRGB;

	// Adapt slip position
	if ((splitScreenX >= 0) && (splitScreenX < image.cols))
		splitX = splitScreenX;

	// Apply unsharp masking
	if (alphaPercent != lastAlphaPercent) {
		lastAlphaPercent = alphaPercent;
		unsharpMasking(image, sharpenedImage, alphaPercent / 100.0);
	}

	// Create gray-valued split image (left: unsharp masking, right: not processed)
	cv::Mat splitImage = image.clone();
	cv::Mat srcROI = sharpenedImage.colRange(0, splitX);
	cv::Mat dstROI = splitImage.colRange(0, splitX);
	srcROI.copyTo(dstROI);

	// Create and display RGB image with colored split position
	cv::cvtColor(splitImage, splitImageRGB, cv::COLOR_GRAY2BGR);
	cv::line(splitImageRGB, cv::Point(splitX, 0), cv::Point(splitX, splitImage.rows - 1), cv::Scalar(0, 0, 255), 2);
	cv::imshow(WINDOW_NAME, splitImageRGB);
}

/*! Apply unsharp masking for image sharpening.
* 
* A 9x9 binomial filter is applied as low-pass filter.
* 
* \param source Source image to be sharpened (8-bit grayscale)
* \param processed Resulting sharpened image
* \param alpha Masking factor alpha in [0.0, 1.0]
*/
void unsharpMasking(const cv::Mat& source, cv::Mat& processed, double alpha) {
	// Check parameter values
	if (source.type() != CV_8U) {
		cout << "[WARNING] Incorrect image type. Expected 8-bit grayscale." << endl;
		return;
	}
	if ((alpha < 0.0) || (alpha > 1.0)) {
		cout << "[WARNING] Parameter alpha not within [0.0, 1.0]." << endl;
		return;
	}

	// Create low-pass kernel (binomial)
	cv::Mat kernelLP = (cv::Mat_<double>(9, 1) << 1, 8, 28, 56, 70, 56, 28, 8, 1);
	kernelLP /= cv::sum(kernelLP);

	// Apply low-pass filter
	cv::Mat imageLP;
	cv::sepFilter2D(source, imageLP, source.depth(), kernelLP, kernelLP);

	// Apply unsharp masking
	processed = (1.0 + alpha) * source - alpha * imageLP;
}
