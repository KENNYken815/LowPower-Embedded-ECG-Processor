#include "ecg_filter.h"
#include <string.h>

void ecg_fir_init(ecg_fir_filter_t *filter, const float *coeffs) {
    if (!filter) return;
    memset(filter, 0, sizeof(*filter));
    if (coeffs) memcpy(filter->coeffs, coeffs, sizeof(filter->coeffs));
}

float ecg_fir_process(ecg_fir_filter_t *filter, float sample) {
    if (!filter) return sample;
    filter->delay[filter->index] = sample;
    float y = 0.0f;
    size_t j = filter->index;
    for (size_t i = 0; i < ECG_FILTER_TAPS; ++i) {
        y += filter->coeffs[i] * filter->delay[j];
        if (j == 0) j = ECG_FILTER_TAPS - 1u;
        else --j;
    }
    filter->index = (filter->index + 1u) % ECG_FILTER_TAPS;
    return y;
}

void ecg_filter_default_coefficients(float *coeffs, size_t count) {
    if (!coeffs || count < ECG_FILTER_TAPS) return;
    const float c[ECG_FILTER_TAPS] = {
        -0.010f,-0.008f,0.000f,0.018f,0.045f,0.069f,0.076f,0.070f,0.054f,
         0.040f,0.054f,0.070f,0.076f,0.069f,0.045f,0.018f,-0.008f
    };
    memcpy(coeffs, c, sizeof(c));
}
