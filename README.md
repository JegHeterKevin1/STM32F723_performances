# STM32 Performance Testing with RAM Internal and External

This project evaluates the performance of sorting a large array using the `qsort` function on both the internal RAM and external RAM of an STM32 microcontroller. It tests how various cache configurations impact the performance, providing insights into the behavior of memory access times for internal and external memory.

## Project Structure

- **Main Code File**: The main code is implemented in `main.c` and demonstrates the performance tests with sorting.
- **Linker Script**: The project includes a linker script to define memory regions for internal and external RAM usage.

## Features

- Uses `qsort` to sort an array of integers.
- Measures and displays execution times for sorting operations with various cache configurations.
- Compares performance between internal and external RAM usage.

## Getting Started

### Prerequisites

- STM32CubeIDE
- STM32 microcontroller with configured external RAM (starting at address `0x60000000`).
- Proper hardware setup with external RAM correctly connected and initialized.

### Installation

1. Clone this repository to your local machine:
   ```bash
   git clone https://github.com/JegHeterKevin1/STM32F723_performances.git
