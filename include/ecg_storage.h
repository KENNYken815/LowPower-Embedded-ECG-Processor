#ifndef ECG_STORAGE_H
#define ECG_STORAGE_H

#include <stdint.h>
#include <stdbool.h>
#include "ecg_types.h"

typedef struct {
    ecg_event_t events[ECG_EVENT_QUEUE_SIZE];
    uint32_t head;
    uint32_t count;
    uint32_t dropped;
} ecg_event_store_t;

void ecg_store_init(ecg_event_store_t *store);
bool ecg_store_push(ecg_event_store_t *store, const ecg_event_t *event);
bool ecg_store_pop(ecg_event_store_t *store, ecg_event_t *event);
uint32_t ecg_store_dropped(const ecg_event_store_t *store);

#endif
