# Architecture

## Signal path

ECG AFE (AD8232 / ADS1292R)
  -> ADC + DMA
  -> Sample buffer
  -> FIR filtering
  -> Signal quality check
  -> QRS detection
  -> Heart-rate estimate
  -> Event ring buffer
  -> SPI Flash / SD card

A power manager observes signal activity and recent beat detections to select a low-power or active processing mode.

## Firmware separation

- ecg_filter.*: deterministic sample-by-sample FIR DSP.
- ecg_qrs.*: lightweight adaptive threshold QRS/beat detector with refractory timing.
- ecg_quality.*: saturation, weak-signal and noise classification.
- ecg_storage.*: bounded in-memory event queue, replaceable by SPI flash/SD storage.
- ecg_power.*: activity-driven power-state policy.
- ecg_pipeline.*: orchestration layer suitable for an MCU task or ISR-to-task pipeline.

## MCU mapping

A practical STM32L4/L5 or nRF52/nRF53 implementation can map:
- ADC/SAADC + DMA/PPI/DPPI to acquisition;
- a fixed-size DMA buffer to the processing task;
- CMSIS-DSP or equivalent optimized FIR routines;
- FreeRTOS or Zephyr for task scheduling;
- RTC/timer timestamps for events;
- SPI/QSPI flash or SD for persistence.

The repository intentionally keeps MCU-specific BSP code outside the algorithmic reference implementation.
