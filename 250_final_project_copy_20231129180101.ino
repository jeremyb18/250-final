#include "SevSeg.h"
SevSeg sevseg;


int ThermistorPin = 0;
int Vo;
float R1 = 100000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

void setup() {
  Serial.begin(9600);
  byte numDigits = 4;
  byte digitPins[] = {10, 11, 12, 13};
  byte segmentPins[] = {9, 2, 3, 5, 6, 8, 7, 4};
  bool resistorsOnSegments = false;
  //bool leadingZeros = false;
  byte hardwareConfig = COMMON_CATHODE;

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  
}


void loop()
 {
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  T = T - 273.15;
  T = (T * 9.0) / 5.0 + 32.0; //Comment out for Celsius

  static unsigned long timer = millis();
if (millis() >= timer)
 {
  timer += 700;
  
  int N10 = floor(T/10);
  int N1 = floor(T)-N10*10;
  int N01 = floor(T*10)-N1*10-N10*100;
  int N001 = floor(T*100)-N01*10 -N1*100-N10*1000;
  float NF=N01*10+N1+N10*0.1+ N001*0.01;

  sevseg.setNumber(NF*100,2);
  Serial.print(NF);
  Serial.print(":");
  Serial.println(T);
  }

  sevseg.refreshDisplay();
}