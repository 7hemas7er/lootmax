# Marlin per Lotmaxx SC-10 — mainboard "Alpha M V1.03" (STM32F407VET6)

Firmware Marlin **funzionante** per la SC-10 con main board **Lotmaxx Alpha M V1.03**.

> ⚠️ Questa board NON è un MKS Robin Nano F103. È un **STM32F407VET6**. I config
> F103 nelle altre cartelle di questo repo (`Marlin-2.0.x/`) **non funzionano**
> su questo hardware (girano sull'MCU sbagliato → schermo bianco / nessun avvio).

## Hardware (verificato)
- MCU: **STM32F407VET6**, app a offset **0x08011000**
- Driver: **TMC2208** standalone
- Display: ST7796 FSMC (qui **non** usato — build **headless**, controllo via OctoPrint)
- Host serial: **USART1 (PA9/PA10) @ 115200** via bridge CH340 (USB nativa disabilitata)
- BLTouch: control PB1, sense PB0
- EEPROM: AT24C64 I2C su **SCL=PB6 / SDA=PB7**

## Mappa pin (Alpha M, da sorgente stock + schema)
| Funzione | Pin |
|---|---|
| Heater hotend | **PC8** |
| Heater bed | **PD15** |
| Termistore hotend / bed | **PA3 / PA2** |
| Ventola layer | PD13 |
| X STEP/DIR/EN | PE11 / PE10 / PE12 |
| Y STEP/DIR/EN | PE8 / PE7 / PE9 |
| Z STEP/DIR/EN | PC2 / PC1 / PC3 |
| E0 STEP/DIR/EN | PC14 / PC13 / PC15 |
| Endstop X/Y/Z | PA8 / PC9 / PD11 |
| BLTouch servo/sense | PB1 / PB0 |

## Come si costruisce
Base: fork del costruttore **`sm3dp/firmware_marlin2.0`**, branch **`V1.7_BOARD`**
(env `lotmaxx_V1_7`, `MOTHERBOARD BOARD_LOTMAXX_V1_7`, offset 0x11000).

1. `git clone --branch V1.7_BOARD https://github.com/sm3dp/firmware_marlin2.0`
2. Sovrascrivi con i file in `config/`:
   - `Configuration.h`, `Configuration_adv.h` → `Marlin/`
   - `pins_LOTMAXX_V1_7.h` → `Marlin/src/pins/stm32f4/`
3. `pio run -e lotmaxx_V1_7` → output `firmware.bin`.
4. Rinomina in **`SM3DP.bin`** (plain, niente encryption).

## Come si flasha
Copia `SM3DP.bin` nella **root** della microSD (FAT32) → inserisci a stampante
spenta → accendi. Il bootloader lo scrive e lo rinomina `OLDSM3DP.bin`.
Recupero: ricopia il `SM3DP.bin` originale (firmware Lotmaxx di serie) e riavvia.

## Config principali rispetto al fork
- **Headless** (DGUS/LOTMAXX_MULTI_LANGUAGE/LASER off), `EMERGENCY_PARSER` on
- `SERIAL_PORT 1` @ 115200, `EXTRUDERS 1`
- Pin Alpha M corretti (vedi `pins_LOTMAXX_V1_7.h`) — heater hotend **PC8**, bed **PD15**
- EEPROM I2C su **PB6/PB7**
- `INVERT_Z_DIR false`, `INVERT_E0_DIR true`, `Z_MIN_ENDSTOP_INVERTING true`
- `E steps 134.3`, area piatto 245×245
- Leveling **BILINEAR**, `USE_PROBE_FOR_Z_HOMING` + `Z_SAFE_HOMING`
- `NOZZLE_TO_PROBE_OFFSET {60,0,-3}` (sonda 60mm a destra dell'ugello)

## Da fare dopo il flash (una tantum, salvati con M500)
- `M502` `M500` (init EEPROM)
- PID hotend: `M303 E0 S200 C8 U1` → `M500` (bed in bang-bang)
- Z-offset primo layer: paper test → `M851 Z…` → `M500`
- `G29` → `M500`; nello start gcode slicer: `G28` poi `G29` (o `M420 S1`)

## TODO / note
- Doppio estrusore (E1: STEP PE5 / DIR PE4 / EN PE6) non ancora configurato
- Display ST7796 FSMC non portato (build headless)
</content>
