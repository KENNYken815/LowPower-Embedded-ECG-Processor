#ifndef ECG_QUALITY_H
#define ECG_QUALITY_H

#include "ecg_types.h"

ecg_signal_quality_t ecg_quality_classify(float sample, float filtered, float noise_metric);
bool ecg_quality_accept(ecg_signal_quality_t quality);

#endif
