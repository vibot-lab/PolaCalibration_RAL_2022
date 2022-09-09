// OpenCV includes
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

// STD includes
#include <cmath>
#include <iostream>

// Qt Includes

// Pylon includes

// Custom includes
#include "PolarimetricImagesProcessing/PolarimetricImagesProcessing.hpp"

template<typename T>
void templateSplit(const cv::Mat &input, std::vector<cv::Mat> &output)
{
    if (input.empty())
    {
        return;
    }
    int width = static_cast<int>(input.cols);
    int height = static_cast<int>(input.rows);
    int columns = static_cast<int>(width / 2.0);
    int rows = static_cast<int>(height / 2.0);

    output.clear();
    output.resize(4);
    output[0] = cv::Mat(rows, columns, input.type());
    output[1] = cv::Mat(rows, columns, input.type());
    output[2] = cv::Mat(rows, columns, input.type());
    output[3] = cv::Mat(rows, columns, input.type());
    for (int i = 0; i < height; i += 2)
    {
        for (int j = 0; j < width; j += 2)
        {
            output[0].at<T>(i / 2, j / 2) = input.at<T>(i, j);
            output[1].at<T>(i / 2, j / 2) = input.at<T>(i, j + 1);
            output[2].at<T>(i / 2, j / 2) = input.at<T>(i + 1, j);
            output[3].at<T>(i / 2, j / 2) = input.at<T>(i + 1, j + 1);
        }
    }
}

void PolarimetricImagesProcessing::splitImages(const cv::Mat &input, std::vector<cv::Mat> &output)
{
    uchar depth = input.type() & CV_MAT_DEPTH_MASK;
    assert(input.channels() == 1);
    switch(depth)
    {
        case CV_8U:
        {
            templateSplit<uchar>(input, output);
            break;
        }
        case CV_16U:
        {
            templateSplit<ushort>(input, output);
            break;
        }
        case CV_32F:
        {
            templateSplit<float>(input, output);
            break;
        }
        case CV_64F:
        {
            templateSplit<double>(input, output);
            break;
        }
        default:
        {
            std::cout << "ERROR: Data type not handled when splitting images" << std::endl;
            assert(0);
            break;
        }
    }
}

void PolarimetricImagesProcessing::demosaickImages(std::vector<cv::Mat> &input, std::vector<cv::Mat> &output)
{
    if (input.empty())
    {
        return;
    }
    output.clear();
    output.resize(input.size());
    for (size_t i = 0; i < input.size(); i++)
    {
        // IMPORTANT! The output is in RGB!!!! I checked the input pattern, and
        // it is correct, BayerRG (for a yellow light, I have pixel[0] and pixel[1] high,
        // and pixel[2] is low). Nonetheless, the pattern that gives me this is
        // BayerRG2BGR, and not the inverse.
        // I found the following official issues, that reports the problem:
        // https://github.com/opencv/opencv/issues/4857
        // https://github.com/opencv/opencv/issues/19629
        cv::demosaicing(input[i], output[i], cv::COLOR_BayerRG2BGR, 3);
    }
}

void PolarimetricImagesProcessing::convertFloatToIntsImgs(std::vector<cv::Mat>& imgs, std::vector<double> scaling, int intType)
{
    // res is a dummy variable. We change the input variable in place.
    std::vector<int> res(imgs.size());
    std::transform(
        imgs.begin(),
        imgs.end(),
        scaling.begin(),
        res.begin(),
        [intType](cv::Mat& img, const double& scale)
        {
            img.convertTo(img, intType, scale);
            return 0;
        }
    );
}

void PolarimetricImagesProcessing::processImage(const cv::Mat &input, const ProcessingParameters& params, std::vector<cv::Mat> &output)
{
    output.clear();
    switch(params.action)
    {
        case RAW_IMAGE:
        {
            output.push_back(input.clone());
            break;
        }
        case RAW_SPLITTED_IMAGES:
        {
            splitImages(input, output);
            break;
        }
        case COLOR_SPLITTED:
        {
            std::vector<cv::Mat> splitted;
            splitImages(input, splitted);
            demosaickImages(splitted, output);
            break;
        }
        case COLOR_ORIGINAL:
        {
            std::vector<cv::Mat> splitted;
            splitImages(input, splitted);
            /// We should divide only by 2, but since it can create a pixel
            // saturation, we divide by 4.
            cv::Mat stokesInt = (splitted[0] + splitted[1] + splitted[2] + splitted[3]) * 0.25;
            std::vector<cv::Mat> color(1);
            stokesInt.convertTo(color[0], CV_16UC1);
            demosaickImages(color, output);
            break;
        }
        case STOKES:
        {
            std::vector<cv::Mat> stokes;
            computeStokes(input, params.filtersMap, stokes);

            // We divide the intensity by 2 since if there are close to saturation areas
            // in the original image, we will saturate them when showing them as integers.
            // It is just a visualization problem, not a mathematical problem.
            stokes[0] = 0.5 * stokes[0];
            for(cv::Mat& img : stokes)
            {
                img = cv::abs(img);
                img.convertTo(img, CV_16UC1);
            }

            std::vector<cv::Mat> channels;
            // After each split, we have 4 channels
            output = std::vector<cv::Mat>(4 * stokes.size());
            int outputCounter = 0;

            for(size_t i = 0; i < stokes.size(); i++)
            {
                channels.clear();
                splitImages(stokes[i], channels);
                channels[0].copyTo(output[outputCounter++]);
                channels[1].copyTo(output[outputCounter++]);
                channels[2].copyTo(output[outputCounter++]);
                channels[3].copyTo(output[outputCounter++]);
            }
            break;
        }
        case I_RO_PHI:
        {
            std::vector<cv::Mat> stokes;
            std::vector<cv::Mat> iRoPhi;

            computeStokes(input, params.filtersMap, stokes);
            computeIRoPhi(stokes, iRoPhi);

            /// IMPORTANT: We should divide the AoP by 2 * M_PI, but since it is shown with a circular
            // color palette, we need the angles go from 0 until 4095. This way, the angles
            // at the maximum level (180 degrees) will have the same color as the lowest
            // values (0 degrees).
            // We multiply the intensity by 0.5 since if there are close to saturation areas
            // in the original image, we will saturate them when showing them as integers.
            // It is just a visualization problem, not a mathematical problem.
            std::vector<double> scalingFactors = {0.5, 4095.0 / M_PI, 4095.0};

            convertFloatToIntsImgs(iRoPhi, scalingFactors, CV_16UC1);

            std::vector<cv::Mat> channels;
            // After each split, we have 4 channels
            output = std::vector<cv::Mat>(4 * iRoPhi.size());
            int outputCounter = 0;

            for(size_t i = 0; i < iRoPhi.size(); i++)
            {
                channels.clear();
                splitImages(iRoPhi[i], channels);
                channels[0].copyTo(output[outputCounter++]);
                channels[1].copyTo(output[outputCounter++]);
                channels[2].copyTo(output[outputCounter++]);
                channels[3].copyTo(output[outputCounter++]);
            }
            break;
        }
        default:
        {
            std::cerr << "Unrecognized processing type: " << params.action << std::endl;
            assert(0);
            break;
        }
    }
}

void PolarimetricImagesProcessing::computeStokes(const cv::Mat& input, std::map<int,int> filtersMap, std::vector<cv::Mat> &output)
{
    output.clear();
    output.resize(3);

    if (!input.empty())
    {
        cv::Mat floatImg;
        std::vector<cv::Mat> splittedImg;
        input.convertTo(floatImg, CV_64FC1);
        splitImages(floatImg, splittedImg);

        output[0] = (splittedImg[0] + splittedImg[1] + splittedImg[2] + splittedImg[3]) * 0.5;
        output[1] = splittedImg[filtersMap[0]] - splittedImg[filtersMap[90]];
        output[2] = splittedImg[filtersMap[45]] - splittedImg[filtersMap[135]];
    }
}

void PolarimetricImagesProcessing::computeIRoPhi(std::vector<cv::Mat>& stokesVector, std::vector<cv::Mat> &output)
{
    cv::Mat mergedInput;
    cv::merge(stokesVector, mergedInput);

    cv::Mat matrixOutput(mergedInput.size(), mergedInput.type());

    /// IMPORTANT: I know we can use the cv::phase function, but it requires
    // the images to be CV_32FC1, and we have two images that must be converted
    // to this type, from CV_64FC1. This method saves these two convertions,
    // and the last convertion, from CV_64FC1 to CV_16UC1
    std::transform(
        mergedInput.begin<cv::Vec3d>(),
        mergedInput.end<cv::Vec3d>(),
        matrixOutput.begin<cv::Vec3d>(),
        [](const cv::Vec3d& vals)
        {
            double s0 = vals[0];
            double s1 = vals[1];
            double s2 = vals[2];
            double angle = std::atan2(s2, s1) * 0.5;

            // atan2 gives values between -pi and pi. If we multiply by 0.5, we will have
            // values between -pi / 2 and pi / 2. If we add M_PI, the
            // result must be between 0 and PI, thus it is impossible to have negative values,
            // unless they are really tiny (quantization problem).
            if (angle < 0)
            {
                angle += M_PI;
            }

            double dolp = std::sqrt((s1 * s1) + (s2 * s2)) / s0;
            return cv::Vec3d(s0, angle, dolp);
        }
    );
    cv::split(matrixOutput, output);
}
