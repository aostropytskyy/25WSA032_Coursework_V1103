#include <Arduino.h>
// Loovee @ 2015-8-26
#include <math.h>
const int B = 4275000; // B value of the thermistor
const int R0 = 100000; // R0 = 100k
const int pinTempSensor = A0; // Grove - Temperature Sensor connect to A0
const int N = 10; // number of samples to collect for DFT (3 mins)
float magnitude[N];
float freq[N];

float collect_temperature_data(int time);
float* apply_dft(float x[],int time);
void send_data_to_pc(float temp[]);

void setup()
{
  Serial.begin(9600);
}

float collect_temperature_data(int time){

  int a = analogRead(pinTempSensor);
  float R = 1023.0/a-1.0;
  R = R0*R;
  float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
  //Serial.print("temperature = ");
  //Serial.println(temperature);

  // int temp_store[180]; //record values for 3 minutes (180 samples)
  // for (int i =0; i<180;i++){
  //   temp_store[i] = analogRead(pinTempSensor);
  // }

  delay(time);
  return temperature;
}

float* apply_dft(float x[],int time){
  static float real[N];
  static float imag[N];

  float f_s = 1000.0/time; // sampling frequency (1 sample per second)

  for (int k=0; k<N; k++){
     real[k] = 0;
     imag[k] = 0;
    
    for(int n =0; n<N ;n++){
      
      real[k] += x[n]*cos((2*PI*k*n)/N);  //Eq. 3.3
      imag[k] -=  x[n]*sin((2*PI*k*n)/N);  //Eq. 3.4

    }
    float f_k  = (k * f_s) / N;  //eq. 3.2
    magnitude[k] = sqrt(pow(real[k],2) + pow(imag[k],2)); //Eq. 3.5
    freq[k] = f_k;
    
  }
  return freq; //return the frequency as an array
}

void send_data_to_pc(float temp[]){
  Serial.println("Time,Temperature,Frequency,Magnitude");

  for (int i=0; i<N; i++){
    Serial.print(i);        //Time count 
    Serial.print(", ");
    Serial.print(temp[i]);
    Serial.print(", ");
    Serial.print(freq[i]);
    Serial.print(", ");
    Serial.println(magnitude[i]);      //formatted as time, temp, frequency, magnitude
             
  }
  }

void loop()
{ 
  int time = 1000;

  float samples[N];

  // Collect temperature data
  for (int i = 0; i < N; i++)
  {
    samples[i] = collect_temperature_data(time);
    Serial.println(samples[i]);
  }

  // Apply DFT
  apply_dft(samples, time);

  // Send results
  send_data_to_pc(samples);

  delay(5000);
}