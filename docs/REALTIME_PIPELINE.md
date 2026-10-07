# Real-Time Pipeline

Recommended execution model:

1. ADC samples ECG at 250 Hz.
2. DMA fills a small buffer, for example 32 samples.
3. A DMA-complete interrupt signals a processing task.
4. The processing task executes FIR filtering and signal-quality analysis.
5. Accepted samples enter the QRS detector.
6. A detected beat generates an event containing timestamp, amplitude, heart rate and quality.
7. The event queue is drained by a storage task.
8. The power manager keeps acquisition alive while allowing compute/storage/peripheral activity to drop when signal activity is low.

The algorithmic work is bounded per sample. The reference code is portable C and can be integrated into a periodic RTOS task.

## Latency budget

For 250 Hz sampling and a 32-sample block, the nominal collection interval is 128 ms. A hardware design should complete acquisition servicing and processing with margin before the next required DMA service window.

The desktop reference implementation does not claim a measured MCU execution time.
