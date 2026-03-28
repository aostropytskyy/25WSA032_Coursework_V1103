#include <Arduino.h>
// Loovee @ 2015-8-26
#include <math.h>
const int B = 4275000; // B value of the thermistor
const int R0 = 100000; // R0 = 100k
const int pinTempSensor = A0; // Grove - Temperature Sensor connect to A0

float collect_temperature_data(int time);

void apply_dft();

void setup()
{
  Serial.begin(9600);

}

float collect_temperature_data(int time){

  int a = analogRead(pinTempSensor);
  float R = 1023.0/a-1.0;
  R = R0*R;
  float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
  Serial.print("temperature = ");
  Serial.println(temperature);

  // int temp_store[180]; //record values for 3 minutes (180 samples)
  // for (int i =0; i<180;i++){
  //   temp_store[i] = analogRead(pinTempSensor);
  // }

  delay(time);
  return temperature;
}



void apply_dft(float x[]){
  float real[180];
  float imag[180];
  float magnitude[180];

  int N=180;
  //$X[k] = Σ_{n=0}^{N-1} x[n] · e^{-j 2πkn/N}$   (Eq. 3.1)
  //$f_k = (k · f_s) / N   (Eq. 3.2)$
  //$real[k] = Σ_{n=0}^{N-1} x[n] · cos(2πkn/N)$      (Eq. 3.3)
  //$imag[k] = - Σ_{n=0}^{N-1} x[n] · sin(2πkn/N)$    (Eq. 3.4)
  //$Magnitude[k] = sqrt( real[k]^2 + imag[k]^2 )$   (Eq. 3.5)
  for (int k=0; k<N; k++){
     real[k] = 0;
     imag[k] = 0;
    
    for(int n =0; n<N ;n++){
      
      real[k] += x[n]*cos((2*M_PI*k*n)/N);  //Eq. 3.3
      imag[k] -=  x[n]*sin((2*M_PI*k*n)/N);  //Eq. 3.4

      
    }
    magnitude[k] = sqrt(pow(real[k],2) + pow(imag[k],2)); //Eq. 3.5
  }
  return 0;
}

void loop()
{
  int time=1000;
  float x[180]; //record values for 3 minutes (180 samples)
  for (int i =0; i<180;i++){
    //Serial.println(analogRead(pinTempSensor));
    x[i] = collect_temperature_data(time);
  }
  apply_dft(x);
  // int a = analogRead(pinTempSensor);
  // float R = 1023.0/a-1.0;
  // R = R0*R;
  // float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
  // Serial.print("temperature = ");
  // Serial.println(temperature);
  delay(1000);

}