/***********************************
Rafty - The ultimate raft controller
Compass, GPS, DC/Stepper-motor control

v0.1
Mikael Hirn
mikael.hirn@gmail.com
www.raftycontroller.com

Oled display size is 128x64px.

Used libraries:
Oled display - Adafruit SH1106 - https://github.com/wonho-maker/Adafruit_SH1106
Graphics - Adafruit GFX - https://github.com/adafruit/Adafruit-GFX-Library
Compass - Jarzebski, HMC5883L - https://github.com/jarzebski/Arduino-HMC5883L
Rotary ncoder - Paul Stoffregen, Encoder - https://github.com/PaulStoffregen/Encoder

************************************/
//Libraries ========================================
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <Encoder.h>
//#include <HMC5883L.h>

//Initialize ========================================
Encoder encMainMotor(2, 3);
#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);
//HMC5883L compass;

//Pins ==============================================
#define rudderPin A0
#define beepPin 4
#define motorpwm  5
#define motordira  6
#define motordirb 7



//Variables =========================================
double rudder = 0;
int oldPosMainMotor = -999;
int oldPosRudd = 0;
int barStart = 0;
int barLen = 0;
int turnSpeed = 0;
int calc = 0;
int maxTurnSteps = 500; //The amount or steps from hard stop to hard stop.
double half = ((maxTurnSteps / 2)); //The center point of the rudder.
int fromCenter = 0;
int rudderScaled = 0;

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

  void turnMotorTo(double rudd) {
    
    //Calculate the difference between set and desired rudder
    Serial.write("ruddIN: ");
    Serial.println(rudd);
    
    calc = encMainMotor.read()-rudd;
    
    Serial.write("Calc: ");
    Serial.println(calc);
    if(calc > 10) {
      digitalWrite(motordira, LOW);
      digitalWrite(motordirb, HIGH);
    }else if(calc < -10){
      digitalWrite(motordira, HIGH);
      digitalWrite(motordirb, LOW);
    }else{
      digitalWrite(motordira, LOW);
      digitalWrite(motordirb, LOW);
    }

    //Set the motor speed, you need to set your threshold for  
    
      if(abs(calc) < maxTurnSteps){
        turnSpeed = map(abs(calc),0,maxTurnSteps,170,255);
      }else{
        turnSpeed = 255;
      }
 
    Serial.write("turnSpeed: ");
    Serial.println(turnSpeed);        
    analogWrite(motorpwm, turnSpeed);
 
  }



void setup() {
Serial.begin(9600);
  
  //Configure Pins
  pinMode(beepPin, OUTPUT);
  digitalWrite(beepPin, HIGH);
  pinMode(motorpwm, OUTPUT);
  digitalWrite(motorpwm, LOW);
  pinMode(motordira, OUTPUT);
  pinMode(motordirb, OUTPUT);
  digitalWrite(motordira, HIGH);
  digitalWrite(motordirb, HIGH);

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
  //beep();
  delay(500);
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
delay(200);
Serial.println("**************");
//Main motor position
int newPosMainMotor = encMainMotor.read();
if(newPosMainMotor != oldPosMainMotor){
  oldPosMainMotor = newPosMainMotor;
}
Serial.write("mainMotorPos: ");
Serial.println(newPosMainMotor);
  
//Draw screen header
drawHeader(driveMode);

//Which drive mode is selected?
switch (driveMode){
  case 0:
      //Read rudder (Pot)
      rudder = map(analogRead(rudderPin),0,1023,0,maxTurnSteps);
      Serial.write("Rudder pot: ");
      Serial.println(rudder);
      
      
  
      if(rudder > (half+3)) //rudder left
      {
        
      //Draw bar to display ********************************
        fromCenter = rudder - half;
        rudderScaled=map(fromCenter,0,half,0,64);
        barStart = rudderScaled + 19;
        barLen = 64 - rudderScaled;
        
        //Fill the bar with white and half of the bar with blank
        display.fillRect(0,30,128,30,1);
        display.fillRect(64,30,64,30,0);
        
        //Fill the blank part
        display.fillRect(0,30,barLen,30,0);
      //****************************************************
       
      }
      else if(rudder < (half-3)) //rudder right
      {
        
        
        fromCenter = half - rudder;
        rudderScaled=map(fromCenter,0,half,0,64);
        barStart = rudderScaled + 64;
        
        //Fill the bar with white and half of the bar with blank
        display.fillRect(0,30,128,30,1);
        display.fillRect(0,30,64,30,0);
        
        //Fill the blank part
        display.fillRect(barStart,30,64,30,0);
       
      }
      else
      {
        display.fillRect(0,30,128,30,0);
        display.fillCircle(64, 40, 10, 1);
        
      }

      //Turn motor
      turnMotorTo(rudder);
      
      
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

    break;
}












}
