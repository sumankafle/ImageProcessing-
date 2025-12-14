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
#define WRITE_IMAGE_FILES 0

/* Namespaces */
using namespace std;

/* Prototypes */
void binomialFilter3x3(const cv::Mat& image, cv::Mat& filtered);

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

	// Apply box filter
	cv::Mat filtered;
	binomialFilter3x3(image, filtered);

	// Display images in named windows
	cv::imshow("Image", image);
	cv::imshow("Binomial filter", filtered);

#if WRITE_IMAGE_FILES == 1
	// Write images to file
	cv::imwrite("D:/Gray.jpg", image);
	cv::imwrite("D:/Binomial3x3.jpg", filtered);
#endif

	// Wait for keypress and terminate
	cv::waitKey(0);
	return 0;
}

/*! Apply 3x3 binomial filter.
*
* No border handling is applied.
*
* \param image Source image to be filtered
* \param filtered Destination image
*/
void binomialFilter3x3(const cv::Mat& image, cv::Mat& filtered)
{
	// Assert correct parameters
	if (image.type() != CV_8U) {
		cout << "[WARNING] Incorrect image type. CV_8U expected." << endl;
		return;
	}

	// 1D filter in x-direction (except for border)
	cv::Mat filteredX = image.clone();
	for (int y = 1; y < image.rows - 1; y++) {
		const uchar* srcRow = image.ptr<uchar>(y);
		uchar* dstRow = filteredX.ptr<uchar>(y);

		for (int x = 1; x < image.cols - 1; x++) {
			int sum = (int)srcRow[x - 1] + ((int)srcRow[x] << 1) + (int)srcRow[x + 1];
			dstRow[x] = (uchar)(sum >> 2);
		}
	}

	// 1D filter in y-direction (except for border)
	filtered = filteredX.clone();
	for (int y = 1; y < image.rows - 1; y++) {
		const uchar* srcRow1 = filteredX.ptr<uchar>(y - 1);
		const uchar* srcRow2 = filteredX.ptr<uchar>(y);
		const uchar* srcRow3 = filteredX.ptr<uchar>(y + 1);
		uchar* dstRow = filtered.ptr<uchar>(y);

		for (int x = 1; x < image.cols - 1; x++) {
			int sum = (int)srcRow1[x] + ((int)srcRow2[x] << 1) + (int)srcRow3[x];
			dstRow[x] = (uchar)(sum >> 2);
		}
	}
}
