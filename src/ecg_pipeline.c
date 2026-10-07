#include "ecg_pipeline.h"

void ecg_pipeline_init(ecg_pipeline_t *pipeline) {
    if (!pipeline) return;
    float coeffs[ECG_FILTER_TAPS];
    ecg_filter_default_coefficients(coeffs, ECG_FILTER_TAPS);
    ecg_fir_init(&pipeline->filter, coeffs);
    ecg_qrs_init(&pipeline->qrs, 35.0f);
    ecg_power_init(&pipeline->power, ECG_SAMPLE_RATE_HZ * 4u);
    ecg_store_init(&pipeline->store);
    pipeline->status = (ecg_status_t){0};
    pipeline->status.signal_quality = ECG_SIGNAL_WEAK;
    pipeline->status.power_mode = ECG_POWER_ACTIVE;
    pipeline->noise_ema = 0.0f;
    pipeline->sample_index = 0;
}

void ecg_pipeline_process_sample(ecg_pipeline_t *pipeline, uint16_t raw_adc, uint64_t timestamp_ms) {
    if (!pipeline) return;

    float centered = (float)raw_adc - 2048.0f;
    float filtered = ecg_fir_process(&pipeline->filter, centered);
    float abs_centered = centered < 0 ? -centered : centered;
    float noise_error = centered - filtered;
    if (noise_error < 0) noise_error = -noise_error;
    pipeline->noise_ema = 0.98f * pipeline->noise_ema + 0.02f * noise_error;

    ecg_signal_quality_t quality = ecg_quality_classify(raw_adc, filtered, pipeline->noise_ema);
    pipeline->status.signal_quality = quality;

    bool beat = false;
    uint32_t rr_samples = 0;
    if (ecg_quality_accept(quality)) beat = ecg_qrs_process(&pipeline->qrs, filtered, &rr_samples);

    pipeline->status.beat_detected = beat;
    if (beat) {
        pipeline->status.detected_beats++;
        if (rr_samples > 0) {
            uint32_t bpm = (ECG_SAMPLE_RATE_HZ * 60u) / rr_samples;
            if (bpm <= 250u) pipeline->status.heart_rate_bpm = (uint16_t)bpm;
        }
        ecg_event_t event = {
            .sample_index = pipeline->sample_index,
            .timestamp_ms = timestamp_ms,
            .amplitude = raw_adc,
            .heart_rate_bpm = pipeline->status.heart_rate_bpm,
            .quality = quality
        };
        (void)ecg_store_push(&pipeline->store, &event);
        ecg_power_on_activity(&pipeline->power);
    } else {
        (void)abs_centered;
        ecg_power_tick(&pipeline->power);
    }

    pipeline->status.sample_count++;
    pipeline->status.power_mode = ecg_power_mode(&pipeline->power);
    pipeline->status.rejected_samples += quality == ECG_SIGNAL_GOOD ? 0u : 1u;
    pipeline->sample_index++;
}
