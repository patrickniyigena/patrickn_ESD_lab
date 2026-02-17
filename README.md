# patrickn_ESD_lab
# Lab 2: Device Drivers and MMIO
**Course:** 04-633 Embedded Systems Development (CMU Africa)  
**Date:** February 17, 2026  

## Team Members
* **Patrick Niyigena** (ID: patrickn)


## Project Overview
This project implements low-level device drivers for a 4x4 Matrix Keypad and a 16x2 LCD (I2C) on the STM32 Nucleo-F103RB board. The system integrates these peripherals to allow a user to type characters on the keypad and see them displayed on the LCD screen in real-time.

## Hardware Configuration

### 1. Keypad (4x4 Matrix)
* **Logic:** Rows are configured as Outputs; Columns are Inputs with **Internal Pull-Down** resistors.
* **Pin Mapping:**
  * **Row 1:** PA0
  * **Row 2:** PA1
  * **Row 3:** PA4 (Board Label: A2)
  * **Row 4:** PB0 (Board Label: A3)
  * **Column 1:** PC1 (Board Label: A4)
  * **Column 2:** PC0 (Board Label: A5)
  * **Column 3:** PA8 (Board Label: D7)
  * **Column 4:** PA9 (Board Label: D8)

### 2. LCD Display (I2C Interface)
* **Controller:** HD44780 via PCF8574 I2C Expander.
* **I2C Instance:** I2C1
* **Pins:**
  * **SCL:** PB8  
  * **SDA:** PB9  
* **Address:** 0x27
  
## Features Implemented
1.  **Keypad Scanning:** Custom driver scans rows/columns to detect button presses.
2.  **Debouncing:** Software delay (`HAL_Delay`) prevents multiple characters from a single press.
3.  **I2C LCD Driver:** Custom functions to initialize, clear, and print to the LCD.
4.  **Cursor Management:**
    * **Typing:** Characters appear sequentially.
    * **Overflow Protection:** Prevents typing past the 16th character on a line.
    * **New Line (`#`):** Moves the cursor to the start of the second line.
    * **Clear Screen (`*`):** Clears the display and resets the cursor to (0,0).
5.  **UART Debugging:** Key
