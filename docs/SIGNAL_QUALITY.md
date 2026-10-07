# ECG Signal Quality

The reference pipeline classifies samples using:
- ADC saturation detection;
- weak filtered-amplitude condition;
- an exponential noise/error estimate.

Only samples classified as ECG_SIGNAL_GOOD are passed to the QRS detector.

Real ECG products should use application-specific quality indices, lead-off detection, baseline-wander handling, motion-artifact rejection and validated datasets.

## Safety boundary

This repository is a learning and engineering reference. It is not a medical device and must not be used for diagnosis or treatment decisions.
