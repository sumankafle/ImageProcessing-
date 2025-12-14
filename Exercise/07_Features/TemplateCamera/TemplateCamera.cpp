/*! Digital image processing using OpenCV.
*
* \category Excercise Code
* \author Suman Kafle
*/

/* Include files */
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "TemplateMatching.h"

/* Defines */
#define WINDOW_NAME_CAMERA "Camera (press any key to quit)"
#define WINDOW_NAME_TEMPLATE "Template"
#define WINDOW_NAME_SSD "Sum of squared differences"
#define CAMERA_ID 0
#define WAIT_TIME_MS 1

/* Namespaces */
using namespace std;

/* Prototypes */
void onMouseDrag(int event, int x, int y, int flags, void* data);
void annotateFrame(cv::Rect2i* mouseDragBox, cv::Point bestMatch);

/* Global variables */
cv::Mat frame, frame8U, objectImage, ssdImage;
cv::Rect2i* mouseDragBox;
cv::Point bestMatch;

/* Main function */
int main()
{
	// Open the first conencted camera found
	cv::VideoCapture camera(CAMERA_ID);

	if (!camera.isOpened()) {
		cout << "[ERROR] Cannot open camera" << endl;
		return 0;
	}

	// Create frame window and add mouse callback
	cv::namedWindow(WINDOW_NAME_CAMERA);
	cv::setMouseCallback(WINDOW_NAME_CAMERA, onMouseDrag, NULL);

	// Loop through frames
	while (true) {
		// Get current frame from camera
		camera >> frame;
		cv::cvtColor(frame, frame8U, cv::COLOR_BGR2GRAY);

		// Apply template matching and display SSD
		if (!objectImage.empty()) {
			ip::templateMatching(frame8U, objectImage, ssdImage, bestMatch);
			cv::imshow(WINDOW_NAME_SSD, ssdImage);
		}

		// Annotate and display frame
		annotateFrame(mouseDragBox, bestMatch);
		cv::imshow(WINDOW_NAME_CAMERA, frame);

		// Wait (exit loop on keypress)
		if (cv::waitKey(WAIT_TIME_MS) >= 0)
			break;
	}

	return 0;
}

/*! Mouse callback to select template region.
*
* Dragging the mouse with the left button pressed will select the region.
*
* \param event Mouse event
* \param x Mouse position x coordinate
* \param y Mouse position y coordinate
* \param flags Mouse or key press flags
*/
void onMouseDrag(int event, int x, int y, int flags, void* data) {
	// Left button pressed (Start selecting region)
	if (event == cv::EVENT_LBUTTONDOWN) {
		objectImage.create(cv::Size(0, 0), CV_8U);
		mouseDragBox = new cv::Rect2i(x, y, 1, 1);
	}
	
	// Left button dragged (Define box size by lower right corner)
	else if ((event == cv::EVENT_MOUSEMOVE) && (flags == cv::EVENT_FLAG_LBUTTON)) {
		mouseDragBox->width = max(1, (x - mouseDragBox->x) + 1);
		mouseDragBox->height = max(1, (y - mouseDragBox->y) + 1);
	}
	
	// Left button up (Stop selecting region)
	else if (event == cv::EVENT_LBUTTONUP) {
		// Create and display template image
		objectImage = frame8U(*mouseDragBox).clone();
		cv::imshow(WINDOW_NAME_TEMPLATE, cv::Mat::zeros(cv::Size(1, 1), CV_8U));	// Clear
		cv::imshow(WINDOW_NAME_TEMPLATE, objectImage);

		// Free memory (thread-safe)
		cv::Rect2i* temp = mouseDragBox;
		mouseDragBox = NULL;
		delete temp;
	}
}

/*! Add annotations for display to frame.
* 
* \param mouseDragBox [in] Box being dragged with left mouse button (or NULL)
* \param bestMatch [in] Best match in template matching
*/
void annotateFrame(cv::Rect2i* mouseDragBox, cv::Point bestMatch) {
	cv::Scalar red(0, 0, 255);

	// Dragging mouse to select template
	if (mouseDragBox != NULL)
		cv::rectangle(frame, *mouseDragBox, red);

	// Best match in template matching
	else if (!ssdImage.empty()) {
		cv::rectangle(frame, cv::Rect2i(bestMatch, cv::Size(objectImage.cols, objectImage.rows)), red);
	}
}

