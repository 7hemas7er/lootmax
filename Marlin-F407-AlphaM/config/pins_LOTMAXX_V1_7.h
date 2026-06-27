/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Lotmaxx "Alpha M V1.03" mainboard (STM32F407VET6).
 *
 * PIN MAP corretta, estratta dal sorgente stock del costruttore
 * (sm3dp/firmware "shark v1": Hardware/KimHD/App_*.c, SM/pins.h) e
 * CONFERMATA sullo schema SCH/alpha01_V103.pdf.
 *
 * NB: i valori originali di questo file (board "Beta_M V1.7") erano TUTTI
 * sbagliati per la Alpha M (es. heater su PE10/PE11 = in realta' STEP/DIR di X;
 * termistori su PA7/PC4 = inesistenti). Non riusarli.
 *
 * SAFETY: heater hotend=PD15, heater bed=PC8 sono ad alta confidenza ma
 * vanno confermati col multimetro (continuita' al gate MOSFET) prima di
 * comandare riscaldamento.
 */
#pragma once

#define ALLOW_STM32DUINO
#include "env_validate.h"

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "Lotmaxx Alpha M supports up to 2 hotends / E-steppers."
#endif

#define BOARD_INFO_NAME "Lotmaxx Alpha M V1.03"

// Avoid conflict with TIMER_TONE
#define STEP_TIMER 10

//
// BLTOUCH / Z-Probe  (connettore J422: Zmin / Gnd / Data / 5V / Gnd)
//
#define SERVO0_PIN                          PB1   // BLTOUCH control "Data" (TIM8_CH3)
#define Z_MIN_PROBE_PIN                     PB0   // BLTOUCH Zmin sense (input)

//
// EEPROM (I2C 24Cxx)
//
#define I2C_EEPROM
#define SOFT_I2C_EEPROM
#define MARLIN_EEPROM_SIZE                  0x2000  // 8KB (AT24C64)
#define I2C_SDA_PIN                         PB7   // Alpha M EEPROM (stock Driver/iic.h: SDA=PB7)
#define I2C_SCL_PIN                         PB6   //                              SCL=PB6

//
// Limit Switches (Alpha M: XST=PA8, YST=PC9, ZST=PD11)
//
#define X_STOP_PIN                          PA8
#define Y_STOP_PIN                          PC9
#define Z_STOP_PIN                          PD11

//
// Steppers (Alpha M)
//
#define X_STEP_PIN                          PE11
#define X_DIR_PIN                           PE10
#define X_ENABLE_PIN                        PE12

#define Y_STEP_PIN                          PE8
#define Y_DIR_PIN                           PE7
#define Y_ENABLE_PIN                        PE9

#define Z_STEP_PIN                          PC2
#define Z_DIR_PIN                           PC1
#define Z_ENABLE_PIN                        PC3

#define E0_STEP_PIN                         PC14
#define E0_DIR_PIN                          PC13
#define E0_ENABLE_PIN                       PC15

// E1 (2nd extruder, Alpha M PE5/PE4/PE6) - abilitare solo se dual-extruder
//#define E1_STEP_PIN                       PE5
//#define E1_DIR_PIN                        PE4
//#define E1_ENABLE_PIN                     PE6

//
// Temperature Sensors (ADC1)
//
#define TEMP_0_PIN                          PA3   // E1-THERM (hotend)  ADC1_IN3
#define TEMP_BED_PIN                        PA2   // B1-THERM (bed)     ADC1_IN2
//#define TEMP_1_PIN                        PA1   // E2-THERM (2o hotend)

//
// Heaters / Fans (Alpha M)
//   Verificato a runtime (M104 S40): comandando PD15 si scalda il PIATTO ->
//   heater hotend/bed sono INVERTITI rispetto allo schema; uso i valori
//   del pins.h stock, confermati dal test:
//     PC8  = HOTEND  (salita rapida, MOSFET T41)
//     PD15 = HOTBED  (salita lenta = grande massa, MOSFET T42)
//
#define HEATER_0_PIN                        PC8   // HOTEND (verificato a runtime)
#define HEATER_BED_PIN                      PD15  // HOTBED (verificato a runtime)
#define FAN_PIN                             PD13  // part-cooling fan (MOSFET T44)
//#define FAN1_PIN                          PD12  // hotend heatsink fan (auto)

#define LED_PIN                             PB2

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION ONBOARD
#endif

//
// Onboard SD card (SPI3) - non necessario per il controllo via OctoPrint
//
#define ENABLE_SPI3
#define SD_SS_PIN                       -1
#define SDSS                            PD0
#define SD_SCK_PIN                      PC10
#define SD_MISO_PIN                     PC11
#define SD_MOSI_PIN                     PC12
#define SD_DETECT_PIN                   PA15

//
// SPI FLASH (W25Q / MX25L6406E, 8MB)
//
#define SPI_FLASH
#define HAS_SPI_FLASH                          1
#define SPI_DEVICE                             2
#define SPI_FLASH_SIZE                 0x1000000
#if ENABLED(SPI_FLASH)
  #define SPI_FLASH_CS_PIN                  PB12
  #define SPI_FLASH_MOSI_PIN                PB15
  #define SPI_FLASH_MISO_PIN                PB14
  #define SPI_FLASH_SCK_PIN                 PB13
#endif

// NB: POWER_LOSS (stock PC9) e PS_ON (stock PC8) del fork erano in conflitto
// con Y_STOP (PC9) e HEATER_BED (PC8) della Alpha M -> rimossi.
// LASER (SPINDLE_LASER_*) rimosso per il build headless FDM iniziale.
