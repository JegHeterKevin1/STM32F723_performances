# STM32F723 Project: Measuring Power Consumption During Prime Number Generation

This project evaluates the power consumption of the STM32F723 microcontroller during computationally intensive operations. Specifically, it runs a prime number generation task using the **Miller-Rabin primality test** to observe the impact of active CPU usage on power consumption.

## Objective

- Measure the STM32F723's power consumption during different states:
  - Idle state (baseline consumption).
  - Sleep state
  - Stop state


## Key Features

- **Prime Number Generation**: Finds the next prime number greater than a given number using the **Miller-Rabin algorithm**.
- **Power Measurement**: Designed to analyze the microcontroller's power consumption while actively running the prime number generation task.
- **Interactive Test**: Starts a new computation each time a user presses the onboard button.

## How It Works

The project uses the following steps:

1. **Random Number Generation**:
   - Generates random numbers needed by the Miller-Rabin test using the STM32's RNG peripheral.

2. **Prime Number Test (Miller-Rabin)**:
   - Uses the Miller-Rabin algorithm to check if a number is prime.
   - Iteratively generates the next prime number using the `findNextPrime` function.

3. **Power Consumption Measurement**:
   - Runs the prime number generation task repeatedly.
   - Measures power consumption using external tools (e.g., ammeter or power profiler).

4. **Display Results**:
   - Displays the computed prime number on an LCD.
   
   
## RESULTS OF POWER CONSUMPTION 

| Mode                 | Current Consumption |
|----------------------|-------------|------
| Idle                 |  135 mA              |
| Sleep mode   		   |  Y mA                 |
| Stop mode   		   |  Z mA              	 |
| GPIO disabled   	   |  W mA              	 |

