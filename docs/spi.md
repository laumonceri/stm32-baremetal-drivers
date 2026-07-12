# Serial peripheral interface (SPI)

Bare-metal SPI driver for STM32L452RE (Cortex-M4).

Source: 

---



---

## 1. Overview

Full-duplex is the default SPI mode: MOSI and MISO are independent lines, both active on every clock edge, so the master and slave shift a bit out and shift a bit in *simultaneously* — a "transfer" is really two one-directional transfers happening at once, driven by one shared clock.

```mermaid
flowchart LR
    subgraph Master["SPI Master (STM32L452RE)"]
        direction TB
        MTX["TX shift register"]
        MRX["RX shift register"]
    end

    subgraph Slave["SPI Slave"]
        direction TB
        STX["TX shift register"]
        SRX["RX shift register"]
    end

    Master -- "SCK (clock, master-driven)" --> Slave
    Master -- "NSS/CS (select, active low)" --> Slave
    MTX -- "MOSI: master out, slave in" --> SRX
    STX -- "MISO: slave out, master in" --> MRX
```

Each `SCK` edge shifts one bit out of `MTX` onto `MOSI` (captured into `SRX`) **and** one bit out of `STX` onto `MISO` (captured into `MRX`), at the same time. Neither side has to wait for the other to finish — after 8 clock edges, the master has both sent a byte and received a byte, in the time it takes to send one.

---