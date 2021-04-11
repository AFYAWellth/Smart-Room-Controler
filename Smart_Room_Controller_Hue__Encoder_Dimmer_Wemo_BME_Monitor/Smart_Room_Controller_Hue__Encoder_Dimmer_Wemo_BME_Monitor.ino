/*
 *  Project:      Smart Room Controller
 *  Description:  Smart Room Controller Hue Encoder 2wemo BME & Monitor
 *  Authors:      Steffie
 *  Date:         April 10, 2021
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
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>
Adafruit_BME280 bme; // I2C
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
int rot=1-3;
int wemo;
float tempC;
float tempF;
float pressPA;
float pressHg;
float humidRH;
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
//newVal=map(value, fromLow, fromHigh, toLow, toHigh);
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

EthernetClient client;

void setup(){
  Serial.begin(9600);
    while(!Serial);    // time to get serial running
    Serial.println(F("BME280 test"));

    //unsigned int status;
    
    // default settings
    //status=bme.begin(0x76);
   // You can also pass in a Wire library object like &Wire2

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever;
   }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();//must be at END
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);//on example this is located on the top left corner of screen

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

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
//lines 234-235 code is for the BME and monitor
      printValues();
//delay(delayTime);

//lines 238-242 code is to control the dimmer position
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

//lines 261-276 are for OLEDWrite_Temp_Pressure_Humidity 
void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
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


 
