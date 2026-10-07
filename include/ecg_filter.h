#ifndef ECG_FILTER_H
#define ECG_FILTER_H

#include <stddef.h>
#include "ecg_types.h"

typedef struct {
    float coeffs[ECG_FILTER_TAPS];
    float delay[ECG_FILTER_TAPS];
    size_t index;
} ecg_fir_filter_t;

void ecg_fir_init(ecg_fir_filter_t *filter, const float *coeffs);
float ecg_fir_process(ecg_fir_filter_t *filter, float sample);
void ecg_filter_default_coefficients(float *coeffs, size_t count);

#endif
