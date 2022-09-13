[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](LICENSE)

# PoliCalibration - Calibration of Polarization Cameras
## <b>A Practical Calibration Method for RGB Micro-Grid Polarimetric Cameras</b> <br> [[Paper arXiv]](https://arxiv.org/pdf/2208.13485.pdf) [[Code]](Code/) [[Supplem Material]](Supplementary_documents/)

This repository contains the original implementation of the polimetric
calibration paper **"A practical calibration method for RGB micro-grid**
**polarimetric cameras"**, submitted to RA-L 2022.

<table>
  <tr align="center">
    <td><img src="images/00_Uncalibrated_Intensity.png" width=600></td>
    <td><img src="images/00_Uncalibrated_AoLP.png" width=600></td>
    <td><img src="images/00_Calibrated_AoLP.png" width=600></td>
  </tr>
  <tr align="center">
    <td>(a) Intensity image</td>
     <td>(b) Uncalibrated AoLP</td>
     <td>(c) Calibrated AoLP</td>
  </tr>
 </table>

Our calibration approach finds the parameters of camera and lens to generate the super-pixel matrix, as shown in the results of the calibration for the angle of linear polarization (AoLP). (a) RGB Intensity image. (b) Uncalibrated AoLP. (c) Calibrated AoLP.
The AoLP has been colored using the HSV color palette to allow the circularity of the colors (180 and 0 degrees should be represented with the same color).

## Citation
If you find this code useful for your research, please cite the paper:
```TeX
@ARTICLE{rodriguez-ral2022,
  author={Rodriguez, Joaquin and Lew-Yan-Voon, Lew and Martins, Renato and Morel, Olivier},
  journal={IEEE Robotics and Automation Letters}, 
  title={A Practical Calibration Method for RGB Micro-Grid Polarimetric Cameras}, 
  year={2022},
  volume={7},
  number={4},
  pages={9921-9928},
  doi={10.1109/LRA.2022.3192655}}
```

# [Code](Code/)
In this folder, the paper implementation code is included. It consists of
C++ code with the developed method, and some raw images to test its
performance.

# [Supplementary documents](Supplementary_documents/)
In this folder, we detail some points which due to space reasons, could
not be included in the main paper:
1. The theoretical demonstration about the angle of polarization error.
2. The results of using a calibrated camera in real robotics applications.

Nevertheless, these explanations are not part of the paper. The main text stands
by itself. These explanations are addressed to the readers who want to
know the additional details of the calibration modelling presented in the
main paper.

# [Supplementary images](Supplementary_images)
In this folder, images that could not be included in the paper are attached.
Since the RGB channels from the image are considered as three separated images,
then there are three different calibration results per frame. We have
presented the red channel results in the paper, while in this folder includes
the analysis for the red, green and blue channels.

Additionally, two set of images have been added, which correspond to the high
resolution version of the applications result images.
