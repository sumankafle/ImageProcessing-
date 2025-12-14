/*****************************************************************************************************
 * Sample application for camera class AlliedAlvium.
 *****************************************************************************************************
 * Installation, environment variables, and project settings:
 * - OpenCV: see documentation of Camera.h
 * - Allied Vision API: see documentation of AlliedAlvium.h
 *****************************************************************************************************/

 /* Compiler settings and include files */
#include "AlliedAlvium.h"

/* Main function */
int main()
{
	// Connect to camera and show stream
	AlliedAlvium camera = AlliedAlvium();
	camera.showStream();

	// Free resources and terminate
	camera.release();
	return 0;
}

