  // THIS IS THE CODE USED TO COLLECT SOIL MOISTURE DATA FROM 4 SENSORS TYPE SKU:SEN0193
  // CREATOR: DIMAGHI SCHWAMBACK AND ALEX NAOKI ASATO KOBAYASHI
  // DATE: JULY 07, 2022    


  // ------------------ CODE STARTING ------------  
  
int sensor_pin_01 = A0;
int sensor_pin_02 = A1;
int sensor_pin_03 = A2;
int sensor_pin_04 = A3;

int output_value_01;
int output_value_02;
int output_value_03;
int output_value_04;

int sr_01;
int sr_02;
int sr_03;
int sr_04;

int min_01;
int min_02;
int min_03;
int min_04;

int max_01;
int max_02;
int max_03;
int max_04;

int y;
int m;
int d;
int h;
int mi;
int s;


#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;

#define pin1 4
#define pin2 5
#define pin3 6
#define pin4 7


const int chipSelect = 10;
void setup() {

  Serial.begin(9600);

  // -------------TESTE-------------



  // -------------TESTE-------------

  
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);

  // -------------This session checks if the SD card is present and can be initialized-------------
  

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1);
    Serial.println("Card present and starting measurements");    
  }

 
   //------------ This session keeps the Real Time Clock running------------------------------------------
    
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (!rtc.isrunning()) {
    Serial.println("RTC lost power, lets set the time!");
  }

  Serial.println("Card present and starting measurements");

}

   //------------ This session is for the sensors readings------------------------------------------

void loop() {

  //------------ Calibration Setup ------------------------------------------

  // Indicate here the sensor output when the soil moisture is close to the Saturation
  
  min_01 = 307;
  min_02 = 285;
  min_03 = 298;
  min_04 = 300;

  // Indicate here the sensor output when the soil moisture is close to the zero
  max_01 = 547;
  max_02 = 561;
  max_03 = 560;
  max_04 = 560;

  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();


  //  Here we are indicating the time that data are collected (at each 2 minutes and 30 seconds) 
  if ((now.second() == 30)&& (now.minute()%2==0)) {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    digitalWrite(pin3, LOW);
    digitalWrite(pin4, LOW);

  delay (100);

  //  Here we are indicating that data are collected at 5 replicated 
      for (int i = 0; i < 6; i++) {

      String dataString = "";

      y = now.year();
      m = now.month();
      d = now.day();
      h = now.hour();
      mi = now.minute();
      s = now.second();

      dataString += String(y);
      dataString += ',';

      dataString += String(m);
      dataString += ',';
      dataString += String(d);
      dataString += ',';
      dataString += String(h);
      dataString += ',';
      dataString += String(mi);
      dataString += ',';
      dataString += String(s);
      dataString += ',';

      // Printing the reading at monitor screem for each sensor
      // Sensor 01

      output_value_01 = analogRead(sensor_pin_01);
      sr_01 = map(output_value_01, max_01, min_01, 0, 100);
      Serial.print("Sensor 1: ");
      Serial.print(sr_01);
      Serial.print("% | ");
      Serial.println(analogRead(sensor_pin_01));

      dataString += analogRead(sensor_pin_01);;
      dataString += ',';

      // Sensor 02
      output_value_02 = analogRead(sensor_pin_02);
      sr_02 = map(output_value_02, max_02, min_02, 0, 100);
      Serial.print("Sensor 2: ");
      Serial.print(sr_02);
      Serial.print("% | ");
      Serial.println(analogRead(sensor_pin_02));

      dataString += String(output_value_02);
      dataString += ',';

      // Sensor 03
      output_value_03 = analogRead(sensor_pin_03);
      sr_03 = map(output_value_03, max_03, min_03, 0, 100);
      Serial.print("Sensor 3: ");
      Serial.print(sr_03);
      Serial.print("% | ");
      Serial.println(analogRead(sensor_pin_03));

      dataString += String(output_value_03);
      dataString += ',';

      // Sensor 04
      output_value_04 = analogRead(sensor_pin_04);
      sr_04 = map(output_value_04,max_04,min_04,0,100);
      Serial.print("Sensor 4: ");
      Serial.print(sr_04);
      Serial.print("% | ");
      Serial.println(analogRead(sensor_pin_04));
      //
      dataString += String(output_value_04);


      // Saving the data at .txt file
      File dataFile = SD.open("SensorData.txt", FILE_WRITE);

      // if the file is available, write to it:
      if (dataFile) {
        dataFile.println(dataString);
        dataFile.close();
        // print to the serial port too:
        Serial.println(dataString);
      }
      // if the file isn't open, pop up an error:
      else {
        Serial.println("error opening .txt file");
      }


      delay(1000);
    }
  }

      //Here we controll the relay shield to save energy
  if ((now.second() == 50) && (now.minute() % 2 == 1)) {
    digitalWrite (pin1, HIGH);
    digitalWrite (pin2, HIGH);
    digitalWrite (pin3, HIGH);
    digitalWrite (pin4, HIGH);

  }
  if ((now.second() == 50) && (now.minute() % 2 == 0)) {
    digitalWrite (pin1, HIGH);
    digitalWrite (pin2, HIGH);
    digitalWrite (pin3, HIGH);
    digitalWrite (pin4, HIGH);


  }
  delay(1000);

}
