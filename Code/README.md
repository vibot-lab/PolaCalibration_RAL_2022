# PoliCalibration

This is the code that implements the paper ***A practical calibration method for RGB micro-grid polarimetric cameras***
published by IEEE Robotics and Automation Letters 2022.

This code is composed of four main components:
- The load module: it will look for the sample images, and it will load them in RAM. It will also classify them by angle of polarization, and all the images that corresponds to the same AoLP will be averaged. The images are expected to be stored in a single folder, so when loading a calibration dataset of images, the module expects to receive two parameters: the folder name where the images are (this folder is called experiment), and the root path where this folder is. The images stored in this folder are expected to have the following name convention:

                    angle_N_sample_M.png

  where N is the ground truth angle of linear polarization of that sample (integer number, in degrees), and M is the sample number (integer number).

- The Pixel Gain Calibration module: it is the implementation of the paper. It consists of two files:
  - one that estimates the AoLP of the samples (as described in the paper).
  - one that estimates first the light intensity and degree of linear polarization, 
    and then it estimates the pixel model parameters.

    This module receives the averaged images, and the AoLP read from the filenames,
  which are supposed to be the ground-truth values. By changing a flag (useGt), the module will use the GT AoLP or the estimation of them. Additionally, this module provides a function that corrects the measured intensities of the input, raw image of the camera. This function will work ONLY if the calibration matrices have been computed beforehand.

- Polarimetric Images Processing module: it provides several functionalities to treat the raw images from the RGB micro-grid polarization camera. All the functionalities are given by a single function (processImage), and the different outputs are obtained by changing the action parameter from the ProcessingParameter struct. It receives a single-channeled, mosaicked image, and it returns a vector of images, with the results. The options are: 
  - RAW_IMAGE: The input image is copied in the output.
  - RAW_SPLITTED_IMAGES: The input image is split into four images. Each image contains all the image pixels with the same filter orientation, and all the colors.
  - COLOR_SPLITTED: Same as RAW_SPLITTED_IMAGES, but each image is a BGR image, obtained by using normal BayerRG demosaicking algorithm.
  - COLOR_ORIGINAL: Only one image is obtained, which is the total intensity, as if it captured by a normal color camera.
  - STOKES: It computes the Stokes vector by color channel. For simplicity, the color channels are four: red, green_1, green_2, blue (four pixels of the Bayer pattern). For each color channel, the first three Stokes parameters are computed (S0, S1, and S2).
  - I_RO_PHI: It computes the total intensity I, the degree of linear polarization Rho, and the angle of linear polarization Phi. Each of these parameters are computed by color channel as for the STOKES case.

- The main code, which is a sample application of the integration of the other modules.

In summary, the workflow of the software to calibrate the input images is:
* We create a ***LoadSaveExperiments*** object, and we pass to it the root path of our sample data, and the folder (experiment) name in which the data is stored.
* We create a ***SuperPixelCalibration*** object, and we pass to it the images and the angles loaded by the ***LoadSaveExperiments*** module.
* We compute the calibration matrices with the function ***computeCalibrationMatrices*** of the module ***SuperPixelCalibration***.
* We correct a raw input image by calling the function ***correctImage*** of the ***SuperPixelCalibration*** module.
* We do some processing to the corrected image with the ***PolarimetricImagesProcessing::processImage*** function.
* We continue our program (in the sample program, we show the AoLP of both, original and corrected images).

### NOTE 1: This software assumes by default BayerRG color pattern.
### NOTE 2: The filter configuration can be changed by providing the corresponding mapping of filters. The example of how to change it is in the main code, stored in the variable ***filtersMap***. The mapping given in this example is the one corresponding to a Basler acA2440-75ucPOL camera (Sony Polarsens IMX250MYR).

# Compilation
This software has been build for Ubuntu 18 and tested also on Ubuntu 20. The software requirements are:
* OpenCV4
* C++11
* CMake
* make

Once the dependencies installed, we download this code, and we place on the root directory of the [Code](./) folder,
and we run these commands:
```bash
mkdir build
cd build
cmake ..
```

If no error occurs, you should have a file called ***polarimetric_calibration***. This file can be run with the sample data provided, or some data can be given for your particular camera. In this last case, you have to place your data in a single folder, name the files with the pattern ***angle_N_sample_M.png***, as explained below. Then, you need to adjust the code considering the Bayer pattern you have and the Super-pixel configuration of your sensor. To run the code, you can do it with the test data:
```bash
./polarimetric_calibration
```
or with your own data:
```bash
./polarimetric_calibration /absolute/path/to/your/data experiment_name
```

# Citation
```TeX
@ARTICLE{9834097,
  author={Rodriguez, Joaquin and Lew-Yan-Voon, Lew and Martins, Renato and Morel, Olivier},
  journal={IEEE Robotics and Automation Letters}, 
  title={A Practical Calibration Method for RGB Micro-Grid Polarimetric Cameras}, 
  year={2022},
  volume={7},
  number={4},
  pages={9921-9928},
  doi={10.1109/LRA.2022.3192655}}
```