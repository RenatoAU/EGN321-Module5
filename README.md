# First Readings: Virtual Sensor Reliability Lab

## Student

Renato Jacinto 
AU_EGN321


## Project Overview

This project studies the reliability of readings from a simulated NTC temperature sensor. The sensor was connected to an Arduino Uno in Velxio. The program collected the raw analog-to-digital converter (ADC) value, converted it to degrees Celsius, assigned a status, and printed the result in CSV format.

The raw dataset contains 103 readings. It includes normal variation, several high and low out-of-range conditions, and one simulated missing reading. No suspicious readings were deleted from the raw file.

## Simulation Platform

- Platform: Velxio
- Board: Arduino Uno
- Sensor: 10K NTC temperature sensor
- Sensor signal pin: Arduino `A0`
- Serial rate: 9600 baud
- Sampling interval: 1 second
- Public project: https://velxio.dev/renato07josue/sensorreadings/

## Circuit Connections

| Sensor pin | Arduino connection |
|---|---|
| VCC | 5V |
| GND | GND |
| OUT | A0 |

The circuit and Serial Monitor are shown in `circuit_and_serial_screenshot.png`.

## Dataset Columns

| Column | Meaning | Unit |
|---|---|---|
| `sample_number` | Sequential reading number | none |
| `raw_adc` | Original Arduino analog reading | ADC count, 0–1023 |
| `temperature_c` | Converted NTC temperature | degrees Celsius (°C) |
| `status` | Software classification | `OK`, `OUT_OF_RANGE`, `MISSING`, or `SENSOR_ERROR` |

## Collection Method

The simulation produced one reading per second. During collection, the virtual temperature control was moved through ordinary room-temperature values and several extreme values. This created normal readings as well as deliberate high and low abnormal conditions. Because the platform normally returns a numeric analog value, sample 85 was intentionally printed as `MISSING` to test unavailable-data handling, as permitted by the assignment.

The complete Serial output was copied into `sensor_readings.csv`. The file contains samples 1 through 103. All original values, including abnormal and missing values, were preserved.

## Summary of Results

| Measurement | Result |
|---|---:|
| Total samples | 103 |
| Numeric samples | 102 |
| Missing samples | 1 |
| Minimum observed temperature | -9.0 °C |
| Maximum observed temperature | 88.1 °C |
| Arithmetic average | 31.01 °C |
| Median / typical value | 23.0 °C |
| Overall numeric range | 97.1 °C |
| Readings marked `OK` | 78 |
| Readings marked `OUT_OF_RANGE` | 24 |

The average is higher than the typical room-temperature value because the dataset intentionally includes several high-temperature spikes. The median of 23.0 °C better represents the center of the normal readings.

## Filtering Rule

The proposed software rule is:

1. Reject readings with a `MISSING` or `SENSOR_ERROR` status from calculations.
2. Mark temperatures below 0 °C or above 50 °C as `OUT_OF_RANGE` and exclude them from the trusted operational dataset.
3. Mark a change greater than 10 °C from the previous valid reading for review.
4. Preserve every original record in the raw CSV file, even when it is rejected or flagged.

## Cost of the Rule

The rule can flag real events. A sensor moved near a heater could correctly report a temperature above 50 °C, and a rapid real temperature change could exceed 10 °C between samples. Rejecting those readings automatically could hide an important event and make the system appear more stable than it actually is. Therefore, the raw data must remain available for investigation.

## Files

- `arduino-temp-sensor-lab.vlx` — corrected Velxio project.
- `temperature_sensor.ino` — Arduino sensor code.
- `sensor_readings.csv` — raw dataset with 103 readings.
- `ANALYSIS.md` — detailed analysis and filtering discussion.
- `circuit_and_serial_screenshot.png` — circuit, code, and Serial Monitor evidence.
- `dataset_statistics_screenshot.png` — spreadsheet statistics evidence.
- `project_link.txt` — public Velxio project URL.
- `SUBMISSION_CHECKLIST.md` — mapping of assignment requirements to files.

## Running the Simulation

1. Open https://velxio.dev/editor/.
2. Use **Open Project** to load `arduino-temp-sensor-lab.vlx`.
3. Start the simulation.
4. Open the Serial Monitor at 9600 baud.
5. Click the NTC sensor while the simulation is running to change its temperature.
6. Copy the Serial output to a CSV file without deleting abnormal readings.

