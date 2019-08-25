/* 
 This example was created by Sérgio Silva
 on the 20th April 2015
 
 This code example is in the public domain. 
 
 Description:
 The robot moves forward the desired distance in cm
 
 */
#include "MMouse.h"
#include "BasicStepperDriver.h"
#include "MultiDriver.h"
#include "SyncDriver.h"
#include "DRV8834.h"                    // MMkit library
#define MOTOR_STEPS 200
// Target RPM for X axis motor
#define MOTOR_X_RPM 100
// Target RPM for Y axis motor
#define MOTOR_Y_RPM 50

//LEFT
#define DIR_X 10
#define STEP_X 11

// RIGHT MOTOR
#define DIR_Y 8
#define STEP_Y 9

#define _STEPSINREV 200
#define ENABLE 12
#define M0 A5
#define M1 A4
byte microsteps = 16;

DRV8834    left(_STEPSINREV, 10, 11, ENABLE, M0, M1);
DRV8834    right(_STEPSINREV, 8, 9, ENABLE, M0, M1);

SyncDriver controllerM(left, right);
MMouse Grigoras(controllerM);   // declaration of object variable to control the MMkit

void setup(){
Grigoras.setupMMouse();
}


void loop(){
  Grigoras.testIRSensors();
delay(5000);
}
