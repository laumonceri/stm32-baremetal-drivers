# I2C Driver

## Overview

The I2C driver provides a bare-metal master-mode interface for STM32 devices. It is designed for short, dependable communication with peripheral devices such as sensors, EEPROMs, and other integrated ICs.

## Why this matters

I2C is one of the most common embedded communication buses. It is especially useful when a system needs to talk to multiple devices using just two wires: SDA and SCL.

## Typical scope

The driver is designed around:

- bus initialization,
- clock configuration,
- master transmit and receive functions,
- device-level communication patterns.

## Design direction

The project keeps the I2C implementation practical and explicit rather than trying to generalize every possible mode. This makes the API easier to understand and easier to integrate into board-level projects.

## Future work

Good next steps include:

- EEPROM examples,
- sensor read/write examples,
- timeout and error handling,
- unit test coverage,
- documentation and architecture notes.
