/*! Digital image processing using OpenCV.
*
* \category Excercise Code
* \author Suman Kafle
*/


/* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <cstdint>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")		// Read environment variable ImagingData
#define VIDEO_PATH "/Videos/Ball.mp4"				// Image file including relative path

/* Namespaces */
using namespace std;

/* Main function */
int main()
{
	// Load image from file
	string videoPath = string(IMAGE_DATA_PATH).append(VIDEO_PATH);
	cv::VideoCapture video(videoPath);

	if (!video.isOpened()) {
		cout << "[ERROR] Cannot open camera" << endl;
		return 0;
	}

	while (true) {
		cv::Mat frame, frame8U;

		// Get frame (RGB and gray)
		video >> frame;

		if (frame.empty())
			break;
		cv::cvtColor(frame, frame8U, cv::COLOR_BGR2GRAY);

		// Display current frame
		cv::imshow("Frame", frame);

		// Wait (exit on pressed key)
		if (cv::waitKey(25) >= 0)
			break;
	}

	return 0;
}
