#include <Arduino.h>
// Loovee @ 2015-8-26
#include <math.h>
const int B = 4275000; // B value of the thermistor
const int R0 = 100000; // R0 = 100k
const int pinTempSensor = A0; // Grove - Temperature Sensor connect to A0

int collect_temperature_data();

void setup()
{
  Serial.begin(9600);
}

int collect_temperature_data()
{
  int temp_store[300];
  for (int i =0; i<300;i++){
    temp_store[i] = analogRead(pinTempSensor);
  }
  return 0;
}

void loop()
{
  int x=0;
  int a = analogRead(pinTempSensor);
  float R = 1023.0/a-1.0;
  R = R0*R;
  float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
  Serial.print("temperature = ");
  Serial.println(temperature);
  delay(1000);
  collect_temperature_data();
  x++;
  if(x==300){
    Serial.println(collect_temperature_data());
  }
}