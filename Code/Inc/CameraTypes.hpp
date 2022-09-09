#ifndef __CAMERA_TYPES_HPP__
#define __CAMERA_TYPES_HPP__

// OpenCV includes
#include "opencv2/core/core.hpp"

// STD includes
#include <map>

// Qt Includes

// Pylon includes

// Custom includes

typedef struct {
    cv::Mat averageImage;
    int anglePol;
    int N;
} experimentSamples;

/**
 * @brief SuperPixelParams: Struct with all the coefficients of the cosine function
 * that we use to fit when we do the super-pixel gain calibration. The equation used is
 *                  Ii(v) = (S0 * Ti / Pi) * (1 + dop * Pi * cos (2*(v - phaseShift_i)))
 * where:
 *  - i indicates the angle of the considered pixel (i = {135, 0, 90, 45})
 *  - v is the input angle at which we want to compute the cosine function
 *  - S0 is the intensity of the unpolarized light
 *  - Ti is the transfer factor (ideally, 0.5)
 *  - Pi is the factor that considers the non ideality of the pixel
 *  - DoP: Degree of polarization that is tranfered through the pixel.
 *  - phaseShift_i is the orientation of the micro polarizer in the pixel.
 *   we cannot split S0, Ti and Pi, nor dop and Pi, we combine them as:
 *                      TiPiS0_i = S0 * Ti / Pi
 *                      TiS0DOP_i = dop * Pi
 *
 *  Since the superpixel consists of 4 pixels, phaseShift, TiPiS0 and TiS0DOP
 * are vectors with 4 elements.
*/
typedef struct
{
    std::vector<double> phaseShift;
    std::vector<double> TiPiS0;
    std::vector<double> TiS0DOP;
} SuperPixelParams;

/**
 * @brief LightEstimationMethod: Enum that defines how we want to estimate the
 * light parameters from the light source. Since we cannot separate the values
 * Ti and Pi from S0 and the degree of polarization if we do not know the last
 * two, we have to find a solution to know them from the samples taken from
 * the camera.
 *  In all the methods, we consider the camera as ideal (Ti = 0.5 and Pi = 1.0),
 * and we estimate S0 and the DoLP by different ways:
 *      - We take do the running average of S0 and DoLP detected by the pixels.
 *      - We take the detected values from the pixel that detects the maximum S0
 *      - We take the detected values from the pixel that detects the median value of S0.
 *
 * The median is better than the average since it is more robust to bad measurements,
 * but it is possible to have degrees of polarization detected by the camera higher
 * than 1. The maximum value is sensible to noise, but it avoids having DoLP values
 * higher than 1.
*/
typedef enum
{
    METHOD_AVERAGE = 120,
    METHOD_MAXIMUM = 125,
    METHOD_MEDIAN = 130,
} LightEstimationMethod;

/**
 * @brief ProcessingLevel enum
 *  This enum contains the enumeration of the different type of processing we can do
 * at the images received by the polarimetric camera. These level means:
 *      - RAW_IMAGE: Do not process the image, leave it as received from the
 *          camera driver (all color and polarization information put in a single image)
 *      - RAW_SPLITTED_IMAGES: Divide the received image into 4,
 *          one per polarization state. No further processing is done, so the
 *          four images are single channel.
 *      - COLOR_SPLITTED: After dividing the images into 4 images, single channel,
 *          we proceed to demosaick them and to create color images (3 channels).
 *      - COLOR_CORRECTED: Same as before, but the intensities are computed based
 *          on the calibration matrix provided. So, from the input intensities and
 *          the calibration matrix, we compute the Stokes vector, and then, from
 *          the Stokes vector, we compute the ideal super-pixel intensities.
 *      - COLOR_ORIGINAL: It means split the images, and create the intensity
 *          color image, as if it were taken by normal camera. This is done using
 *          the I0 and I90 images.
 *      - STOKES: It receives a raw image, and it takes the pixels corresponding to
 *          a single color super-pixel to compute the stokes S0, S1 and S2 parameters
 *          for that wave length. For that, it uses either a matrix with the coefficients
 *          that multiplies each intensity value in order to obtain the corresponding
 *          Stokes values per pixel. Each channel is returned separatedly. It outputs
 *          9 images.
 *      - I_RO_PHI: It does the same as STOKES, but it uses the output Stokes images
 *          to compute the intensity, degree of polarization and angle of polarization
 *          images. The returned images are splitted by color channel.
*/
typedef enum
{
    RAW_IMAGE = 0,
    RAW_SPLITTED_IMAGES,
    COLOR_SPLITTED,
    COLOR_ORIGINAL,
    STOKES,
    I_RO_PHI
} ProcessingLevel;

/**
 * @brief Struct that holds all the parameters required to process a polarization
 * image. This way, we do not have to modify the interfaces each time we add
 * a new parameters to the arguments list. Additionally, this helps to future
 * modifications, since adding a parameters does not change anything from the
 * rest of the code.
 *
 * @param action: ProcessingLevel enum value that tells which operations the
 *      user wants to do after retriving an image from the camera.
 *
 * @param filtersMap: Map from the filter orientations to their position in the super-pixel
 *      configuration. For instance, for the Basler acA2440-75ucPOL, the map is:
 *      135 --> 0
 *      0   --> 1
 *      90  --> 2
 *      45  --> 3
*/
typedef struct {
    ProcessingLevel action;
    std::map<int, int> filtersMap;
} ProcessingParameters;

#endif // __CAMERA_TYPES_HPP__
