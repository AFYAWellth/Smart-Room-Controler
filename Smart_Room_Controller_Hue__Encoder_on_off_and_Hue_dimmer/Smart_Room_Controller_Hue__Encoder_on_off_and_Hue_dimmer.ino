/*
 *  Project:      HueHeader
 *  Description:  Smart Room Controller 5 Hue on/off and working on dimmer
 *  Authors:      Steffie
 *  Date:         April 8, 2021
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
#include <SPI.h>
#include <Ethernet.h>
#include <mac.h>
#include <wemo.h>
#include <Encoder.h>
//Encoder myEnc(2,3);
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

EthernetClient client;
Encoder myEnc(pinB,pinA);

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
  Serial,printf("ClickTeapot is %i \n", buttonStateTeapot);
}

 
