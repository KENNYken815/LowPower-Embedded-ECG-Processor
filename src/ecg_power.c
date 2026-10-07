#include "ecg_power.h"

void ecg_power_init(ecg_power_manager_t *pm, uint32_t low_power_after_samples) {
    if (!pm) return;
    pm->idle_samples = 0;
    pm->active_hold_samples = 0;
    pm->low_power_after_samples = low_power_after_samples;
    pm->mode = ECG_POWER_ACTIVE;
}

void ecg_power_on_activity(ecg_power_manager_t *pm) {
    if (!pm) return;
    pm->idle_samples = 0;
    pm->active_hold_samples = 100;
    pm->mode = ECG_POWER_ACTIVE;
}

void ecg_power_tick(ecg_power_manager_t *pm) {
    if (!pm) return;
    if (pm->active_hold_samples > 0) {
        pm->active_hold_samples--;
        pm->idle_samples = 0;
        pm->mode = ECG_POWER_ACTIVE;
        return;
    }
    if (pm->idle_samples < UINT32_MAX) pm->idle_samples++;
    if (pm->idle_samples >= pm->low_power_after_samples) pm->mode = ECG_POWER_LOW;
}

ecg_power_mode_t ecg_power_mode(const ecg_power_manager_t *pm) {
    return pm ? pm->mode : ECG_POWER_LOW;
}
