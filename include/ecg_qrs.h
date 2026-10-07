#ifndef ECG_QRS_H
#define ECG_QRS_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float threshold;
    float adaptive_level;
    float noise_level;
    uint32_t refractory_samples;
    uint32_t since_last_peak;
    uint32_t sample_index;
    bool initialized;
} ecg_qrs_detector_t;

void ecg_qrs_init(ecg_qrs_detector_t *detector, float threshold);
bool ecg_qrs_process(ecg_qrs_detector_t *detector, float sample, uint32_t *rr_interval_samples);

#endif
