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

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")			// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/Images/Docks.jpg"	// Image file including relative path

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

	// Create ROIs
	cv::Rect upperLeft = cv::Rect(0, 0, image.cols, image.rows);
	cv::Rect above = cv::Rect(image.cols, 0, image.cols, image.rows);
	cv::Rect upperRight = cv::Rect(2 * image.cols, 0, image.cols, image.rows);
	cv::Rect left = cv::Rect(0, image.rows, image.cols, image.rows);
	cv::Rect center = cv::Rect(image.cols, image.rows, image.cols, image.rows);
	cv::Rect right = cv::Rect(2 * image.cols, image.rows, image.cols, image.rows);
	cv::Rect lowerLeft = cv::Rect(0, 2 * image.rows, image.cols, image.rows);
	cv::Rect below = cv::Rect(image.cols, 2 * image.rows, image.cols, image.rows);
	cv::Rect lowerRight = cv::Rect(2 * image.cols, 2 * image.rows, image.cols, image.rows);

	// Create zero padding
	cv::Mat zeroPadded = cv::Mat::zeros(cv::Size(3 * image.cols, 3 * image.rows), CV_8U);
	image.copyTo(cv::Mat(zeroPadded, center));

	// Create cyclic wrap
	cv::Mat cyclicWrap = zeroPadded.clone();
	image.copyTo(cv::Mat(cyclicWrap, upperLeft));
	image.copyTo(cv::Mat(cyclicWrap, above));
	image.copyTo(cv::Mat(cyclicWrap, upperRight));
	image.copyTo(cv::Mat(cyclicWrap, left));
	image.copyTo(cv::Mat(cyclicWrap, right));
	image.copyTo(cv::Mat(cyclicWrap, lowerLeft));
	image.copyTo(cv::Mat(cyclicWrap, below));
	image.copyTo(cv::Mat(cyclicWrap, lowerRight));

	// Create Mirror
	cv::Mat mirror = zeroPadded.clone();
	cv::Mat flipX, flipY, flipXY;
	cv::flip(image, flipX, 1);
	cv::flip(image, flipY, 0);
	cv::flip(image, flipXY, -1);
	flipXY.copyTo(cv::Mat(mirror, upperLeft));
	flipY.copyTo(cv::Mat(mirror, above));
	flipXY.copyTo(cv::Mat(mirror, upperRight));
	flipX.copyTo(cv::Mat(mirror, left));
	flipX.copyTo(cv::Mat(mirror, right));
	flipXY.copyTo(cv::Mat(mirror, lowerLeft));
	flipY.copyTo(cv::Mat(mirror, below));
	flipXY.copyTo(cv::Mat(mirror, lowerRight));

	// Create clamp
	cv::Mat clamp = zeroPadded.clone();
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++)
			clamp.at<uchar>(y, x) = image.at<uchar>(0, 0);
		for (int x = image.cols; x < 2 * image.cols; x++)
			clamp.at<uchar>(y, x) = image.at<uchar>(0, x);
		for (int x = 2 * image.cols; x < 3 * image.cols; x++)
			clamp.at<uchar>(y, x) = image.at<uchar>(0, image.cols - 1);
	}
	for (int y = image.rows; y < 2 * image.rows; y++) {
		for (int x = 0; x < image.cols; x++)
			clamp.at<uchar>(y, x) = image.at<uchar>(y - image.rows, 0);
		for (int x = 2 * image.cols; x < 3 * image.cols; x++)
			clamp.at<uchar>(y, x) = image.at<uchar>(y - image.rows, image.cols - 1);
	}
	for (int y = 2 * image.rows; y < 3 * image.rows; y++) {
		for (int x = 0; x < image.cols; x++)
			clamp.at<uchar>(y, x) = image.at<uchar>(image.rows - 1, 0);
		for (int x = image.cols; x < 2 * image.cols; x++)
			clamp.at<uchar>(y, x) = image.at<uchar>(image.rows - 1, x - image.cols);
		for (int x = 2 * image.cols; x < 3 * image.cols; x++)
			clamp.at<uchar>(y, x) = image.at<uchar>(image.rows - 1, image.cols - 1);
	}

	// Display image in named window
	cv::imshow("Image", image);

	// Write images to files
	cv::imwrite("D:/ZeroPadding.jpg", zeroPadded);
	cv::imwrite("D:/CyclicWrap.jpg", cyclicWrap);
	cv::imwrite("D:/Mirror.jpg", mirror);
	cv::imwrite("D:/Clamp.jpg", clamp);

	// Wait for keypress and terminate
	cv::waitKey(0);
	return 0;
}
