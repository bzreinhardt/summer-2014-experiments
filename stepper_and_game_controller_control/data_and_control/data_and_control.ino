/*
 Example sketch for the PS3 USB library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 
Modified by Benjamin Reinhardt (bzreinhardt on github and gmail to control a stepper motor based on ps3 controller)
 */

#include <PS3USB.h>
#include <SD.h>
// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <AccelStepper.h>

// Define two steppers and the pins they will use
AccelStepper stepper1(1, 3, 2);
AccelStepper stepper2(1, 5, 4);

USB Usb;
/* You can create the instance of the class in two ways */
PS3USB PS3(&Usb); // This will just create the instance
//PS3USB PS3(&Usb,0x00,0x15,0x83,0x3D,0x0A,0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

boolean printAngle;
uint8_t state = 0;
//set the direction and enable pins for the motor controller
int dirpin1 = 0;
int steppin1 = 1;
int dirpin2 = 2;
int steppin2 = 3;
int dirpin3 = 4;
int steppin3 = 5;
int dirpin4 = 6;
int steppin4 = 7;
int i = 0;


int pos1 = 3600;
int pos2 = 5678;
// variable turn data acquisition on and off
boolean dataAcq = false; 
boolean direction = true; 
// Chip Select pin is tied to pin 8 on the SparkFun SD Card Shield
const int chipSelect = 8;  

void setup() {
  
  Serial.begin(9600);
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  
  //Set the control pins as outputs
  pinMode(dirpin1, OUTPUT);
  pinMode(steppin1, OUTPUT);
  pinMode(dirpin2, OUTPUT);
  pinMode(steppin2, OUTPUT);
  pinMode(dirpin3, OUTPUT);
  pinMode(steppin3, OUTPUT);
  pinMode(dirpin4, OUTPUT);
  pinMode(steppin4, OUTPUT);

  stepper1.setMaxSpeed(3000);
  stepper1.setAcceleration(10000);
  stepper2.setMaxSpeed(3000);
  stepper2.setAcceleration(10000);
  stepper1.setSpeed(0);
  stepper2.setSpeed(0);

  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(chipSelect, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  
  //print that you've started
  Serial.print("\r\nTest started");

}


void loop() 
{
  Usb.Task();
  stepper1.runSpeed();
  stepper2.runSpeed();
  i++;
  if(i%20 == 0){
  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    if (PS3.getButtonClick(CROSS)){
      dataAcq = !dataAcq;
    }
    int left_x = PS3.getAnalogHat(LeftHatX);
    int left_y = PS3.getAnalogHat(LeftHatY);
    int right_x = PS3.getAnalogHat(RightHatX);
    int right_y = PS3.getAnalogHat(RightHatX);
    if (dataAcq == true){
      File dataFile = SD.open("datalog2.txt", FILE_WRITE);
      if (dataFile) 
      {  
        int timeStamp = millis();
        dataFile.print(timeStamp);
        dataFile.print(", ");
        Serial.print(timeStamp);
        Serial.print(", ");

      // read three sensors and append to the string:
        dataFile.print(left_x);
        dataFile.print(", ");
        dataFile.print(left_y);
        dataFile.print(", ");
        dataFile.print(right_x);
        dataFile.print(", ");
        dataFile.print(right_y);
        dataFile.println();
        dataFile.close();
        /*
        Serial.print(left_x);
        Serial.print(", ");
        Serial.print(left_y);
        Serial.print(", ");
        Serial.print(right_x);
        Serial.print(", ");
        Serial.print(right_y);
        */

      // print to the serial port too:
      }  
    // if the file isn't open, pop up an error:
      else
      {
        Serial.println("error opening datalog.txt");
      }
    } 
    //Serial.print(" stepper1 ");
    driveMotor(stepper1, (float)left_x);
    //Serial.print(" stepper2 ");
    driveMotor(stepper2, (float)right_x);
  }
    
    
    //Serial.println();
    
/*
    if (PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 ) 
    {
      //Serial.print(F("\r\nLeftHatX: "));
      //Serial.print(PS3.getAnalogHat(LeftHatX));
      driveMotor(dirpin1, steppin1, left_x);
    }

    if (PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 )
    {
      //Serial.print(F("\r\nLeftHatY: "));
      //Serial.print(PS3.getAnalogHat(LeftHatY));
      driveMotor(dirpin2, steppin2, left_y);
    }
    if (PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 )
    {
      //Serial.print(F("\r\nRightHatX: "));
      //Serial.print(PS3.getAnalogHat(RightHatX));
      driveMotor(dirpin3, steppin3, right_x);
    }
    if (PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117)
    {
      //Serial.print(F("\r\nRightHatY: "));
      //Serial.print(PS3.getAnalogHat(RightHatY));
       driveMotor(dirpin4, steppin4, right_y);
    }
      // analog values between 0-255. 0 - 117 direction 1. 137 - 255 direction 2.
    */
  }
  
}

void driveMotor(AccelStepper motor, float speed){
// analog values between 0-255. 0 - 117 direction 1. 137 - 255 direction 2.


  if (speed < 117){
    float motorSpeed = -1*1000*(117-speed)/117;
    Serial.print(motorSpeed);
    motor.setSpeed(motorSpeed);
    
    
  }
  else if (speed > 137){
    float motorSpeed = 1000*(speed - 137)/118;
    Serial.print(motorSpeed);

    motor.setSpeed(motorSpeed);
    
  }
  else {
    Serial.print(0);
    motor.setSpeed(0);
    
  }
  
}

  /* Rumble things - not necessary
  else if (PS3.PS3MoveConnected) { // One can only set the color of the bulb, set the rumble, set and get the bluetooth address and calibrate the magnetometer via USB
    if (state == 0) {
      PS3.moveSetRumble(0);
      PS3.moveSetBulb(Off);
    } else if (state == 1) {
      PS3.moveSetRumble(75);
      PS3.moveSetBulb(Red);
    } else if (state == 2) {
      PS3.moveSetRumble(125);
      PS3.moveSetBulb(Green);
    } else if (state == 3) {
      PS3.moveSetRumble(150);
      PS3.moveSetBulb(Blue);
    } else if (state == 4) {
      PS3.moveSetRumble(175);
      PS3.moveSetBulb(Yellow);
    } else if (state == 5) {
      PS3.moveSetRumble(200);
      PS3.moveSetBulb(Lightblue);
    } else if (state == 6) {
      PS3.moveSetRumble(225);
      PS3.moveSetBulb(Purble);
    } else if (state == 7) {
      PS3.moveSetRumble(250);
      PS3.moveSetBulb(White);
    }

    state++;
    if (state > 7)
      state = 0;
    delay(1000);
  }
}
*/
