#include <Wire.h>
#define SLAVE_ADDRESS 0x08

/*thermistor parameters:
 * RT0: 10 000 Ω
 * B: 3977 K +- 0.75%
 * T0:  25 C
 * +- 5%
 */

//These values are in the datasheet
#define RT0 10000   // Ω
#define B 3435      // K
//--------------------------------------

#define VCC 5    //Supply voltage
#define R 10000  //R=100KΩ

//Variables for thermistor
float RT, VR, ln, TX, T0, VRT;
float data[1]; 
byte data_to_echo = 0;

void setup() 
{
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(sendData);
  Serial.begin(9600);
  T0 = 25 + 273.15;                 //Temperature T0 from datasheet, conversion from Celsius to kelvin
}

void loop() {
  VRT = analogRead(A0);              //Acquisition analog value of VRT
  VRT = (5.00 / 1023.00) * VRT;      //Conversion to voltage
  VR = VCC - VRT;
  RT = VRT / (VR / R);               //Resistance of RT

  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor
  TX = TX - 273.15;                 //Conversion to Celsius
  data[0]=TX; 
  Serial.print("Temperature:");
  Serial.print("\t");
  Serial.print(TX);
  Serial.print("C\t\t");
  Serial.print(TX + 273.15);        //Conversion to Kelvin
  Serial.print("K\t\t");
  Serial.print((TX * 1.8) + 32);    //Conversion to Fahrenheit
  Serial.println("F");
  delay(100);
}


void sendData()
{
   Wire.write((byte*)&data, sizeof(float));
}
