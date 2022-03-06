# Angle of linear polarization (AoLP) estimator - Error shape demonstration
We demonstrate why the error of the AoLP estimator has a
sine-like shape in [this document AoLP_estimator_error_explanation.pdf](AoLP_estimator_error_explanation.pdf). This demonstration is auxiliary to the submitted manuscript and the
explanation was not included in the main paper due to space restrictions.

After finding the mathematical expression of this error, the
estimator algorithm is tested with for two different lenses, showing that
each lens has a different effect in the AoLP estimation when considering the
camera as ideal. Then, by doing least-squares optimization and RANSAC, we
find the formula parameters that fits the measured data. The values obtained
by this method are consistent with our conclusions, and it justifies the
fact that calibration is required.
