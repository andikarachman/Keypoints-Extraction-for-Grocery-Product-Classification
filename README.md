# Keypoints Extraction for Grocery Product Classification

This software is used to extract keypoints from products that will be used for performing product classification. Several keypoint detectors (i.e. SHITOMASI, HARRIS, FAST, BRISK, ORB, AKAZE, SIFT) and descriptors (i.e. BRIEF, ORB, FREAK, AKAZE, SIFT, BRISK) can be used for extracting and describing keypoints. The keypoints extracted and described using this software is the requirements for performing classification of grocery products in [this repo](https://github.com/andikarachman/Non-Deep-Learning-Grocery-Product-Classification).  

## Dependencies for Running Locally
* cmake >= 2.8
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* OpenCV >= 4.1
  * This must be compiled from source using the `-D OPENCV_ENABLE_NONFREE=ON` cmake flag for testing the SIFT and SURF detectors.
  * The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build and Use Instructions

1. Clone this repo.
2. Put the directory of the build file of your OpenCV in the `CMakeLists.txt`.
3. Put all of product images, which keypoints to be extracted, in the `refImages` folder. The images should be named based on the product name. Some examples are provided for references. 
3. Make a build directory in the top level directory: `mkdir build && cd build`
4. Compile: `cmake .. && make`
5. Run it: `./keypoints_extraction`.
6. The results are located in the `refImages` folder.
  - Files with `.txt` are the keypoints.
  - Files with `.xml` are the keypoint descriptors.