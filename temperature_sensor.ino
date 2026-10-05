// First Readings: Virtual Sensor Reliability Lab
// Reads an NTC temperature sensor on A0 and classifies each reading.

#include <math.h>

const int SENSOR_PIN = A0;
const float BETA = 3950.0;

int sampleCounter = 1;

void setup() {
  Serial.begin(9600);
  Serial.println("sample_number,raw_adc,temperature_c,status");
}

void loop() {
  // The assignment permits a simulated failure when the platform does not
  // naturally provide missing data. Sample 85 represents that condition.
  if (sampleCounter == 85) {
    Serial.print(sampleCounter);
    Serial.println(",,,MISSING");

    sampleCounter++;
    delay(1000);
    return;
  }

  int rawValue = analogRead(SENSOR_PIN);

  // Values at the ADC endpoints cannot be safely converted by the NTC formula.
  if (rawValue <= 0 || rawValue >= 1023) {
    Serial.print(sampleCounter);
    Serial.print(",");
    Serial.print(rawValue);
    Serial.println(",,SENSOR_ERROR");

    sampleCounter++;
    delay(1000);
    return;
  }

  // Convert the 10K NTC reading to degrees Celsius using Beta = 3950.
  float temperatureC =
      1.0 /
      (log(1.0 / (1023.0 / rawValue - 1.0)) / BETA
      + 1.0 / 298.15)
      - 273.15;

  String status;
  if (temperatureC >= 0.0 && temperatureC <= 50.0) {
    status = "OK";
  } else {
    status = "OUT_OF_RANGE";
  }

  Serial.print(sampleCounter);
  Serial.print(",");
  Serial.print(rawValue);
  Serial.print(",");
  Serial.print(temperatureC, 1);
  Serial.print(",");
  Serial.println(status);

  sampleCounter++;
  delay(1000);
}
