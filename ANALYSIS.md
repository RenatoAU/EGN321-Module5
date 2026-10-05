# Virtual Sensor Reliability Analysis

## Sensor and Measurement

The project uses a simulated 10K NTC temperature sensor connected to analog input `A0` on an Arduino Uno. Each sample contains the original ADC reading and the converted temperature in degrees Celsius. The conversion uses a beta coefficient of 3950.

## Simulation Platform

The circuit was created and tested in Velxio. The public project is available at:

https://velxio.dev/renato07josue/sensorreadings/

## Collection Method

The Arduino produced one sample per second and printed the data as CSV. The temperature control was changed during the simulation to produce normal variation and deliberately abnormal conditions. The final dataset contains 103 samples. Of these, 102 contain numeric temperatures and sample 85 represents a simulated missing reading.

## Observed Range

| Statistic | Value |
|---|---:|
| Minimum observed temperature | -9.0 °C |
| Maximum observed temperature | 88.1 °C |
| Arithmetic average | 31.01 °C |
| Median / typical temperature | 23.0 °C |
| Overall range | 97.1 °C |

The arithmetic average includes the deliberately abnormal readings. High spikes raise the average above the value seen during stable operation. The median of 23.0 °C is therefore a better description of the typical reading.

## Normal Variation

There are 78 readings marked `OK`. The full accepted span is 4.0–45.0 °C, which is a 41.0 °C range. Some of those values occurred while the virtual control was returning from an intentionally extreme condition. During the stable room-temperature portions of the run, the readings were mainly 23.0–29.0 °C, a variation of about 6.0 °C.

## Suspicious or Invalid Readings

| Samples | Observed condition | Reason for investigation |
|---|---|---|
| 37–42 | 70.0–79.2 °C | Sudden high-temperature spike after ordinary readings. All values exceed the assumed 50 °C operating maximum. |
| 60–65 | -9.0 to -1.0 °C | Sudden low-temperature condition below the supported 0 °C minimum. |
| 74–79 | 85.1–88.1 °C | Extreme high plateau that is far from the nearby normal readings. |
| 85 | No ADC or temperature value | The software received no usable value; the sample is explicitly marked `MISSING`. |
| 98–103 | 53.1–87.3 °C | A rapid late rise that crosses the supported maximum and remains abnormal. |

Several large one-step changes also deserve review. Examples include the change from 45.0 °C at sample 36 to 79.2 °C at sample 37, and the change from 70.0 °C at sample 42 to 4.0 °C at sample 43. These changes may be deliberate simulation events, real environmental events, or unreliable sensor input. Context is needed before deciding that they are errors.

## Proposed Rejection and Filtering Rule

The application should use the following rule:

```text
If the value is missing or the ADC conversion fails:
    reject it from calculations and record the failure status.
Else if temperature < 0 °C or temperature > 50 °C:
    mark it OUT_OF_RANGE and exclude it from trusted operational results.
Else if the absolute change from the previous valid reading is greater than 10 °C:
    flag it for review.
Else:
    accept it as OK.
Always preserve the original record in the raw dataset.
```

This rule is simple enough to implement in Python or Arduino code. Range checks catch unsupported extremes, the missing-value check prevents unavailable data from entering calculations, and the change check highlights sudden spikes that still fall within the supported range.

## Cost of the Rule

The rule may remove valid information. A real heat source could push the sensor above 50 °C, and moving the sensor between environments could cause a legitimate change greater than 10 °C in one second. Automatically excluding those readings could hide an important safety event and make the data appear more stable. For that reason, the software should exclude flagged readings only from trusted calculations while keeping them in the raw dataset for later review.

## Conclusion

The sensor was stable near 23 °C for substantial parts of the run, but the complete dataset also contains high spikes, low out-of-range values, rapid transitions, and one missing sample. Software should not trust every numeric value automatically. Clear statuses, range checks, change detection, and preservation of the raw readings provide a reasonable first reliability policy.

