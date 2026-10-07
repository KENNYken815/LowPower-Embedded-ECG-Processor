#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "ecg_filter.h"
#include "ecg_qrs.h"
#include "ecg_power.h"
#include "ecg_storage.h"
#include "ecg_pipeline.h"

static void test_filter(void) {
    ecg_fir_filter_t f;
    float c[ECG_FILTER_TAPS] = {0};
    c[0] = 1.0f;
    ecg_fir_init(&f, c);
    assert(fabsf(ecg_fir_process(&f, 7.0f) - 7.0f) < 0.001f);
}

static void test_qrs(void) {
    ecg_qrs_detector_t q;
    ecg_qrs_init(&q, 10.0f);
    uint32_t rr = 0;
    bool first = false;
    for (int i=0;i<60;i++) {
        float x = (i==50) ? 30.0f : 0.0f;
        first |= ecg_qrs_process(&q, x, &rr);
    }
    assert(first);
    assert(rr >= 50);
}

static void test_power(void) {
    ecg_power_manager_t p;
    ecg_power_init(&p, 4);
    for (int i=0;i<4;i++) ecg_power_tick(&p);
    assert(ecg_power_mode(&p) == ECG_POWER_LOW);
    ecg_power_on_activity(&p);
    assert(ecg_power_mode(&p) == ECG_POWER_ACTIVE);
}

static void test_storage(void) {
    ecg_event_store_t s;
    ecg_store_init(&s);
    ecg_event_t e = {.sample_index=1,.heart_rate_bpm=72};
    assert(ecg_store_push(&s, &e));
    ecg_event_t out;
    assert(ecg_store_pop(&s, &out));
    assert(out.sample_index == 1);
}

static void test_pipeline(void) {
    ecg_pipeline_t p;
    ecg_pipeline_init(&p);
    for (uint32_t n=0;n<1000;n++) {
        float t=(float)n/ECG_SAMPLE_RATE_HZ;
        float sig=2048.0f+50.0f*sinf(2.0f*3.14159265f*1.0f*t);
        ecg_pipeline_process_sample(&p,(uint16_t)sig,(uint64_t)n*4u);
    }
    assert(p.status.sample_count == 1000);
}

int main(void) {
    test_filter();
    test_qrs();
    test_power();
    test_storage();
    test_pipeline();
    puts("All ECG tests passed.");
    return 0;
}
