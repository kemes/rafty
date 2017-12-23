/***********************************
Rafty - The ultimate raft controller
Compass, GPS, DC/Stepper-motor control

v0.1
Mikael Hirn
mikael.hirn@gmail.com
www.raftycontroller.com



Userd libraries:
Adafruit SH1106 - https://github.com/wonho-maker/Adafruit_SH1106
Adafruit GFX - https://github.com/adafruit/Adafruit-GFX-Library
Jarzebski, HMC5883L - https://github.com/jarzebski/Arduino-HMC5883L
Paul Stoffregen, Encoder - https://github.com/PaulStoffregen/Encoder


************************************/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
//#include <HMC5883L.h>
#include <Encoder.h>

//HMC5883L compass;

Encoder encMainMotor(2, 3); 

#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

//Pins
int rudderPin = A0;
int beepPin = 4;


//Variables
int rudder = 0;
long oldPosMainMotor = -999;

//Set default mode
  int driveMode = 0; //0 = Rudder, 1 = Compass, 2 = GPS

//Functions
  int beep()
  {
    digitalWrite(beepPin, LOW);
    delay(50);
    digitalWrite(beepPin, HIGH);
    delay(100);
    digitalWrite(beepPin, LOW);
    delay(400);
    digitalWrite(beepPin, HIGH);
    delay(400);
    digitalWrite(beepPin, LOW);
    delay(50);
    digitalWrite(beepPin, HIGH);
    delay(100);
    digitalWrite(beepPin, LOW);
    delay(50);
    digitalWrite(beepPin, HIGH);
    delay(100);
  }

//Draw given header to the display
  void drawHeader(int givenHeader){
        display.fillRect(0,0,128,11,1);
        display.setTextSize(1);
        display.setTextColor(0, 1);
        switch(givenHeader){
          case 0:
            display.setCursor(48, 2);
            display.print("RUDDER");
            display.display();
  
            break;
            
          case 1:
            display.setCursor(46, 2);
            display.print("COMPASS");
            display.display();
            break;
            
          case 2:
            display.setCursor(48, 2);
            display.print("GPS");
            display.display();
            break;
        }
        
  }



void setup() {
Serial.begin(9600);
  
//Configure Pins
pinMode(beepPin, OUTPUT);
digitalWrite(beepPin, HIGH);

//Init display (Def I2C address: 0x3C, Mega2560 20 (SDA), 21(SCL))
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.clearDisplay();
  display.display();
  display.setTextColor(1, 0);
  display.setCursor(37, 20);
  display.setTextSize(2);
  display.print("Rafty");
  display.setTextSize(1);
  display.setCursor(55, 40);
  display.print("v0.1");
  display.display();
  beep();
  delay(2000);
  display.clearDisplay();
  display.display();


//Init compass
 /* display.setTextSize(1);
  display.setCursor(27, 30);
  display.print("Compass init..");
  display.display();

  delay(2000);
  display.clearDisplay();
  display.display();
  */

//Init GPS

//Home motor



}

void loop() {
long newPosMainMotor = encMainMotor.read();
if(newPosMainMotor != oldPosMainMotor){
  oldPosMainMotor = newPosMainMotor;
  Serial.println(newPosMainMotor);
}
  
//Read encoder


//Which drive mode is selected?
switch (driveMode){
  case 0:
      //Read rudder
      rudder = analogRead(rudderPin);
      rudder = map(rudder, 0,1023,0,90);
      
      drawHeader(0);
      
      display.setTextColor(1, 0);
      display.setTextSize(3);
      display.setCursor(40, 30);
      
      if(rudder > 46)
      {
        rudder = rudder - 45;
        display.print("L");
        if(rudder < 10)
        {
          display.print("0");
        }
        display.print(rudder);
      }
      else if(rudder < 44)
      {
        rudder = 45 - rudder;
        display.print("R");
        if(rudder < 10)
        {
          display.print("0");
        }
        display.print(rudder);
      }
      else
      {
        display.print("000");
      }
      
      
      /*
      display.setCursor(56, 20);
      display.print("SET");
      
      display.setCursor(59, 30);
      display.print(rudder);
      
      display.setCursor(43, 45);
      display.print("CURRENT");
      
      display.setCursor(59, 55);
      display.print("00");
      */
      display.display();


      
      
      

    break;

  case 1:
      drawHeader(1);
      
      display.setTextColor(1, 0);
      
      display.setCursor(32, 20);
      display.print("SET HEADING");
      
      display.setCursor(56, 30);
      display.print("000");
      
      display.setCursor(22, 45);
      display.print("CURRENT HEADING");
      
      display.setCursor(56, 55);
      
      display.print("000");
      display.display();
      delay(1000);


    break;

  case 2:
      drawHeader(2);

    break;
}












}
