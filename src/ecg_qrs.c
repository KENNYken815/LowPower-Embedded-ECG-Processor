#include "ecg_qrs.h"

#define REFRACTORY_MIN 50u

void ecg_qrs_init(ecg_qrs_detector_t *detector, float threshold) {
    if (!detector) return;
    detector->threshold = threshold;
    detector->adaptive_level = threshold;
    detector->noise_level = threshold * 0.25f;
    detector->refractory_samples = REFRACTORY_MIN;
    detector->since_last_peak = REFRACTORY_MIN;
    detector->sample_index = 0;
    detector->initialized = true;
}

bool ecg_qrs_process(ecg_qrs_detector_t *detector, float sample, uint32_t *rr_interval_samples) {
    if (!detector || !detector->initialized) return false;
    detector->sample_index++;
    if (detector->since_last_peak < UINT32_MAX) detector->since_last_peak++;

    float magnitude = sample < 0.0f ? -sample : sample;
    detector->adaptive_level = 0.995f * detector->adaptive_level + 0.005f * magnitude;
    detector->noise_level = 0.99f * detector->noise_level + 0.01f * magnitude;

    float dynamic_threshold = detector->noise_level +
                              0.35f * (detector->adaptive_level - detector->noise_level);
    if (dynamic_threshold < detector->threshold) dynamic_threshold = detector->threshold;

    if (magnitude >= dynamic_threshold && detector->since_last_peak >= detector->refractory_samples) {
        if (rr_interval_samples) *rr_interval_samples = detector->since_last_peak;
        detector->since_last_peak = 0;
        return true;
    }
    return false;
}
