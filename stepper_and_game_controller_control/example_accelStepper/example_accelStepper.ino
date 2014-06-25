#include <AccelStepper.h>

// Define two steppers and the pins they will use
AccelStepper stepper1(1, 3, 2);
AccelStepper stepper2(1, 5, 4);

int pos1 = 3600;
int pos2 = 5678;

void setup()
{  
  stepper1.setMaxSpeed(3000);
  stepper1.setAcceleration(1000);
  stepper2.setMaxSpeed(3000);
  stepper2.setAcceleration(1000);
  stepper1.setSpeed(3000);
  stepper2.setSpeed(3000);
}

void loop()
{
  stepper1.runSpeed();
  stepper2.runSpeed();
}