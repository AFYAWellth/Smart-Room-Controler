/*
 *  Project:      HueHeader
 *  Description:  Smart Room Controller Hue Encoder 2wemo BME & Monitor
 *  Authors:      Steffie
 *  Date:         April 9, 2021
 */
 /*
  * Ethernet Port Wiring
  * 3.3V to Teensy 3.3V
  * GND to Teensy GND
  * MISO to Teensy DI (Pin 12)
  * MOSI to Teensy DO (Pin 11)
  * SCLK to Teensy SCK (Pin 13)
  * SCNn to Teensy Pin 10 (for Chip Select)
  * RSTn to Teensy Pin 9
  * INTn and NC (No Connection) not connected
  */
  /*
 * Project: I2C_Scanner
 * Description: scan the I2C bus for devices
 * Author: Brian Rashap
 * Date: 10-Mar-2020
 */
/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.

 This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
  See the LICENSE file for details.
 **************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Ethernet.h>
#include <mac.h>
#include <wemo.h>
#include <Encoder.h>
Encoder myEnc(2,3);
#include <hue.h>
#include <OneButton.h>
OneButton buttonHue(23,true, true);
OneButton buttonLavalamp(16,false);
OneButton buttonTeapot(15,false);
const int pinA=2;
const int pinB=3;
const int Hue1=1;
const int Hue2=2;
const int Hue3=3;
const int Hue4=4;
const int Hue5=5;
const int maxPos=96;
const int Lavalamp=0;
const int Teapot=3;
int wemo;
bool buttonStateHue;
bool buttonStateLavalamp;
bool buttonStateTeapot;
bool statusHue;
bool statusLavalamp;
bool statusTeapot;
bool HueState;
bool LavalampState;
bool TeapotState;
int position;
int brightness;
int n;
int lastpos;
int newPos;
byte count=0;
byte i;

EthernetClient client;

void setup(){
  pinMode(10, OUTPUT);
  digitalWrite(10,HIGH);
    
  Serial.begin(9600);
  buttonHue.attachClick(clickHue);
  buttonLavalamp.attachClick(clickLavalamp);
  buttonTeapot.attachClick(clickTeapot);

  Serial.println("Starting Program");

  Ethernet.begin(mac);
  delay(200);//ensure Serial Monitor is up and running           
  printIP();
  Serial.printf("LinkStatus: %i \n",Ethernet.linkStatus());
  
  Serial.println("Begin I2C scanninga");
  Wire.begin();
  for (i=0; i<= 127;i++)
  
  Wire.beginTransmission (i);
  //Serial.printf("Wire transmission end returned: %i \n",Wire.endTransmission());
  if (Wire.endTransmission () ==0)
  {
  Serial.printf("Found address: %03i (0x%02X) \n",i,i);
  count++;
      delay (1);
      } 
  Serial.printf("Done: Found %i device(s). \n",count);
}
void loop() {
  /* setHue function needs 5 parameters
   *  int bulb - this is the bulb number
   *  bool activated - true for bulb on, false for off
   *  int color - Hue color from hue.h
   *  int - brightness - from 0 to 255
   *  int - saturation - from 0 to 255
   */

   //The following code is to control the dimmer position
  buttonHue.tick();
  position = myEnc.read();
    if(position>maxPos){
    myEnc.write(maxPos);
    }
    if(position<0){
      position=0;
      myEnc.write(0);
    }
  newPos=map(position,0,maxPos,0,12);
   //Serial.printf("%i\n",newPos);
    brightness=newPos*20;
    if(position!=lastpos){
      lastpos=position;
      
      SetHueLights();//changing brightness only when encoder is turned
    }
    
    buttonLavalamp.tick();
    buttonTeapot.tick();   
}
void clickHue(){
  buttonStateHue = !buttonStateHue;
  Serial.printf("clickHue is %i \n",buttonStateHue);
  SetHueLights();
  }  
void printIP() {
  Serial.printf("My IP address: ");
  for (byte thisByte = 0; thisByte < 3; thisByte++) {
    Serial.printf("%i.",Ethernet.localIP()[thisByte]);
    Serial.printf("%i\n",Ethernet.localIP()[3]);
} 
}
void SetHueLights(){
  if(buttonStateHue == true){
    setHue(1,true,HueBlue,brightness,255);
    setHue(2,true,HueBlue,brightness,255);
    setHue(3,true,HueBlue,brightness,255);
    setHue(4,true,HueBlue,brightness,255);
    setHue(5,true,HueBlue,brightness,255);
    Serial.printf("buttonHue is pressed \n");
     }
  else {
        Serial.printf("buttonHue is not pressed \n");
        setHue(1,false,0,0,0);
        setHue(2,false,0,0,0);
        setHue(3,false,0,0,0);
        setHue(4,false,0,0,0);
        setHue(5,false,0,0,0);
    }
}
  void clickLavalamp(){
  buttonStateLavalamp = !buttonStateLavalamp;
  if(buttonStateLavalamp == true){
    switchON (Lavalamp);
  }
  else{
    switchOFF (Lavalamp);
  }
  
  Serial.printf("ClickLavalamp is %i \n",buttonStateLavalamp);
}
  void clickTeapot(){
  buttonStateTeapot = !buttonStateTeapot;
  if(buttonStateTeapot == true){
    switchON(Teapot);
  }
  else{
    switchOFF(Teapot);
  }
  Serial.printf("ClickTeapot is %i \n", buttonStateTeapot);
}


 
