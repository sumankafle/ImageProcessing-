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
#include "BinaryRegions.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")			// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/Images/LandingPad.jpg"	// Image file including relative path
#define BINARY_THRESHOLD 205
#define MORPH_SIZE 5
#define IS_INVERT_BINARY false
#define IS_DRAW_STATISTICS true
#define IS_SAVE_FILES false

/* Namespaces */
using namespace std;

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

	// To binary image
	int threshMethod = (IS_INVERT_BINARY) ? cv::THRESH_BINARY_INV : cv::THRESH_BINARY;
	cv::Mat binary;
	cv::threshold(image, binary, BINARY_THRESHOLD, 255, threshMethod);

	// Remove small BLOBS
	cv::Mat structure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(MORPH_SIZE, MORPH_SIZE));
	cv::morphologyEx(binary, binary, cv::MORPH_OPEN, structure);

	// Region labeling
	cv::Mat labeled = binary.clone() / 255;		// => Values in [0, 1]
	cv::Mat labeledRGB;
	ip::labelRegions(labeled);
	ip::labels2RGB(labeled, labeledRGB);

	// Annotate BLOB statistics
#if IS_DRAW_STATISTICS == true
	ip::blob blobs[256];
	ip::labels2BlobFeatures(labeled, blobs);
	ip::annotateBlobs(labeledRGB, blobs);
#endif

	// Maximize contrast of gray-valued labeled image
	double min, max;
	cv::minMaxLoc(labeled, &min, &max);
	labeled *= 255.0 / max;

	// Display images
	cv::imshow("Image", image);
	cv::imshow("Binary", binary);
	cv::imshow("Labeled (max. contrast)", labeled);
	cv::imshow("Labeled (colored)", labeledRGB);

	// Save images to files
#if IS_SAVE_FILES == true
	string suffix = string("_t").append(to_string(BINARY_THRESHOLD)).append("_k").append(to_string(MORPH_SIZE)).append(".jpg");

	cv::imwrite(string("D:/_Binary").append(suffix), binary);
	cv::imwrite(string("D:/_GrayLabels").append(suffix), labeled);
#if IS_DRAW_STATISTICS == true
	cv::imwrite(string("D:/_Annotated").append(suffix), labeledRGB);
#else
	cv::imwrite(string("D:/_Labeled").append(suffix), labeledRGB);
#endif
#endif

	// Wait for keypress and terminate
	cv::waitKey(0);
	return 0;
}
