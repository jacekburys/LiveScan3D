//   Copyright (C) 2015  Marek Kowalski (M.Kowalski@ire.pw.edu.pl), Jacek Naruniec (J.Naruniec@ire.pw.edu.pl)
//   License: MIT Software License   See LICENSE.txt for the full license.

//   If you use this software in your research, then please use the following citation:

//    Kowalski, M.; Naruniec, J.; Daniluk, M.: "LiveScan3D: A Fast and Inexpensive 3D Data
//    Acquisition System for Multiple Kinect v2 Sensors". in 3D Vision (3DV), 2015 International Conference on, Lyon, France, 2015

//    @INPROCEEDINGS{Kowalski15,
//        author={Kowalski, M. and Naruniec, J. and Daniluk, M.},
//        booktitle={3D Vision (3DV), 2015 International Conference on},
//        title={LiveScan3D: A Fast and Inexpensive 3D Data Acquisition System for Multiple Kinect v2 Sensors},
//        year={2015},
//    }

#include "arucoCalibration.h"
#include "opencv2/aruco/charuco.hpp"

ArucoCalibration::ArucoCalibration()
{
	bCalibrated = false;
	nSampleCounter = 0;
	nRequiredSamples = 20;

	pDetector = new MarkerDetector();
}

ArucoCalibration::~ArucoCalibration()
{
	if (pDetector != NULL)
	{
		delete pDetector;
		pDetector = NULL;
	}
}

bool ArucoCalibration::Calibrate(RGB *pBuffer, Point3f *pCameraCoordinates, int cColorWidth, int cColorHeight)
{
	// Convert RGB array [pBuffer] into opencv image
	// TODO: Maybe implement this earlier (before this function is called), 
	//       or by changing how the image is read from the camrea?
	cv::Mat img(cColorWidth, cColorHeight, CV_8UC3, cv::Vec3b(0,0,0));
	for (int y = 0; y < cColorHeight; y++)
	{
		for (int x = 0; x < cColorWidth; x++)
		{
			// get pixel at (x, y) 
			RGB pixel = pBuffer[cColorWidth * y + x];
			// set the img map at x, y to the rgb specified in pbuffer
			img.at<cv::Vec3b>(cv::Point(x, y)) = cv::Vec3b(pixel.rgbRed, pixel.rgbGreen, pixel.rgbBlue);
		}
	}

	// DICT_ARUCO_ORIGINAL uses 5x5 markers afaik
	cv::aruco::Dictionary* dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_ARUCO_ORIGINAL);

	// output structures
	vector<vector<Point2f>> corners;
	vector<int> ids;
	
	// find markers in img, store corners in corners and ids in ids
	cv::aruco::detectMarkers(img, dict, corners, ids);

	cv::aruco::drawDetectedMarkers(img, corners, ids);
	return true;
}