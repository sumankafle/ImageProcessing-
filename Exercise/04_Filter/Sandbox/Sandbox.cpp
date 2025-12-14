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

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")			// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/Images/Docks.jpg"	// Image file including relative path

/* Namespaces */
using namespace std;

/* Prototypes */

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

	// Display image
	cv::imshow("Image", image);

	// Wait for keypress and terminate
	cv::waitKey(0);
	return 0;
}
