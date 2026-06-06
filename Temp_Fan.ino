#include <math.h>


const int thermistorPin = A0;
const int enablePin = 3;
const int in1 = 7;
const int in2 = 6;

bool fanOn = false;


const float seriesResistor = 10000.0; // Series resistor
const float nominalResistance = 1000.0; // thermistor resistance @ 25 degrees
const float nominalTemperature = 25.0; 
const float betaCoefficient = 3950.0; // Beta value according to the Seller



void setup() {
  Serial.begin(9600);

pinMode(enablePin, OUTPUT);
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);

digitalWrite(in1, HIGH);
digitalWrite(in2, LOW);
  


}

void loop() {
  int reading = analogRead(thermistorPin);

  if (reading <= 0 || reading >= 1023) {
  Serial.println("Bad thermistor reading");
  analogWrite(enablePin, 80);
  delay(500);
  return;
}
 
float resistance = seriesResistor / (1023.0 / reading - 1.0); // Rearranging the voltage divider

  // Applying the Steinhart-Hart equation 
float steinhart = resistance / nominalResistance;
steinhart = log(steinhart);
steinhart /= betaCoefficient;
steinhart += 1.0 / (nominalTemperature + 273.15);
steinhart = 1.0 / steinhart;
steinhart -= 273.15; // Kelvin to Degrees celsius


Serial.print("  Temperature: ");
Serial.print(steinhart);

Serial.println(" C");

// FAN CONTROL
if (fanOn)
  Serial.println("ON");
else
  Serial.println("OFF");


  if (steinhart >= 26.0) {
    fanOn = true;
  } 
  else if (steinhart <= 24.0) {
    fanOn = false;
  }

  analogWrite(enablePin, fanOn ? 255 : 0); // ternary operator '?' for logic control
delay(1000);
}
