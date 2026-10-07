#ifndef ECG_PIPELINE_H
#define ECG_PIPELINE_H

#include <stdint.h>
#include "ecg_types.h"
#include "ecg_filter.h"
#include "ecg_qrs.h"
#include "ecg_power.h"
#include "ecg_storage.h"

typedef struct {
    ecg_fir_filter_t filter;
    ecg_qrs_detector_t qrs;
    ecg_power_manager_t power;
    ecg_event_store_t store;
    ecg_status_t status;
    float noise_ema;
    uint64_t sample_index;
} ecg_pipeline_t;

void ecg_pipeline_init(ecg_pipeline_t *pipeline);
void ecg_pipeline_process_sample(ecg_pipeline_t *pipeline, uint16_t raw_adc, uint64_t timestamp_ms);
const ecg_status_t *ecg_pipeline_status(const ecg_pipeline_t *pipeline);
const ecg_event_store_t *ecg_pipeline_events(const ecg_pipeline_t *pipeline);

#endif
