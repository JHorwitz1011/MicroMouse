#include <AccelStepper.h>
#include <MMouse.h>
AccelStepper motorLeft(AccelStepper::DRIVER, 9, 8);
AccelStepper motorRight(AccelStepper::DRIVER, 11, 10);
MMouse MMouse(motorLeft, motorRight);

void setup(){
  Serial.begin(9600);
  MMouse.setupMMouse();
  MMouse.rotate(360); // or use MMouse.rotate(-90);
  MMouse.setForwardMotionAcceleration(1000);
  MMouse.setForwardMotionSpeed(1);
  MMouse.setMaxSpeeds(10000, 10000);
  delay(2000);
}

void loop(){
  if(MMouse.running()==true) {
    MMouse.run();
  }else{
    MMouse.stop();
  }
}
