#ifndef ECG_TYPES_H
#define ECG_TYPES_H

#include <stdint.h>
#include <stdbool.h>

#define ECG_SAMPLE_RATE_HZ 250u
#define ECG_BLOCK_SIZE 32u
#define ECG_FILTER_TAPS 17u
#define ECG_EVENT_QUEUE_SIZE 32u

typedef enum {
    ECG_POWER_LOW = 0,
    ECG_POWER_ACTIVE = 1
} ecg_power_mode_t;

typedef enum {
    ECG_SIGNAL_GOOD = 0,
    ECG_SIGNAL_WEAK = 1,
    ECG_SIGNAL_SATURATED = 2,
    ECG_SIGNAL_NOISY = 3
} ecg_signal_quality_t;

typedef struct {
    uint64_t sample_index;
    uint64_t timestamp_ms;
    uint16_t amplitude;
    uint16_t heart_rate_bpm;
    ecg_signal_quality_t quality;
} ecg_event_t;

typedef struct {
    uint32_t sample_count;
    uint32_t detected_beats;
    uint32_t rejected_samples;
    uint16_t heart_rate_bpm;
    ecg_signal_quality_t signal_quality;
    ecg_power_mode_t power_mode;
    bool beat_detected;
} ecg_status_t;

#endif
