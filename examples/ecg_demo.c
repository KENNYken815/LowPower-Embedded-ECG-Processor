#include <math.h>
#include <stdio.h>
#include "ecg_pipeline.h"

static uint16_t synthetic_ecg(uint32_t n) {
    const float t = (float)n / ECG_SAMPLE_RATE_HZ;
    const float phase = fmodf(t, 1.0f);
    float signal = 2048.0f + 60.0f * sinf(2.0f * 3.14159265f * 1.2f * t);
    if (phase < 0.045f) signal += 500.0f * expf(-180.0f * phase);
    return (uint16_t)(signal < 0 ? 0 : signal > 4095 ? 4095 : signal);
}

int main(void) {
    ecg_pipeline_t pipeline;
    ecg_pipeline_init(&pipeline);

    for (uint32_t n = 0; n < ECG_SAMPLE_RATE_HZ * 8u; ++n)
        ecg_pipeline_process_sample(&pipeline, synthetic_ecg(n), (uint64_t)(n * 1000u / ECG_SAMPLE_RATE_HZ));

    const ecg_status_t *s = ecg_pipeline_status(&pipeline);
    printf("Low-power embedded ECG processor demo\n");
    printf("samples=%u beats=%u heart_rate=%u bpm quality=%d power=%s events=%u dropped=%u\n",
           s->sample_count, s->detected_beats, s->heart_rate_bpm, s->signal_quality,
           s->power_mode == ECG_POWER_ACTIVE ? "ACTIVE" : "LOW",
           pipeline.store.count, ecg_store_dropped(&pipeline.store));
    return 0;
}
