/*! Digital image processing using OpenCV.
*
* \category Lab 3 Dice Detection with Camera in Real-Time
* \author Suman Kafle
*/

/* Include files */
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "DiceDetection.h"

/* Defines */
#define CAMERA_ID 0
#define WAIT_TIME_MS 30

/* Namespaces */
using namespace std;

/* Main function */
int main()
{
	// Open the first conencted camera found
	cv::VideoCapture camera(CAMERA_ID);

	if (!camera.isOpened()) {
		cout << "[ERROR] Cannot open camera" << endl;
		return 0;
	}

	// Loop through frames
	cv::Mat frame, image;

	while (true) {
		// Get current frame from camera
		camera >> frame;
		cv::cvtColor(frame, image, cv::COLOR_BGR2GRAY);

		// Detect dice regions
		vector<ip::blob> dices = ip::locateDices(image);

		// Analyse dice regions for pips
		for (int i = 0; i < dices.size(); i++) {
			// Detect pip regions
			vector<ip::blob> pips = ip::locateDicePips(image(dices.at(i).boundingBox));

			// Annotate detected number of pips in original image
			cv::Rect2i box = dices.at(i).boundingBox;
			cv::putText(frame, to_string(pips.size()), cv::Point(box.x, box.y), cv::FONT_HERSHEY_PLAIN, 4.0, cv::Scalar(0, 0, 255), 3);
		}

		// Annotate and display frame
		ip::annotateBlobs(frame, dices);
		cv::imshow("Camera (press any key to quit)", frame);

		// Wait (exit loop on keypress)
		if (cv::waitKey(WAIT_TIME_MS) >= 0)
			break;
	}

	return 0;
}

