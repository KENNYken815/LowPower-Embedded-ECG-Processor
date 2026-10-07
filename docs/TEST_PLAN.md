# Test Plan

## Unit-level
- FIR initialization and impulse response.
- QRS refractory behavior and beat interval calculation.
- Power-mode transitions.
- Event queue push/pop and overflow handling.
- End-to-end sample ingestion.

## Integration
- Feed deterministic synthetic ECG-like waveforms.
- Inject saturation and noisy samples.
- Verify events contain monotonically increasing sample indices.
- Verify poor-quality samples do not generate normal beat detections.

## Hardware validation

For a physical prototype, measure:
- ADC/DMA timing;
- CPU utilization;
- worst-case processing time;
- storage write latency;
- current in each power mode;
- wake/sleep transition timing;
- detection performance on representative recordings.

Software tests are not a substitute for clinical validation.
