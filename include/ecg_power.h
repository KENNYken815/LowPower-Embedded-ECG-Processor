#ifndef ECG_POWER_H
#define ECG_POWER_H

#include "ecg_types.h"

typedef struct {
    uint32_t idle_samples;
    uint32_t active_hold_samples;
    uint32_t low_power_after_samples;
    ecg_power_mode_t mode;
} ecg_power_manager_t;

void ecg_power_init(ecg_power_manager_t *pm, uint32_t low_power_after_samples);
void ecg_power_on_activity(ecg_power_manager_t *pm);
void ecg_power_tick(ecg_power_manager_t *pm);
ecg_power_mode_t ecg_power_mode(const ecg_power_manager_t *pm);

#endif
