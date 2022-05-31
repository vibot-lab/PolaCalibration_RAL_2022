# Supplementary documents
In this folder, we have included a document with complementary explanations
to our paper "A practical calibration method for RGB micro-grid polarimetric cameras".

The information given in this document are auxiliary results and explanations
that are not the focus of the paper, and due to space reasons they could not
be included in the main text. This document contains the following topics:

## Angle of linear polarization (AoLP) estimator - Error shape demonstration
We demonstrate why the error of the AoLP estimator has a
sine-like shape in the document [Supplementary_material.pdf](Supplementary_material.pdf).

After finding the mathematical expression of this error, the
estimator algorithm is tested with for two different lenses, showing that
each lens has a different effect in the AoLP estimation when considering the
camera as ideal. Then, by doing least-squares optimization and RANSAC, we
find the formula parameters that fits the measured data. The values obtained
by this method are consistent with our conclusions, and it justifies the
fact that calibration is required.

## Calibration effect over real applications
To show the importance of doing calibration of the RGB polarization cameras,
two applications have been tested:
1. Linear depth estimation from an uncalibrated, monocular polarisation image”, ECCV, 2016
2. P2D: a self-supervised method for depth estimation from polarimetry”, ICPR 2020

Then, the resulting images without and with calibration of each application,
and a discussion about them are included in
[Supplementary_material.pdf](Supplementary_material.pdf). The full resolution
images can be found in the folder [Supplementary_images](../Supplementary_images).

