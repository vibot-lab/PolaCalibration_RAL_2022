# Supplementary images
In this folder, additional images are included, which correspond to tests
explained in the main paper but, for space reasons, they could not be submitted.
In the main text, only red channel results are shown. This folder includes the
missing channels results.

# Table of Contents
1. ### [Figure 5](#figure5)
2. ### [Figure 6](#figure6)
3. ### [Figure 7](#figure7)
4. ### [Figure 8](#figure8)

# Figure 5 <a name="figure5"></a>
This figure corresponds to the improvements with respect to
the number of samples. The improvements are considered for the angle
of linear polarization (AoLP), degree of linear
polarization (DoLP), and the intensity. The black curve
corresponds to the mean values over all the pixels in the
image after calibration, and the vertical gray bars
corresponds to the standard deviation of the pixel values
after calibration. The horizontal axis is the amount of
samples used to calibrate. The file names are composed as:

***Fig5_X_Samples_Improvement_Y.pdf***

where ***X*** is the polarization parameter that it is
considered in the image (S0, AoLP, or DoLP), and ***Y*** corresponds to the
color channel considered (Red, Green, or Blue).

# Figure 6 <a name="figure6"></a>
This figure corresponds to the polarization parameters estimations for all the super-pixels, before and after calibration. The filenames are composed as:

***Fig6_R_S_T.pdf***

where ***R*** is the polarization parameter shown (either
S0, or the degree of linear polarzation DoLP, or the angle
of linear polarization AoLP), ***S*** is either a
Uncalibrated or Calibrated image, and ***T*** is the color
channel (Red, Green, or Blue). For example, ***Fig6_DoLP_Calibrated_Blue*** is
the Figure 6 that corresponds to the calibrated DoLP for the
blue channel. As in the case of the red channel (shown in
the paper), all the uncalibrated images present the 
vignetting effect produced by the lens in all the
polarization parameters. After calibration, this effect
is greatly reduced, since the interval of values present
in the image for a uniform, linearly polarized light image
is small.

In the following table, the interval of values of the
uncalibrated images are shown for each of the polarization
parameters, and for each color channel.

|      | Red channel      | Green channel    | Blue channel     |
|------|------------------|------------------|------------------|
| S0   | [1011, 1463]     | [1127, 1622]     | [264, 390]       |
| AoLP | [41.099, 44.879] | [41.011, 45.055] | [40.615, 45.451] |
| DoLP | [0.947, 1.01]    | [0.941, 1.003]   | [0.906, 1.003]   |

Table 1: Interval of values of the polarization parameters images, when
the camera is uncalibrated, for all the color channels.

The corresponding intervals for the calibrated camera are shown in the Tab. 2:
|      | Red channel      | Green channel    | Blue channel     |
|------|------------------|------------------|------------------|
| S0   | [1370, 1521]     | [1528, 1631]     | [366, 386]       |
| AoLP | [41.934, 43.824] | [42.066, 43.692] | [42.066, 43.956] |
| DoLP | [0.974, 1.0]     | [0.967, 0.994]   | [0.961, 1.007]   |

Table 2: Interval of values of the polarization parameters images, when
the camera is calibrated, for all the color channels.

The differences in the intensities intervals for each channels are expected
since the used light is yellow, and not white.

# Figure 7 <a name="figure7"></a>
This image corresponds to the reconstructed surface of a paraboloid piece,
by using the SfP method detailed in the paper **Linear depth estimation from an**
**uncalibrated, monocular polarisation image**. These images corresponds to the
high-resolution versions of the images included in the main paper.

# Figure 8 <a name="figure8"></a>
This image is not included in the paper, but in the supplementary material. It
corresponds to the high-resolution version of the results obtained with the
Depth from polarization method implemented in **P2D: a self-supervised method**
**for depth estimation from polarimetry**. This set of four images contains:
1. The color image captured with the RGB polarization camera
2. The equivalent Grayscale image that we will give to the DNN to produce
the disparity image.
3. The disparity obtained when the camera is uncalibrated
4. The disparity when the camera is calibrated.

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