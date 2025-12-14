/*! Digital image processing using OpenCV.
*
* \category Lab 3 Code 
* \author Suman Kafle
*/

/* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "DiceDetection.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")			// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/Images/Dice/45.jpg"	// Image file including relative path

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

	// Detect dice regions
	cv::Mat labeledImage;
	vector<ip::blob> dices;
	ip::locateDices(image, labeledImage, dices);

	// Create RGB images for display
	cv::Mat imageRGB, dicesRGB;
	cv::cvtColor(image, imageRGB, cv::COLOR_GRAY2BGR);
	cv::imshow("Image", imageRGB);

	ip::labels2RGB(labeledImage, dicesRGB);
	ip::annotateBlobs(dicesRGB, dices);
	cv::imshow("Detected dices", dicesRGB);

	// Analyse dice regions for pips
	for (int i = 0; i < dices.size(); i++) {
		// Detect pip regions
		cv::Mat labeledPips;
		vector<ip::blob> pips;
		ip::locateDicePips(image(dices.at(i).boundingBox), labeledPips, pips);

		// Create RGB image for display
		cv::Mat rgbROI;
		ip::labels2RGB(labeledPips, rgbROI);
		ip::annotateBlobs(rgbROI, pips);
		cv::imshow(string("Dice ").append(to_string(i + 1)), rgbROI);

		// Annotate detected number of pips in original image
		cv::Rect2i box = dices.at(i).boundingBox;
		cv::putText(imageRGB, to_string(pips.size()), cv::Point(box.x, box.y), cv::FONT_HERSHEY_PLAIN, 4.0, cv::Scalar(0, 0, 255), 3);
		cv::imshow("Image", imageRGB);
	}

	// Wait for keypress and terminate
	cv::waitKey(0);
	return 0;
}

