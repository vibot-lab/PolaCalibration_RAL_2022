# Supplementary documents
In this folder, PDF files with supplementary information to the paper are added.
The documents placed here have been written by the authors, and they don't replace
the paper. They just justify results or equations that are in the paper, but their
detailed justification is not part of the aim of the main text.

# Table of Contents
- ### [AoLP estimator - Error shape demonstration](#aolp-error)

<b name="aolp-error"></b>
# AoLP estimator - Error shape demonstration
In this document, we demonstrate why the error of the AoLP estimator has a
sine-like shape. This demonstration is auxiliary to the submitted manuscript and the
explanation was not included in the main paper due to space restrictions.

In this supplementary material, after finding the mathematical expression of this error, the estimator algorithm is tested with for four different lenses, showing that
the each lens has an different effect in the AoLP estimation when considering the
camera as ideal. Then, by doing least-squares optimization and RANSAC, we
find the formula parameters that fits the measured data. The values obtained
by this method are consistent with our conclusions, and it justifies the
fact that calibration is required.