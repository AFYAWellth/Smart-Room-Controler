/*
 *  Project:      HueHeader
 *  Description:  Smart Room Controller 5 Hue on/off and working on dimmer
 *  Authors:      Steffie
 *  Date:         April 8, 2021
 */
#include <SPI.h>
#include <Ethernet.h>
#include <mac.h>
#include <Encoder.h>
//Encoder myEnc(2,3);
#include <hue.h>
#include <OneButton.h>
OneButton button1 (23,true, true);
const int pinA=2;
const int pinB=3;
const int Hue1=1;
const int Hue2=2;
const int Hue3=3;
const int Hue4=4;
const int Hue5=5;
const int maxPos=96;
bool buttonState;
int position;
int brightness;
int n;
int lastpos;
int newPos;

Encoder myEnc(pinB,pinA);

void setup(){
  Serial.begin(9600);
  button1.attachClick(click1);
  
  Ethernet.begin(mac);
  delay(200);          //ensure Serial Monitor is up and running           
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
  button1.tick();
  position = myEnc.read();
    if(position>maxPos){
    myEnc.write(maxPos);
    }
    if(position<0){
      position=0;
      myEnc.write(0);
    }
  newPos=map(position,0,maxPos,0,12);
    Serial.printf("%i\n",newPos);
    brightness=newPos*20;
    if(position!=lastpos){
      lastpos=position;
      
      SetHueLights();//changing brightness only when encoder is turned
    }
 }
void click1(){
  buttonState = !buttonState;
  Serial.printf("Click1 is %i \n",buttonState);
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
if(buttonState == true){
    setHue(1,true,HueBlue,brightness,255);
    setHue(2,true,HueBlue,brightness,255);
    setHue(3,true,HueBlue,brightness,255);
    setHue(4,true,HueBlue,brightness,255);
    setHue(5,true,HueBlue,brightness,255);
    Serial.printf("button1 is pressed \n");
     }
  else {
        Serial.printf("button1 is not pressed \n");
        setHue(1,false,0,0,0);
        setHue(2,false,0,0,0);
        setHue(3,false,0,0,0);
        setHue(4,false,0,0,0);
        setHue(5,false,0,0,0);
    }
}
