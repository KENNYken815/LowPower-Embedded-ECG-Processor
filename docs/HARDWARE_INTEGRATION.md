# Hardware Integration

## Suggested prototype

- STM32L4/L5 or Nordic nRF52/nRF53
- AD8232 analog front end or ADS1292R
- ECG electrodes with appropriate analog protection
- MCU ADC/SAADC with DMA-style acquisition
- SPI/QSPI NOR flash or SD card
- optional BLE telemetry

## Acquisition

Use a timer-triggered 250 Hz sampling plan and DMA double-buffering where supported.

## DSP

The repository includes a portable FIR implementation. On Cortex-M, the same pipeline can be mapped to CMSIS-DSP primitives for optimized execution.

## Storage

The in-memory event ring buffer is an abstraction boundary. Replace it with a wear-aware flash journal or filesystem-backed SD logger while keeping the event structure stable.

## RTOS mapping

FreeRTOS:
- ECG_AcquireTask
- ECG_ProcessTask
- ECG_StorageTask
- ECG_PowerTask

Zephyr can use equivalent threads/work queues plus device-driver bindings.

The repository does not claim board-level implementation, electrical validation, or medical certification.
