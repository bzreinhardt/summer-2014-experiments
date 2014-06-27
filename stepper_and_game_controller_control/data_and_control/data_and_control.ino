/*
 Example sketch for the PS3 USB library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 
Modified by Benjamin Reinhardt (bzreinhardt on github and gmail to control a stepper motor based on ps3 controller)
 */

 #include <SD.h>

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
//


#include <PS3USB.h>
// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

#include <AccelStepper.h>

USB Usb;
/* You can create the instance of the class in two ways */
PS3USB PS3(&Usb); // This will just create the instance
//PS3USB PS3(&Usb,0x00,0x15,0x83,0x3D,0x0A,0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

// Chip Select pin is tied to pin 8 on the SparkFun SD Card Shield
const int chipSelect = 8;  

boolean printAngle;
uint8_t state = 0;
//set the direction and enable pins for the motor controller
/*
int dirpin1 = 0; 
int steppin1 = 1;
int dirpin2 = 2;
int steppin2 = 3;
int dirpin3 = 4;
int steppin3 = 5;
int dirpin4 = 6;
int steppin4 = 7;
*/
// variables keeping track of the controller commands
int LX = 0;
int LY = 0;
int RX = 0;
int RY = 0;
int oldtime = 0;
int newtime = 0;

AccelStepper stepper1(1, 3, 2);
AccelStepper stepper2(1, 5, 4);
AccelStepper stepper3(1, 7, 6);

// keep track of positions of stepper motors
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int d1 = 0;
int d2 = 0;
float speed = 4000;

boolean direction = true; 

void setup() {
  pinMode(chipSelect, OUTPUT);
  pinMode(1, OUTPUT);
  
  //Serial.begin(9600);
  //while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  if (Usb.Init() == -1) {
    //Serial.print(F("\r\nOSC did not start"));
    //while (1); //halt
  }

  // see if the card is present and can be initialized:
  
  if (!SD.begin(chipSelect)) {
    //Serial.println("\r\nCard failed, or not present");
    // don't do anything more:
    //return;
  }
  else{
   //Serial.println("\r\ncard initialized.");
 }
 


  
  //Set the control pins as outputs
  /*
  pinMode(dirpin1, OUTPUT);
  pinMode(steppin1, OUTPUT);
  pinMode(dirpin2, OUTPUT);
  pinMode(steppin2, OUTPUT);
  pinMode(dirpin3, OUTPUT);
  pinMode(steppin3, OUTPUT);
  pinMode(dirpin4, OUTPUT);
  pinMode(steppin4, OUTPUT);
  */

  stepper1.setMaxSpeed(7000);
  stepper1.setAcceleration(10000);
  stepper1.setSpeed(0);
  stepper1.setMinPulseWidth(20);

  stepper2.setMaxSpeed(7000);
  stepper2.setAcceleration(10000);
  stepper2.setSpeed(0);
  stepper2.setMinPulseWidth(20);

  stepper3.setMaxSpeed(7000);
  stepper3.setAcceleration(10000);
  stepper3.setSpeed(0);
  stepper3.setMinPulseWidth(20);

  
  //print that you've started
  //Serial.print(F("\r\nPS3 USB Library Started"));

}


void loop() {
  // check the controller values
  Usb.Task();
  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    if (PS3.getButtonClick(UP)) {
      digitalWrite(1, HIGH);
      Serial.print("U");
      if (d2 < 0){d2 = 0;}
      else if (d2 == 0){d2 = 1;}
      else if (d2 > 0){d2 = 1;}
      stepper2.setSpeed(d2*speed);
      stepper3.setSpeed(d2*speed);
      
      recordData(millis(), stepper1.speed(), stepper2.speed());
      File dataFile = SD.open("data.txt", FILE_WRITE);
      if(dataFile){
        int timeStamp = millis();
        float s1 = stepper1.speed();
        float s23 = stepper2.speed();
        dataFile.print(timeStamp);
        dataFile.print(", ");
        dataFile.print(s1);
        dataFile.print(", ");
        dataFile.print(s23);
        dataFile.print("\n");
        dataFile.close();
      }
      

    }
    if (PS3.getButtonClick(DOWN)){
      digitalWrite(1, LOW);
      if (d2 < 0){d2 = -1;}
      else if (d2 == 0){d2 = -1;}
      else if (d2 > 0){d2 = 0;}
      stepper2.setSpeed(d2*speed);
      stepper3.setSpeed(d2*speed);
      //recordData(millis(), stepper1.speed(), stepper2.speed());
    }

    if (PS3.getButtonClick(RIGHT)) {
      if (d1 < 0){d1 = 0;}
      else if (d1 == 0){d1 = 1;}
      else if (d1 > 0){d1 = 1;}
      stepper1.setSpeed(d1*speed);
      //recordData(millis(), stepper1.speed(), stepper2.speed());
      
      
    }
    
    if (PS3.getButtonClick(LEFT)) {
      if (d1 < 0){d1 = -1;}
      else if (d1 == 0){d1 = -1;}
      else if (d1 > 0){d1 = 0;}
      stepper1.setSpeed(d1*speed);
      //recordData(millis(), stepper1.speed(), stepper2.speed());
    }

    //Serial.print(" LX = ");
    //Serial.print(LX);
    //Serial.print(" RX = ");
    //Serial.print(RX);
    //Serial.println();
  }
  
  // find delta in position based on inputs
  /*
  if (PS3.getButtonClick()){
    d1 = 10*(LX - 127);
    pos1 = stepper1.currentPosition();
    pos3 = stepper3.currentPosition();
    stepper1.moveTo(pos1 + d1);
    stepper3.moveTo(pos3 + d1);
  }
  if (RX < 117 || RX > 137){
    d2 = 20*(RX - 127);
    pos2 = stepper2.currentPosition();
    stepper2.moveTo(pos2 + d2);
  }
  // set the new positions for each motor
  // run the motors towards the new position
  */
stepper1.runSpeed();
stepper2.runSpeed();
stepper3.runSpeed();

}

void recordData(float time, float s1, float s23){
      
}

