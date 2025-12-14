/*! Digital image processing using OpenCV.
*
* \category Excercise Code
* \author Suman Kafle
*/


/* Include files */
#include "Histogram.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/* Namespaces */
using namespace std;

namespace ip
{
	/*! Determine the maximum value inside an array.
	*
	* \param values Array to search for maximum value
	* \param size Number of elements in the array
	* \return the largest value inside the array
	*/
	unsigned max(unsigned values[], int size)
	{
		unsigned max = values[0];
		for (int i = 1; i < size; i++) {
			if (values[i] > max)
				max = values[i];
		}
		return max;
	}

	/*! Calculate the histogram data for 8-bit grayscale images.
	*
	* \param image Image to calculate histogram for (8-bit grayscale)
	* \param histogram Array to store histogram data in
	*/
	void calcHistogram(const cv::Mat& image, unsigned histogram[256], unsigned cumulative[256]) {
		const int NUMBER_BINS = 256;

		// Check for correct image type (8-bit grayscale)
		if (image.type() != CV_8U)
			return;

		// Init array
		for (int i = 0; i < NUMBER_BINS; i++)
			histogram[i] = 0;

		// Count pixel values
		for (int y = 0; y < image.rows; y++) {
			const uchar* data = image.ptr<uchar>(y);
			for (int x = 0; x < image.cols; x++) {
				histogram[data[x]]++;
			}
		}

		// Cumulative histogram
		if (cumulative != NULL) {
			cumulative[0] = histogram[0];
			for (int i = 1; i < NUMBER_BINS; i++)
				cumulative[i] = cumulative[i - 1] + histogram[i];
		}
	}

	/*! Create image depicting the histogram and cumulative histogram of a 8-bit grayscale image.
	*
	* \param image Image to create histogram for
	* \param histogram Resulting histogram chart
	* \param isCumulative Draw cumulative histogram on chart, if true
	*/
	void createHistogramImage(const cv::Mat& image, cv::Mat& histogram, bool isCumulative)
	{
		const int NUMBER_BINS = 256;
		const int WEDGE_HEIGHT = 10;

		// Check for correct image type (8-bit grayscale)
		if (image.type() != CV_8U)
			return;

		// Init array and count pixels
		unsigned hist[NUMBER_BINS] = { 0 };
		unsigned cumHist[NUMBER_BINS] = { 0 };
		calcHistogram(image, hist);

		cumHist[0] = hist[0];
		for (int i = 1; i < NUMBER_BINS; i++)
			cumHist[i] = cumHist[i - 1] + hist[i];

		// Find maximum histogram value
		unsigned maxCount = max(hist, NUMBER_BINS);

		// Create image
		int height = NUMBER_BINS, maxY = NUMBER_BINS - (WEDGE_HEIGHT + 1);
		double scale = 0.95 * height / (double)maxCount;
		double scaleCum = 0.95 * height / (double)(image.rows * image.cols);

		histogram.create(NUMBER_BINS, NUMBER_BINS, CV_8UC3);
		histogram = cv::Scalar(240, 240, 240);

		// Draw histogram bars
		for (int x = 0; x < NUMBER_BINS; x++) {
			cv::line(histogram,
				cv::Point(x, maxY),
				cv::Point(x, maxY - (int)(scale * hist[x])),
				cv::Scalar(200, 175, 175));
			cv::line(histogram,
				cv::Point(x, maxY + 1),
				cv::Point(x, maxY + WEDGE_HEIGHT),
				cv::Scalar(x, x, x));
		}

		// Plot cumulative histogram
		if (isCumulative) {
			for (int x = 1; x < NUMBER_BINS; x++) {
				cv::line(histogram,
					cv::Point(x - 1, maxY - (int)(scaleCum * cumHist[x - 1])),
					cv::Point(x, maxY - (int)(scaleCum * cumHist[x])),
					cv::Scalar(0, 0, 255));
			}
		}
	}

	void addLineToHistogramImage(cv::Mat& histogram, uchar level, cv::Scalar color) {
		cv::line(histogram, cv::Point(level, 0), cv::Point(level, histogram.rows - 1), color);
	}
}
