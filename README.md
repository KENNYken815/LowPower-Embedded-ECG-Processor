# Low-Power Embedded ECG Processor

A host-runnable embedded C reference implementation for a wearable-style ECG processor. The project models the firmware pipeline from ADC samples through digital filtering, signal-quality assessment, lightweight QRS/beat detection, heart-rate estimation, local event storage, and activity-driven power-mode management.

> **Scope:** Educational/reference firmware architecture. It is not a medical device, does not provide diagnostic advice, and does not claim clinical validation, board-level electrical validation, or measured battery life.

## System Architecture

    ECG electrodes
         |
    AD8232 / ADS1292R
         |
      ADC + DMA
         |
    Sample buffer
         |
     FIR filtering
         |
    Signal-quality gate
         |
     QRS detector
         |
    Heart-rate estimate
         |
     Event ring buffer
         |
    SPI/QSPI Flash or SD

              +----------------------+
              | Power Manager        |
              | LOW <-> ACTIVE       |
              +----------^-----------+
                         |
                  signal / beat activity

## Implemented Features

### Real-time ECG DSP
- Fixed 250 Hz reference sample rate.
- 17-tap FIR filter with deterministic per-sample processing.
- Portable C implementation suitable for a Cortex-M port.
- CMSIS-DSP can replace the portable FIR backend on the target.

### QRS / Heart-Rate Detection
- Lightweight adaptive amplitude threshold.
- Refractory interval to reduce repeated detections.
- RR interval measurement in samples.
- Heart-rate estimation in BPM.

This is an engineering/demo detector, not a clinically validated algorithm.

### Signal-Quality Handling
The pipeline classifies signal conditions as GOOD, WEAK, SATURATED, or NOISY. Only good-quality samples are forwarded to the QRS detector.

### Event Storage
Detected beats are stored as bounded event records containing sample index, timestamp, raw amplitude, estimated heart rate, and signal-quality state. The in-memory ring buffer is the abstraction point for a future SPI/QSPI flash journal or SD-card logger.

### Dynamic Low-Power Operation
Two firmware modes are modeled. ACTIVE enables immediate processing and event handling. LOW represents reduced non-critical compute/storage activity while acquisition remains conceptually available. The reference code models the transition policy; it does not claim a specific current draw.

## Repository Structure

    LowPower-Embedded-ECG-Processor/
    +-- .github/workflows/ci.yml
    +-- docs/
    |   +-- ARCHITECTURE.md
    |   +-- HARDWARE_INTEGRATION.md
    |   +-- POWER_MODES.md
    |   +-- REALTIME_PIPELINE.md
    |   +-- SIGNAL_QUALITY.md
    |   +-- TEST_PLAN.md
    +-- examples/ecg_demo.c
    +-- include/
    |   +-- ecg_filter.h
    |   +-- ecg_pipeline.h
    |   +-- ecg_power.h
    |   +-- ecg_qrs.h
    |   +-- ecg_quality.h
    |   +-- ecg_storage.h
    |   +-- ecg_types.h
    +-- src/
    |   +-- ecg_filter.c
    |   +-- ecg_pipeline.c
    |   +-- ecg_power.c
    |   +-- ecg_qrs.c
    |   +-- ecg_quality.c
    |   +-- ecg_storage.c
    +-- tests/test_ecg.c
    +-- tools/analyze_ecg.py
    +-- .gitignore
    +-- LICENSE
    +-- Makefile
    +-- README.md

## Build

Requirements: C11 compiler such as GCC or Clang, GNU Make, and Python 3 for the optional offline analysis helper.

Build and run:

    make
    make demo

Run tests:

    make test

Clean:

    make clean

GitHub Actions is configured to build and run the test suite on pushes and pull requests.

## Demonstration Model

The demo generates a deterministic synthetic ECG-like waveform and pushes it through the same processing pipeline used by the library. It reports processed sample count, detected beats, estimated heart rate, signal quality, current power mode, and queued/dropped events.

The synthetic source exists only for deterministic software testing and is not intended to represent a clinical ECG recording.

## Offline Analysis

`tools/analyze_ecg.py` provides a small CSV inspection utility for recordings exported from target firmware or analysis workflows. Minimum columns are `timestamp_ms,raw_adc`; optional columns include `filtered`, `heart_rate_bpm`, and `quality`.

Example:

    python3 tools/analyze_ecg.py samples.csv

## RTOS / DMA Mapping

A target architecture can map the modules onto FreeRTOS or Zephyr:

    ADC/DMA ISR
        |
        v
    ECG_AcquireTask
        |
        v
    ECG_ProcessTask
      |       |
      |       +--> QRS / HR
      +----------> Signal quality
        |
        v
    ECG_StorageTask

A power task or power-policy callback can switch the MCU between performance states based on activity and application policy.

At 250 Hz, a 32-sample DMA block represents 128 ms of samples. Hardware implementations must size processing and storage budgets against the actual MCU, interrupt latency, flash latency, and power-state transition timing.

## Hardware Integration Path

Suggested prototype: STM32L4/L5 or nRF52/nRF53; AD8232 or ADS1292R ECG front end; timer-triggered ADC/SAADC; DMA/PPI/DPPI-style buffering where supported; CMSIS-DSP or equivalent optimized DSP; FreeRTOS or Zephyr; SPI/QSPI NOR flash or SD card; optional BLE telemetry.

A real ECG hardware design also requires appropriate analog protection, lead-off handling, grounding/isolation considerations, calibration, and validation appropriate to the intended use.

## Verification Scope

Software tests cover FIR operation, QRS refractory and RR handling, power-mode transitions, event storage, and end-to-end pipeline ingestion. Hardware validation should additionally measure worst-case processing time, DMA servicing, storage latency, current in each power state, and detection behavior on representative recordings.

No claim is made that software tests establish clinical accuracy or medical-device compliance.

## Portfolio / Interview Angle

This project demonstrates ADC/DMA-oriented acquisition, real-time DSP, FIR filtering, event-driven QRS detection, heart-rate estimation, signal-quality handling, bounded event storage, low-power firmware policy, RTOS task decomposition, hardware abstraction, unit testing/CI, and offline signal-analysis workflow.

### Presentation Flow

1. Acquire ECG samples at a deterministic rate.
2. Buffer them with DMA so the CPU does not service every conversion.
3. Filter the waveform before detection logic.
4. Reject saturated, weak, or noisy samples.
5. Detect QRS/beat events with bounded processing.
6. Estimate RR interval and heart rate.
7. Store compact event records locally.
8. Adapt power based on signal activity.
9. Validate execution time and current consumption on the selected MCU.

## Safety Boundary

This repository is for embedded-systems education and engineering practice. It is not a medical device and must not be used to diagnose or treat a person.

## License

MIT License.
