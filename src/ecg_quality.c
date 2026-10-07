#include "ecg_quality.h"
#include <math.h>

ecg_signal_quality_t ecg_quality_classify(float sample, float filtered, float noise_metric) {
    if (sample <= 1.0f || sample >= 4094.0f) return ECG_SIGNAL_SATURATED;
    if (noise_metric > 350.0f) return ECG_SIGNAL_NOISY;
    if (fabsf(filtered) < 2.0f) return ECG_SIGNAL_WEAK;
    return ECG_SIGNAL_GOOD;
}

bool ecg_quality_accept(ecg_signal_quality_t quality) {
    return quality == ECG_SIGNAL_GOOD;
}
