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
#define IMAGE_DATA_PATH getenv("ImagingData")		// Read environment variable ImagingData
#define VIDEO_PATH "/Videos/Ball.mp4"				// Video file including relative path
#define TEMPLATE_POINT cv::Point(165, 355)
#define TEMPLATE_SIZE cv::Size(35, 35)
#define WAIT_TIME_MS 20

/* Namespaces */
using namespace std;

/* Prototypes */
void nextVideoFrame(cv::VideoCapture& video, cv::Mat& frame, string videoFilePath);
void templateMatching(const cv::Mat& imageRGB, const cv::Mat& objectImage, cv::Point& bestMatch);
void imageTranslation(const cv::Mat& image, cv::Mat& translated, cv::Point& fromPoint, cv::Point& toPoint);

/* Main function */
int main()
{
	cv::Mat frame, objectImage;
	cv::Point originalLocation = TEMPLATE_POINT;

	// Load video from file
	string inputVideoPath = string(IMAGE_DATA_PATH).append(VIDEO_PATH);
	cv::VideoCapture video(inputVideoPath);

	if (!video.isOpened()) {
		cout << "[ERROR] Cannot open camera" << endl;
		return 0;
	}

	// Create object image ("template")
	video >> frame;
	cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
	objectImage = frame(cv::Rect2i(TEMPLATE_POINT, TEMPLATE_SIZE)).clone();
	cv::imshow("Template", objectImage);

	// Loop through frames (repeating video)
	while (true) {
		// Get next frame (repeat on end of file)
		nextVideoFrame(video, frame, inputVideoPath);

		// Apply template matching
		cv::Point bestMatch;
		templateMatching(frame, objectImage, bestMatch);

		// Camera stabilization (image translation)
		cv::Mat stableImage;
		imageTranslation(frame, stableImage, bestMatch, originalLocation);
		cv::imshow("Stabilization (press any key to quit)", stableImage);

		// Track object
		cv::Mat trackImage = frame.clone();
		cv::rectangle(trackImage, cv::Rect2i(bestMatch, cv::Size(objectImage.cols, objectImage.rows)), cv::Scalar(0, 0, 255));
		cv::imshow("Tracking (press any key to quit)", trackImage);

		// Wait (exit loop on keypress)
		if (cv::waitKey(WAIT_TIME_MS) >= 0)
			break;
	}

	return 0;
}

/*! Get next frame from vido file.
* 
* Starts at beginning when end of file has been reached.
* 
* \param video [in] Video file (must be opened)
* \param frame [out] Frame read from video
* \param videoFilePath [in] Path to video file (to reopen when end of file reached)
*/
void nextVideoFrame(cv::VideoCapture& video, cv::Mat& frame, string videoFilePath) {
	// Get current frame from camera
	video >> frame;

	// Repeat when no frame left
	if (frame.empty()) {
		video = cv::VideoCapture(videoFilePath);
		video >> frame;
	}
}

/*! Find best match in template matching.
* 
* Matching uses squared differences.
* 
* \param imageRGB [in] Input image to find object in
* \param objectImage [in] Object to find in image
* \param bestMatch [out] Location (upper left corner) of best match in image
*/
void templateMatching(const cv::Mat& imageRGB, const cv::Mat& objectImage, cv::Point& bestMatch) {
	cv::Mat image8U, ssdImage;
	cv::cvtColor(imageRGB, image8U, cv::COLOR_BGR2GRAY);
	cv::matchTemplate(image8U, objectImage, ssdImage, cv::TM_SQDIFF);
	cv::minMaxLoc(ssdImage, NULL, NULL, &bestMatch);
}

/*! Shift image by difference between two points.
* 
* Translation vector = toPoint - fromPoint
* 
* \param image [in] Image to shift
* \param translated [out] Shifted image
* \param fromPoint [in] First point defining the translation vector
* \param toPoint [in] Second point defining the translation vector
*/
void imageTranslation(const cv::Mat& image, cv::Mat& translated, cv::Point& fromPoint, cv::Point& toPoint) {
	float dx = (float)(toPoint.x - fromPoint.x);
	float dy = (float)(toPoint.y - fromPoint.y);
	float transMatrix[] = { 1.0, 0.0, dx, 0.0, 1.0, dy };
	cv::Mat translation = cv::Mat(2, 3, CV_32F, transMatrix);

	translated = image.clone();
	warpAffine(image, translated, translation, image.size());
}
