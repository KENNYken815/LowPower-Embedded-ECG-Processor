#include "ecg_storage.h"
#include <string.h>

void ecg_store_init(ecg_event_store_t *store) {
    if (!store) return;
    memset(store, 0, sizeof(*store));
}

bool ecg_store_push(ecg_event_store_t *store, const ecg_event_t *event) {
    if (!store || !event) return false;
    if (store->count >= ECG_EVENT_QUEUE_SIZE) {
        store->dropped++;
        return false;
    }
    uint32_t pos = (store->head + store->count) % ECG_EVENT_QUEUE_SIZE;
    store->events[pos] = *event;
    store->count++;
    return true;
}

bool ecg_store_pop(ecg_event_store_t *store, ecg_event_t *event) {
    if (!store || !event || store->count == 0) return false;
    *event = store->events[store->head];
    store->head = (store->head + 1u) % ECG_EVENT_QUEUE_SIZE;
    store->count--;
    return true;
}

uint32_t ecg_store_dropped(const ecg_event_store_t *store) {
    return store ? store->dropped : 0;
}
