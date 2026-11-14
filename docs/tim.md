# TIMERS

A timer is a 16-bit counter (32-bit on TIM2) that reloads automatically and is controlled by a programmable prescaler.

By using the timer prescaler and the RCC clock controller prescalers, pulse lengths and waveform periods can be set anywhere from a few microseconds to several milliseconds.

Some uses include:
- Measuring the pulse lengths of input signals
- Generating output waveforms, like pulse width modulation (PWM) and others

Although the advanced-control and general-purpose timers are independent of each other (they do not share resources), they can be synchronized together.

## General-purpose timers
A stopwatch with switches. It counts up and, at the values you set, flips a pin on or off.

Usually used for blinking an LED, positioning a servo, setting a buzzer's tone, or measuring how long a pulse lasts.

On this board: TIM2, TIM3, TIM15 and TIM16.

## Advanced-control timers
The same stopwatch as a general-purpose timer, plus safety features for driving power electronics like motors, H-bridges and power converters.

On this board: TIM1 only.

### Features only advanced-control timers have

#### Paired outputs (CHx and CHxN)
Each channel can drive two pins that are always opposites: when one is high, the other is low.

#### Dead-time
In a half-bridge, the two switches must never be on at the same moment, or current shorts straight through and burns the transistors. Dead-time adds a short gap where both are off before the other turns on.

```
CH1   ‾‾‾‾‾‾\______________/‾‾‾‾‾‾
CH1N  _________/‾‾‾‾‾‾‾\_________
             ↑↑         ↑↑
         dead-time   dead-time
```

#### Break inputs (emergency stop)
If a fault pin goes active, for example from an overcurrent sensor, the hardware turns every output off at once, with no software involved.

That is why TIM1 has the MOE "master switch": the outputs stay off until MOE is turned on.

---

## General-purpose vs advanced-control timers
|                      | General-purpose  | Advanced-control  |
| -------------------  | ---------------- | ----------------- |
| Count, compare, PWM  | Yes              | Yes               |
| Input capture        | Yes              | Yes               |
| Paired outputs       | No               | Yes               |
| Dead-time            | No               | Yes               |
| Emergency stop       | No               | Yes               |
| Needs MOE to output  | No               | Yes               |
