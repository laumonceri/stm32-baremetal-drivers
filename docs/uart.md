# UART Driver

Bare-metal USART driver for the STM32L452RE (Cortex-M4). Polling TX, interrupt-driven RX via a ring buffer, no HAL/CMSIS. Supports USART1 and USART2.

Source: [`drivers/uart/`](../drivers/uart/) · Example: [`examples/uart-cli/`](../examples/uart-cli/)

---

## 1. Overview

| Feature | Status |
|---|---|
| TX | Polling only (`UART_WriteByteRaw` blocks on `TXE`) |
| RX | Interrupt-driven, fed into a per-handle ring buffer |
| Instances | USART1, USART2 (registry has reserved slots for USART3/LPUART1, not wired — see [§6](#6-known-limitations)) |
| Word length | 7 / 8 / 9 bits |
| Parity | None / Even / Odd |
| Stop bits | 1 / 0.5 / 2 / 1.5 |
| Clock source (per instance) | PCLK / SYSCLK / HSI16 / LSE, via `RCC_CCIPR` |
| Multi-instance safety | IRQ→handle registry rejects claiming an IRQ already owned by a different handle |

---

## 2. Where it sits

```mermaid
flowchart TB
    example["examples/uart-cli/main.c"] --> uartdrv["drivers/uart/uart_driver.c"]
    uartdrv --> ringbuf["drivers/uart/ring_buffer.c"]
    uartdrv --> platform["platform/ rcc.c · nvic.c"]
    uartdrv --> gpio["drivers/gpio/gpio.c"]
    platform --> hw["hw/ stm32_uart_hw.h · stm32_rcc_hw.h · stm32_nvic_hw.h"]
    gpio --> hw
    hw --> silicon["STM32L452RE registers"]
```

`uart_driver.c` never touches a register that isn't declared in `hw/`, and never makes clock/GPIO/interrupt decisions that belong to `platform/`/`drivers/gpio/` — it composes them.

---

## 3. Clock architecture

This is the part that's easy to get wrong (and did, during development): **the CPU's system clock and a UART's kernel clock are two independent muxes that happen to both offer "HSI16" as an option.**

```mermaid
flowchart LR
    subgraph OSC["Oscillators — RCC_CR"]
        HSI16osc["HSI16, 16 MHz fixed"]
        MSIosc["MSI, ~4 MHz default"]
        LSEosc["LSE, 32.768 kHz"]
    end

    subgraph SYS["System clock — RCC_CFGR.SW\nset by RCC_SetSysclk()"]
        HSI16osc --> SW{{SW mux}}
        MSIosc --> SW
        SW --> SYSCLK["SYSCLK\n(CPU clock)"]
        SYSCLK --> PCLK["PCLK\n(APB bus clock)"]
    end

    subgraph KCLK["USART kernel clock — RCC_CCIPR\nset by RCC_CCIPR_SelectClock()"]
        PCLK --> CCIPR{{USARTxSEL mux}}
        SYSCLK --> CCIPR
        HSI16osc --> CCIPR
        LSEosc --> CCIPR
        CCIPR --> USARTCLK["USART kernel clock\n-> BRR baud divider"]
    end
```

**Why bypass PCLK?** Selecting `HSI16` or `LSE` directly for a UART's kernel clock means its baud rate stays fixed no matter what the CPU is doing with `SYSCLK` (e.g. power-mode frequency scaling), and `LSE` specifically keeps the UART alive in STOP mode for wakeup-on-RX. `BOARD_UART2` uses `RCC_CLK_HSI16` for exactly the first reason — its baud rate is independent of whatever `RCC_SetSysclk()` picks for the core.

`RCC_GetClockSourceFreq()` resolves this to a real Hz value for the baud-rate math — but only for the two hardware-fixed sources:

| `RCC_ClockSource` | Frequency | Why |
|---|---|---|
| `RCC_CLK_HSI16` | 16,000,000 Hz | Fixed internal RC oscillator |
| `RCC_CLK_LSE` | 32,768 Hz | Standard watch-crystal assumption |
| `RCC_CLK_PCLK` | **0** | Runtime-variable (SYSCLK ÷ APB prescaler), not tracked |
| `RCC_CLK_SYSCLK` | **0** | Runtime-variable, not tracked |

Selecting `PCLK`/`SYSCLK` today will divide-by-zero in `UART_set_baudrate` — don't, until that tracking exists.

---

## 4. Word length & parity — the M1/M0/PCE gotcha

The STM32 `M` bits set the **total frame size**, not "data bits plus a bonus parity bit." Enabling parity on an 8-bit word steals the MSB for parity instead of adding a 9th bit — this caused real garbled output during development.

| M1 | M0 | PCE | Actual frame | Driver config |
|:--:|:--:|:--:|---|---|
| 0 | 0 | 0 | 8 data, no parity | `UART_WORD_LENGTH_8B` + `UART_PARITY_NONE` |
| 0 | 0 | 1 | **7 data** + 1 parity | `UART_WORD_LENGTH_8B` + parity enabled — only 7 real data bits reach you |
| 0 | 1 | 0 | 9 data, no parity | `UART_WORD_LENGTH_9B` + `UART_PARITY_NONE` |
| 0 | 1 | 1 | **8 data** + 1 parity | `UART_WORD_LENGTH_9B` + `UART_PARITY_EVEN`/`ODD` — the correct pairing for real 8-bit-plus-parity |
| 1 | 0 | 0 | 7 data, no parity | `UART_WORD_LENGTH_7B` + `UART_PARITY_NONE` |
| 1 | 0 | 1 | 6 data + 1 parity | `UART_WORD_LENGTH_7B` + parity enabled |

If you want 8 real data bits *with* parity, you must set `UART_WORD_LENGTH_9B`, not `_8B`. The receiving terminal must also be configured to match (e.g. 8-O-1), or it'll misinterpret the parity bit as data.

---

## 5. Interrupt-driven RX

### 5.1 Registry — how an ISR finds its handle

There's no per-instance hardcoded `USART2_IRQHandler` reaching into a global by name. `UART_Init` registers the handle in a small array indexed by IRQn; the ISR looks it up generically.

```mermaid
sequenceDiagram
    participant HW as USART2 peripheral
    participant NVIC
    participant ISR as USART2_IRQHandler
    participant Dispatch as UART_DispatchIRQ
    participant Reg as uart_handle_registry[]
    participant Core as UART_IRQHandler
    participant RB as RingBuffer (h->rx)
    participant Main as main() loop

    HW->>NVIC: RXNE asserted (byte received)
    NVIC->>ISR: vector table dispatch
    ISR->>Dispatch: UART_DispatchIRQ(USART2_IRQn)
    Dispatch->>Reg: lookup by IRQn
    Reg-->>Dispatch: uart_handle_t* h
    Dispatch->>Core: UART_IRQHandler(h)
    Core->>HW: read UART_RDR (clears RXNE)
    Core->>RB: RingBuffer_Push(byte)
    Note over Main: asynchronous — polls whenever it gets there
    Main->>RB: UART_DataAvailable_RingBuffer / UART_ReadChar_RingBuffer
```

If the ring buffer is full when a byte arrives, `UART_IRQHandler` reads and discards it (`RDR` must be read regardless, or `RXNE` never clears and the ISR fires forever).

### 5.2 Instance lifecycle

```mermaid
stateDiagram-v2
    [*] --> Uninitialized
    Uninitialized --> Initialized: UART_Init(h, dev)
    Initialized --> Initialized: UART_Init(h, dev) again\n(same handle — idempotent)
    Initialized --> InterruptEnabled: UART_EnableInterrupt(h, priority)
    InterruptEnabled --> Initialized: UART_DisableInterrupt(h)
    Initialized --> Uninitialized: UART_DeInit(h)
    InterruptEnabled --> Uninitialized: UART_DeInit(h)

    note right of Uninitialized
        UART_Init(h2, dev) with a DIFFERENT
        handle on an already-claimed IRQ
        returns UART_ERROR_INSTANCE_IN_USE
        instead of transitioning — the first
        handle must be UART_DeInit'd first.
    end note
```

`UART_DeInit` disables the interrupt, disables the peripheral (`UE`), clears the handle's registry slot (only if it still points at that exact handle), and nulls `h->dev` so any further use of a torn-down handle fails validation instead of touching dead hardware state.

**Handle lifetime contract:** a handle passed to `UART_Init` must have static or global storage duration. The registry keeps its address and the ISR dereferences it at any time until `UART_DeInit` — a stack-local handle becomes a dangling pointer the moment its function returns.

---

## 6. Ring buffer

Generic single-producer/single-consumer byte ring buffer ([`ring_buffer.h`](../drivers/uart/include/ring_buffer.h)), used for `h->rx` (fed by the ISR) and reserved on `h->tx` (unused — TX is polling-only today).

```
                 tail                    head
                  ↓                       ↓
  buffer:  [ B ][ C ][ D ][ . ][ . ][ . ][ . ][ A ]
             ↑                              (next Push lands here,
        (next Pop reads this)                then head advances)

  Push: buffer[head] = byte; head = (head + 1) % RING_BUFFER_SIZE   (no-op if full)
  Pop:  byte = buffer[tail]; tail = (tail + 1) % RING_BUFFER_SIZE   (fails if empty)

  Empty  when head == tail
  Full   when (head + 1) % RING_BUFFER_SIZE == tail   (one slot always kept empty)
```

`head` is only ever written by the producer (the ISR), `tail` only by the consumer (main-loop code) — that split is what makes it safe without disabling interrupts around every access.

---

## 7. API reference

### Lifecycle
| Function | Purpose |
|---|---|
| `UART_Init(h, dev)` | Clock + GPIO + peripheral bring-up, registers `h` for its IRQ |
| `UART_DeInit(h)` | Disable interrupt + peripheral, unregister, invalidate `h` |

### Low-level (blocking register access)
| Function | Purpose |
|---|---|
| `UART_WriteByteRaw(h, c)` | Block on `TXE`, write one byte |
| `UART_ReadByteRaw(h, c)` | Block on `RXNE`, read one byte |

### Polling transfer layer
| Function | Purpose |
|---|---|
| `UART_PollWriteString(h, s)` | Write a NUL-terminated string, blocking |
| `UART_PollReadChar(h, c)` | Alias for `UART_ReadByteRaw` |
| `UART_ReadCharEcho(h, c)` | Blocking read + echo the byte back |
| `UART_PollReadString(h, buf, max)` | Blocking read-with-echo until `\r`/`\n` |

### Interrupt-driven layer
| Function | Purpose |
|---|---|
| `UART_EnableInterrupt(h, priority)` | Enable `RXNEIE` + NVIC for `h`'s IRQ |
| `UART_DisableInterrupt(h)` | Disable both |
| `UART_IRQHandler(h)` | Generic ISR body — called via the registry, not directly |

### Non-blocking ring-buffer read (fed by the ISR)
| Function | Purpose |
|---|---|
| `UART_DataAvailable_RingBuffer(h, &n)` | Bytes currently buffered |
| `UART_ReadChar_RingBuffer(h, &c)` | Pop one byte, `'\0'` if none available |
| `UART_ReadString_RingBuffer(h, buf, max)` | Drain up to a terminator — see [§6 caveat](#6-known-limitations) |

---

## 8. Example

From [`examples/uart-cli/main.c`](../examples/uart-cli/main.c):

```c
static uart_handle_t uart2_handle;

static const uart_device_t BOARD_UART2 = {
    .clk = {.bus = RCC_UART_BUS_APB1,
            .enr = {.apb1 = RCC_APB1ENR1_USART2},
            .clk_sel = RCC_SEL_USART2,
            .clk_src = RCC_CLK_HSI16},
    .uart = {
        .base = USART2_BASE,
        .irq = USART2_IRQn,
        .word_length = UART_WORD_LENGTH_8B,
        .parity = UART_PARITY_NONE,
        .stop_bits = UART_STOP_1,
        .baudrate = 115200,
        .tx = {.port = GPIO_PORT_A, .pin = PIN_2, .mode = GPIO_MODE_AF, .af = AF_7},
        .rx = {.port = GPIO_PORT_A, .pin = PIN_3, .mode = GPIO_MODE_AF, .af = AF_7}}};

int main(void) {
    RCC_SetSysclk(RCC_SYSCLK_HSI16);           // board clock bring-up, once

    UART_Init(&uart2_handle, &BOARD_UART2);
    UART_EnableInterrupt(&uart2_handle, IRQ_PRIO_1);
    UART_PollWriteString(&uart2_handle, "UART READY\r\n");

    while (1) {
        int available = 0;
        UART_DataAvailable_RingBuffer(&uart2_handle, &available);

        while (available-- > 0) {
            char c;
            UART_ReadChar_RingBuffer(&uart2_handle, &c);
            UART_WriteByteRaw(&uart2_handle, c);   // echo
        }
    }
}
```

---

## 9. Known limitations

- **USART3/LPUART1 are registry slots, not real support.** `UART_RegistryIndex` and the vector table both reserve space for them, but `UART_validate_device` still only whitelists `USART1_BASE`/`USART2_BASE`, and `USART3_BASE` isn't even defined in `stm32_uart_hw.h` yet. `UART_Init` will reject either today.
- **`PCLK`/`SYSCLK` as a UART clock source is a footgun.** `RCC_GetClockSourceFreq` returns `0` for both (see [§3](#3-clock-architecture)), which divides-by-zero the baud-rate calculation. Only `HSI16`/`LSE` are safe to select until real bus-frequency tracking exists.
- **Five declared API functions have no implementation:** `UART_PollWriteChar`, `UART_WriteChar`, `UART_WriteString`, `UART_ReadChar`, `UART_ReadString` are all declared in `uart_driver.h` under "Compatibility wrappers for the simple API" but never defined in `uart_driver.c`. Calling any of them is a link error.
- **`UART_ReadString_RingBuffer` can't accumulate across calls.** It restarts its index at 0 every call, so if a full line hasn't arrived in the ring buffer yet, it terminates early instead of waiting — it only works correctly if you know the whole line is already buffered. The `uart-cli` example works around this by draining char-by-char with its own persistent index instead of calling this function.
- **TX has no interrupt path.** `h->tx` (the `RingBuffer`) exists on the handle but nothing pushes to or pops from it — all writes are blocking (`UART_WriteByteRaw`/`UART_PollWriteString`).
- **No overrun/parity/framing error handling.** `UART_ISR` only exposes `RXNE`/`TC`/`TXE` — `ORE`/`PE`/`FE` aren't defined or checked, so a receiver overrun (ISR falling behind) or a parity mismatch is silently ignored rather than recovered from.
