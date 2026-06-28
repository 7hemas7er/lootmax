# CLAUDE.md — Lotmaxx SC-10 firmware config

> ⚠️ **READ THIS BEFORE TOUCHING ANY CONFIG.** The root `README.md` is the upstream
> README and is **WRONG** for this machine (it describes an MKS Robin Nano **F103** /
> `Lotmaxx.bin` / env `mks_robin_nano35` — none of that applies here).

## The machine
Lotmaxx SC-10, mainboard **"Lotmaxx Alpha M V1.03" = STM32F407VET6** (NOT F103, NOT
MKS Robin Nano). TMC2208 standalone drivers. **Headless** (ST7796 FSMC display not
ported) — controlled over serial / OctoPrint at **115200**.

### ⚠️ Board is V1.03, NOT V1.7 — pins were hand-resolved, do not "fix" them
The sm3dp fork's `BOARD_LOTMAXX_V1_7` and its stock `pins_LOTMAXX_V1_7.h` target a
**different board (Beta M V1.7)** whose heater/thermistor/endstop pins are WRONG for
this V1.03 hardware. A previous session studied the **stock Lotmaxx firmware source**
and **rewrote every pin for the V1.03 board**; that corrected map lives in
`Marlin-F407-AlphaM/config/pins_LOTMAXX_V1_7.h` (the filename/`BOARD_LOTMAXX_V1_7` id is
kept only as F407 build scaffolding — the pin *values* inside are V1.03 ground-truth,
runtime-verified). **Treat that pins file as authoritative. Do NOT replace it with the
fork's, and do NOT "correct" pins from the V1.7 schematic.**

## Which tree to edit — #1 GOTCHA
This repo holds several config-overlay trees. Only ONE is live:

- ✅ **`Marlin-F407-AlphaM/`** — the LIVE F407 build. **Edit firmware HERE.** Contains
  `config/{Configuration.h, Configuration_adv.h, pins_LOTMAXX_V1_7.h}`, a built
  `SM3DP.bin`, and its own accurate README. Its config matches the running machine
  (verified via `M503`: `NOZZLE_TO_PROBE_OFFSET {60,0,-3}`, `AUTO_BED_LEVELING_BILINEAR`
  grid 3×3, E-steps 134.3).
- ❌ **`Marlin-2.0.x/`** (2.0.7.2 / 2.0.8 / 2.0.9.2) — DEAD. Old MKS Robin Nano **F103**
  overlays. Wrong MCU → an F103 binary hardfaults on the F407 (white screen / no boot).
  Their pins (e.g. `SERVO0 PB2`, `Z_MIN PA11`) are NOT real. Never debug the machine
  against these trees.

## Real Alpha M pins (`Marlin-F407-AlphaM/config/pins_LOTMAXX_V1_7.h`)
| Function | Pin |
|---|---|
| BLTouch control / sense | **PB1 / PB0** |
| Endstop X / Y / Z | PA8 / PC9 / **PD11** (PD11 = real mechanical Z switch) |
| Heater hotend / bed | PC8 / PD15 |
| Thermistor hotend / bed | PA3 / PA2 |
| EEPROM I2C SCL / SDA | PB6 / PB7 |
| Host serial | USART1 (PA9/PA10) @ 115200 via CH340 |

## Build & flash (see `Marlin-F407-AlphaM/README.md`)
1. `git clone --branch V1.7_BOARD https://github.com/sm3dp/firmware_marlin2.0`
2. Overlay `Marlin-F407-AlphaM/config/`: `Configuration.h` + `Configuration_adv.h` →
   `Marlin/`; `pins_LOTMAXX_V1_7.h` → `Marlin/src/pins/stm32f4/` (this overlay file is
   the **V1.03-corrected** pins map — it overrides the fork's stock V1.7 pins; see the
   board-version warning above)
3. `pio run -e lotmaxx_V1_7` (`MOTHERBOARD BOARD_LOTMAXX_V1_7`, app offset `0x11000`)
4. Rename `firmware.bin` → **`SM3DP.bin`** (plain, NO encryption). Copy to microSD root
   (FAT32), power off → insert → power on. Bootloader flashes it and renames it
   `OLDSM3DP.bin`. Recovery: copy the stock `SM3DP.bin` back.

## Current state / open issue (2026-06)
Bring-up done: motion, endstops, heaters, thermistors verified. **Homing is flaky** —
the build has `USE_PROBE_FOR_Z_HOMING` ON and the clone BLTouch is unreliable in the
fast/slow double-touch homing sequence → intermittent `Homing Failed` kill (it detects
the 1st trigger, fails the 2nd touch). Verified NOT EMI / NOT motor dir / NOT dead probe.
- **Fix in test:** `BLTOUCH_FORCE_SW_MODE` + `BLTOUCH_DELAY 750` + `MULTIPLE_PROBING 2`.
- **Proven fallback:** disable `USE_PROBE_FOR_Z_HOMING` → Z homes on the **PD11**
  mechanical switch (reliable), BLTouch for G29 only. (Z switch hardware confirmed present.)

## Conventions
- Keep firmware changes in `Marlin-F407-AlphaM/config/` only.
- After leveling-type/grid/offset changes: `M502` → `M500` (re-init EEPROM). Pure
  compile-time tweaks (BLTouch/probing) do NOT need M502 — EEPROM (incl. calibrated
  `M851 Z`) persists.
- Deep history/decisions live in the project memory (`MEMORY.md`): `lotmaxx-sc10-flashing`,
  `lotmaxx-sc10-firmware-source-mismatch` (two-trees), `lotmaxx-sc10-homing-bltouch-alarm`,
  `lotmaxx-sc10-factory-params`.
