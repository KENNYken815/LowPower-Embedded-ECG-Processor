# Power Mode Policy

## ACTIVE

Used after beat/activity detection.

Typical hardware actions:
- full DSP processing enabled;
- higher CPU performance state;
- storage writes allowed;
- optional wireless telemetry enabled.

## LOW

Entered after a configurable idle period without detected activity.

Typical hardware actions:
- retain ECG acquisition;
- reduce CPU clock where supported;
- batch non-critical work;
- defer optional storage/telemetry activity;
- retain timestamp and supervision functions.

A real wearable must characterize current consumption for the selected MCU, AFE, memory and radio. This repository models the policy, not measured battery life.
