// OpenCV includes
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

// STD includes
#include <cassert>
#include <iostream>
#include <string>

// Qt Includes

// Pylon includes

// Custom includes
#include "LoadSaveExperiments/LoadSaveExperiments.hpp"
#include "PixelGainCalibration/SuperPixelCalibration.hpp"
#include "PolarimetricImagesProcessing/PolarimetricImagesProcessing.hpp"

template<typename T>
void printVector(std::string name, std::vector<T> v)
{
    std::cout << name << " = [ " << v[0];
    for (size_t i = 1; i < v.size(); i++)
    {
        std::cout << ", " << v[i];
    }
    std::cout << "]" << std::endl;
}

int main(int argc, char** argv)
{
    std::string rootDir = std::string(PROJECT_PATH);
    std::string dataPath = rootDir + "data";
    std::string expName = "experiment_1";
    if (argc > 2)
    {
        dataPath = std::string(argv[1]);
        expName = std::string(argv[2]);
    }

    std::cout << "Using calibration data in: " << dataPath << "/" << expName << std::endl;

    LoadSaveExperiments loader;
    loader.loadExperiment(dataPath, expName);

    // The angles obtained with the loader corresponds to the
    // ones retrieved from the images filenames.
    std::vector<double> angles = loader.getAngles();
    std::vector<cv::Mat> imgs = loader.getImages();

    SuperPixelCalibration calibrator;
    // filterOrder is a vector with the position of the filters of the 2 x 2
    // Super-pixel of your camera. The possible positions are:
    // (0, 0) --> Index 0
    // (0, 1) --> Index 1
    // (1, 0) --> Index 2
    // (1, 1) --> Index 3
    // The first element of the vector is the position of the filter
    // oriented at 135. The second element is the position of the filter
    // oriented at 0 degrees, the third element is the filter oriented at
    // 90, and the last element is the position of the filter oriented at 45 degrees.
    /// The example given here corresponds to this disposition:
    //                  | 135  |   0  |
    //                  |  90  |  45  |
    std::map<int, int> filtersMap;
    filtersMap[135] = 0;
    filtersMap[0] = 1;
    filtersMap[90] = 2;
    filtersMap[45] = 3;
    std::vector<int> filterOrder({filtersMap[135],filtersMap[0],filtersMap[90],filtersMap[45]});

    // If this flag is true, the orientations obtained from the images filename
    // is used. If false, the AoLP estimator defined in the paper is used.
    bool useGt = false;
    calibrator.loadData(filterOrder, angles, imgs, useGt);

    // If this flag is true, the light parameters estimations and the pixel
    // parameters are done as the super-pixel. If this flag is false,
    // these estimations are done using a single pixel. Note that this method
    // should not be confused with the single-pixel method by Powell and Gruev.
    // In that case, a different model is used for the super-pixel and for the
    // single-pixel method. In our implementation, the same pixel model is used.
    bool useSP = true;
    calibrator.computeCalibrationMatrices(useSP);

    std::cout << "During calibration, the estimated light parameters are:" << std::endl;
    printVector("S0", calibrator.getCalibrationS0());
    printVector("DoLP", calibrator.getCalibrationDoP());

    // Test in image: stored in 16 bits, but coded in 12 bits (max val == 4095)
    std::string testImgPath = dataPath + "/testing_images/cars_1.png";
    cv::Mat img = cv::imread(testImgPath, cv::IMREAD_UNCHANGED | cv::IMREAD_ANYDEPTH);
    if (img.empty())
    {
        std::cout << "ERROR: cannot open image: " << testImgPath << std::endl;
        return -1;
    }

    cv::Mat correctedImg;
    calibrator.correctImage(img, true, correctedImg);

    std::vector<cv::Mat> origPostProcessed;
    std::vector<cv::Mat> correctedPostProcessed;

    ProcessingParameters params;
    // In the I_RO_PHI case, we compute the Total intensity, the DoLP and the AoLP,
    // separated by color channel. They are grouped by 4 (red, green_1, green_2, blue).
    // In the output vector we have that:
    // Indexes [0-3] are the intensity images
    // Indexes [4-7] are the AoLP images
    // Indexes [8-1] are the DoLP images
    params.action = I_RO_PHI;
    params.filtersMap = filtersMap;

    PolarimetricImagesProcessing::processImage(img, params, origPostProcessed);
    PolarimetricImagesProcessing::processImage(correctedImg, params, correctedPostProcessed);

    assert(img.channels() == 1);
    cv::namedWindow("Original image", cv::WINDOW_NORMAL);
    cv::imshow("Original image", origPostProcessed[4] * (65535.0 / 4095.0));

    cv::namedWindow("Corrected image", cv::WINDOW_NORMAL);
    cv::imshow("Corrected image", correctedPostProcessed[4] * (65535.0 / 4095.0));

    cv::waitKey();
    cv::destroyAllWindows();
    return 0;
}