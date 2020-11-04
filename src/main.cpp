/* INCLUDES FOR THIS PROJECT */
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>
#include <filesystem>
#include <iterator>
#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

#include "dataStructures.h"
#include "matching2D.hpp"

using namespace std;
namespace fs = std::__fs::filesystem;


/****************/
/* MAIN PROGRAM */
/****************/

int main(int argc, const char *argv[])
{
    double t = (double)cv::getTickCount();
    /* INIT VARIABLES AND DATA STRUCTURES */

    // data location
    string dataPath = "../refImages/";

    // detector and descriptor config
    string detectorType = "SIFT";            // SHITOMASI, HARRIS, FAST, BRISK, ORB, AKAZE, SIFT  
    string descriptorType = "SIFT";              // BRIEF, ORB, FREAK, AKAZE, SIFT, BRISK

    // matcher config
    string matcherType = "MAT_FLANN";               // MAT_BF, MAT_FLANN
    string matcherDescriptorType = "DES_HOG"; // DES_BINARY, DES_HOG
    string selectorType = "SEL_KNN";              // SEL_NN, SEL_KNN

    // result
    double maxMatches = -1.0;
    string closestImg;
    
    // misc
    bool bVis = false;  // visualize results
    
    /***************************************/
    /* MAIN LOOP OVER ALL REFERENCE IMAGES */
    /***************************************/

    for (const auto& fn : fs::directory_iterator(dataPath))
    {
        /**************************/
        /* LOAD IMAGE INTO BUFFER */
        /**************************/

        // assemble filenames for current index
        string refImgFullFilename = fn.path().string();
        std::string productName = refImgFullFilename.substr(0, refImgFullFilename.find(".jpg"));
        if (refImgFullFilename.substr(refImgFullFilename.length()-3, 3) != "jpg") {
            continue;
        }
        // load reference image from file and convert to grayscale
        cv::Mat refImg, refImgGray;
        refImg = cv::imread(refImgFullFilename);
        cv::cvtColor(refImg, refImgGray, cv::COLOR_BGR2GRAY);

        /**************************/
        /* DETECT IMAGE KEYPOINTS */
        /**************************/
 
        // extract 2D keypoints from the reference image
        vector<cv::KeyPoint> refKeypoints; // create empty feature list for the reference image
    
        if (detectorType.compare("SHITOMASI") == 0)
        {
            detKeypointsShiTomasi(refKeypoints, refImgGray, false);
        }
        else if (detectorType.compare("HARRIS") == 0)
        {
            detKeypointsHarris(refKeypoints, refImgGray, false);
        }
        else
        {
            detKeypointsModern(refKeypoints, refImgGray, detectorType, false);
        }

        bool bLimitKpts = true;
        if (bLimitKpts)
        {
            int maxKeypoints = 1000;

            if (detectorType.compare("SHITOMASI") == 0)
            { // there is no response info, so keep the first 50 as they are sorted in descending quality order
                refKeypoints.erase(refKeypoints.begin() + maxKeypoints, refKeypoints.end());
            }
            cv::KeyPointsFilter::retainBest(refKeypoints, maxKeypoints);
            cout << " NOTE: Keypoints have been limited!" << endl;
        }
    

        // export keypoints to txt
        string txtName = productName + ".txt";
        ofstream outFile(txtName);
        for (const auto &kpt : refKeypoints) 
        {
            outFile << kpt.pt.x << "\t" << kpt.pt.y << "\t" << kpt.size << "\t" << 
            kpt.response << "\t" << kpt.octave << "\t" << kpt.angle << "\t" << kpt.class_id << "\n";
        }        

        /********************************/
        /* EXTRACT KEYPOINT DESCRIPTORS */
        /********************************/

        // extract keypoint descriptors of the reference image
        cv::Mat refDescriptors;
        descKeypoints(refKeypoints, refImgGray, refDescriptors, descriptorType);

        // export descriptors to xml
        string xmlName = productName + ".xml";
        cv::FileStorage file(xmlName, cv::FileStorage::WRITE);
        file << "descriptor" << refDescriptors;

    } // eof loop over all images
    
    // Results
    t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
    cout << "Extraction elapsed time in " << t << " s" << endl;

    return 0;
}

